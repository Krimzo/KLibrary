#include "examples.h"


int examples::imaginary_numbers_main(const int argc, const char** argv)
{
    // Complex
    kl::Complex<float> position_2d{ 1, 1 };
    kl::Complex<float> rotator_2d{ -45 };
    kl::print(position_2d * rotator_2d);
    
    // Quaternions
    kl::Quaternion<float> position_3d{ 1, 0, 0 };
    kl::Quaternion<float> rotator_3d{ { 0, 0, 1 }, 90 };
    kl::Quaternion<float> rotator_3d_inv = kl::inverse(rotator_3d);
    kl::print(rotator_3d * position_3d * rotator_3d_inv);

    return 0;
}
