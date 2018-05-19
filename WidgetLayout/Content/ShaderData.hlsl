// A constant buffer that stores the three basic column-major matrices for composing geometry.
cbuffer ModelViewProjectionConstantBuffer : register(b0)
{
	matrix model;
	matrix view;
	matrix projection;
};



// Per-vertex data used as input to the vertex shader.
struct VertexShaderInput
{
	float3 pos : POSITION;
	float3 color : COLOR0;
//	uint cloudIndex : COLOR1;
};

// Per-pixel color data passed through the pixel shader.
struct VertexToGeometryShaderData
{
	float4 pos : SV_POSITION;
	float3 color : COLOR0;
//	uint cloudIndex : COLOR1;
};

// Per-pixel color data passed through the pixel shader.
struct GeometryToPixelShaderData
{
	float4 pos : SV_POSITION;
	float3 color : COLOR0;
//	float2 uv : TEXTURE0;
};
