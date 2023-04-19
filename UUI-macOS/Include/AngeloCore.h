//
//  AngeloCore.metal
//  UniversalUI
//
//  Created by Seb on 18/04/2023.
//

const char* AngeloCore = R"(
#include <metal_stdlib>
using namespace metal;

struct InputData {
	float2 pos;
	float2 normalPos;
};

struct ShaderData {
	float4 pos [[position]];
	float4 col;
};

vertex ShaderData AngeloCommandVS(uint vertexID [[vertex_id]], constant InputData* vertices[[buffer(0)]]) {
	
	ShaderData out;
	
	out.pos = float4(0.0, 0.0, 0.0, 1.0);
	out.pos.xy = vertices[vertexID].pos;
	
	out.col = float4(0.0, 0.0, 1.0, 1.0);
	
	return out;
}


fragment float4 AngeloCommandFS(ShaderData in [[stage_in]]) {
	// Return the interpolated color.
	return in.col;
}

vertex ShaderData AngeloBufferVS(uint vertexID [[vertex_id]], constant InputData* vertices[[buffer(0)]]) {
	
	ShaderData out;
	
	out.pos = float4(0.0, 0.0, 0.0, 1.0);
	out.pos.xy = vertices[vertexID].pos;
	
	out.col = float4(0.0, 0.0, 1.0, 1.0);
	
	return out;
}


fragment float4 AngeloBufferFS(ShaderData in [[stage_in]]) {
	// Return the interpolated color.
	return float4(1.0, 0.0, 1.0, 1.0);
}

)";
