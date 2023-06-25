struct OUT {
    float4 color : COLOR;
    float4 pos   : SV_POSITION;
};

OUT main(float3 pos : POSITION, float4 color : COLOR)
{
    OUT output;

    output.color = color;
    output.pos = float4(pos, 1.0f);

	return output;
}