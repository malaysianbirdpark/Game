struct VS_IN {
    float2 pos      : POSITION;
};

struct VS_OUT {
    float2 texcoord  : TEXCOORD;
    float4 sv_pos    : SV_POSITION;
};

VS_OUT main(VS_IN input)
{
    VS_OUT output;

    output.texcoord = float2((input.pos.x + 1) / 2.0f, -(input.pos.y - 1.0f) / 2.0f);
    output.sv_pos = float4(input.pos, 0.0f, 1.0f);

	return output;
}