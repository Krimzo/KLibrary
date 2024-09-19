#define SPHERE_COUNT 10

struct Sphere
{
    float3 center;
    float radius;
    float4 color;
};

struct Plane
{
    float3 position;
    float3 normal;
    float3 color;
};

struct Ray
{
    float3 origin;
    float3 direction;
	
    bool intersect_sphere(Sphere sphere, out float3 out_inter)
    {
        float3 center_ray = sphere.center - origin;
        float cd_dot = dot(center_ray, direction);
        if (cd_dot < 0.0f)
            return false;
		
        float cc_dot = dot(center_ray, center_ray) - cd_dot * cd_dot;
        float rr = sphere.radius * sphere.radius;
        if (cc_dot > rr)
            return false;
		
        float thc = sqrt(rr - cc_dot);
        float dis0 = cd_dot - thc;
        float dis1 = cd_dot + thc;
        out_inter = origin + direction * (dis0 >= 0.0f ? dis0 : dis1);
        return true;
    }
	
    bool intersect_plane(Plane plane, out float3 out_inter)
    {
        float denom = dot(normalize(plane.normal), direction);
        if (abs(denom) > 0.0001f)
        {
            float t = dot(plane.position - origin, plane.normal) / denom;
            if (t >= 0.0f)
            {
                out_inter = origin + direction * t;
                return true;
            }
        }
        return false;
    }
};

static const Plane XZ_PLANE = { float3(0.0f, 0.0f, 0.0f), float3(0.0f, 1.0f, 0.0f), float3(0.4f, 0.4f, 0.4f) };
static const float3 SKY_TOP = { 0.62f, 0.77f, 0.88f };
static const float3 SKY_BOTTOM = { 0.89f, 0.93f, 0.96f };
static const float3 SUN_COLOR = { 0.98f, 0.9f, 0.76f };
static const float2 SUN_SIZE = { 0.75f, 1.55f };

Sphere SPHERES[SPHERE_COUNT];
float4x4 INVERSE_CAMERA;
float4 FRAME_SIZE;
float4 CAMERA_POSITION;
float4 SUN_DIRECTION;

bool sphere_point_in_shadow(Plane plane, float3 sphere_point, int sphere_index)
{
    Ray light_ray = { sphere_point, -SUN_DIRECTION.xyz };
    
    float3 ignore_out = 0;
    if (light_ray.intersect_plane(plane, ignore_out))
        return true;
	
    for (int i = 0; i < SPHERE_COUNT; i++)
    {
        if (i != sphere_index && light_ray.intersect_sphere(SPHERES[i], ignore_out))
            return true;
    }
    return false;
}

bool plane_point_in_shadow(float3 plane_point)
{
    Ray light_ray = { plane_point, -SUN_DIRECTION.xyz };
	
    float3 ignore_out = 0;
    for (int i = 0; i < SPHERE_COUNT; i++)
    {
        if (light_ray.intersect_sphere(SPHERES[i], ignore_out))
            return true;
    }
    return false;
}

float3 trace_ray(Ray ray)
{
    float3 intersect_point = 0;
    float intersect_distance = 3.14e38f;
    int intersected_sphere_id = -2;
    float3 temp_intersect = 0;

    if (ray.intersect_plane(XZ_PLANE, temp_intersect))
    {
        intersect_distance = length(temp_intersect - CAMERA_POSITION.xyz);
        intersect_point = temp_intersect;
        intersected_sphere_id = -1;
    }
	
    for (int i = 0; i < SPHERE_COUNT; i++)
    {
        if (ray.intersect_sphere(SPHERES[i], temp_intersect))
        {
            float temp_distance = length(temp_intersect - CAMERA_POSITION.xyz);
            if (temp_distance < intersect_distance)
            {
                intersect_distance = temp_distance;
                intersect_point = temp_intersect;
                intersected_sphere_id = i;
            }
        }
    }
	
    float3 pixel;
    if (intersected_sphere_id == -1)
    {
        float light_intensity = saturate(dot(-SUN_DIRECTION.xyz, XZ_PLANE.normal));
        pixel = XZ_PLANE.color * light_intensity * !plane_point_in_shadow(intersect_point);
    }
    else if (intersected_sphere_id > -1)
    {
        float3 intersect_normal = normalize(intersect_point - SPHERES[intersected_sphere_id].center);
        float sun_intensity = saturate(dot(-SUN_DIRECTION.xyz, intersect_normal));
        pixel = SPHERES[intersected_sphere_id].color.xyz * sun_intensity * !sphere_point_in_shadow(XZ_PLANE, intersect_point, intersected_sphere_id);
    }
    else
    {
        float sky_mix_value = (dot(-ray.direction, float3(0.0f, 1.0f, 0.0f)) + 1.0f) * 0.5f;
        pixel = lerp(SKY_TOP, SKY_BOTTOM, sky_mix_value);
        pixel *= saturate(dot(-SUN_DIRECTION.xyz, float3(0.0f, 1.0f, 0.0f)));
		
        float sun_angle = degrees(acos(dot(ray.direction, -SUN_DIRECTION.xyz)));
        pixel = lerp(SUN_COLOR, pixel, smoothstep(SUN_SIZE.x, SUN_SIZE.y, sun_angle));
    }
    return pixel;
}

float4 v_shader(float3 position : KL_Position) : SV_Position
{
    return float4(position, 1.0f);
}

float4 p_shader(float4 position : SV_Position) : SV_Target0
{
    float2 ndc = float2(position.x, FRAME_SIZE.y - position.y) / FRAME_SIZE.xy;
    ndc = ndc * 2.0f - 1.0f;
	
    float4 ray_direction = mul(float4(ndc, 1.0f, 1.0f), INVERSE_CAMERA);
    ray_direction /= ray_direction.w;

    Ray ray = { CAMERA_POSITION.xyz, normalize(ray_direction.xyz) };
    return float4(trace_ray(ray), 1.0f);
}
