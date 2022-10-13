// UniversalUI-Geometry.h (cross-platform)
// types first written in early 2022 by Seb Hall, this version of each first written on 13/10/2022 also by Seb Hall
//
// UniversalUI files are intended for use with UniversalUI programs only. I accept no responsibility for performance or stability, use at your own risk
// 
// This file contains basic types and structures for the UniversalUI Framework
// For more information, please see our website.

#ifndef UNIVERSALUI_GEOMETRY_H
#define UNIVERSALUI_GEOMETRY_H

struct uSize {
    float width;
    float height;
};

struct uRect {
    float x;
    float y;
    float width;
    float height;
};

struct uColour {
    float r;
    float g;
    float b;
    float a;
};

#endif