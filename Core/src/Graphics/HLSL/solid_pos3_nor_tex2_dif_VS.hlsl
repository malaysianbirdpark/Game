struct IN {
    float3 pos      : POSITION;
    float3 normal   : NORMAL;
    float2 texcoord : TEXCOORD;
};

struct OUT {
    float4 view_pos : POSITION;
    float2 texcoord : TEXCOORD;
    float4 sv_pos   : SV_POSITION;
};

cbuffer mvp {
    matrix mv;
    matrix mvp;
};

OUT main(IN input)
{
    OUT output;

    output.view_pos = mul(mv, input.pos);
    output.texcoord = input.texcoord;
    output.sv_pos = mul(mvp, float4(input.pos, 1.0f));

	return output;
}