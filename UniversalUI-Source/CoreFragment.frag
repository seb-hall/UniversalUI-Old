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


float LineDistance(vec2 p, vec2 a, vec2 b) {
  vec2 pa = p - a;
  vec2 ba = b - a;
  float h = clamp(dot(pa, ba) / dot(ba, ba), 0.0, 1.0);
  return length(pa - ba * h);
}

vec4 VectorMain(vec2 pos) { 

	vec4 drawColour = vec4(1.0, 0.0, 0.0, 1.0);
	vec4 clearColour = vec4(0.0, 0.0, 0.0, 1.0);

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
				//if (sdLine(pos, vec2(50.0, 50.0), vec2(150.0, 50.0)) <= drawWeight) {
					return vec4(0.0, 0.0, 0.0, 1.0); // Red for object
				//}
				break;
		}

		i++; // Increment the loop counter
		op = OPS(i); // Get the next operation
	}

	return drawColour;// Black for background

}

void main() {

	FragColor = VectorMain(gl_FragCoord.xy);
}