#include "examples.h"


int examples::imaginary_numbers_main()
{
    // Complex
    kl::complex position_2d = { 1, 1 };
    kl::complex rotator_2d = kl::complex(-45);
    kl::print(rotator_2d * position_2d);
    
    // Quaternions
    kl::quaternion position_3d = { 1, 0, 0 };
    kl::quaternion rotator_3d = kl::quaternion({ 0, 0, 1 }, 90);
    kl::quaternion rotator_3d_inv = kl::inverse(rotator_3d);
    kl::print(rotator_3d * position_3d * rotator_3d_inv);

    kl::get();
    return 0;
}
