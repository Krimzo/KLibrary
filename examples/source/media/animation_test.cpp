#include "examples.h"

static constexpr auto SHADER_SOURCE =
R"(
struct VS_DATA
{
    float4 position : SV_Position;
    float3 color : VS_Color;
};

float2 SCREEN_SIZE;

VS_DATA v_shader(float3 position : KL_Position, float3 color : KL_Normal)
{
    position.xy /= SCREEN_SIZE * .5f;

    VS_DATA data;
    data.position = float4(position, 1.0f);
    data.color = color;
    return data;
}

float4 p_shader(VS_DATA vs_data) : SV_Target0
{
    return float4(vs_data.color, 1.0f);
}
)";

struct State
{
    static inline const float screen_hperc = 1000.f;
    static inline const float tan30 = kl::tan_d( 30.0f );
    static inline const float b = kl::sqrt( screen_hperc / ( tan30 * tan30 + 1.f ) );
    static inline const float a = b * tan30;
    static inline const float c = kl::sqrt( a * a + b * b );
    static inline const float ab = a * b;
    static inline const float ac = a * c;
    static inline const float bc = b * c;

    static inline const kl::RGB FIRST_COLOR = { 251, 84, 43 };
    static inline const kl::RGB SECOND_COLOR = { 133, 251, 43 };
    static inline const kl::RGB THIRD_COLOR = { 43, 142, 251 };

