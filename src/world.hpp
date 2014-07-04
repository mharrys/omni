#ifndef WORLD_HPP_INCLUDED
#define WORLD_HPP_INCLUDED

#include "clock.hpp"

class World {
public:
    virtual bool on_create(int window_width, int window_height);
    virtual void on_update(seconds delta, seconds elapsed);
    virtual void on_render();
    virtual void on_destroy();
};

#endif
