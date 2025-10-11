/*
 *
 * vectpr a simple vector of pointers.
 *
 *  Copyright (c) 2025 Pasqualino Ferrentino (lino.ferrentino@gmail.com)
 *
 * License: GPL3 or above.
 *
 */



#include <stddef.h>


struct vecptr;


int vptr_init(struct vecptr **vecp);
void vptr_free(struct vecptr  *vec);
void vptr_free_dealloc(struct vecptr *vec, void (*fr_fnc)(void*));

/* Clears the vector without deallocating the elements,
 * but reallocating its size to the initial size. */
void vptr_clear(struct vecptr *vec);


int vptr_push_tail(struct vecptr  *vec, void *elm);
void* vptr_pop_tail(struct vecptr  *vec);
size_t vptr_size(struct vecptr  *vec);

/* Get the values at position pos */
void* vptr_get(struct vecptr *vec, unsigned pos);

/* peek the last value, without removing it.
 *
 * NULL is returned if the vector is empty.
 *
 * be aware that you cannot distinguish in this
 * case between a NULL valid value from a NULL
 * caused by an empty vector (use vptr_size for this)
 * */
void* vptr_last_el(struct vecptr *vec);

/* 
 *
 * return the first element in the vector.
 *
 * NULL in case of vector empty.
 *
 *
 * */
void* vptr_first(struct vecptr *vec);

typedef void** const vptr_it_c;
typedef void** vptr_it;

/* 
 * Deletes the iterator, putting it to NULL, size
 * is decremented and the vector is compacted. 
 *
 * */
int vptr_delete_it(struct vecptr *vec, vptr_it elm);

/* removes the element at position ix, compating the array. 
 * if fr_fnc is not NULL it is called to free the element
 * */
void vptr_delete_ix(struct vecptr *vec, size_t ix, void (*fr_fnc)(void*));

/* removes the element at position ix, it does compact the array */
void* vptr_remove_ix(struct vecptr *vec, size_t ix);

vptr_it_c const vptr_begin(struct vecptr *vec);

/* points to one past the last element. */
vptr_it_c const vptr_end(struct vecptr *vec);

/* points to the last element, NULL if the vector is empty */
vptr_it_c const vptr_last(struct vecptr *vec);



