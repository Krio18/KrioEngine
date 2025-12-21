#include "Core/Application/Application.hpp"

int main() {
    Krio::Application app;
    if (!app.initialize())
        return 1;

    app.run();

    return 0;
}