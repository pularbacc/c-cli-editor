#ifndef _VECTOR_H
#define _VECTOR_H

typedef struct _vector *vector;

typedef char value_type;

vector vector_create();

void vector_destroy(vector);

value_type vector_get_at(vector, int);

value_type * vector_get(vector);

void vector_put(vector, int, value_type);

void vector_add(vector, value_type);

void vector_add_at(vector, int, value_type);

value_type vector_remove_at(vector, int);

int vector_is_empty(vector);

int vector_size(vector);

void vector_clear(vector);

#endif