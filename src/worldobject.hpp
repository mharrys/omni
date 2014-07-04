#ifndef WORLDOBJECT_HPP_INCLUDED
#define WORLDOBJECT_HPP_INCLUDED

#include "lib/glm.hpp"

#include <functional>
#include <memory>
#include <vector>

class WorldObject {
public:
    WorldObject();

    virtual void draw();

    void add(std::shared_ptr<WorldObject> object);
    void traverse(std::function<void(WorldObject &)> callback);

    void translate_x(float distance);
    void translate_y(float distance);
    void translate_z(float distance);

    void rotate_x(float angle);
    void rotate_y(float angle);
    void rotate_z(float angle);

    void set_position(glm::vec3 position);
    glm::vec3 get_position() const;

    void update_local_transform();
    void update_world_transform();

    glm::mat4 local_transform() const;
    glm::mat4 world_transform() const;
private:
    void translate(float distance, glm::vec3 axis);
    void rotate(float angle, glm::vec3 axis);
private:
    WorldObject * parent;
    std::vector<std::shared_ptr<WorldObject>> children;

    glm::vec3 position;
    glm::quat orientation;

    glm::mat4 local;
    glm::mat4 world;
};

#endif
