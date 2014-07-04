#include "runner.hpp"

#include "clock.hpp"

#include <iostream>

int Runner::control(Window & window, World & world)
{
    if (!window.is_open()) {
        std::cerr << "Runner::control: window has not been opened." << std::endl;
        return 1;
    }

    auto window_dimension = window.dimension();
    if (!world.on_create(window_dimension.first, window_dimension.second)) {
        std::cerr << "Runner::control: world could not be created without errors." << std::endl;
        return 1;
    }

    Clock clock;
    while (!window.should_close()) {
        window.poll();

        world.on_update(clock.delta(), clock.elapsed());
        world.on_render();

        window.swap();
    }

    world.on_destroy();

    return 0;
}
