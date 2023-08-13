struct VS_IN {
    float3 pos : POSITION;
    float3 normal : NORMAL;
};

struct VS_OUT {
    float4 color : COLOR;
    float4 pos   : SV_POSITION;
};

cbuffer mvp : register(b0) {
    matrix mv;
    matrix mvp;
};

VS_OUT main(VS_IN input)
{
    VS_OUT output;

    output.color = float4(1.0f, 1.0f, 1.0f, 1.0f);
    output.pos = mul(mvp, float4(input.pos, 1.0f));

	return output;
}