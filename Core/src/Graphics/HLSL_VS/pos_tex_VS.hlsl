struct VS_IN {
    float3 pos      : POSITION;
    float2 texcoord : TEXCOORD;
};

struct VS_OUT {
    float4 world_pos : POSITION;
    float2 texcoord  : TEXCOORD;
    float4 sv_pos    : SV_POSITION;
};

cbuffer mvp : register(b0)
{
    matrix m;
    matrix mit;
    matrix mvp;
};

VS_OUT main(VS_IN input)
{
    VS_OUT output;

    output.world_pos = mul(m, float4(input.pos, 1.0f));
    output.texcoord = input.texcoord;
    output.sv_pos = mul(mvp, float4(input.pos, 1.0f));

	return output;
}