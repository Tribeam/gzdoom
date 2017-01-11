//
// Copyright (C) 1993-1996 by id Software, Inc.
//
// This source is available for distribution and/or modification
// only under the terms of the DOOM Source Code License as
// published by id Software. All rights reserved.
//
// The source is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// FITNESS FOR A PARTICULAR PURPOSE. See the DOOM Source Code License
// for more details.
//

#pragma once

#include "swrenderer/line/r_line.h"
#include "swrenderer/scene/r_bsp.h"

#define MINZ double((2048*4) / double(1 << 20))

struct particle_t;
struct FVoxel;

namespace swrenderer
{
	struct drawseg_t;

	struct vissprite_t
	{
		struct posang
		{
			FVector3 vpos; // view origin
			FAngle vang; // view angle
		};

		short x1, x2;
		FVector3 gpos; // origin in world coordinates
		union
		{
			struct
			{
				float gzb, gzt; // global bottom / top for silhouette clipping
			};
			struct
			{
				int y1, y2; // top / bottom of particle on screen
			};
		};
		DAngle Angle;
		fixed_t xscale;
		float yscale;
		float depth;
		float idepth; // 1/z
		float deltax, deltay;
		uint32_t FillColor;
		double floorclip;
		union
		{
			FTexture *pic;
			struct FVoxel *voxel;
		};
		union
		{
			// Used by face sprites
			struct
			{
				double texturemid;
				fixed_t	startfrac; // horizontal position of x1
				fixed_t	xiscale; // negative if flipped
			};
			// Used by wall sprites
			FWallCoords wallc;
			// Used by voxels
			posang pa;
		};
		sector_t *heightsec; // killough 3/27/98: height sector for underwater/fake ceiling
		sector_t *sector; // [RH] sector this sprite is in
		F3DFloor *fakefloor;
		F3DFloor *fakeceiling;
		uint8_t bIsVoxel : 1; // [RH] Use voxel instead of pic
		uint8_t bWallSprite : 1; // [RH] This is a wall sprite
		uint8_t bSplitSprite : 1; // [RH] Sprite was split by a drawseg
		uint8_t bInMirror : 1; // [RH] Sprite is "inside" a mirror
		WaterFakeSide FakeFlatStat; // [RH] which side of fake/floor ceiling sprite is on
		short renderflags;
		uint32_t Translation; // [RH] for color translation
		visstyle_t Style;
		int CurrentPortalUniq; // [ZZ] to identify the portal that this thing is in. used for clipping.

		vissprite_t() {}
	};

	class VisibleSpriteList
	{
	public:
		static int MaxVisSprites;
		static vissprite_t **vissprites;
		static vissprite_t **firstvissprite;
		static vissprite_t **vissprite_p;

		static void Deinit();
		static void Clear();
		static vissprite_t *Add();

	private:
		static vissprite_t **lastvissprite;
	};

	class SortedVisibleSpriteList
	{
	public:
		static void Deinit();

		static void Sort(bool(*compare)(vissprite_t *, vissprite_t *), size_t first);

		static bool sv_compare(vissprite_t *a, vissprite_t *b);
		static bool sv_compare2d(vissprite_t *a, vissprite_t *b);

		static vissprite_t **spritesorter;
		static int vsprcount;

	private:
		static int spritesortersize;
	};

	class RenderTranslucent
	{
	public:
		static void Deinit();
		static void Clear();
		static void Render();

		static bool DrewAVoxel;

		static bool ClipSpriteColumnWithPortals(int x, vissprite_t* spr);

	private:
		static void CollectPortals();
		static void DrawSprite(vissprite_t *spr);
		static void DrawMaskedSingle(bool renew);

		static TArray<drawseg_t *> portaldrawsegs;
	};
}
