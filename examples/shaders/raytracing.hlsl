// Vertex shader
struct vOut {
    float4 screen : SV_POSITION;
};

vOut vShader(float3 pos : POS_IN, float2 tex : TEX_IN, float3 norm : NORM_IN) {
    vOut data;

    data.screen = float4(pos, 1.0f);

    return data;
}

// Pixel shader
class Sphere {
    // Geometry
    float3 center;
    float radius;

    // Light
    float3 color;
    float reflectivity;
    float4 emission;
};

struct Ray {
    float3 origin;
    float3 direction;

    // Intersection with a sphere
    bool intersect(Sphere sphere, out float3 outInter, out float outDis) {
        // Ray sphere center ray
        const float3 centerRay = sphere.center - origin;

        // Center ray and main ray direction dot product
        const float cdDot = dot(centerRay, direction);
        if (cdDot < 0.0f) {
            return false;
        }

        // Calculations I don't understand
        const float ccDot = dot(centerRay, centerRay) - cdDot * cdDot;
        const float rr = sphere.radius * sphere.radius;
        if (ccDot > rr) {
            return false;
        }

        // Intersect distance calculation
        const float thc = sqrt(rr - ccDot);
        const float dis0 = cdDot - thc;
        const float dis1 = cdDot + thc;

        // Origin in sphere test
        if (dis0 < 0.0f) {
            outInter = origin + direction * dis1;
            outDis = dis1;
        }
        else {
            outInter = origin + direction * dis0;
            outDis = dis0;
        }
        return true;
    }
};

#define SPHERE_COUNT 6

cbuffer PS_CB : register(b0) {
    float4 frameSize;
    matrix invCam;
    float4 camPos;
    Sphere spheres[SPHERE_COUNT];
};

float3 TraceRayDefault(Ray ray);
float3 TraceRayDiffuse(Ray ray);

float4 pShader(vOut data) : SV_TARGET {
    float4 pixel;

    // Calculating ndc pixel coordinates
    float2 ndc = float2(data.screen.x, frameSize.y - data.screen.y) / float2(frameSize.x, frameSize.y);
    ndc *= 2.0f;
    ndc -= 1.0f;

    // Calculating pixel ray direction
    float4 raydir = mul(float4(ndc, 1.0f, 1.0f), invCam);
    raydir /= raydir.w;

    // Tracing the pixel color
    Ray ray = { camPos.xyz, normalize(raydir.xyz) };
    pixel = float4(TraceRayDefault(ray), 1.0f);

    return pixel;
}

