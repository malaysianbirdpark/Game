struct OUT {
    float4 color : COLOR;
    float4 pos   : SV_POSITION;
};

cbuffer buf {
    matrix rot;
    matrix scale;
    matrix translate;
};

OUT main(float3 pos : POSITION, float3 color : COLOR)
{
    OUT output;

    output.color = float4(color, 1.0f);
    const matrix transform = scale * translate;
    output.pos = mul(transform, float4(pos, 1.0f));

	return output;
}