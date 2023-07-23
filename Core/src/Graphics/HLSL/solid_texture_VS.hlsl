struct IN {
    float3 pos : POSITION;
    float3 normal : NORMAL;
    float2 texcoord : TEXCOORD;
};

struct OUT {
    float4 color : COLOR;
    float4 pos   : SV_POSITION;
    float2 texcoord : TEXCOORD;
};

cbuffer mvp {
    matrix model;
    matrix view;
    matrix projection;
};

OUT main(IN input)
{
    OUT output;

    const matrix transform = mul(projection, mul(view, model));
    output.pos = mul(transform, float4(input.pos, 1.0f));
    output.texcoord = input.texcoord;

	return output;
}