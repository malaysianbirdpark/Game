struct VS_IN {
    float3 pos      : POSITION;
    float3 normal   : NORMAL;
};

struct VS_OUT {
    float4 world_pos : POSITION;
    float3 normal   : NORMAL;
    float4 sv_pos   : SV_POSITION;
};

cbuffer mvp {
    matrix m;
    matrix mit;
    matrix mvp;
};

VS_OUT main(VS_IN input)
{
    VS_OUT output;

    output.world_pos = mul(m, float4(input.pos, 1.0f));
    output.normal = normalize(mul((float3x3) mit, input.normal));
    output.sv_pos = mul(mvp, float4(input.pos, 1.0f));

	return output;
}