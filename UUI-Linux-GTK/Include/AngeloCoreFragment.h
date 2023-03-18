//  AngeloCoreFragment.h    written by sebhall in March 2023

#ifndef ANGELOCOREFRAGMENT_H
#define ANGELOCOREFRAGMENT_H

const char* AngeloCoreFragment = R"(

#version 330 core

out vec4 FragColor; // bind to location 0

void main()
{
	FragColor = vec4(1.0f, 0.0f, 1.0f, 1.0f);
}

)";

#endif