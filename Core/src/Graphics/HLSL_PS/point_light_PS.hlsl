struct PS_IN {
    float3 world_pos : POSITION;
    float3 normal    : NORMAL;
    float2 texcoord  : TEXCOORD;
};

float4 main(PS_IN input) : SV_TARGET
{
    return float4(1.0f, 1.0f, 1.0f, 1.0f);
}