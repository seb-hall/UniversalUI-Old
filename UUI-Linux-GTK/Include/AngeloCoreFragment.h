//  AngeloCoreFragment.h    written by sebhall in March 2023

#ifndef ANGELOCOREFRAGMENT_H
#define ANGELOCOREFRAGMENT_H

const char* AngeloCoreFragment = R"(

#version 330 core

#define OPS(i) uint(texelFetch(ops, ivec2(i, 0), 0).r)
#define IDS(i) int(texelFetch(ids, ivec2(i, 0), 0).r)
#define PMS(i) texelFetch(pms, ivec2(i, 0), 0).r

#define CLEAR_COLOUR 	0u
#define DRAW_COLOUR		2u
#define DRAW_WEIGHT		3u

#define LINE_TWOPOINT	10u
#define CIRCLE_STD		11u
#define ARC_STD			12u
#define RECT_STD		13u
#define RECT_ROUND		14u			

out vec4 FragColor; 

uniform usampler2D 	ops;
uniform usampler2D 	ids;
uniform sampler2D 	pms;

float sdLine(vec2 p, vec2 a, vec2 b) {
  vec2 pa = p - a;
  vec2 ba = b - a;
  float h = clamp(dot(pa, ba) / dot(ba, ba), 0.0, 1.0);
  return length(pa - ba * h);
}

vec4 VectorMain(vec2 pos) { 

	vec4 drawColour = vec4(1.0f ,0.0f ,0.0f ,1.0f);
	vec4 clearColour = vec4(0.0f, 0.0f, 0.0f, 1.0f);

	float drawWeight = 0;

	ivec2 size = textureSize(ops , 0);
	int count = size.x;

	for(int i=0; i<count; i++) {
		switch (OPS(i)) {
			case CLEAR_COLOUR:
				clearColour = vec4(PMS(IDS(i)), PMS(IDS(i) + 1), PMS(IDS(i) + 2), PMS(IDS(i) + 3));
				return vec4(PMS(IDS(i)), PMS(IDS(i) + 1), PMS(IDS(i) + 2), PMS(IDS(i) + 3));
				//return vec4(0.0, 1.0, 1.0, 1.0);
				break;
			case DRAW_COLOUR:
				drawColour = vec4(PMS(IDS(i)), PMS(IDS(i) + 1), PMS(IDS(i) + 2), PMS(IDS(i) + 3));
				//return vec4(PMS(IDS(i)), PMS(IDS(i) + 1), PMS(IDS(i) + 2), PMS(IDS(i) + 3));
				break;
			case DRAW_WEIGHT:
				drawWeight = PMS(IDS(i));
				break;
		}
	}
	
	float d = sdLine(pos, vec2(50.0, 50.0), vec2(150.0, 50.0));
    
    if (d <= 2.0) {
        return drawColour; // Red for object
    }

	return vec4(0.0, 1.0, 1.0, 1.0);// Black for background

}

void main() {

	FragColor = VectorMain(gl_FragCoord.xy);

}

)";

#endif