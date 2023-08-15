struct VS_IN {
    float3 pos      : POSITION;
    float3 normal   : NORMAL;
    float2 texcoord : TEXCOORD;
};

struct VS_OUT {
    float4 view_pos : POSITION;
    float3 normal   : NORMAL;
    float2 texcoord : TEXCOORD;
    float4 sv_pos   : SV_POSITION;
};

cbuffer mvp : register(b0)
{
    matrix mv;
    matrix mvit;
    matrix mvp;
};

VS_OUT main(VS_IN input)
{
    VS_OUT output;

    output.view_pos = mul(mv, input.pos);
    output.normal = normalize(mul((float3x3) mvit, input.normal));
    output.texcoord = input.texcoord;
    output.sv_pos = mul(mvp, float4(input.pos, 1.0f));

	return output;
}