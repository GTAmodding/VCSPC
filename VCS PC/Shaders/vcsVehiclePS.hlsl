uniform sampler2D tex : register(s0);
uniform sampler2D refltex : register(s1);

struct PS_INPUT
{
	float2 texcoord0	: TEXCOORD0;
	float2 texcoord1	: TEXCOORD1;
	float4 color		: COLOR0;
	float4 reflcolor	: COLOR1;
};

float4
main(PS_INPUT IN) : COLOR
{
	float4 c1 = tex2D(tex, IN.texcoord0)*IN.color;
	float4 c2 = tex2D(refltex, IN.texcoord1);
	c1.rgb += (c2*IN.reflcolor).rgb;
	return c1;
}
