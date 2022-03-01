#include "KrimzLib/KrimzLib.hpp"


kl::vec3 TraceRay(const kl::ray& ray, const std::vector<kl::sphere>& spheres, int bounce, int maxBounces, const kl::color& background = { 150, 150, 150 }) {
    // Intersected sphere
    const kl::sphere* sphere = nullptr;

    // Intersect point and distance
    kl::vec3 interPoin;
    float interDis = INFINITY;

    // Finding the intersection point
    for (int i = 0; i < spheres.size(); i++) {
        // Data buffers
        kl::vec3 tempInter;
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
                sphere = &spheres[i];
            }
        }
    }

    // Background draw
    if (!sphere) {
        return background;
    }

    // Intersection normal calculation
    kl::vec3 interNorm = (interPoin - sphere->center).normalize();

    // In sphere and bias
    bool insideSph = false;
    const float bias = 1e-4f;

    // Ray and normal direction check
    if (ray.direction.dot(interNorm) > 0.0f) {
        interNorm = interNorm.negate();
        insideSph = true;
    }

    // Transparency and reflection test
    kl::vec3 rayColor;
    if ((sphere->transparency > 0.0f || sphere->reflection > 0.0f) && bounce < maxBounces) {
        // Facing ratio
        const float facingRatio = -ray.direction.dot(interNorm);

        // Fresnel effect
        const float mixRatio = 0.1f;
        const float fresnelEffect = mixRatio + pow(1.0f - facingRatio, 3.0f) * (1.0f - mixRatio);

        // Reflection
        kl::vec3 reflection;
        if (sphere->reflection > 0.0f) {
            kl::vec3 reflDir = (ray.direction - interNorm * 2 * ray.direction.dot(interNorm)).normalize();
            reflection = TraceRay(kl::ray(interPoin + interNorm * bias, reflDir), spheres, bounce + 1, maxBounces);
        }

        // Refraction
        kl::vec3 refraction;
        if (sphere->transparency > 0.0f) {
            const float eta = (insideSph) ? 1.1f : 0.9091f;
            const float cosi = -interNorm.dot(ray.direction);
            const float k = 1.0f - eta * eta * (1.0f - cosi * cosi);
            kl::vec3 refrDir = (ray.direction * eta + interNorm * (eta * cosi - sqrt(k))).normalize();
            refraction = TraceRay(kl::ray(interPoin - interNorm * bias, refrDir), spheres, bounce + 1, maxBounces);
        }

        // Ray color
        rayColor = (reflection * fresnelEffect + refraction * (1 - fresnelEffect) * sphere->transparency) * sphere->surfColor;
    }
    else {
        for (int i = 0; i < spheres.size(); i++) {
            // Light direction           
            kl::vec3 lightDirection = (spheres[i].center - interPoin).normalize();

            // Shadow testing
            bool inShadow = false;
            for (int j = 0; j < spheres.size(); j++) {
                if (i != j) {
                    if (kl::ray(interPoin + interNorm * bias, lightDirection).intersect(spheres[j])) {
                        inShadow = true;
                        break;
                    }
                }
            }

            // Ray color
            rayColor += (sphere->surfColor * spheres[i].emissColor * max(0.0f, interNorm.dot(lightDirection))) * !inShadow;
        }
    }

    // Calculating the pixel color
    kl::vec3 pixel = rayColor + sphere->emissColor;

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

    // Normal spheres
    spheres.push_back(kl::sphere(kl::vec3( 0.0f, -10004.0f, -20.0f), 10000.0f, kl::vec3(0.20f, 0.20f, 0.20f), kl::vec3(0), 0.0f, 0.0f));
    spheres.push_back(kl::sphere(kl::vec3( 0.0f,      0.0f, -20.0f),     4.0f, kl::vec3(1.00f, 0.32f, 0.36f), kl::vec3(0), 0.5f, 1.0f));
    spheres.push_back(kl::sphere(kl::vec3( 5.0f,     -1.0f, -15.0f),     2.0f, kl::vec3(0.90f, 0.76f, 0.46f), kl::vec3(0), 0.0f, 1.0f));
    spheres.push_back(kl::sphere(kl::vec3( 5.0f,      0.0f, -25.0f),     3.0f, kl::vec3(0.65f, 0.77f, 0.97f), kl::vec3(0), 0.0f, 1.0f));
    spheres.push_back(kl::sphere(kl::vec3(-5.5f,      0.0f, -15.0f),     3.0f, kl::vec3(0.90f, 0.90f, 0.90f), kl::vec3(0), 0.0f, 1.0f));

    // Light sphere
    spheres.push_back(kl::sphere(kl::vec3( 0.0f,     20.0f, -30.0f),     3.0f, kl::vec3(0.00f, 0.00f, 0.00f), kl::vec3(3), 0.0f, 0.0f));

    // Image resolution
    const int howManyKs = 16;
    kl::image frame(kl::ivec2(1920, 1080) * (howManyKs / 2));

    // Camera properties
    kl::camera cam;
    cam.fov = 45.0f;
    cam.aspect = float(frame.getWidth()) / frame.getHeight();
    cam.forward = kl::vec3(0.0f, 0.0f, -1.0f);

    // Inverse cam calculation
    const kl::mat4 invCam = cam.matrix().inverse();

    // Prep
    kl::console::hideCursor();
    float threadTimes[4] = {};
    const float pixelsPerThread = frame.getWidth() * frame.getHeight() * 0.25f;

    // Tracing the rays
    kl::timer tim;
    kl::time::interval();
    kl::thread::ParallelFor(0, frame.getHeight(), [&](int t, int y) {
        for (int x = 0; x < frame.getWidth(); x++) {
            // Calculating ndc pixel coordinates
            kl::vec2 ndc = kl::vec2(float(x), float(frame.getHeight() - y)) / kl::vec2(float(frame.getWidth()), float(frame.getHeight()));
            ndc *= 2.0f;
            ndc -= 1.0f;

            // Calculating pixel ray direction
            kl::vec4 raydir = invCam * kl::vec4(ndc, 1.0f, 1.0f);
            raydir /= raydir.w;

            // Tracing the pixel color
            const kl::vec3 pixel = TraceRay(kl::ray(kl::vec3(0), raydir.xyz().normalize()), spheres, 0, 1);

            // Saving the pixel
            frame.setPixel(kl::ivec2(x, y), kl::convert::toColor(pixel));

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
    kl::console::setCursor(kl::ivec2(0, 4));
    std::cout << "Saving..\n";

    // Saving the image
    frame.toFile("ren/raytraced.png");

    // Saving the info
    std::ofstream file("ren/info.txt");
    file << std::fixed << std::setprecision(2);
    file << "Resolution: " << frame.getWidth() << "x" << frame.getHeight() << "\n";
    file << "Pixel count: " << frame.getWidth() * frame.getHeight() << "\n";
    for (int i = 0; i < 4; i++) {
        file << "Thread " << (i + 1) << " time: " << std::setw(6) << threadTimes[i] << " seconds\n";
    }
    file << "Total time: " << totalT << " seconds\n";
    file.close();
}
