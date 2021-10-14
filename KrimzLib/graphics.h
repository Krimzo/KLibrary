#pragma once
#include <iostream>
#include <vector>
#include "KrimzLib/types.h"


namespace kl
{
	class graphics
	{
	public:
		// Converts a color/bitmap from RGBA to BGRA
		static colorf_bgra ToBGRA(colorf c)
		{
			return { c.b, c.g, c.r, c.a };
		}
		static colord_bgra ToBGRA(colord c)
		{
			return { c.b, c.g, c.r, c.a };
		}
		static color_bgra ToBGRA(color c)
		{
			return { c.b, c.g, c.r, c.a };
		}
		static bitmap_bgra ToBGRA(bitmap& bm)
		{
			bitmap_bgra tempBM(bm.GetWidth(), bm.GetHeight());
			for (int i = 0; i < bm.GetSize(); i++)
			{
				tempBM.GetPixelData()[i] = ToBGRA(bm.GetPixelData()[i]);
			}
			return tempBM;
		}
		static bitmap_bgra ToBGRA(bitmap&& bm)
		{
			bitmap_bgra tempBM(bm.GetWidth(), bm.GetHeight());
			for (int i = 0; i < bm.GetSize(); i++)
			{
				tempBM.GetPixelData()[i] = ToBGRA(bm.GetPixelData()[i]);
			}
			return tempBM;
		}

		// Converts a color/bitmap from BGRA to RGBA
		static colorf ToRGBA(colorf_bgra c)
		{
			return { c.r, c.g, c.b, c.a };
		}
		static colord ToRGBA(colord_bgra c)
		{
			return { c.r, c.g, c.b, c.a };
		}
		static color ToRGBA(color_bgra c)
		{
			return { c.r, c.g, c.b, c.a };
		}
		static bitmap ToRGBA(bitmap_bgra& bm)
		{
			bitmap tempBM(bm.GetWidth(), bm.GetHeight());
			for (int i = 0; i < bm.GetSize(); i++)
			{
				tempBM.GetPixelData()[i] = ToRGBA(bm.GetPixelData()[i]);
			}
			return tempBM;
		}
		static bitmap ToRGBA(bitmap_bgra&& bm)
		{
			bitmap tempBM(bm.GetWidth(), bm.GetHeight());
			for (int i = 0; i < bm.GetSize(); i++)
			{
				tempBM.GetPixelData()[i] = ToRGBA(bm.GetPixelData()[i]);
			}
			return tempBM;
		}
	};
}
