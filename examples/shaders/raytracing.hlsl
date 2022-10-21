// Vertex shader
float4 v_shader(float3 pos : KL_Position) : SV_Position
{
    return float4(pos, 1.0f);
}

// Pixel shader
struct colored_sphere
{
    float3 center;
    float radius;
    float4 color;
};

struct plane
{
    float3 position;
    float3 normal;
    float3 color;
};

struct ray
{
    float3 origin;
    float3 direction;
	
    bool intersect_sphere(const colored_sphere sphere, out float3 out_inter)
    {
        const float3 center_ray = sphere.center - origin;
		
        const float cd_dot = dot(center_ray, direction);
        if (cd_dot < 0.0f) {
            return false;
        }
		
        const float cc_dot = dot(center_ray, center_ray) - cd_dot * cd_dot;
        const float rr = sphere.radius * sphere.radius;
        if (cc_dot > rr) {
            return false;
        }
		
        const float thc = sqrt(rr - cc_dot);
        const float dis0 = cd_dot - thc;
        const float dis1 = cd_dot + thc;
		
        if (dis0 < 0.0f) {
            out_inter = origin + direction * dis1;
        }
        else {
            out_inter = origin + direction * dis0;
        }
        return true;
    }
	
    bool intersect_plane(const plane plane, out float3 out_inter)
    {
        const float denom = dot(normalize(plane.normal), direction);
        if (abs(denom) > 0.0001f) {
            const float t = dot(plane.position - origin, plane.normal) / denom;
            if (t >= 0.0f) {
                out_inter = origin + direction * t;
                return true;
            }
        }
        return false;
    }
};

#define SPHERE_COUNT 10

cbuffer PS_CB : register(b0)
{
    float4 frame_size;
    matrix inverse_camera;
    float4 camera_position;
    float4 sun_direction;
    colored_sphere spheres[SPHERE_COUNT];
};

float3 trace_ray(ray ray);

float4 p_shader(float4 screen : SV_Position) : SV_Target
{
    float2 ndc = float2(screen.x, frame_size.y - screen.y) / float2(frame_size.x, frame_size.y);
    ndc *= 2.0f;
    ndc -= 1.0f;
	
    float4 ray_direction = mul(float4(ndc, 1.0f, 1.0f), inverse_camera);
    ray_direction /= ray_direction.w;

    const ray ray = { camera_position.xyz, normalize(ray_direction.xyz) };
    return float4(trace_ray(ray), 1.0f);
}

bool sphere_point_in_shadow(const plane plane, const float3 sphere_point, const int sphere_index)
{
    ray light_ray = { sphere_point, -sun_direction.xyz };
	
    float3 ignore_out = 0;
	
    if (light_ray.intersect_plane(plane, ignore_out)) {
        return true;
    }
	
    for (int i = 0; i < SPHERE_COUNT; i++) {
        if (i != sphere_index && light_ray.intersect_sphere(spheres[i], ignore_out)) {
            return true;
        }
    }
	
    return false;
}

bool plane_point_in_shadow(const float3 plane_point)
{
    ray light_ray = { plane_point, -sun_direction.xyz };
	
    float3 ignore_out = 0;
	
    for (int i = 0; i < SPHERE_COUNT; i++) {
        if (light_ray.intersect_sphere(spheres[i], ignore_out)) {
            return true;
        }
    }
	
    return false;
}

static const plane xz_plane = { float3(0.0f, 0.0f, 0.0f), float3(0.0f, 1.0f, 0.0f), float3(0.4f, 0.4f, 0.4f) };
static const float3 sky_top_color = { 0.62f, 0.77f, 0.88f };
static const float3 sky_bottom_color = { 0.89f, 0.93f, 0.96f };
static const float3 sun_sky_color = { 0.98f, 0.9f, 0.76f };
static const float2 sun_size = { 0.75f, 1.55f };

float3 trace_ray(ray ray)
{
    float3 intersect_point = 0;
    float intersect_distance = 3.14e38f;
    int intersected_sphere_id = -2;
    float3 temp_intersect = 0;

    if (ray.intersect_plane(xz_plane, temp_intersect)) {
        intersect_distance = length(temp_intersect - camera_position.xyz);
        intersect_point = temp_intersect;
        intersected_sphere_id = -1;
    }
	
    for (int i = 0; i < SPHERE_COUNT; i++) {
        if (ray.intersect_sphere(spheres[i], temp_intersect)) {
            const float temp_distance = length(temp_intersect - camera_position.xyz);
            if (temp_distance < intersect_distance) {
                intersect_distance = temp_distance;
                intersect_point = temp_intersect;
                intersected_sphere_id = i;
            }
        }
    }
	
    float3 pixel;
    if (intersected_sphere_id == -1) {
        const float light_intensity = dot(-sun_direction.xyz, xz_plane.normal);
        pixel = xz_plane.color * light_intensity * !plane_point_in_shadow(intersect_point);
    }
    else if (intersected_sphere_id > -1) {
        const float3 intersect_normal = normalize(intersect_point - spheres[intersected_sphere_id].center);
        const float sun_intensity = dot(-sun_direction.xyz, intersect_normal);
        pixel = spheres[intersected_sphere_id].color.xyz * sun_intensity * !sphere_point_in_shadow(xz_plane, intersect_point, intersected_sphere_id);
    }
    else {
        const float sky_mix_value = (dot(-ray.direction, float3(0.0f, 1.0f, 0.0f)) + 1.0f) * 0.5f;
        pixel = lerp(sky_top_color, sky_bottom_color, sky_mix_value);
        pixel *= dot(-sun_direction.xyz, float3(0.0f, 1.0f, 0.0f));
		
        const float sun_angle = degrees(acos(dot(ray.direction, -sun_direction.xyz)));
        pixel = lerp(sun_sky_color, pixel, smoothstep(sun_size.x, sun_size.y, sun_angle));
    }
    
    return pixel;
}
