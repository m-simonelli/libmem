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

#include <stddef.h>

/*
  @function lm_alloc
  Allocate a chunk of memory with reference counting
*/
void* lm_alloc(size_t sz);

/*
  @function lm_copy
  Created a copy of a reference counted memory chunk allocated with lm_alloc
*/
void* lm_copy(const void* m);

/*
  @function lm_retain
  Increase the reference count of a chunk of memory
*/
void lm_retain(const void* m);

/*
  @function lm_release
  Decrease the reference count of a chunk of memory. If the refernce count is
  0 after it is decreased, the chunk of memory will automatically be freed
*/
void lm_release(const void* m);

/*
  @function lm_override_dealloc
  Override the function to be called when the reference count reaches 0.
  Useful in cases where further cleanup is required to deinit an object.
*/
void lm_override_dealloc(const void* m, void (*dealloc)(void*));

/*
  @attribute autorelease
  Autodecrement the reference count of this object as soon as it leaves scope
*/
void _lm_priv_autorelease_callback(void *);
#define autorelease __attribute__((cleanup(_lm_priv_autorelease_callback)))
 