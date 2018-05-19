


// Per-vertex data used as input to the vertex shader.
struct VertexShaderInput
{
	float3 pos : POSITION;
	float3 size : COLOR1;
	float3 color : COLOR0;
//	uint cloudIndex : COLOR1;
};

// Per-pixel color data passed through the pixel shader.
struct VertexToGeometry
{
	float4 pos : SV_POSITION;
	float3 size : COLOR1;
	float3 color : COLOR0;
	//	uint cloudIndex : COLOR1;
};

// Per-pixel color data passed through the pixel shader.
struct GeometryToPixel
{
	float4 pos : SV_POSITION;
	float3 color : COLOR0;
//	float2 uv : TEXTURE0;
};
