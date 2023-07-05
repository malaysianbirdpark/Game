struct OUT {
    float4 color : COLOR;
    float4 pos   : SV_POSITION;
};

cbuffer buf {
    matrix model;
    matrix view;
    matrix projection;
};

OUT main(float3 pos : POSITION, float3 normal : NORMAL, float2 tex : TEXCOORD)
{
    OUT output;

    output.color = float4(1.0f, 1.0f, 1.0f, 1.0f);
    output.pos = float4(pos, 1.0f);

	return output;
}