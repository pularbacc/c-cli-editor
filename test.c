
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "vector.h"

int main() {
	vector v;
	v = vector_create();

	/*for(int i = 0; i < 1000000; i++) {
		vector_add(v, i);
		assert(vector_size(v) == i+1);
		assert(vector_get(v, i) == i);
		printf("%d\n",vector_get(v, i));
	}

	assert(vector_size(v) == 1000000);
	while(!vector_is_empty(v)) {
		vector_remove_at(v, vector_size(v)-1);
	}

	assert(vector_is_empty(v));
	assert(vector_size(v) == 0);*/

	vector_add(v,'a');
	vector_add(v,'b');

	printf("size :%d\n", vector_size(v));
	printf("%s\n", vector_get(v));

	vector_add(v,'c');

	printf("size :%d\n", vector_size(v));
	printf("%s\n", vector_get(v));

	vector_clear(v);

	printf("size :%d\n", vector_size(v));
	printf("%s\n", vector_get(v));


	vector_destroy(v);
	
	return 0;
}