float4x4 gWorldViewProj : WorldViewProjection;

Texture2D gNoiseMap;
Texture2D gColorMap;

float gHeightMultiplier = 25.f;

int gSubLevelVoxels = 96;

SamplerState samLinear
{
	Filter = MIN_MAG_MIP_POINT;
	AddressU = Clamp;// or Mirror or Clamp or Border
	AddressV = Clamp;// or Mirror or Clamp or Border
};
RasterizerState Solid
{
	FillMode = SOLID;
	CullMode = BACK;
};
// ------------------------
// Input/Output structs
// ------------------------
struct VS_INPUT
{
	float3 Position : POSITION;
	float3 Color : COLOR;
	float2 Uv : TEXCOORD;
	float3 InstancePosition : INSTANCEPOS;
	float4 InstanceLengthVOne : INSTANCELENGTHVONE;
	float4 InstanceLengthVTwo : INSTANCELENGTHVTWO;
};
struct VS_OUTPUT
{
	float4 Position : SV_POSITION;			// SV = system variable
	float3 Color : COLOR;
	float2 Uv : TEXCOORD;
	float3 InstancePosition : INSTANCEPOS;
	float4 InstanceLengthVOne : INSTANCELENGTHVONE;
	float4 InstanceLengthVTwo : INSTANCELENGTHVTWO;
};

// ------------------------
// Vertex shader
// ------------------------
VS_OUTPUT VS(VS_INPUT input)
{
	VS_OUTPUT output = (VS_OUTPUT)0;
	float4 noiseHeight = gNoiseMap.SampleLevel(samLinear, input.Uv, 0);
	float height    = pow(noiseHeight.y, 3.9f);
	input.Position  = float3(input.Position.x, input.Position.y + (height * gHeightMultiplier), input.Position.z);
	output.Position = mul(float4(input.Position, 1.0f), gWorldViewProj);
	output.Color    = input.Color;
	output.Uv		= input.Uv;
	
	return output;
}

//**********************************//
//         GEOMETRY SHADER          //
//**********************************//
void CreateVertex(inout TriangleStream<VS_OUTPUT> triStream, float3 pos, float3 color, float2 uv)
{
	VS_OUTPUT data = (VS_OUTPUT)0;
	data.Position = mul(float4(pos, 1), gWorldViewProj);
	data.Color = color;
	data.Uv = uv;
	triStream.Append(data);
}