float3 TraceRayDefault(Ray ray) {
    // Intersected point and sphere
    float3 interPoin;
    int interSphereID = -1;

    // Finding the intersection point
    float interDis = 3.14e38f;
    for (int i = 0; i < SPHERE_COUNT; i++) {
        // Data buffers
        float3 tempInter;
        float tempDis = 0;

        // Intersection test
        if (ray.intersect(spheres[i], tempInter, tempDis)) {
            // Depth test
            if (tempDis < interDis) {
                // Saving the depth
                interDis = tempDis;

                // Saving the point
                interPoin = tempInter;

                // Saving the sphere
                interSphereID = i;
            }
        }
    }

    // Background draw
    if (interSphereID < 0) {
        return 0.6f;
    }

    // Intersection normal calculation
    float3 interNorm = normalize(interPoin - spheres[interSphereID].center);

    // In sphere test
    bool insideSph = false;
    if (dot(ray.direction, interNorm) > 0.0f) {
        interNorm = -interNorm;
        insideSph = true;
    }

    // Reflection test
    float3 rayColor = 0.0f;
    if (spheres[interSphereID].reflectivity > 0.0f) {
        // Facing ratio
        const float facingRatio = dot(-ray.direction, interNorm);

        // Fresnel effect
        const float mixRatio = 0.1f;
        const float fresnelEffect = mixRatio + pow(1.0f - facingRatio, 3.0f) * (1.0f - mixRatio);

        // Reflection
        float3 reflDir = normalize(ray.direction - interNorm * 2 * dot(ray.direction, interNorm));
        Ray reflRay = { interPoin + interNorm, reflDir };
        float3 reflection = TraceRayDiffuse(reflRay);

        // Ray color
        rayColor = (reflection * fresnelEffect) * spheres[interSphereID].color;
    }
    else {
        for (int i = 0; i < SPHERE_COUNT; i++) {
            // Light direction           
            float3 lightDirection = normalize(spheres[i].center - interPoin);
            
            // Shadow testing
            bool inShadow = false;
            for (int j = 0; j < SPHERE_COUNT; j++) {
                if (j != i) {
                    Ray lightRay = { interPoin + interNorm, lightDirection };
                    float3 ignoreOut1; float ignoreOut2;
                    if (lightRay.intersect(spheres[j], ignoreOut1, ignoreOut2)) {
                        inShadow = true;
                        break;
                    }
                }
            }
        
            // Ray color
            rayColor += (spheres[interSphereID].color * spheres[i].emission.xyz * max(0.0f, dot(interNorm, lightDirection))) * !inShadow;
        }
    }

    // Calculating the pixel color
    float3 pixel = rayColor + spheres[interSphereID].emission.xyz;

    // Clamping the color channels
    pixel.x = max(min(pixel.x, 1.0f), 0.0f);
    pixel.y = max(min(pixel.y, 1.0f), 0.0f);
    pixel.z = max(min(pixel.z, 1.0f), 0.0f);

    // Color return
    return pixel;
}
float3 TraceRayDiffuse(Ray ray) {
    // Intersected point and sphere
    float3 interPoin;
    int interSphereID = -1;

    // Finding the intersection point
    float interDis = 3.14e38f;
    for (int i = 0; i < SPHERE_COUNT; i++) {
        // Data buffers
        float3 tempInter;
        float tempDis = 0;

        // Intersection test
        if (ray.intersect(spheres[i], tempInter, tempDis)) {
            // Depth test
            if (tempDis < interDis) {
                // Saving the depth
                interDis = tempDis;

                // Saving the point
                interPoin = tempInter;

                // Saving the sphere
                interSphereID = i;
            }
        }
    }

    // Background draw
    if (interSphereID < 0) {
        return 0.6f;
    }

    // Intersection normal calculation
    float3 interNorm = normalize(interPoin - spheres[interSphereID].center);

    // In sphere test
    bool insideSph = false;
    if (dot(ray.direction, interNorm) > 0.0f) {
        interNorm = -interNorm;
        insideSph = true;
    }

    float3 rayColor = 0.0f;
    for (i = 0; i < SPHERE_COUNT; i++) {
        // Light direction           
        float3 lightDirection = normalize(spheres[i].center - interPoin);

        // Shadow testing
        bool inShadow = false;
        for (int j = 0; j < SPHERE_COUNT; j++) {
            if (j != i) {
                Ray lightRay = { interPoin + interNorm, lightDirection };
                float3 ignoreOut1;
                float ignoreOut2;
                if (lightRay.intersect(spheres[j], ignoreOut1, ignoreOut2)) {
                    inShadow = true;
                    break;
                }
            }
        }

        // Ray color
        rayColor += (spheres[interSphereID].color * spheres[i].emission.xyz * max(0.0f, dot(interNorm, lightDirection))) * !inShadow;
    }

    // Calculating the pixel color
    float3 pixel = rayColor + spheres[interSphereID].emission.xyz;

    // Clamping the color channels
    pixel.x = max(min(pixel.x, 1.0f), 0.0f);
    pixel.y = max(min(pixel.y, 1.0f), 0.0f);
    pixel.z = max(min(pixel.z, 1.0f), 0.0f);

    // Color return
    return pixel;
}
