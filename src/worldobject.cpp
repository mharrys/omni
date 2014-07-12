#include "worldobject.hpp"

#include <iostream>

WorldObject::WorldObject()
    : parent(nullptr)
{
}

void WorldObject::draw()
{
}

void WorldObject::add(std::shared_ptr<WorldObject> object)
{
    children.push_back(object);
    object->parent = this;
}

void WorldObject::traverse(std::function<void(WorldObject &)> callback)
{
    callback(*this);

    for (auto child : children) {
        child->traverse(callback);
    }
}

void WorldObject::translate_x(float distance)
{
    translate(distance, X_UNIT);
}

void WorldObject::translate_y(float distance)
{
    translate(distance, Y_UNIT);
}

void WorldObject::translate_z(float distance)
{
    translate(distance, Z_UNIT);
}

void WorldObject::rotate_x(float angle)
{
    rotate(angle, X_UNIT);
}

void WorldObject::rotate_y(float angle)
{
    rotate(angle, Y_UNIT);
}

void WorldObject::rotate_z(float angle)
{
    rotate(angle, Z_UNIT);
}

void WorldObject::set_position(glm::vec3 position)
{
    this->position = position;
}

glm::vec3 WorldObject::get_position() const
{
    return position;
}

void WorldObject::update_local_transform()
{
    glm::mat4 T = glm::translate(glm::mat4(1.0f), position);
    glm::mat4 R = glm::toMat4(orientation);

    local = T * R;
}

void WorldObject::update_world_transform()
{
    update_local_transform();

    if (parent == nullptr) {
        world = local;
    } else {
        world = parent->world * local;
    }

    for (auto child : children) {
        child->update_world_transform();
    }
}

glm::mat4 WorldObject::local_transform() const
{
    return local;
}

glm::mat4 WorldObject::world_transform() const
{
    return world;
}

void WorldObject::translate(float distance, glm::vec3 axis)
{
    glm::quat v = orientation * glm::quat(0.0f, axis) * glm::conjugate(orientation);

    position.x += v.x * distance;
    position.y += v.y * distance;
    position.z += v.z * distance;
}

void WorldObject::rotate(float angle, glm::vec3 axis)
{
    orientation = glm::rotate(orientation, angle, axis);
}
