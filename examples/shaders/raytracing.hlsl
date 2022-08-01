// Vertex shader
float4 vShader(float3 pos : POS_IN) : SV_POSITION {
    return float4(pos, 1.0f);
}

// Pixel shader
struct Sphere {
    float3 center;
    float radius;
    float4 color;
};

struct Plane {
    float3 position;
    float3 normal;
    float3 color;
};

struct Ray {
    float3 origin;
    float3 direction;
	
    bool intersectSphere(Sphere sphere, out float3 outInter) {
        const float3 centerRay = sphere.center - origin;
		
        const float cdDot = dot(centerRay, direction);
        if (cdDot < 0.0f) {
            return false;
        }
		
        const float ccDot = dot(centerRay, centerRay) - cdDot * cdDot;
        const float rr = sphere.radius * sphere.radius;
        if (ccDot > rr) {
            return false;
        }
		
        const float thc = sqrt(rr - ccDot);
        const float dis0 = cdDot - thc;
        const float dis1 = cdDot + thc;
		
        if (dis0 < 0.0f) {
            outInter = origin + direction * dis1;
        }
        else {
            outInter = origin + direction * dis0;
        }
        return true;
    }
	
    bool intersectPlane(Plane plane, out float3 outInter) {
        float denom = dot(normalize(plane.normal), direction);
        if (abs(denom) > 0.0001f) {
            float t = dot(plane.position - origin, plane.normal) / denom;
            if (t >= 0.0f) {
                outInter = origin + direction * t;
                return true;
            }
        }
        return false;
    }
};

#define SPHERE_COUNT 8

cbuffer PS_CB : register(b0) {
    float4 frameSize;
    matrix inverseCamera;
    float4 cameraPosition;
    float4 sunDirection;
    Sphere spheres[SPHERE_COUNT];
};

float3 TraceRay(Ray ray);

float4 pShader(float4 screen : SV_POSITION) : SV_TARGET {
    float2 ndc = float2(screen.x, frameSize.y - screen.y) / float2(frameSize.x, frameSize.y);
    ndc *= 2.0f;
    ndc -= 1.0f;
	
    float4 raydir = mul(float4(ndc, 1.0f, 1.0f), inverseCamera);
    raydir /= raydir.w;
	
    Ray ray = { cameraPosition.xyz, normalize(raydir.xyz) };
    return float4(TraceRay(ray), 1.0f);
}

bool SpherePointInShadow(Plane plane, float3 spherePoint, int sphereIndex) {
    Ray lightRay = { spherePoint, -sunDirection.xyz };
	
    float3 ignoreOut;
	
    if (lightRay.intersectPlane(plane, ignoreOut)) {
        return true;
    }
	
    for (int i = 0; i < SPHERE_COUNT; i++) {
        if (i != sphereIndex && lightRay.intersectSphere(spheres[i], ignoreOut)) {
            return true;
        }
    }
	
    return false;
}

bool PlanePointInShadow(float3 planePoint) {
    Ray lightRay = { planePoint, -sunDirection.xyz };
	
    float3 ignoreOut;
	
    for (int i = 0; i < SPHERE_COUNT; i++) {
        if (lightRay.intersectSphere(spheres[i], ignoreOut)) {
            return true;
        }
    }
	
    return false;
}

float3 TraceRay(Ray ray) {
    Plane xzPlane = { float3(0.0f, 0.0f, 0.0f), float3(0.0f, 1.0f, 0.0f), float3(0.15f, 0.15f, 0.15f) };
	
    float3 intersectPoint;
    float intersectDistance = 3.14e38f;
    int intersectedSphereID = -2;
    float3 tempIntersect;

    if (ray.intersectPlane(xzPlane, tempIntersect)) {
        intersectDistance = length(tempIntersect - cameraPosition.xyz);
        intersectPoint = tempIntersect;
        intersectedSphereID = -1;
    }
	
    for (int i = 0; i < SPHERE_COUNT; i++) {
        if (ray.intersectSphere(spheres[i], tempIntersect)) {
            float tempDistance = length(tempIntersect - cameraPosition.xyz);
            if (tempDistance < intersectDistance) {
                intersectDistance = tempDistance;
                intersectPoint = tempIntersect;
                intersectedSphereID = i;
            }
        }
    }
	
    const float3 skyTopColor = { 0.03f, 0.04f, 0.05f };
    const float3 skyBottomColor = { 0.08f, 0.09f, 0.09f };
    const float3 sunSkyColor = { 0.98f, 0.9f, 0.76f };
	
    const float2 sunRadiuses = { 0.75f, 1.55f };
    const float2 lightRadiuses = { 40.0f, 50.0f };
	
    float3 pixel;
    if (intersectedSphereID == -1) {
        const float lightIntensity = dot(-sunDirection.xyz, xzPlane.normal);
        const float centerDistance = length(intersectPoint);
        const float lightValue = 1.0f - smoothstep(lightRadiuses.x, lightRadiuses.y, centerDistance);
        pixel = xzPlane.color * lightIntensity * lightValue * !PlanePointInShadow(intersectPoint) + skyTopColor;
    }
    else if (intersectedSphereID > -1) {
        const float3 intersectNormal = normalize(intersectPoint - spheres[intersectedSphereID].center);
        const float sunIntensity = dot(-sunDirection.xyz, intersectNormal);
        pixel = spheres[intersectedSphereID].color.xyz * sunIntensity * !SpherePointInShadow(xzPlane, intersectPoint, intersectedSphereID);
    }
    else {
        const float skyMixValue = (dot(-ray.direction, float3(0.0f, 1.0f, 0.0f)) + 1.0f) * 0.5f;
        pixel = lerp(skyTopColor, skyBottomColor, skyMixValue);
		
        const float sunAngle = degrees(acos(dot(ray.direction, -sunDirection.xyz)));
        pixel = lerp(sunSkyColor, pixel, smoothstep(sunRadiuses.x, sunRadiuses.y, sunAngle));
    }
    return pixel;
}
