/// FontData.h
/// Header of the struct FontData and the Font used to render texts
#pragma once
#include <algorithm>
namespace xixEngine {
	struct FontData {
		int id;
		int x;
		int y;
		int width;
		int height;
		int xoffset;
		int yoffset;
		int advance;
	};

	inline bool operator<(const FontData& glyph, int id) { return glyph.id < id; }
	inline bool operator<(int id, const FontData& glyph) { return id < glyph.id; }

	extern FontData Font[191];
	extern FontData& FindFontChar(char c);
	extern float CalculateStringWidth(const char* text, const float TextScale);
}