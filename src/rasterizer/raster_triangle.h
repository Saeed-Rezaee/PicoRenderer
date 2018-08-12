/*
 * raster_triangle.h
 * 
 * This file is part of the "PicoRenderer" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef PR_RASTER_TRIANGLE_H
#define PR_RASTER_TRIANGLE_H


#include "raster_vertex.h"


//!REMOVE THIS!
typedef struct pr_raster_triangle
{
    pr_raster_vertex a;
    pr_raster_vertex b;
    pr_raster_vertex c;
}
pr_raster_triangle;


#endif
