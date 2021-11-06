#include "KrimzLib.h"


int main() {
    kl::InitLib();


    std::string toFind = "Ivan";

    kl::time::StaticGetElapsed();
    kl::uint64 n = 0;
    while (kl::random::String(toFind.size()) != toFind) {
        n++;
    }
    std::cout << "Found \"" << toFind << "\" in " << n << " tries and " << kl::time::StaticGetElapsed() << " seconds\n";


    kl::console::WaitFor(' ');
    return 0;
}
