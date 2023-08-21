struct VS_IN {
    float3 pos      : POSITION;
};

struct VS_OUT {
    float3 pos      : POSITION;
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

    output.pos = mul(m, float4(input.pos, 1.0f));
    output.sv_pos = mul(mvp, float4(input.pos, 1.0f));

	return output;
}