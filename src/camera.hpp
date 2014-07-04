#ifndef CAMERA_HPP_INCLUDED
#define CAMERA_HPP_INCLUDED

#include "worldobject.hpp"

class Camera : public WorldObject {
public:
    virtual glm::mat4 projection() const = 0;
};

#endif
