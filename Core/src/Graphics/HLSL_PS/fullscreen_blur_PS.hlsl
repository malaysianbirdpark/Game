Texture2D input_texture : register(t23);
SamplerState sampler1 : register(s1);

static const int loop = 3;
static const int divisor = (2 * loop - 1) * (2 * loop + 1);

float4 main(float2 texcoord : TEXCOORD, float4 sv_pos : SV_POSITION) : SV_TARGET
{
    return saturate(input_texture.Load(sv_pos));
    static uint w, h;
    input_texture.GetDimensions(w, h);
    const float dw = 1.0f / w;
    const float dh = 1.0f / h;
    float4 result = float4(0.0f, 0.0f, 0.0f, 0.0f);
    for (int y = -loop; y != loop; ++y) {
        for (int x = -loop; x != loop; ++x) {
            const float2 tc = texcoord + float2(dw * x, dh * y);
            result += input_texture.Sample(sampler1, tc);
        }
    }
    return result / divisor;
}