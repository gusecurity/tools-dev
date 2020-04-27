/*
 * Dynamic array
 * Author: Mate Kukri
 * License: ISC
 */
#include <stdlib.h>
#include <string.h>
#include "dynarr.h"

#define DYNARR_GROW_FACTOR 2
#define DYNARR_START_SLOTS 10

static void dynarr_grow(dynarr *x)
{
	x->slot_cnt = x->elem_cnt * DYNARR_GROW_FACTOR;
	x->buffer   = realloc(x->buffer, x->slot_cnt * x->slot_siz);
	if (!x->buffer)
		abort();
}

void dynarr_alloc(dynarr *x, size_t slot_siz)
{
	x->slot_siz = slot_siz;
	x->slot_cnt = DYNARR_START_SLOTS;
	x->elem_cnt = 0;

	x->buffer   = realloc(NULL, x->slot_cnt * x->slot_siz);
	if (!x->buffer)
		abort();
}

void dynarr_free(dynarr *x)
{
	free(x->buffer);
}

void *dynarr_ptr(dynarr *x, size_t i)
{
	return (char *) x->buffer + i * x->slot_siz;
}

void dynarr_add(dynarr *x, size_t cnt, void *src)
{
	x->elem_cnt += cnt;
	if (x->elem_cnt > x->slot_cnt)
		dynarr_grow(x);

	memcpy(dynarr_ptr(x, x->elem_cnt - cnt), src, cnt * x->slot_siz);
}

void dynarr_get(dynarr *x, size_t i, size_t cnt, void *dest)
{
	memcpy(dest, dynarr_ptr(x, i), cnt * x->slot_siz);
}

void dynarr_addc(dynarr *x, char c)
{
	++x->elem_cnt;
	if (x->elem_cnt > x->slot_cnt);
		dynarr_grow(x);

	((char *) x->buffer)[x->elem_cnt - 1] = c;
}

void dynarr_addp(dynarr *x, void *p)
{
	++x->elem_cnt;
	if (x->elem_cnt > x->slot_cnt);
		dynarr_grow(x);

	((void **) x->buffer)[x->elem_cnt - 1] = p;
}

char dynarr_getc(dynarr *x, size_t i)
{
	return ((char *) x->buffer)[i];
}

void *dynarr_getp(dynarr *x, size_t i)
{
	return ((void **) x->buffer)[i];
}
