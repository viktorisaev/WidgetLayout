
#include "ShaderData.hlsl"




// A pass-through function for the (interpolated) color data.
float4 main(GeometryToPixelShaderData input) : SV_TARGET
{
	return float4(input.color, 1.0f);
}
