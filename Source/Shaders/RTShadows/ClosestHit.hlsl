#include "Common.hlsl"

struct Vertex
{
    float3 position;
    float3 normal;
    float3 tangent;
    float2 texCoord0;
};
StructuredBuffer<Vertex> VertexData : register(t0);
StructuredBuffer<int> indices : register(t1);
RaytracingAccelerationStructure SceneBVH : register(t2);

[shader("closesthit")]
void ClosestHit(inout HitInfo payload, Attributes attrib)
{
    payload.color = 0.5;
    return;
}