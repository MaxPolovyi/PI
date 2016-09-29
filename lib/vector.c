/* Copyright 2013-present Barefoot Networks, Inc.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *   http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

/*
 * Antonin Bas (antonin@barefootnetworks.com)
 *
 */

#include "vector.h"

#include <stdlib.h>
#include <string.h>
#include <assert.h>

struct vector_s {
  size_t e_size;
  size_t size;
  size_t capacity;
  void *data;
};

vector_t *vector_create(size_t e_size, size_t init_capacity) {
  assert(e_size > 0);
  assert(init_capacity > 0);
  vector_t *v = malloc(sizeof(vector_t));
  v->e_size = e_size;
  v->size = 0;
  v->capacity = init_capacity;
  v->data = malloc(init_capacity * e_size);
  return v;
}

void vector_destroy(vector_t *v) {
  free(v->data);
  free(v);
}

static void vector_expand(vector_t *v) {
  v->capacity *= 2;
  v->data = realloc(v, v->capacity * v->e_size);
}

static void *access(const vector_t *v, size_t index) {
  return (char *)v->data + (index * v->e_size);
}

void vector_push_back(vector_t *v, void *e) {
  assert(v->size <= v->capacity);
  if (v->size == v->capacity) vector_expand(v);
  memcpy(access(v, v->size), e, v->e_size);
  v->size++;
}

void *vector_at(const vector_t *v, size_t index) {
  assert(index < v->size);
  return access(v, index);
}

void *vector_data(const vector_t *v) { return v->data; }

size_t vector_size(vector_t *v) { return v->size; }

void vector_remove(vector_t *v, size_t index) {
  assert(index < v->size);
  v->size--;
  if (index == v->size) return;
  memmove(access(v, index), access(v, index + 1),
          (v->size - index) * v->e_size);
}

void vector_remove_e(vector_t *v, void *e) {
  assert(e >= v->data);
  size_t index = (char *)e - (char *)v->data;
  vector_remove(v, index);
}
