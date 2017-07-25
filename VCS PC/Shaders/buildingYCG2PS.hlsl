float4 main(uniform sampler2D Tex : register(s0),

				in float2 Tex0 : TEXCOORD0,
				in float4 VertCol : COLOR0) : COLOR0
{
	float4 ycocg = tex2D(Tex, Tex0);
	float scale = ( ycocg.z * ( 255.0 / 8.0 ) ) + 1.0;
	float Co = ( ycocg.x - ( 0.5 * 256.0 / 255.0 ) ) / scale;
	float Cg = ( ycocg.y - ( 0.5 * 256.0 / 255.0 ) ) / scale;
	float Y = ycocg.w;

	return float4(Y + Co - Cg, Y + Cg, Y - Co - Cg, 1.0f) * VertCol * 255.0/128.0;;
}