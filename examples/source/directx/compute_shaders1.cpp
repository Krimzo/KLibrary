#include "examples.h"


struct TestStruct
{
    kl::Float3 position;
};

int examples::compute_shaders1_main( int argc, char** argv )
{
    static constexpr int DATA_SIZE = 10;
    TestStruct data[DATA_SIZE] = {};

    kl::GPU gpu{ nullptr };
    kl::dx::Buffer buffer = gpu.create_structured_buffer( data, DATA_SIZE, sizeof( TestStruct ), true, true );
    kl::dx::AccessView access_view = gpu.create_access_view( buffer, nullptr );
    std::string shader_source = kl::read_file( "shaders/compute_test1.hlsl" );
    kl::ComputeShader compute_shader = gpu.create_compute_shader( shader_source );

    gpu.bind_access_view_for_compute_shader( access_view, 0 );
    gpu.bind_compute_shader( compute_shader.shader );
    gpu.dispatch_compute_shader( DATA_SIZE, 1, 1 );
    gpu.read_from_buffer( data, buffer, DATA_SIZE * sizeof( TestStruct ) );

    for ( auto const& object : data )
    {
        print( object.position );
    }
    return 0;
}
