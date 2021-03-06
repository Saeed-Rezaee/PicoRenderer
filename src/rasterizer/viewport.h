/*
 * viewport.h
 * 
 * This file is part of the "PicoRenderer" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef PR_VIEWPORT_H
#define PR_VIEWPORT_H


#include "types.h"


//! The viewport has only floats, becuase they will be
//! used to transform the raster_vertex coordinates.
typedef struct pr_viewport
{
    PRfloat x;
    PRfloat y;
    PRfloat halfWidth;
    PRfloat halfHeight;
    PRfloat minDepth;
    PRfloat maxDepth;
    PRfloat depthSize;  // (maxDepth - minDepth)
}
pr_viewport;


void _pr_viewport_init(pr_viewport* viewport);


#endif
