struct VS_IN {
    float3 pos      : POSITION;
};

struct VS_OUT {
    float3 world_pos : POSITION;
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

    output.world_pos = mul(m, float4(input.pos, 1.0f)).xyz;
    output.sv_pos = mul(mvp, float4(input.pos, 1.0f));

	return output;
}