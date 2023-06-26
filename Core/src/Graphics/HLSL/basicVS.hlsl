struct OUT {
    float4 color : COLOR;
    float4 pos   : SV_POSITION;
};

struct Rotation {
    matrix transform;
};

ConstantBuffer<Rotation> rot : register(b0);

OUT main(float3 pos : POSITION, float4 color : COLOR)
{
    OUT output;

    output.color = color;
    output.pos = mul(rot.transform, float4(pos, 1.0f));

	return output;
}