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

#include <stdatomic.h>

#define libmem_atomic_fetch_inc(v) __sync_fetch_and_add(v, 1)
#define libmem_atomic_fetch_dec(v) __sync_fetch_and_sub(v, 1)