void CreateFrontFace(VS_OUTPUT vert, float mod, float heightDiff, inout TriangleStream<VS_OUTPUT> triStream)
{
	float3 posLT = (float3)0;
	float3 posRT = (float3)0;
	float3 posLB = (float3)0;
	float3 posRB = (float3)0;

	if (heightDiff != 0.f)
	{
		float diffAbove0 = 0 - (vert.Position.y - mod);
		heightDiff = diffAbove0 + abs(heightDiff);
	}

	posLT = float3(vert.Position.x - mod, vert.Position.y + mod			    , vert.Position.z - mod);
	posRT = float3(vert.Position.x + mod, vert.Position.y + mod			    , vert.Position.z - mod);
	posLB = float3(vert.Position.x - mod, vert.Position.y - mod - heightDiff, vert.Position.z - mod);
	posRB = float3(vert.Position.x + mod, vert.Position.y - mod - heightDiff, vert.Position.z - mod);

	CreateVertex(triStream, posLT, vert.Color, vert.Uv);	// LT
	CreateVertex(triStream, posRT, vert.Color, vert.Uv);	// RT
	CreateVertex(triStream, posLB, vert.Color, vert.Uv);	// LB
	CreateVertex(triStream, posRB, vert.Color, vert.Uv);	// RB
}
void CreateLeftFace(VS_OUTPUT vert, float mod, float heightDiff, inout TriangleStream<VS_OUTPUT> triStream)
{
	float3 posLT = (float3)0;
	float3 posRT = (float3)0;
	float3 posLB = (float3)0;
	float3 posRB = (float3)0;

	if (heightDiff != 0.f)
	{
		float diffAbove0 = 0 - (vert.Position.y - mod);
		heightDiff = diffAbove0 + abs(heightDiff);
	}

	posLT = float3(vert.Position.x - mod, vert.Position.y + mod, vert.Position.z + mod);
	posRT = float3(vert.Position.x - mod, vert.Position.y + mod, vert.Position.z - mod);
	posLB = float3(vert.Position.x - mod, vert.Position.y - mod - heightDiff, vert.Position.z + mod);
	posRB = float3(vert.Position.x - mod, vert.Position.y - mod - heightDiff, vert.Position.z - mod);

	CreateVertex(triStream, posLT, vert.Color, vert.Uv);	// LT
	CreateVertex(triStream, posRT, vert.Color, vert.Uv);	// RT
	CreateVertex(triStream, posLB, vert.Color, vert.Uv);	// LB
	CreateVertex(triStream, posRB, vert.Color, vert.Uv);	// RB
}
void CreateRightFace(VS_OUTPUT vert, float mod, float heightDiff, inout TriangleStream<VS_OUTPUT> triStream)
{
	float3 posLT = (float3)0;
	float3 posRT = (float3)0;
	float3 posLB = (float3)0;
	float3 posRB = (float3)0;

	if (heightDiff != 0.f)
	{
		float diffAbove0 = 0 - (vert.Position.y - mod);
		heightDiff = diffAbove0 + abs(heightDiff);
	}

	posLT = float3(vert.Position.x + mod, vert.Position.y + mod, vert.Position.z - mod);
	posRT = float3(vert.Position.x + mod, vert.Position.y + mod, vert.Position.z + mod);
	posLB = float3(vert.Position.x + mod, vert.Position.y - mod - heightDiff, vert.Position.z - mod);
	posRB = float3(vert.Position.x + mod, vert.Position.y - mod - heightDiff, vert.Position.z + mod);

	CreateVertex(triStream, posLT, vert.Color, vert.Uv);	// LT
	CreateVertex(triStream, posRT, vert.Color, vert.Uv);	// RT
	CreateVertex(triStream, posLB, vert.Color, vert.Uv);	// LB
	CreateVertex(triStream, posRB, vert.Color, vert.Uv);	// RB

}
void CreateBottomFace(VS_OUTPUT vert, float mod, float heightDiff, inout TriangleStream<VS_OUTPUT> triStream)
{
	float3 posLT = (float3)0;
	float3 posRT = (float3)0;
	float3 posLB = (float3)0;
	float3 posRB = (float3)0;

	if (heightDiff != 0.f)
	{
		float diffAbove0 = 0 - (vert.Position.y - mod);
		heightDiff = diffAbove0 + abs(heightDiff);
	}

	posLT = float3(vert.Position.x - mod, vert.Position.y - mod - heightDiff, vert.Position.z - mod);
	posRT = float3(vert.Position.x + mod, vert.Position.y - mod - heightDiff, vert.Position.z - mod);
	posLB = float3(vert.Position.x - mod, vert.Position.y - mod - heightDiff, vert.Position.z + mod);
	posRB = float3(vert.Position.x + mod, vert.Position.y - mod - heightDiff, vert.Position.z + mod);

	CreateVertex(triStream, posLT, vert.Color, vert.Uv);	// LT
	CreateVertex(triStream, posRT, vert.Color, vert.Uv);	// RT
	CreateVertex(triStream, posLB, vert.Color, vert.Uv);	// LB
	CreateVertex(triStream, posRB, vert.Color, vert.Uv);	// RB
}
void CreateTopFace(VS_OUTPUT vert, float mod, float heightDiff, inout TriangleStream<VS_OUTPUT> triStream)
{
	float3 posLT = (float3)0;
	float3 posRT = (float3)0;
	float3 posLB = (float3)0;
	float3 posRB = (float3)0;

	if (heightDiff != 0.f)
	{
		float diffAbove0 = 0 - (vert.Position.y - mod);
		heightDiff = diffAbove0 + abs(heightDiff);
	}

	posLT = float3(vert.Position.x - mod, vert.Position.y + mod, vert.Position.z + mod);
	posRT = float3(vert.Position.x + mod, vert.Position.y + mod, vert.Position.z + mod);
	posLB = float3(vert.Position.x - mod, vert.Position.y + mod, vert.Position.z - mod);
	posRB = float3(vert.Position.x + mod, vert.Position.y + mod, vert.Position.z - mod);

	CreateVertex(triStream, posLT, vert.Color, vert.Uv);	// LT
	CreateVertex(triStream, posRT, vert.Color, vert.Uv);	// RT
	CreateVertex(triStream, posLB, vert.Color, vert.Uv);	// LB
	CreateVertex(triStream, posRB, vert.Color, vert.Uv);	// RB
}
void CreateBackFace(VS_OUTPUT vert, float mod, float heightDiff, inout TriangleStream<VS_OUTPUT> triStream)
{
	float3 posLT = (float3)0;
	float3 posRT = (float3)0;
	float3 posLB = (float3)0;
	float3 posRB = (float3)0;

	if (heightDiff != 0.f)
	{
		float diffAbove0 = 0 - (vert.Position.y - mod);
		heightDiff = diffAbove0 + abs(heightDiff);
	}

	posLT = float3(vert.Position.x + mod, vert.Position.y + mod, vert.Position.z + mod);
	posRT = float3(vert.Position.x - mod, vert.Position.y + mod, vert.Position.z + mod);
	posLB = float3(vert.Position.x + mod, vert.Position.y - mod - heightDiff, vert.Position.z + mod);
	posRB = float3(vert.Position.x - mod, vert.Position.y - mod - heightDiff, vert.Position.z + mod);

	CreateVertex(triStream, posLT, vert.Color, vert.Uv);	// LT
	CreateVertex(triStream, posRT, vert.Color, vert.Uv);	// RT
	CreateVertex(triStream, posLB, vert.Color, vert.Uv);	// LB
	CreateVertex(triStream, posRB, vert.Color, vert.Uv);	// RB
}

void CreateVoxel(VS_OUTPUT vert, float mod, inout TriangleStream<VS_OUTPUT> triStream, float heightDiff)
{
	CreateFrontFace(vert, mod, heightDiff, triStream);
	CreateLeftFace(vert, mod, heightDiff, triStream);
	CreateRightFace(vert, mod, heightDiff, triStream);
	CreateBottomFace(vert, mod, heightDiff, triStream);
	CreateTopFace(vert, mod, heightDiff, triStream);
	CreateBackFace(vert, mod, heightDiff, triStream);
}

[maxvertexcount(48)]
void GS(point VS_OUTPUT vertices[1], inout TriangleStream<VS_OUTPUT> triStream)
{
	float modifier = 0.5f;
	float heightDiff = -40.f;

	// Top layer
	CreateVoxel(vertices[0], modifier, triStream, 0.f);
	// Bottom layer
	CreateVoxel(vertices[0], modifier, triStream, heightDiff);
}

// ------------------------
// Pixel shader
// ------------------------
float4 PS(VS_OUTPUT input) : SV_TARGET
{
	return gColorMap.Sample(samLinear, input.Uv);
}

// ------------------------
// Technique
// ------------------------
technique11 DefaultTechnique
{
	pass P0
	{
		SetVertexShader(CompileShader(vs_5_0, VS()));
		SetGeometryShader(CompileShader(gs_5_0, GS()));
		SetPixelShader(CompileShader(ps_5_0, PS()));
	}
}