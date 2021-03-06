/*
 * texture.h
 * 
 * This file is part of the "PicoRenderer" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef PR_TEXTURE_H
#define PR_TEXTURE_H


#include "types.h"
#include "enums.h"
#include "vector2.h"
#include "color.h"


// Maximal 11 MIP-maps restricts the textures to have a
// maximum size of (2^(11-1) = 1024) in width and height.
#define PR_MAX_NUM_MIPS             11
#define PR_MAX_TEX_SIZE             1024

#define PR_MIP_SIZE(size, mip)      ((size) >> (mip))
#define PR_TEXTURE_HAS_MIPS(tex)    ((tex)->mips > 1)


//! Textures can have a maximum size of 256x256 texels.
//! Textures store all their mip maps in a single texel array for compact memory access.
typedef struct pr_texture
{
    PRtexsize           width;                      //!< Width of the first MIP level.
    PRtexsize           height;                     //!< Height of the first MIP level.
    PRubyte             mips;                       //!< Number of MIP levels.
    PRcolorindex*       texels;                     //!< Texel MIP chain.
    const PRcolorindex* mipTexels[PR_MAX_NUM_MIPS]; //!< Texel offsets for the MIP chain (Use a static array for better cache locality).
}
pr_texture;


pr_texture* _pr_texture_create();
void _pr_texture_delete(pr_texture* texture);

void _pr_texture_singular_init(pr_texture* texture);
void _pr_texture_singular_clear(pr_texture* texture);

//! Sets the single color to the specified texture. No null pointer assertion!
PR_INLINE void _pr_texture_singular_color(pr_texture* texture, PRcolorindex colorIndex)
{
    texture->texels[0] = colorIndex;
}

//! Sets the 2D image data to the specified texture.
PRboolean _pr_texture_image2d(
    pr_texture* texture,
    PRtexsize width, PRtexsize height,
    PRenum format, const PRvoid* data, PRboolean dither, PRboolean generateMips
);

PRboolean _pr_texture_subimage2d(
    pr_texture* texture,
    PRubyte mip, PRtexsize x, PRtexsize y,
    PRtexsize width, PRtexsize height,
    PRenum format, const PRvoid* data, PRboolean dither
);

//! Returns the number of MIP levels for the specified maximal texture dimension (width or height).
PRubyte _pr_texture_num_mips(PRubyte maxSize);

//! Returns a pointer to the specified texture MIP level.
const PRcolorindex* _pr_texture_select_miplevel(const pr_texture* texture, PRubyte mip, PRtexsize* width, PRtexsize* height);

//! Returns the MIP level index for the specified texture.
//PRubyte _pr_texture_compute_miplevel(const pr_texture* texture, PRfloat r1x, PRfloat r1y, PRfloat r2x, PRfloat r2y);

//! Samples the nearest texel from the specified MIP-map level.
PRcolorindex _pr_texture_sample_nearest_from_mipmap(const PRcolorindex* mipTexels, PRtexsize mipWidth, PRtexsize mipHeight, PRfloat u, PRfloat v);

//! Samples the nearest texel from the specified texture. MIP-map selection is compuited by tex-coord derivations ddx and ddy.
PRcolorindex _pr_texture_sample_nearest(const pr_texture* texture, PRfloat u, PRfloat v, PRfloat ddx, PRfloat ddy);

//! Returns a parameter of the specified texture MIP-map level.
PRint _pr_texture_get_mip_parameter(const pr_texture* texture, PRubyte mip, PRenum param);


#endif
