//  AngeloCoreFragment.h    written by sebhall in March 2023

#ifndef ANGELOCOREFRAGMENT_H
#define ANGELOCOREFRAGMENT_H

const char* AngeloCoreFragment = R"(

#version 330 core

layout (std140) uniform RenderOperations {
  	uint ops [];
};

layout (std140) uniform RenderIndices {
  	uint indices [];
};

layout (std140) uniform RenderParameters {
  	float params [];
};

out vec4 FragColor; 

void main()
{
	FragColor = vec4(1.0f, 0.0f, 1.0f, 1.0f);
}

)";

#endif