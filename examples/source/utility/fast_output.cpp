#include "examples.h"


static constexpr int N = 10000;

/* NOTE: Use Release mode for proper results. */

int examples::fast_output_main()
{
    float times[3] = {};

    kl::console::clear();
    kl::time::delta();
    for (int i = 0; i < N; i++) {
        printf("%d\n", i);
    }
    times[0] = kl::time::delta();

    kl::console::clear();
    kl::time::delta();
    for (int i = 0; i < N; i++) {
        std::cout << i << '\n';
    }
    times[1] = kl::time::delta();

    kl::console::clear();
    kl::time::delta();
    for (int i = 0; i < N; i++) {
        kl::print(i);
    }
    times[2] = kl::time::delta();

    print(kl::colors::YELLOW, "printf time: ", times[0]);
    print(kl::colors::CYAN, "std::cout time: ", times[1]);
    print(kl::colors::ORANGE, "kl::print time: ", times[2]);

    std::cin.get();
    return 0;
}
