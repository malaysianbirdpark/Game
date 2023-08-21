struct VS_IN {
    float3 pos      : POSITION;
    float3 normal   : NORMAL;
    float2 texcoord : TEXCOORD;
};

struct VS_OUT {
    float2 texcoord : TEXCOORD;
    float4 pos      : SV_POSITION;
};

cbuffer mvp : register(b0) {
    matrix m;
    matrix mit;
    matrix mvp;
};

VS_OUT main(VS_IN input)
{
    VS_OUT output;

    output.texcoord = input.texcoord;
    output.pos = mul(mvp, float4(input.pos, 1.0f));

	return output;
}