    State( const kl::Int2 frame_size )
    {
        // create 1
        parts.emplace_back( 3.0f, [this]( float time_perc )
            {
                auto& triangle = triangles[0];
                triangle.a = kl::Vertex{ { ab / 2, -( ( c * c ) / 2 ), 0.0f }, FIRST_COLOR, {} };
                triangle.c = kl::Vertex{ { triangle.a.x - b, triangle.a.y, 0.0f }, FIRST_COLOR, {} };
                triangle.b = kl::Vertex{ { triangle.c.x, triangle.c.y + a, 0.0f }, FIRST_COLOR, {} };

                gpu.text_data = {
                    kl::Text{ labels_text_format, FIRST_COLOR, scale_ndc( ( triangles[0].b.position.xy() + triangles[0].c.position.xy() ) * 0.5f ), {}, L"a" },
                    kl::Text{ labels_text_format, FIRST_COLOR, scale_ndc( ( triangles[0].a.position.xy() + triangles[0].c.position.xy() ) * 0.5f ), {}, L"b" },
                    kl::Text{ labels_text_format, FIRST_COLOR, scale_ndc( ( triangles[0].a.position.xy() + triangles[0].b.position.xy() ) * 0.5f ), {}, L"c" },
                    kl::Text{ info_text_format, kl::colors::WHITE, { -0.75f, 0.0f }, {}, L"Create triangle 1." },
                };
            } );

        // scale 1
        parts.emplace_back( 5.0f, [this]( float time_perc )
            {
                auto& triangle = triangles[0];
                triangle.a = kl::Vertex{ { ab / 2, -( ( c * c ) / 2 ), 0.0f }, FIRST_COLOR, {} };
                triangle.c = kl::Vertex{ { triangle.a.x - b, triangle.a.y, 0.0f }, FIRST_COLOR, {} };
                triangle.b = kl::Vertex{ { triangle.c.x, triangle.c.y + a, 0.0f }, FIRST_COLOR, {} };

                triangle.c.position.xy() = { triangle.a.x - kl::lerp( time_perc, b, a * b ), triangle.a.y };
                triangle.b.position.xy() = { triangle.c.x, triangle.c.y + kl::lerp( time_perc, a, a * a ) };

                gpu.text_data = {
                    kl::Text{ labels_text_format, FIRST_COLOR, scale_ndc( ( triangles[0].b.position.xy() + triangles[0].c.position.xy() ) * 0.5f ), {}, kl::wformat( std::fixed, std::setprecision( 2 ), "a * ", time_perc, "a" ) },
                    kl::Text{ labels_text_format, FIRST_COLOR, scale_ndc( ( triangles[0].a.position.xy() + triangles[0].c.position.xy() ) * 0.5f ), {}, kl::wformat( std::fixed, std::setprecision( 2 ), "b * ", time_perc, "a" ) },
                    kl::Text{ labels_text_format, FIRST_COLOR, scale_ndc( ( triangles[0].a.position.xy() + triangles[0].b.position.xy() ) * 0.5f ), {}, kl::wformat( std::fixed, std::setprecision( 2 ), "c * ", time_perc, "a" ) },
                    kl::Text{ info_text_format, kl::colors::WHITE, { -0.75f, 0.0f }, {}, L"Scale triangle 1 by a." },
                };
            } );

        // create 2
        parts.emplace_back( 3.0f, [this]( float time_perc )
            {
                auto& triangle = triangles[1];
                triangle.a = kl::Vertex{ { ab / 2, -( ( c * c ) / 2 ), 0.0f }, SECOND_COLOR, {} };
                triangle.c = kl::Vertex{ { triangle.a.x - b, triangle.a.y, 0.0f }, SECOND_COLOR, {} };
                triangle.b = kl::Vertex{ { triangle.c.x, triangle.c.y + a, 0.0f }, SECOND_COLOR, {} };

                gpu.text_data = {
                    kl::Text{ labels_text_format, FIRST_COLOR, scale_ndc( ( triangles[0].b.position.xy() + triangles[0].c.position.xy() ) * 0.5f ), {}, L"aa" },
                    kl::Text{ labels_text_format, FIRST_COLOR, scale_ndc( ( triangles[0].a.position.xy() + triangles[0].c.position.xy() ) * 0.5f ), {}, L"ba" },
                    kl::Text{ labels_text_format, FIRST_COLOR, scale_ndc( ( triangles[0].a.position.xy() + triangles[0].b.position.xy() ) * 0.5f ), {}, L"ca" },
                    kl::Text{ labels_text_format, SECOND_COLOR, scale_ndc( ( triangles[1].b.position.xy() + triangles[1].c.position.xy() ) * 0.5f ), {}, L"a" },
                    kl::Text{ labels_text_format, SECOND_COLOR, scale_ndc( ( triangles[1].a.position.xy() + triangles[1].c.position.xy() ) * 0.5f ), {}, L"b" },
                    kl::Text{ labels_text_format, SECOND_COLOR, scale_ndc( ( triangles[1].a.position.xy() + triangles[1].b.position.xy() ) * 0.5f ), {}, L"c" },
                    kl::Text{ info_text_format, kl::colors::WHITE, { -0.75f, 0.0f }, {}, L"Create triangle 2." },
                };
            } );

        // scale, rotate and move 2
        parts.emplace_back( 5.0f, [this]( float time_perc )
            {
                auto& triangle = triangles[1];
                triangle.a = kl::Vertex{ { ab / 2, -( ( c * c ) / 2 ), 0.0f }, SECOND_COLOR, {} };
                triangle.c = kl::Vertex{ { triangle.a.x - b, triangle.a.y, 0.0f }, SECOND_COLOR, {} };
                triangle.b = kl::Vertex{ { triangle.c.x, triangle.c.y + a, 0.0f }, SECOND_COLOR, {} };

                triangle.c.position.xy() = triangle.a.position.xy() + ( ( triangle.c.position.xy() - triangle.a.position.xy() ) * kl::lerp( time_perc, 1.0f, b ) );
                triangle.b.position.xy() = triangle.a.position.xy() + ( ( triangle.b.position.xy() - triangle.a.position.xy() ) * kl::lerp( time_perc, 1.0f, b ) );

                triangle.c.position.xy() = kl::rotate( triangle.c.position.xy() - triangle.a.position.xy(), -kl::lerp( time_perc, 0.0f, 90.0f ) ) + triangle.a.position.xy();
                triangle.b.position.xy() = kl::rotate( triangle.b.position.xy() - triangle.a.position.xy(), -kl::lerp( time_perc, 0.0f, 90.0f ) ) + triangle.a.position.xy();

                triangle.a.position.xy() += kl::Float2{ -kl::lerp( time_perc, 0.0f, ab ), kl::lerp( time_perc, 0.0f, a * a ) };
                triangle.b.position.xy() += kl::Float2{ -kl::lerp( time_perc, 0.0f, ab ), kl::lerp( time_perc, 0.0f, a * a ) };
                triangle.c.position.xy() += kl::Float2{ -kl::lerp( time_perc, 0.0f, ab ), kl::lerp( time_perc, 0.0f, a * a ) };

                gpu.text_data = {
                    kl::Text{ labels_text_format, FIRST_COLOR, scale_ndc( ( triangles[0].b.position.xy() + triangles[0].c.position.xy() ) * 0.5f ), {}, L"aa" },
                    kl::Text{ labels_text_format, FIRST_COLOR, scale_ndc( ( triangles[0].a.position.xy() + triangles[0].c.position.xy() ) * 0.5f ), {}, L"ba" },
                    kl::Text{ labels_text_format, FIRST_COLOR, scale_ndc( ( triangles[0].a.position.xy() + triangles[0].b.position.xy() ) * 0.5f ), {}, L"ca" },
                    kl::Text{ labels_text_format, SECOND_COLOR, scale_ndc( ( triangles[1].b.position.xy() + triangles[1].c.position.xy() ) * 0.5f ), {}, kl::wformat( std::fixed, std::setprecision( 2 ), "a * ", time_perc, "b" ) },
                    kl::Text{ labels_text_format, SECOND_COLOR, scale_ndc( ( triangles[1].a.position.xy() + triangles[1].c.position.xy() ) * 0.5f ), {}, kl::wformat( std::fixed, std::setprecision( 2 ), "b * ", time_perc, "b" ) },
                    kl::Text{ labels_text_format, SECOND_COLOR, scale_ndc( ( triangles[1].a.position.xy() + triangles[1].b.position.xy() ) * 0.5f ), {}, kl::wformat( std::fixed, std::setprecision( 2 ), "c * ", time_perc, "b" ) },
                    kl::Text{ info_text_format, kl::colors::WHITE, { -0.75f, 0.0f }, {}, L"Rotate, move and scale\ntriangle 2 by b." },
                };
            } );

        // create 3
        parts.emplace_back( 3.0f, [this]( float time_perc )
            {
                auto& triangle = triangles[2];
                triangle.a = kl::Vertex{ { ab / 2, -( ( c * c ) / 2 ), 0.0f }, THIRD_COLOR, {} };
                triangle.c = kl::Vertex{ { triangle.a.x - b, triangle.a.y, 0.0f }, THIRD_COLOR, {} };
                triangle.b = kl::Vertex{ { triangle.c.x, triangle.c.y + a, 0.0f }, THIRD_COLOR, {} };

                gpu.text_data = {
                    kl::Text{ labels_text_format, FIRST_COLOR, scale_ndc( ( triangles[0].b.position.xy() + triangles[0].c.position.xy() ) * 0.5f ), {}, L"aa" },
                    kl::Text{ labels_text_format, FIRST_COLOR, scale_ndc( ( triangles[0].a.position.xy() + triangles[0].c.position.xy() ) * 0.5f ), {}, L"ba" },
                    kl::Text{ labels_text_format, FIRST_COLOR, scale_ndc( ( triangles[0].a.position.xy() + triangles[0].b.position.xy() ) * 0.5f ), {}, L"ca" },
                    kl::Text{ labels_text_format, SECOND_COLOR, scale_ndc( ( triangles[1].b.position.xy() + triangles[1].c.position.xy() ) * 0.5f ), {}, L"ab" },
                    kl::Text{ labels_text_format, SECOND_COLOR, scale_ndc( ( triangles[1].a.position.xy() + triangles[1].c.position.xy() ) * 0.5f ), {}, L"bb" },
                    kl::Text{ labels_text_format, SECOND_COLOR, scale_ndc( ( triangles[1].a.position.xy() + triangles[1].b.position.xy() ) * 0.5f ), {}, L"cb" },
                    kl::Text{ labels_text_format, THIRD_COLOR, scale_ndc( ( triangles[2].b.position.xy() + triangles[2].c.position.xy() ) * 0.5f ), {}, L"a" },
                    kl::Text{ labels_text_format, THIRD_COLOR, scale_ndc( ( triangles[2].a.position.xy() + triangles[2].c.position.xy() ) * 0.5f ), {}, L"b" },
                    kl::Text{ labels_text_format, THIRD_COLOR, scale_ndc( ( triangles[2].a.position.xy() + triangles[2].b.position.xy() ) * 0.5f ), {}, L"c" },
                    kl::Text{ info_text_format, kl::colors::WHITE, { -0.75f, 0.0f }, {}, L"Create triangle 3." },
                };
            } );

        // reflect 3
        parts.emplace_back( 5.0f, [this]( float time_perc )
            {
                auto& triangle = triangles[2];
                triangle.a = kl::Vertex{ { ab / 2, -( ( c * c ) / 2 ), 0.0f }, THIRD_COLOR, {} };
                triangle.c = kl::Vertex{ { triangle.a.x - b, triangle.a.y, 0.0f }, THIRD_COLOR, {} };
                triangle.b = kl::Vertex{ { triangle.c.x, triangle.c.y + a, 0.0f }, THIRD_COLOR, {} };

                const kl::Float2 reflect_c = triangle.a.position.xy() + kl::reflect( triangle.a.position.xy() - triangle.c.position.xy(), triangle.b.position.xy() - triangle.a.position.xy() );
                triangle.c.position.xy() = kl::lerp( time_perc, triangle.c.position.xy(), reflect_c );

                gpu.text_data = {
                    kl::Text{ labels_text_format, FIRST_COLOR, scale_ndc( ( triangles[0].b.position.xy() + triangles[0].c.position.xy() ) * 0.5f ), {}, L"aa" },
                    kl::Text{ labels_text_format, FIRST_COLOR, scale_ndc( ( triangles[0].a.position.xy() + triangles[0].c.position.xy() ) * 0.5f ), {}, L"ba" },
                    kl::Text{ labels_text_format, FIRST_COLOR, scale_ndc( ( triangles[0].a.position.xy() + triangles[0].b.position.xy() ) * 0.5f ), {}, L"ca" },
                    kl::Text{ labels_text_format, SECOND_COLOR, scale_ndc( ( triangles[1].b.position.xy() + triangles[1].c.position.xy() ) * 0.5f ), {}, L"ab" },
                    kl::Text{ labels_text_format, SECOND_COLOR, scale_ndc( ( triangles[1].a.position.xy() + triangles[1].c.position.xy() ) * 0.5f ), {}, L"bb" },
                    kl::Text{ labels_text_format, SECOND_COLOR, scale_ndc( ( triangles[1].a.position.xy() + triangles[1].b.position.xy() ) * 0.5f ), {}, L"cb" },
                    kl::Text{ labels_text_format, THIRD_COLOR, scale_ndc( ( triangles[2].b.position.xy() + triangles[2].c.position.xy() ) * 0.5f ), {}, L"a" },
                    kl::Text{ labels_text_format, THIRD_COLOR, scale_ndc( ( triangles[2].a.position.xy() + triangles[2].c.position.xy() ) * 0.5f ), {}, L"b" },
                    kl::Text{ labels_text_format, THIRD_COLOR, scale_ndc( ( triangles[2].a.position.xy() + triangles[2].b.position.xy() ) * 0.5f ), {}, L"c" },
                    kl::Text{ info_text_format, kl::colors::WHITE, { -0.75f, 0.0f }, {}, L"Reflect triangle 3." },
                };
            } );

        // scale, rotate and move 3
        parts.emplace_back( 5.0f, [this]( float time_perc )
            {
                auto& triangle = triangles[2];
                triangle.a = kl::Vertex{ { ab / 2, -( ( c * c ) / 2 ), 0.0f }, THIRD_COLOR, {} };
                triangle.c = kl::Vertex{ { triangle.a.x - b, triangle.a.y, 0.0f }, THIRD_COLOR, {} };
                triangle.b = kl::Vertex{ { triangle.c.x, triangle.c.y + a, 0.0f }, THIRD_COLOR, {} };

                const kl::Float2 reflect_c = triangle.a.position.xy() + kl::reflect( triangle.a.position.xy() - triangle.c.position.xy(), triangle.b.position.xy() - triangle.a.position.xy() );
                triangle.c.position.xy() = reflect_c;

                triangle.c.position.xy() = triangle.a.position.xy() + ( ( triangle.c.position.xy() - triangle.a.position.xy() ) * kl::lerp( time_perc, 1.0f, c ) );
                triangle.b.position.xy() = triangle.a.position.xy() + ( ( triangle.b.position.xy() - triangle.a.position.xy() ) * kl::lerp( time_perc, 1.0f, c ) );

                triangle.c.position.xy() = kl::rotate( triangle.c.position.xy() - triangle.a.position.xy(), kl::lerp( time_perc, 0.0f, 120.0f ) ) + triangle.a.position.xy();
                triangle.b.position.xy() = kl::rotate( triangle.b.position.xy() - triangle.a.position.xy(), kl::lerp( time_perc, 0.0f, 120.0f ) ) + triangle.a.position.xy();

                triangle.a.y += kl::lerp( time_perc, 0.0f, c * c );
                triangle.b.y += kl::lerp( time_perc, 0.0f, c * c );
                triangle.c.y += kl::lerp( time_perc, 0.0f, c * c );

                const kl::Float2 last_tr_bias = { 0.0f, 25.0f };
                gpu.text_data = {
                    kl::Text{ labels_text_format, FIRST_COLOR, scale_ndc( ( triangles[0].b.position.xy() + triangles[0].c.position.xy() ) * 0.5f ), {}, L"aa" },
                    kl::Text{ labels_text_format, FIRST_COLOR, scale_ndc( ( triangles[0].a.position.xy() + triangles[0].c.position.xy() ) * 0.5f ), {}, L"ba" },
                    kl::Text{ labels_text_format, FIRST_COLOR, scale_ndc( ( triangles[0].a.position.xy() + triangles[0].b.position.xy() ) * 0.5f ), {}, L"ca" },
                    kl::Text{ labels_text_format, SECOND_COLOR, scale_ndc( ( triangles[1].b.position.xy() + triangles[1].c.position.xy() ) * 0.5f ), {}, L"ab" },
                    kl::Text{ labels_text_format, SECOND_COLOR, scale_ndc( ( triangles[1].a.position.xy() + triangles[1].c.position.xy() ) * 0.5f ), {}, L"bb" },
                    kl::Text{ labels_text_format, SECOND_COLOR, scale_ndc( ( triangles[1].a.position.xy() + triangles[1].b.position.xy() ) * 0.5f ), {}, L"cb" },
                    kl::Text{ labels_text_format, THIRD_COLOR, scale_ndc( last_tr_bias + ( triangles[2].b.position.xy() + triangles[2].c.position.xy() ) * 0.5f ), {}, kl::wformat( std::fixed, std::setprecision( 2 ), "a * ", time_perc, "c" ) },
                    kl::Text{ labels_text_format, THIRD_COLOR, scale_ndc( last_tr_bias + ( triangles[2].a.position.xy() + triangles[2].c.position.xy() ) * 0.5f ), {}, kl::wformat( std::fixed, std::setprecision( 2 ), "b * ", time_perc, "c" ) },
                    kl::Text{ labels_text_format, THIRD_COLOR, scale_ndc( ( triangles[2].a.position.xy() + triangles[2].b.position.xy() ) * 0.5f ), {}, kl::wformat( std::fixed, std::setprecision( 2 ), "c * ", time_perc, "c" ) },
                    kl::Text{ info_text_format, kl::colors::WHITE, { -0.75f, 0.0f }, {}, L"Rotate, move and scale\ntriangle 3 by c." },
                };
            } );

        // proof
        parts.emplace_back( 5.0f, [this]( float time_perc )
            {
                const kl::Float2 last_tr_bias = { 0.0f, 25.0f };
                gpu.text_data = {
                    kl::Text{ labels_text_format, FIRST_COLOR, scale_ndc( ( triangles[0].b.position.xy() + triangles[0].c.position.xy() ) * 0.5f ), {}, L"aa" },
                    kl::Text{ labels_text_format, FIRST_COLOR, scale_ndc( ( triangles[0].a.position.xy() + triangles[0].c.position.xy() ) * 0.5f ), {}, L"ba" },
                    kl::Text{ labels_text_format, FIRST_COLOR, scale_ndc( ( triangles[0].a.position.xy() + triangles[0].b.position.xy() ) * 0.5f ), {}, L"ca" },
                    kl::Text{ labels_text_format, SECOND_COLOR, scale_ndc( ( triangles[1].b.position.xy() + triangles[1].c.position.xy() ) * 0.5f ), {}, L"ab" },
                    kl::Text{ labels_text_format, SECOND_COLOR, scale_ndc( ( triangles[1].a.position.xy() + triangles[1].c.position.xy() ) * 0.5f ), {}, L"bb" },
                    kl::Text{ labels_text_format, SECOND_COLOR, scale_ndc( ( triangles[1].a.position.xy() + triangles[1].b.position.xy() ) * 0.5f ), {}, L"cb" },
                    kl::Text{ labels_text_format, THIRD_COLOR, scale_ndc( last_tr_bias + ( triangles[2].b.position.xy() + triangles[2].c.position.xy() ) * 0.5f ), {}, L"ac" },
                    kl::Text{ labels_text_format, THIRD_COLOR, scale_ndc( last_tr_bias + ( triangles[2].a.position.xy() + triangles[2].c.position.xy() ) * 0.5f ), {}, L"bc" },
                    kl::Text{ labels_text_format, THIRD_COLOR, scale_ndc( ( triangles[2].a.position.xy() + triangles[2].b.position.xy() ) * 0.5f ), {}, L"cc" },
                    kl::Text{ info_text_format, kl::colors::WHITE, { -0.75f, 0.0f }, {}, L"Proof: aa + bb = cc" },
                };
            } );

        window.on_resize.emplace_back( [&]( kl::Int2 size )
            {
                gpu.resize_internal( size );
                gpu.set_viewport_size( size );
            } );
        window.resize( frame_size );
        window.set_resizeable( false );

        gpu.bind_raster_state( gpu.create_raster_state( true, false ) );
        gpu.bind_depth_state( gpu.create_depth_state( false ) );
        gpu.bind_blend_state( gpu.create_blend_state( true ) );

        shaders = gpu.create_shaders( SHADER_SOURCE );
        gpu.bind_shaders( shaders );

        kl::dx::BufferDescriptor buff_desc{};
        buff_desc.ByteWidth = sizeof( triangles );
        buff_desc.Usage = D3D11_USAGE_DYNAMIC;
        buff_desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
        buff_desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
        buffer = gpu.create_buffer( &buff_desc, nullptr );

        labels_text_format = gpu.create_text_format(
            L"JetBrains Mono L",
            DWRITE_FONT_WEIGHT_NORMAL,
            DWRITE_FONT_STYLE_NORMAL,
            25.0f );

        info_text_format = gpu.create_text_format(
            L"JetBrains Mono L",
            DWRITE_FONT_WEIGHT_NORMAL,
            DWRITE_FONT_STYLE_NORMAL,
            35.0f );

        staging_texture = gpu.create_staging_texture( gpu.target_buffer( gpu.back_index() ) );
    }

