#include "ShaderData.hlsl"





VertexToGeometryShaderData main(VertexShaderInput input)
{
	VertexToGeometryShaderData output;
	float4 pos = float4(input.pos, 1.0f);

	// Transform the vertex position into projected space.
//	pos = mul(pos, model);
//	pos = mul(pos, view);
//	pos = mul(pos, projection);
	output.pos = pos;

	// Pass the color through without modification.
	output.color = input.color;

	return output;
}
