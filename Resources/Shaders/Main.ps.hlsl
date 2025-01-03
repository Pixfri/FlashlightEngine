struct VSOutput
{
    float4 Position: SV_Position;
    float3 Color: COLOR0;
    float2 Uv: TEXCOORD0;
};

sampler Sampler : register(s0);

Texture2D Texture : register(t0);

float4 Main(VSOutput input): SV_Target
{
    float4 texel = Texture.Sample(Sampler, input.Uv);
    return float4(input.Color, 0.50f) * texel;
}