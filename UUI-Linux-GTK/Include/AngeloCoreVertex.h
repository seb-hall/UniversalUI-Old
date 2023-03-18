//  AngeloCoreVertex.h    written by sebhall in March 2023

#ifndef ANGELOCOREVERTEX_H
#define ANGELOCOREVERTEX_H

const char* AngeloCoreVertex = R"(

#version 330 core
layout (location = 0) in vec3 aPos;

void main()
{
	gl_Position = vec4(aPos, 1.0);
}

)";

#endif