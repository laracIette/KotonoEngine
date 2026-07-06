#include "common.glsl"

// Helpers
#define UNPACK_PUSH_CONSTANTS                            \
    FrameContext    frame        = pc.frameContext.data; \
    LightBuf        lights       = frame.lights;         \
    uint            lightCount   = frame.lightCount;     \
    ClusterAABBBuf  clusterAABBs = frame.clusterAABBs;   \
    ClusterGridBuf  clusterGrids = frame.clusterGrids;   \
    LightIndexBuf   lightIndices = frame.lightIndices;   \
    LightCounterBuf lightCounter = frame.lightCounter;

vec3 clipToView(mat4 inverseProjection, vec4 clip) {
    vec4 view = inverseProjection * clip;
    return view.xyz / view.w;
}

vec3 lineIntersectionToZPlane(vec3 rayDirection, float zDistance) {
    // Assuming camera is at (0,0,0) in View Space
    float t = zDistance / rayDirection.z;
    return rayDirection * t;
}

bool testSphereAABB(vec3 sphereCenter, float sphereRadius, vec3 aabbMin, vec3 aabbMax) {
    // Find the closest point on the AABB to the sphere center
    vec3 closestPoint = clamp(sphereCenter, aabbMin, aabbMax);
    // Calculate distance from center to this closest point
    vec3 diff = closestPoint - sphereCenter;
    float distSquared = dot(diff, diff);
    // Intersects if distance squared is less than radius squared
    return distSquared <= (sphereRadius * sphereRadius);
}