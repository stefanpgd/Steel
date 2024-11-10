Texture2D sceneRender : register(t0);
RWTexture2D<float4> shadowMask : register(u0);
RWTexture2D<float4> outputBuffer : register(u1);

[numthreads(8, 8, 1)]
void main(uint3 dispatchThreadID : SV_DispatchThreadID)
{
    return;
}