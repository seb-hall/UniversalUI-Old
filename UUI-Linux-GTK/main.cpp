#include <UUI/CoreHost.h>

int main(int argc, char** argv) {
    static CoreHost host;
    return host.main(argc, argv);
}