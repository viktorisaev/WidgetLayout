
#include "ShaderData.hlsl"




// A pass-through function for the (interpolated) color data.
float4 main(GeometryToPixel input) : SV_TARGET
{
	return float4(input.color);
}
