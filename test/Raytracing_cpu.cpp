#include "KrimzLib/KrimzLib.hpp"


kl::float3 TraceRay(const kl::ray& ray, const std::vector<kl::sphere>& spheres, int bounce, int maxBounces, const kl::color& background = { 150, 150, 150 }) {
    // Intersected point and sphere
    kl::float3 interPoin;
    const kl::sphere* interSphere = nullptr;

    // Finding the intersection point
    float interDis = INFINITY;
    for (int i = 0; i < spheres.size(); i++) {
        // Data buffers
        kl::float3 tempInter;
        float tempDis = 0;

        // Intersection test
        if (ray.intersect(spheres[i], &tempInter, &tempDis)) {
            // Depth test
            if (tempDis < interDis) {
                // Saving the depth
                interDis = tempDis;

                // Saving the point
                interPoin = tempInter;

                // Saving the sphere
                interSphere = &spheres[i];
            }
        }
    }

    // Background draw
    if (!interSphere) {
        return background;
    }

    // Intersection normal calculation
    kl::float3 interNorm = (interPoin - interSphere->center).normalize();

    // In sphere test
    bool insideSph = false;
    if (ray.direction.dot(interNorm) > 0.0f) {
        interNorm = interNorm.negate();
        insideSph = true;
    }

    // Reflection test
    kl::float3 rayColor;
    if (interSphere->reflectivity > 0.0f && bounce < maxBounces) {
        // Facing ratio
        const float facingRatio = -ray.direction.dot(interNorm);

        // Fresnel effect
        const float mixRatio = 0.1f;
        const float fresnelEffect = mixRatio + pow(1.0f - facingRatio, 3.0f) * (1.0f - mixRatio);

        // Reflection
        kl::float3 reflection;
        if (interSphere->reflectivity > 0.0f) {
            kl::float3 reflDir = (ray.direction - interNorm * 2 * ray.direction.dot(interNorm)).normalize();
            reflection = TraceRay(kl::ray(interPoin + interNorm, reflDir), spheres, bounce + 1, maxBounces);
        }

        // Ray color
        rayColor = (reflection * fresnelEffect) * interSphere->color;
    }
    else {
        for (int i = 0; i < spheres.size(); i++) {
            // Light direction           
            kl::float3 lightDirection = (spheres[i].center - interPoin).normalize();

            // Shadow testing
            bool inShadow = false;
            for (int j = 0; j < spheres.size(); j++) {
                if (i != j) {
                    if (kl::ray(interPoin + interNorm, lightDirection).intersect(spheres[j])) {
                        inShadow = true;
                        break;
                    }
                }
            }

            // Ray color
            rayColor += (interSphere->color * spheres[i].calcEmiss() * max(0.0f, interNorm.dot(lightDirection))) * !inShadow;
        }
    }

    // Calculating the pixel color
    kl::float3 pixel = rayColor + interSphere->calcEmiss();

    // Clamping the color channels
    pixel.x = max(min(pixel.x, 1.0f), 0.0f);
    pixel.y = max(min(pixel.y, 1.0f), 0.0f);
    pixel.z = max(min(pixel.z, 1.0f), 0.0f);

    // Color return
    return pixel;
}

int main() {
    // Sphere buffer
    std::vector<kl::sphere> spheres;

    // Scene spheres
    spheres.push_back(kl::sphere(kl::float3( 0.0f, -5004.0f, 20.0f), 5000.0f, kl::color( 50,  50,  50), 0.0f));
    spheres.push_back(kl::sphere(kl::float3( 0.0f,     0.0f, 20.0f),    4.0f, kl::color(255,  85,  90), 0.8f));
    spheres.push_back(kl::sphere(kl::float3(-5.0f,    -1.0f, 15.0f),    2.0f, kl::color(230, 195, 115), 0.9f));
    spheres.push_back(kl::sphere(kl::float3(-5.0f,     0.0f, 25.0f),    3.0f, kl::color(165, 195, 245), 0.9f));
    spheres.push_back(kl::sphere(kl::float3( 5.5f,     0.0f, 15.0f),    3.0f, kl::color( 30, 230, 230), 0.9f));

    // Light sphere
    spheres.push_back(kl::sphere(kl::float3( 0.0f,    20.0f, 30.0f),    3.0f, kl::color(255, 255, 255), 0.0f, 3.0f));

    // Image resolution
    kl::image frame(kl::int2(1920, 1080) * 4);

    // Camera properties
    kl::camera cam;
    cam.fov = 45.0f;
    cam.aspect = float(frame.width()) / frame.height();

    // Inverse cam calculation
    const kl::mat4 invCam = cam.matrix().inverse();

    // Prep
    kl::console::hideCursor();
    float threadTimes[4] = {};
    const float pixelsPerThread = frame.width() * frame.height() * 0.25f;
    const int bounceLimit = 3;

    // Tracing the rays
    kl::timer tim;
    kl::time::interval();
    kl::thread::parallelFor(0, frame.height(), [&](int t, int y) {
        for (int x = 0; x < frame.width(); x++) {
            // Calculating ndc pixel coordinates
            kl::float2 ndc = kl::float2(float(x), float(frame.height() - y)) / kl::float2(float(frame.width()), float(frame.height()));
            ndc *= 2.0f;
            ndc -= 1.0f;

            // Calculating pixel ray direction
            kl::float4 raydir = invCam * kl::float4(ndc, 1.0f, 1.0f);
            raydir /= raydir.w;

            // Tracing the pixel color
            const kl::float3 pixel = TraceRay(kl::ray(cam.position, raydir.xyz().normalize()), spheres, 0, bounceLimit);

            // Saving the pixel
            frame.spixel(kl::int2(x, y), kl::convert::toColor(pixel));

            // Progress bar
            static std::mutex lock;
            static int counters[4] = {};
            if (++counters[t] % 10000 == 0 || counters[t] == pixelsPerThread) {
                lock.lock();
                kl::console::progressBar("Thread " + std::to_string(t + 1) + " progress", t, counters[t] / pixelsPerThread);
                threadTimes[t] = tim.elapsed();
                lock.unlock();
            }
        }
    });
    const float totalT = kl::time::interval();

    // Echo
    kl::console::setCursor(kl::int2(0, 4));
    std::cout << "Saving..\n";

    // Saving the info
    std::ofstream file("ren/info.txt");
    file << std::fixed << std::setprecision(2);
    file << "Resolution: " << frame.width() << " x " << frame.height() << "\n";
    file << "Pixel count: " << frame.width() * frame.height() << "\n";
    file << "Bounce limit: " << bounceLimit << "\n\n";
    for (int i = 0; i < 4; i++) {
        file << "Thread " << (i + 1) << " time: " << std::setw(6) << threadTimes[i] << " seconds\n";
    }
    file << "Total time: " << totalT << " seconds\n\n";
    for (int i = 0; i < spheres.size(); i++) {
        file << "Sphere " << (i + 1) << " | Center: " << spheres[i].center << " | Radius: " << std::setprecision(2) << std::setw(8) << spheres[i].radius << " | Color: " << spheres[i].color << "\n";
    }
    file.close();

    // Saving the image
    frame.toFile("ren/raytraced.png");
}
