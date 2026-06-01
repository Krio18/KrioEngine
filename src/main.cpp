#include "../Sandbox/Sandbox.hpp"

int main() {
    Voxel::Sandbox sandbox;
    if (!sandbox.initialize())
        return 1;

    sandbox.run();

    if (!sandbox.shutdown())
        return 1;
    return 0;
}