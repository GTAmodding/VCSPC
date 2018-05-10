struct VS_INPUT
{
	float4 Position		: POSITION;
	float3 Normal		: NORMAL;
};

struct VS_OUTPUT
{
	float4 position		: POSITION;
	float2 texcoord0	: TEXCOORD0;
	float4 color		: COLOR0;
};

float4x4	combined	: register(c0);
float4x4	world		: register(c4);
float4		reflProps	: register(c31);

VS_OUTPUT
main(in VS_INPUT In)
{
	VS_OUTPUT Out;

	Out.position = mul(In.Position, combined);
	float3 N = normalize(mul(In.Normal, (float3x3)world).xyz);
	Out.texcoord0 = N.xy*float2(0.5, -0.5) + 0.5;

	Out.color = float4(1, 1, 1, reflProps.x/2);

	return Out;
}
