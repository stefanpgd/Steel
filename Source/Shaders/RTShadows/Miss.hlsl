#include "Common.hlsl"

[shader("miss")]
void Miss(inout HitInfo payload : SV_RayPayload)
{
    payload.color = float3(1.0, 1.0, 1.0);
    payload.depth = 0.0f;
}