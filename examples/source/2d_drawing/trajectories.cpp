#include "examples.h"


struct SceneObject
{
    float radius = kl::random::gen_float( 10.0f, 20.0f );
    kl::Float2 position = {};
    kl::Float2 velocity = {};
    kl::RGB color = kl::random::gen_rgb();
    std::vector<kl::Float2> position_history = {};
};

static float frame_lower_limit( kl::Image const& frame )
{
    return frame.height() * 0.9f;
}

static void draw_background( kl::Image& frame )
{
    float lower_limit = frame_lower_limit( frame );
    frame.fill( { 60, 60, 60 } );
    frame.draw_rectangle( { 0, (int) lower_limit }, frame.size(), kl::colors::GRAY, true );
    frame.draw_line( { 0, (int) lower_limit }, { frame.width(), (int) lower_limit }, { 30, 30, 30 } );
}

static void draw_objects( kl::Image& frame, std::vector<SceneObject>& objects )
{
    for ( auto& [radius, position, velocity, color, position_history] : objects )
    {
        position_history.push_back( position );
        int start_position_index = (int) (position_history.size() * 0.75f);
        for ( int i = start_position_index; i < (int) position_history.size() - 1; i++ )
        {
            frame.draw_line( kl::Int2( position_history[i] ), kl::Int2( position_history[i + 1] ),
                kl::RGB( 60, 60, 60 ).mix( color, kl::unlerp( (float) i, (float) start_position_index, (float) position_history.size() ) ) );
        }
        frame.draw_circle( kl::Int2( position ), radius, color, true );
        frame.draw_circle( kl::Int2( position ), radius, { 30, 30, 30 } );
    }
}

static void draw_interface( kl::Image& frame, bool draw_arrow, kl::Float2 arrow_start, kl::Float2 arrow_end )
{
    if ( !draw_arrow )
        return;

    for ( int i = -1; i <= 1; i++ )
    {
        kl::Float2 adder = { (float) i, (float) i };
        frame.draw_line( kl::Int2( arrow_start + adder ), kl::Int2( arrow_end + adder ), kl::colors::SKY );
    }
}

static void process_objects( std::vector<SceneObject>& objects, kl::Timer const& timer, kl::Image const& frame, kl::Float2 gravity )
{
    static constexpr float energy_retain = 0.8f;

    for ( auto& [radius, position, velocity, color, position_history] : objects )
    {
        velocity += gravity * timer.delta();
        position += velocity * timer.delta();
    }

    for ( int i = 0; i < (int) objects.size(); i++ )
    {
        int intersect_count = 0;
        kl::Float2 velocity_sum, position_sum;
        for ( int j = 0; j < (int) objects.size(); j++ )
        {
            if ( i != j )
            {
                kl::Float2 positions_vector = objects[i].position - objects[j].position;
                float radius_sum = objects[i].radius + objects[j].radius;
                if ( positions_vector.length() < radius_sum )
                {
                    position_sum += objects[j].position + kl::normalize( positions_vector ) * radius_sum;
                    velocity_sum += kl::reflect( objects[i].velocity, objects[j].position - objects[i].position );
                    intersect_count += 1;
                }
            }
        }
        if ( intersect_count > 0 )
        {
            float count_rec = 1.0f / intersect_count;
            objects[i].position = position_sum * count_rec;
            objects[i].velocity = velocity_sum * count_rec * energy_retain;
        }
    }

    for ( auto& [radius, position, velocity, color, position_history] : objects )
    {
        if ( position.x - radius < 0 )
        {
            position.x = radius;
            velocity.x *= -1.0f;
            velocity *= energy_retain;
        }
        if ( position.x + radius > frame.width() )
        {
            position.x = frame.width() - radius;
            velocity.x *= -1.0f;
            velocity *= energy_retain;
        }
        if ( position.y - radius < 0 )
        {
            position.y = radius;
            velocity.y *= -1.0f;
            velocity *= energy_retain;
        }
        if ( position.y + radius > frame_lower_limit( frame ) )
        {
            position.y = frame_lower_limit( frame ) - radius;
            velocity.y *= -1.0f;
            velocity *= energy_retain;
        }
    }
}

int examples::trajectories_main( int argc, char** argv )
{
    kl::Window window{ "Trajectories" };
    kl::Image frame{ window.size() };
    kl::Timer timer;

    kl::Float2 gravity{ 0, 98.1f };
    std::vector<SceneObject> objects;

    window.on_resize.emplace_back( [&]( kl::Int2 size )
    {
        frame.resize( size );
    } );

    bool object_being_added = false;
    kl::Float2 object_add_position;
    kl::Float2 object_second_position;

    while ( window.process() )
    {
        timer.update();

        if ( window.mouse.left.pressed() )
        {
            kl::Int2 position = window.mouse.position();
            if ( position.x >= 0 && position.x < (int) window.width() && position.y > 0 && position.y < frame_lower_limit( frame ) )
            {
                object_add_position = position;
                object_second_position = position;
                object_being_added = true;
            }
        }
        if ( window.mouse.left.released() )
        {
            if ( object_being_added )
            {
                SceneObject new_object = {};
                new_object.position = object_second_position;
                new_object.velocity = object_add_position - object_second_position;
                objects.push_back( new_object );
                object_being_added = false;
            }
        }
        if ( window.mouse.left )
        {
            object_second_position = window.mouse.position();
        }
        if ( window.keyboard.g.pressed() )
        {
            for ( int i = 0; i < 20; i++ )
            {
                SceneObject random_object = {};
                random_object.position.x = kl::random::gen_float( (float) frame.width() );
                random_object.position.y = kl::random::gen_float( frame_lower_limit( frame ) );
                random_object.velocity = kl::random::gen_float2( -500.0f, 500.0f );
                objects.push_back( random_object );
            }
        }
        if ( window.keyboard.r.pressed() )
            objects.clear();

        process_objects( objects, timer, frame, gravity );

        draw_background( frame );
        draw_objects( frame, objects );
        draw_interface( frame, object_being_added, object_add_position, object_second_position );

        window.draw_image( frame );
    }
    return 0;
}
