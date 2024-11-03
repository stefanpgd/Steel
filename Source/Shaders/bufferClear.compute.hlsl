RWTexture2D<float4> targetTexture : register(u0);

[numthreads(1, 1, 1)]
void main(uint3 dispatchThreadID : SV_DispatchThreadID)
{
    int width;
    int height;
    targetTexture.GetDimensions(width, height);
    
    if(dispatchThreadID.x > width || dispatchThreadID.y > height)
    {
        return;
    }
    
    targetTexture[dispatchThreadID.xy] = float4(0.0f, 0.0f, 0.0f, 1.0f);
}