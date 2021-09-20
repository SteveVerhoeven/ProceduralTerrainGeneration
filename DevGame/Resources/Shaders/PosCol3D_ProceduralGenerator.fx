float4x4 gWorldViewProj : WorldViewProjection;

Texture2D gNoiseMap;
Texture2D gColorMap;

float gHeightMultiplier = 25.f;

SamplerState samLinear
{
	Filter = ANISOTROPIC;
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
};
struct VS_OUTPUT
{
	float4 Position : SV_POSITION;			// SV = system variable
	float3 Color : COLOR;
	float2 Uv : TEXCOORD0;
};

// ------------------------
// Vertex shader
// ------------------------
VS_OUTPUT VS(VS_INPUT input)
{
	VS_OUTPUT output = (VS_OUTPUT)0;
	float4 noiseHeight = gNoiseMap.SampleLevel(samLinear, input.Uv, 0);
	float height = pow(noiseHeight.y, 3.9f);
	input.Position = float3(input.Position.x, height * gHeightMultiplier, input.Position.z);
	output.Position = mul(float4(input.Position, 1.0f), gWorldViewProj);
	output.Color    = input.Color;
	output.Uv		= input.Uv;
	
	return output;
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
		SetVertexShader( CompileShader(vs_5_0, VS()) );
		SetGeometryShader( NULL );
		SetPixelShader( CompileShader(ps_5_0, PS()) );
	}
}