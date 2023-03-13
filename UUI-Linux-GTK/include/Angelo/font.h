#ifndef FONT_H
#define FONT_H

#include <UUI/geometry.h>

#include <string>
#include <map>
#include <vector>

enum aFontStyle {
    serif,
    sans,
    mono
};

enum aFontWeight {
    thin,
    light,
    regular,
    medium,
    bold
};

struct aGlyph {
    uSize size;
    uSize offset;
    float span;
};

struct aFont {
    std::string family;
    aFontStyle style;
    aFontWeight weight;
    std::map<std::string, std::vector<aGlyph>> glyphs;
};

#endif