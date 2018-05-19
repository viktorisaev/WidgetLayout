#include "ShaderData.hlsl"


//float4 AppyViewProject(float4 _position)
//{
//	float4 resultPosition;
//	resultPosition = mul(_position, view);
//	resultPosition = mul(resultPosition, projection);
//
//	return resultPosition;
//}


[maxvertexcount(4)]	// maximun number of output vertices
void main(point VertexToGeometryShaderData input[1], inout TriangleStream<GeometryToPixelShaderData> outputStream)
{
	GeometryToPixelShaderData output = (GeometryToPixelShaderData)0;

	float4 pos = input[0].pos;

	float4 p1 = pos + float4(-0.3f, -0.3f, 0.0f, 0.0f);
	float4 p2 = pos + float4( 0.3f, -0.3f, 0.0f, 0.0f);
	float4 p3 = pos + float4(-0.3f,  0.3f, 0.0f, 0.0f);
	float4 p4 = pos + float4( 0.3f,  0.3f, 0.0f, 0.0f);


	float2 cloudsUV[3] = {
		float2(0.0f, 0.3f),
		float2(0.3f, 0.69f),
		float2(0.69f, 1.0f),
	};



//	int cloudIndex = input[0].cloudIndex;

	float t;
	
	t = clamp(1.0f - p1.zzz * 1.6f, 0.0f, 1.0f);
	output.color = float3(t,t,t);
//	p1 = AppyViewProject(p1);
	output.pos = p1;
//	output.uv = float2(cloudsUV[cloudIndex].y, 1.0f);
	outputStream.Append(output);

	t = clamp(1.0f - p2.zzz* 1.6f, 0.0f, 1.0f);
	output.color = float3(t, t, t);
//	p2 = AppyViewProject(p2);
	output.pos = p2;
//	output.uv = float2(cloudsUV[cloudIndex].x, 1.0f);
	outputStream.Append(output);

	t = clamp(1.0f - p3.zzz* 1.6f, 0.0f, 1.0f);
	output.color = float3(t, t, t);
//	p3 = AppyViewProject(p3);
	output.pos = p3;
//	output.uv = float2(cloudsUV[cloudIndex].y, 0.0f);
	outputStream.Append(output);

	t = clamp(1.0f - p4.zzz* 1.6f, 0.0f, 1.0f);
	output.color = float3(t, t, t);
//	p4 = AppyViewProject(p4);
	output.pos = p4;
//	output.uv = float2(cloudsUV[cloudIndex].x, 0.0f);
	outputStream.Append(output);

	outputStream.RestartStrip();
}





#if 0
// invariant triangle geometry shader
[maxvertexcount(3)]
void main(triangle VertexToGeometryShaderData input[3], inout TriangleStream<GeometryToPixelShaderData> outputStream)
{
	GeometryToPixelShaderData output = (GeometryToPixelShaderData)0;

	for (uint i = 0; i < 3; ++i)
	{
		float4 pos = input[i].pos;

		pos = mul(pos, view);
		pos = mul(pos, projection);
		output.pos = pos;

		output.color = input[i].color;

		outputStream.Append(output);
	}

	outputStream.RestartStrip();
}


// invariant point geometry shader
[maxvertexcount(1)]
void main(point VertexToGeometryShaderData input[1], inout PointStream<GeometryToPixelShaderData> outputStream)
{
	GeometryToPixelShaderData output = (GeometryToPixelShaderData)0;

	float4 pos = input[0].pos;

	pos = mul(pos, view);
	pos = mul(pos, projection);
	output.pos = pos;

	output.color = input[0].color * float3(0.6f, 0.1f, 1.0f);

	outputStream.Append(output);

	outputStream.RestartStrip();
}


// invariant line geometry shader
[maxvertexcount(2)]
void main(line VertexToGeometryShaderData input[2], inout LineStream<GeometryToPixelShaderData> outputStream)
{
	GeometryToPixelShaderData output = (GeometryToPixelShaderData)0;

	for (int i = 0; i < 2; ++i)
	{
		float4 pos = input[i].pos;

		pos = mul(pos, view);
		pos = mul(pos, projection);
		output.pos = pos;

		output.color = input[i].color;

		outputStream.Append(output);
	}

	outputStream.RestartStrip();
}

#endif