    bool update( kl::VideoWriter& writer, kl::Image& frame )
    {
        const float current_time = writer.video_duration_seconds();
        if ( !update_parts( current_time ) )
            return false;

        struct alignas( 16 ) CB
        {
            kl::Float2 SCREEN_SIZE;
        } cb = {};

        cb.SCREEN_SIZE = window.size();
        shaders.upload( cb );

        gpu.clear_internal( kl::RGB{ 30, 30, 30 } );
        gpu.write_to_buffer( buffer, triangles, sizeof( triangles ) );
        gpu.draw( buffer );
        gpu.draw_text();

        load_frame( writer, frame );

        gpu.swap_buffers( true );
        return window.process();
    }

private:
    struct AnimationPart
    {
        float duration = 0.0f;
        std::function<void( float )> func;
    };

    kl::Window window{ "Hello World! (D3D11)" };
    kl::GPU gpu{ window.ptr() };

    kl::Shaders shaders;
    kl::dx::Buffer buffer;
    kl::TextFormat labels_text_format;
    kl::TextFormat info_text_format;
    kl::dx::Texture staging_texture;

    kl::Triangle triangles[3] = {};
    std::vector<AnimationPart> parts;

    bool update_parts( const float time )
    {
        float part_end_time = 0.f;
        for ( auto& [part_duration, part_func] : parts )
        {
            const float part_start_time = part_end_time;
            part_end_time += part_duration;
            if ( time <= part_end_time )
            {
                part_func( ( time - part_start_time ) / part_duration );
                return true;
            }
        }
        return false;
    }

    void load_frame( kl::VideoWriter& writer, kl::Image& frame )
    {
        kl::dx::Texture back_buffer = gpu.target_buffer( gpu.back_index() );
        gpu.copy_resource( staging_texture, back_buffer );
        gpu.read_from_texture( frame.ptr(), staging_texture, frame.size(), sizeof( kl::RGB ) );
        writer.add_frame( frame );
    }

    kl::Float2 scale_ndc( kl::Float2 pos ) const
    {
        return pos / ( window.size() / 2 );
    }
};

int examples::animation_test_main( int argc, char** argv )
{
    kl::VideoWriter writer{ "media/pythagoras.mp4", kl::VideoType::h264(), { 1920, 1080 }, 30, 2.0f, 0 };
    kl::Image frame{ writer.frame_size() };
    State state{ writer.frame_size() };
    while ( state.update( writer, frame ) );
    writer.finalize();
    return 0;
}
