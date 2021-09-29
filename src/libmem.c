/*
  Copyright 2021 Marco Simonelli
 
  Licensed under the Apache License, Version 2.0 (the "License");
  you may not use this file except in compliance with the License.
  You may obtain a copy of the License at
 
  http://www.apache.org/licenses/LICENSE-2.0
 
  Unless required by applicable law or agreed to in writing, software
  distributed under the License is distributed on an "AS IS" BASIS,
  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  See the License for the specific language governing permissions and
  limitations under the License.
 */

#include "atomic.h"
#include "libmem.h"

#include <stdlib.h>
#include <string.h>

typedef size_t refcnt_t;
typedef struct
{
  refcnt_t refcnt; // number of ownerships of this allocation
  size_t alloc_sz; // bytes allocated *not including this header*
  void (*dealloc)();
} ref_alloc_hdr;

// takes a pointer to data, returns pointer to ref_alloc_hdr
static inline ref_alloc_hdr* get_ref_hdr(const void* m)
{
  return (ref_alloc_hdr*)((char*)m - sizeof(ref_alloc_hdr));
}

// takes a pointer to ref_alloc_hdr, returns pointer to data
static inline void* get_data(const void* m)
{
  return (void*)((char*)m + sizeof(ref_alloc_hdr));
}

static inline void ref_free(const void* m)
{
  ((ref_alloc_hdr*)m)->dealloc();
}

static inline void ref_dec(ref_alloc_hdr* m)
{
  if(libmem_atomic_fetch_dec(&m->refcnt) == 1)
  {
    ref_free(m);
  }
}

static inline void ref_inc(ref_alloc_hdr* m)
{
  libmem_atomic_fetch_inc(&m->refcnt);
}

void* lm_alloc(size_t sz)
{
  // alloc and null header
  void* r = malloc(sz + sizeof(ref_alloc_hdr));
  bzero(r, sizeof(ref_alloc_hdr));
  
  // set size
  ((ref_alloc_hdr*)r)->alloc_sz = sz;

  // dealloc callback
  ((ref_alloc_hdr*)r)->dealloc = free;

  // initial refcnt
  ref_inc((ref_alloc_hdr*)r);

  return get_data(r);
}

void* lm_copy(const void* m)
{
  // m is a pointer to the start of the data
  // get the size now incase it changes during this function
  size_t sz = get_ref_hdr(m)->alloc_sz;

  // clone mem and set refcnt to one
  void* r = malloc(sz + sizeof(ref_alloc_hdr));
  bzero(r, sizeof(ref_alloc_hdr));
  ref_inc((ref_alloc_hdr*)r);

  // set size
  ((ref_alloc_hdr*)r)->alloc_sz = sz;

  // dealloc callback
  ((ref_alloc_hdr*)r)->dealloc = get_ref_hdr(m)->dealloc;

  // copy data
  memcpy(get_data(r), m, sz);

  return get_data(r);
}

void lm_retain(const void* m)
{
  ref_inc(get_ref_hdr(m));
}

void lm_release(const void* m)
{
  ref_dec(get_ref_hdr(m));
}

void lm_override_dealloc(const void* m, void (*dealloc)(void*))
{
  get_ref_hdr(m)->dealloc = dealloc;
}

void _lm_priv_autorelease_callback(void **m)
{
  ref_dec(*m);
}
