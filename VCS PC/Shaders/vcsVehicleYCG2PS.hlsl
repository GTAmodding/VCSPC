uniform sampler2D tex : register(s0);
uniform sampler2D refltex : register(s1);

struct PS_INPUT
{
	float2 texcoord0	: TEXCOORD0;
	float2 texcoord1	: TEXCOORD1;
	float4 color		: COLOR0;
	float4 reflcolor	: COLOR1;
};

float4 main(PS_INPUT IN) : COLOR0
{
	float4 ycocg = tex2D(tex, IN.texcoord0);
	float scale = ( ycocg.z * ( 255.0 / 8.0 ) ) + 1.0;
	float Co = ( ycocg.x - ( 0.5 * 256.0 / 255.0 ) ) / scale;
	float Cg = ( ycocg.y - ( 0.5 * 256.0 / 255.0 ) ) / scale;
	float Y = ycocg.w;

	float4 c1 = float4(Y + Co - Cg, Y + Cg, Y - Co - Cg, 1.0f) * IN.color;
	float4 c2 = tex2D(refltex, IN.texcoord1);
	c1.rgb += (c2*IN.reflcolor).rgb;
	return c1;
}
