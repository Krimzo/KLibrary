#include "klib.h"


struct scene_object
{
    float radius = kl::random::get_float(10.0f, 20.0f);
    kl::float2 position = {};
    kl::float2 velocity = {};
    kl::color color = kl::random::get_color();
    std::vector<kl::float2> position_history = {};
};

static float get_frame_lower_limit(const kl::image& frame)
{
    return frame.height() * 0.9f;
}

static void draw_background(kl::image& frame)
{
    float lower_limit = get_frame_lower_limit(frame);
    frame.fill({ 60, 60, 60 });
    frame.draw_rectangle({ 0, (int) lower_limit }, frame.size(), kl::colors::gray, true);
    frame.draw_line({ 0, (int) lower_limit }, { frame.width(), (int) lower_limit }, { 30, 30, 30 });
}

static void draw_objects(kl::image& frame, std::vector<scene_object>& objects)
{
    for (auto& [radius, position, velocity, color, position_history] : objects) {
        position_history.push_back(position);
        int start_position_index = int(position_history.size() * 0.75f);
        for (int i = start_position_index; i < int(position_history.size()) - 1; i++) {
            frame.draw_line(kl::int2(position_history[i]), kl::int2(position_history[i + 1]),
                kl::color(60, 60, 60).mix(color, kl::math::normalize(i, start_position_index, position_history.size())));
        }
        frame.draw_circle(kl::int2(position), radius, color, true);
        frame.draw_circle(kl::int2(position), radius, { 30, 30, 30 });
    }
}

static void draw_interface(kl::image& frame, const bool draw_arrow, kl::float2 arrow_start, kl::float2 arrow_end)
{
    if (draw_arrow) {
        for (int i = -1; i <= 1; i++) {
            const kl::float2 adder = { (float) i, (float) i };
            frame.draw_line(kl::int2(arrow_start + adder), kl::int2(arrow_end + adder), kl::colors::sky);
        }
    }
}

static void process_objects(std::vector<scene_object>& objects, const kl::timer& timer, const kl::image& frame, const kl::float2& gravity)
{
    static constexpr float energy_retain = 0.8f;

    for (auto& [radius, position, velocity, color, position_history] : objects) {
        velocity += gravity * timer.get_interval();
        position += velocity * timer.get_interval();
    }

    for (int i = 0; i < int(objects.size()); i++) {
        int intersect_count = 0;
        kl::float2 velocity_sum, position_sum;
        for (int j = 0; j < int(objects.size()); j++) {
            if (i != j) {
                kl::float2 positions_vector = objects[i].position - objects[j].position;
                if (float radius_sum = objects[i].radius + objects[j].radius; positions_vector.length() < radius_sum) {
                    position_sum += objects[j].position + kl::math::normalize(positions_vector) * radius_sum;
                    velocity_sum += kl::math::reflect(objects[i].velocity, objects[j].position - objects[i].position);
                    intersect_count += 1;
                }
            }
        }
        if (intersect_count > 0) {
            const float count_rec = 1.0f / intersect_count;
            objects[i].position = position_sum * count_rec;
            objects[i].velocity = velocity_sum * count_rec * energy_retain;
        }
    }

    for (auto& [radius, position, velocity, color, position_history] : objects) {
        if (position.x - radius < 0) {
            position.x = radius;
            velocity.x *= -1.0f;
            velocity *= energy_retain;
        }
        if (position.x + radius > frame.width()) {
            position.x = frame.width() - radius;
            velocity.x *= -1.0f;
            velocity *= energy_retain;
        }
        if (position.y - radius < 0) {
            position.y = radius;
            velocity.y *= -1.0f;
            velocity *= energy_retain;
        }
        if (position.y + radius > get_frame_lower_limit(frame)) {
            position.y = get_frame_lower_limit(frame) - radius;
            velocity.y *= -1.0f;
            velocity *= energy_retain;
        }
    }
}

int main()
{
    auto window = kl::window({ 1600, 900 }, "Trajectories");
    kl::image frame = kl::image(window.size());
    kl::timer timer = {};

    const kl::float2 gravity = { 0, 98.1f };
    std::vector<scene_object> objects = {};

    window.on_resize.push_back([&](const kl::int2 new_size)
    {
        frame.resize(new_size);
    });

    bool object_being_added = false;
    kl::float2 object_add_position = {};
    kl::float2 object_second_position = {};
    window.mouse.left.on_press.push_back([&]()
    {
        const kl::int2 position = window.mouse.position();
        if (position.x >= 0 && position.x < (int) window.width() && position.y > 0 && position.y < get_frame_lower_limit(frame)) {
            object_add_position = kl::float2(position);
            object_being_added = true;
        }
    });
    window.mouse.left.on_down.push_back([&]
    {
        object_second_position = window.mouse.position();
    });
    window.mouse.left.on_release.push_back([&]
    {
        if (object_being_added) {
            scene_object new_object = {};
            new_object.position = object_second_position;
            new_object.velocity = object_add_position - object_second_position;
            objects.push_back(new_object);
            object_being_added = false;
        }
    });

    window.keyboard.r.on_press.push_back([&]
    {
        objects.clear();
    });
    window.keyboard.g.on_press.push_back([&]
    {
        for (int i = 0; i < 20; i++) {
            scene_object random_object = {};
            random_object.position.x = kl::random::get_float(float(frame.width()));
            random_object.position.y = kl::random::get_float(get_frame_lower_limit(frame));
            random_object.velocity = kl::random::get_float2(-500.0f, 500.0f);
            objects.push_back(random_object);
        }
    });

    while (window.process(false)) {
        timer.update_interval();

        process_objects(objects, timer, frame, gravity);

        draw_background(frame);
        draw_objects(frame, objects);
        draw_interface(frame, object_being_added, object_add_position, object_second_position);

        window.draw_image(frame);
    }
}
