//  AngeloCoreFragment.h    written by sebhall in March 2023

#ifndef ANGELOCOREFRAGMENT_H
#define ANGELOCOREFRAGMENT_H

const char* AngeloBufferFS = R"(

#version 330 core

out vec4 FragColor;
in vec2 TexCoord;
uniform sampler2D bufferTexture;

void main() {

	FragColor = texture(bufferTexture, TexCoord);

}

)";
								

const char* AngeloCommandFS = R"(

#version 330 core

//  access macros for reading Operations, IDs, and Parameters arrays respectively
#define OPS(i) uint(texelFetch(ops, ivec2(i, 0), 0).r)
#define IDS(i) uint(texelFetch(ids, ivec2(i, 0), 0).r)
#define PMS(i) texelFetch(pms, ivec2(i, 0), 0).r

//  define opcodes
#define CLEAR_COLOUR 	0u
#define DRAW_COLOUR		2u
#define DRAW_WEIGHT		3u

#define LINE_TWOPOINT	10u
#define CIRCLE_STD		11u
#define ARC_STD			12u
#define RECT_STD		13u
#define RECT_ROUND		14u			

//  output vector definition
out vec4 FragColor; 

//  data buffers
uniform usampler2D 	ops;
uniform usampler2D 	ids;
uniform sampler2D 	pms;

uniform int numIndices;


float LineTwoPoint(vec2 p, vec2 a, vec2 b) {
  vec2 pa = p - a;
  vec2 ba = b - a;
  float h = clamp(dot(pa, ba) / dot(ba, ba), 0.0, 1.0);
  return length(pa - ba * h);
}

float RectStd(vec2 p, vec2 a, vec2 b) {
	// Compute the center and size of the rectangle
	vec2 c = (a + b) * 0.5;
	vec2 s = abs(b - a);
	// Transform the point to the rectangle's local space
	p -= c;
	// Mirror the point to the first quadrant
	p = abs(p);
	// Subtract half the size to get the point relative to the border
	p -= s * 0.5;
	// Clamp the point to the positive quadrant
	p = max(p, vec2(0.0));
	// Return the length of the point
	return length(p);
}

float RectRound(vec2 p, vec2 a, vec2 b, float r) {
	// Compute the center and size of the rectangle
	vec2 c = (a + b) * 0.5;
	vec2 s = abs(b - a);
	// Adjust the size to account for the radius
	s -= 2.0 * r;
	// Transform the point to the rectangle's local space
	p -= c;
	// Mirror the point to the first quadrant
	p = abs(p);
	// Subtract half the size and the radius to get the point relative to the border
	p -= s * 0.5 + r;
	// Clamp the point to the positive quadrant
	p = max(p, vec2(0.0));
	// Return the length of the point minus the radius
	return length(p) - r;
}

vec4 VectorMain(vec2 pos) { 

	vec4 drawColour = vec4(1.0, 0.0, 0.0, 1.0);
	vec4 clearColour = vec4(0.0, 0.0, 0.0, 0.0);

	float drawWeight = 2.0;

	ivec2 size = textureSize(ops , 0);

	int i = 0; // Initialize a loop counter
	uint op = OPS(i); // Get the first operation


	while (i < numIndices) { // Loop until end-of-instructions marker or texture size limit
		switch (op) {
			case CLEAR_COLOUR:
				clearColour = vec4(PMS(IDS(i)), PMS(IDS(i) + 1u), PMS(IDS(i) + 2u), PMS(IDS(i) + 3u));
				break;
			case DRAW_COLOUR:
				drawColour = vec4(PMS(IDS(i)), PMS(IDS(i) + 1u), PMS(IDS(i) + 2u), PMS(IDS(i) + 3u));
				break;
			case DRAW_WEIGHT:
				drawWeight = PMS(IDS(i));
				break;
			case LINE_TWOPOINT:
				if (LineTwoPoint(pos, vec2(PMS(IDS(i)), PMS(IDS(i) + 1u)), vec2(PMS(IDS(i) + 2u), PMS(IDS(i) + 3u))) <= drawWeight) {
					return drawColour; // Red for object
				}
				break;
			case RECT_STD:
				if (RectStd(pos, vec2(PMS(IDS(i)), PMS(IDS(i) + 1u)), vec2(PMS(IDS(i) + 2u), PMS(IDS(i) + 3u))) <= 0) {
					return drawColour; // Red for object
				}
				break;
			case RECT_ROUND:
				if (RectRound(pos, vec2(PMS(IDS(i)), PMS(IDS(i) + 1u)), vec2(PMS(IDS(i) + 2u), PMS(IDS(i) + 3u)), PMS(IDS(i) + 4u)) <= 0) {
					return drawColour; // Red for object
				}
				break;
			
		}

		i++; // Increment the loop counter
		op = OPS(i); // Get the next operation
	}

	return clearColour;// Black for background

}

void main() {

	FragColor = VectorMain(gl_FragCoord.xy);
	//FragColor = vec4(1.0, 0.0, 0.0, 1.0);
}
)";

#endif