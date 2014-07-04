#include "world.hpp"

bool World::on_create(int window_width, int window_height)
{
    (void) window_width;
    (void) window_height;
    return true;
}

void World::on_update(seconds delta, seconds elapsed)
{
    (void) delta;
    (void) elapsed;
}

void World::on_render()
{
}

void World::on_destroy()
{
}
