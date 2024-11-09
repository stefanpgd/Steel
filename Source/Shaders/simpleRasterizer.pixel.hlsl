struct PixelIN
{
    float3 WorldPosition : WorldPosition;
    float3 Normal : Normal;
    float2 TexCoord0 : TexCoord0;
};

Texture2D Diffuse : register(t0);
SamplerState LinearSampler : register(s0);

float4 main(PixelIN IN) : SV_TARGET
{
    float3 albedo = Diffuse.Sample(LinearSampler, IN.TexCoord0).rgb;
    float3 tempDir = float3(-1.0f, 0.0f, 0.0f);
    float diffuse = saturate(dot(IN.Normal, tempDir));
    
    return float4(albedo * diffuse, 1.0f);
}