#ifndef ARRAYS_H
#define ARRAYS_H

typedef struct {
	int max_size, item_size, size;
	void **items;
} rarray;

#define MALLOC_CREATE(type, val) ({ \
	type *x = malloc(sizeof(val)); \
	*x = val; \
	x; \
})

// requires including 'utils.h' too
#define RARRAY_ASSIGN(array, items) { \
	int i = 0; \
	while (items[i] != NULL) { \
		if (!rarray_add(array, items[i++])) {\
			fprintf(stderr, "Failed to add item to array."); \
			exit(1); \
		} \
	} \
}

rarray *rarray_create(int size, int item_size);
int rarray_free(rarray *array);
int rarray_resize(rarray *array);
int rarray_add(rarray *array, void *item);

#endif

