#ifndef DYNARR_H
#define DYNARR_H

typedef struct {
	/* Slot size */
	size_t slot_siz;
	/* # of slots */
	size_t slot_cnt;
	/* # of elements */
	size_t elem_cnt;
	/* Buffer */
	void  *buffer;
} dynarr;

/*
 * Allocate a dynamic array
 */
void dynarr_alloc(dynarr *x, size_t slot_siz);
/*
 * Free a dynamic array
 */
void dynarr_free(dynarr *x);
/*
 * Returns the pointer to the ith element
 */
void *dynarr_ptr(dynarr *x, size_t i);

/*
 * Add cnt elements from src
 */
void dynarr_add(dynarr *x, size_t cnt, void *src);
/*
 * Store cnt elements starting from i into dest
 */
void dynarr_get(dynarr *x, size_t i, size_t cnt, void *dest);

/*
 * Add a character
 * NOTE: x->slot_siz must be sizeof(char)
 */
void dynarr_addc(dynarr *x, char c);
/*
 * Add a pointer
 * NOTE: x->slot_siz must be sizeof(void *)
 */
void dynarr_addp(dynarr *x, void *p);
/*
 * Return the ith character
 * NOTE: x->slot_siz must be sizeof(char)
 */
char dynarr_getc(dynarr *x, size_t i);
/*
 * Return the ith pointer
 * NOTE: x->slot_siz must be sizeof(void *)
 */
void *dynarr_getp(dynarr *x, size_t i);

#endif
