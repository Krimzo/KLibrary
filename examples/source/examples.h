#pragma once

#include "klibrary.h"


namespace examples {
    // 2D Drawing
    int data_line_main();
    int interpolation_main();
    int tic_tac_main();
    int times_table_main();
    int trajectories_main();
    int visual_sort_main();

    // DirectX
    int compute_shaders1_main();
    int compute_shaders2_main();
    int geometry_shaders_main();
    int hello_world_main();
    int hello_world_ext_main();
    int mandelbrot_main();
    int plane_playground_main();
    int raytracing_main();

    // Math
    int imaginary_numbers_main();

    // Utility
    int async_test_main();
    int dynamic_linking_main();
    int encryption_main();
    int fast_output_main();
    int hashing_main();
    int safety_test_main();
    int sockets_main();

    // Video
    int video_reading_main(int argc, const char** argv);
    int video_writing_main();
}
