#include "ShaderData.hlsl"


// A constant buffer that stores the three basic column-major matrices for composing geometry.
cbuffer ModelViewProjectionConstantBuffer : register(b0)
{
	matrix model;
	matrix view;
	matrix projection;
};



VertexToGeometry main(VertexShaderInput input)
{
	VertexToGeometry output;
	float4 pos = float4(input.pos, 1.0f);

	// Transform the vertex position into projected space.
//	pos = mul(pos, model);
//	pos = mul(pos, view);
//	pos = mul(pos, projection);
	output.pos = pos;

	output.size = input.size;

	// Pass the color through without modification.
	output.color = input.color;

	return output;
}
