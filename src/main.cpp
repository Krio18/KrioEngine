#include "Core/Application/Application.hpp"

int main() {
    Krio::Application app;
    if (!app.initialize())
        return 1;

    app.run();

    if (!app.shutdown())
        return 1;
    return 0;
}