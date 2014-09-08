/*
 * indexbuffer.h
 * 
 * This file is part of the "PicoRenderer" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __PR_INDEXBUFFER_H__
#define __PR_INDEXBUFFER_H__


#include "types.h"


typedef struct pr_indexbuffer
{
    PRsizei numIndices;
    PRushort* indices;
}
pr_indexbuffer;


pr_indexbuffer* _pr_indexbuffer_create(PRsizei numIndices);
void _pr_indexbuffer_delete(pr_indexbuffer* indexbuffer);


#endif
