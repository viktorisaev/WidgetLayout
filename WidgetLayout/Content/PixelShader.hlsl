
#include "ShaderData.hlsl"

Texture2D texDiffuse : register(t0);
SamplerState textureSampler : register(s0);


// A pass-through function for the (interpolated) color data.
float4 main(GeometryToPixel input) : SV_TARGET
{
//	return float4(input.color);

	float4 output;

	// fetch texture
	float4 diffuse = texDiffuse.Sample(textureSampler, input.uv);
	output = diffuse * input.color;

	// fog
	//float3 fogColor = float3(0.392156899f, 0.584313750f, 0.929411829f);
	//float3 res = lerp(backColor, diffuse, input.color.r);
	//output = float4(res, diffuse.a);

	//	return float4(0.7f, 0.1f, 1.0f, 0.5f);	// color
	return output;
}
