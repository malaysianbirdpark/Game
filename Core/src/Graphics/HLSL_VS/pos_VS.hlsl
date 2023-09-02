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

cbuffer global_constants : register (b2) {
    matrix vp;
    float4 cam_pos;
    int light_type;
}

VS_OUT main(VS_IN input)
{
    VS_OUT output;

    output.world_pos = mul(input.pos, (float3x3) m);
    output.sv_pos = mul(float4(input.pos, 1.0f), mvp);

	return output;
}