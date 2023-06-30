struct OUT {
    float4 color : COLOR;
    float4 pos   : SV_POSITION;
};

cbuffer buf {
    matrix rot;
};

OUT main(float3 pos : POSITION, float4 color : COLOR)
{
    OUT output;

    output.color = color;
    //output.pos = mul(rot, float4(pos, 1.0f));
    output.pos = float4(pos, 1.0f);

	return output;
}