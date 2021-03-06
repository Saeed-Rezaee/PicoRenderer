/*
 * color_palette.h
 * 
 * This file is part of the "PicoRenderer" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef PR_COLOR_PALETTE_H
#define PR_COLOR_PALETTE_H


#include "color.h"


#define PR_COLORINDEX_SCALE_RED     36
#define PR_COLORINDEX_SCALE_GREEN   36
#define PR_COLORINDEX_SCALE_BLUE    85

#define PR_COLORINDEX_SELECT_RED    32
#define PR_COLORINDEX_SELECT_GREEN  32
#define PR_COLORINDEX_SELECT_BLUE   64


//! Color palette for 8-bit color indices.
typedef struct pr_color_palette//_r3g3b2
{
    pr_color colors[256];
}
pr_color_palette;


//! Fills the specified color palette with the encoding R3G3B2.
void _pr_color_palette_fill_r3g3b2(pr_color_palette* colorPalette);

//! Converts the specified RGB color into a color index with encoding R3G3B2.
PRcolorindex _pr_color_to_colorindex(PRubyte r, PRubyte g, PRubyte b);


#endif
