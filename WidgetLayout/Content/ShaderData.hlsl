


// Per-vertex data used as input to the vertex shader.
struct VertexShaderInput
{
	float3 pos : POSITION;
	float3 size : COLOR1;
	float4 color : COLOR0;
	uint textureIndex : COLOR2;
};

// Per-pixel color data passed through the pixel shader.
struct VertexToGeometry
{
	float4 pos : SV_POSITION;
	float3 size : COLOR1;
	float4 color : COLOR0;
	uint textureIndex : COLOR2;
};

// Per-pixel color data passed through the pixel shader.
struct GeometryToPixel
{
	float4 pos : SV_POSITION;
	float4 color : COLOR0;
	uint textureIndex : COLOR1;
	float2 uv : TEXTURE0;
};
