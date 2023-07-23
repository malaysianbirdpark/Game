struct IN {
    float3 pos : POSITION;
    float3 normal : NORMAL;
};

struct OUT {
    float4 color : COLOR;
    float4 pos   : SV_POSITION;
};

cbuffer mvp : register(b0) {
    matrix model;
    matrix view;
    matrix projection;
};

OUT main(IN input)
{
    OUT output;

    output.color = float4(input.pos, 1.0f);
    const matrix transform = mul(projection, mul(view, model));
    output.pos = mul(transform, float4(input.pos, 1.0f));

	return output;
}