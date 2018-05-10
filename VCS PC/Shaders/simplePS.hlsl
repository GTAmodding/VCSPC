uniform sampler2D tex : register(s0);

struct PS_INPUT
{
	float2 texcoord0	: TEXCOORD0;
	float4 color		: COLOR0;
};

float4
main(PS_INPUT IN) : COLOR
{
	float4 c1 = tex2D(tex, IN.texcoord0)*IN.color;
	return c1;
}
