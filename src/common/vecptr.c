/*
 *
 *
 * vecptr.c
 *  Copyright (c) 2025 Pasqualino Ferrentino (lino.ferrentino@gmail.com)
 *
 * License: GPL3 or above.
 *
 *
 */

#include <stdlib.h>
#include <assert.h>
#include <string.h>

#include "vecptr.h"

/*#define DBG_PRIV_ENABLED*/
#define DBG_PRIV_MOD "vec"
#include "dbgpriv.h"


#define INITIAL_SIZE 10


struct vecptr
{
	size_t size;
	size_t cap;
	void **elems;
};

void* vptr_get(struct vecptr *vec, unsigned pos)
{
	if (pos >= vec->size) {
		return NULL;
	}
	return vec->elems[pos];
}

void* vptr_first(struct vecptr *vec)
{
	if (vec->size == 0) {
		return NULL;
	}

	return vec->elems[0];

}

void* vptr_last_el(struct vecptr *vec)
{
	if (vec->size == 0) {
		return NULL;
	}

	return vec->elems[vec->size-1];
}


vptr_it_c vptr_begin(struct vecptr *vec)
{
	return &(vec->elems[0]);
}

vptr_it_c vptr_end(struct vecptr *vec)
{
	return &(vec->elems[0 + vec->size]);
}

vptr_it_c vptr_last(struct vecptr *vec)
{
	if (vec->size == 0)
		return NULL;
	return &(vec->elems[0 + vec->size - 1]);
}

int vptr_init(struct vecptr **vecptrp)
{
	struct vecptr *vp = calloc(1, sizeof(struct vecptr));
	vp->elems = malloc(sizeof(void*) * INITIAL_SIZE);
	vp->cap = INITIAL_SIZE;
	*vecptrp = vp;
	/*DBG("created a vector of capacity %zd", vp->cap);*/
	return 0;
}

void vptr_free_dealloc(struct vecptr *vec, void (*fr_fnc)(void*))
{
	if (vec == NULL)
		return;
	void **it = vec->elems;

	for (int i = 0 ; i < vec->size ; ++i) {
		/*DBG("Free %p vec->elems %p", *it, vec->elems);*/
		fr_fnc(*it);
		it++;
	}

	free(vec->elems);
	free(vec);
}

void vptr_free(struct vecptr  *vec)
{
	if (vec == NULL)
		return;
	free(vec->elems);
	free(vec);
}

void* vptr_remove_ix(struct vecptr *vec, size_t ix)
{
	assert(ix < vec->size);

	void **elm = &vec->elems[ix];
	void *eli = *elm;

	vec->size--;
	memmove(elm, elm+1, sizeof(void**) * (vec->size - ix));

	return eli;
}

void vptr_delete_ix(struct vecptr *vec, size_t ix, void (*fr_fnc)(void*))
{
	assert(ix < vec->size);
	void **elm = &vec->elems[ix];
	if (fr_fnc != NULL){
		DBG("Delete %p", *elm);
		(*fr_fnc)(*elm);
	}
	vec->size--;
	memmove(elm, elm+1, sizeof(void**) * (vec->size - ix));


}

int vptr_delete_it(struct vecptr *vec, vptr_it elm)
{

	assert(elm != NULL);
	ssize_t offset = elm - vec->elems;
	(void)(offset);
	assert(offset < vec->size);

	vec->size--;
	DBG("remove %p->%p new size %zd", elm, *elm, vec->size);
	memmove(elm, elm+1, sizeof(void**) * (vec->size - offset));

	/* XXX here you should check the down size */
	
	return 0;
}

int vptr_push_tail(struct vecptr  *vec, void *elm)
{
	if (vec->size == vec->cap) {
		size_t new_size = (vec->size * 1.5) + 1;
		void *elems = realloc(vec->elems, sizeof(void*) *
				new_size);
		vec->elems = elems;
		/*DBG("%zd REALLOC TO %zd", vec->size, new_size);*/
		vec->cap = new_size;
	}

	/*DBG("put %p into %zd", elm, vec->size);*/
	vec->elems[vec->size++] = elm;
	return 0;
}

void *const vptr_vec(struct vecptr *vec)
{
	return vec->elems;
}

void vptr_clear(struct vecptr *vec)
{
	vec->size = 0;

	if (vec->cap <= (INITIAL_SIZE * 2)) {
		return;
	}

	/* reallocation to the initial size. */
	size_t new_size = INITIAL_SIZE;
	void *elems = realloc(vec->elems, sizeof(void*) *
			new_size);
	vec->elems = elems;
	vec->cap = new_size;
}

void* vptr_pop_tail(struct vecptr  *vec)
{
	if (vec->size == 0)
		return NULL;
	void *el = vec->elems[--vec->size];
	if (vec->size < (vec->cap >> 2)) {
		size_t new_size = (vec->size * 1.5) + 1;
		void *elems = realloc(vec->elems, sizeof(void*) *
				new_size);
		vec->elems = elems;
		DBG("down size TO %zd size %zd", new_size, vec->size);
		vec->cap = new_size;
	}
	return el;
}

size_t vptr_size(struct vecptr  *vec)
{
	return vec->size;
}

