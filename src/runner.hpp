#ifndef RUNNER_HPP_INCLUDED
#define RUNNER_HPP_INCLUDED

#include "world.hpp"
#include "window.hpp"

class Runner {
public:
    int control(Window & window, World & world);
};

#endif
