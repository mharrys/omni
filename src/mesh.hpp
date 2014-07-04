#ifndef MESH_HPP_INCLUDED
#define MESH_HPP_INCLUDED

#include "worldobject.hpp"

class GLVertexArray;

class Mesh : public WorldObject {
public:
    Mesh();
    ~Mesh();

    void draw() override;

    void push_position(glm::vec3 position);
    void push_normal(glm::vec3 normal);
    void push_index(glm::uint index);
private:
    void make();
    void destroy();

    void update_buffers();
    bool is_dirty();
private:
    std::unique_ptr<GLVertexArray> gl;

    std::vector<glm::vec3> positions;
    std::vector<glm::vec3> normals;
    std::vector<glm::uint> indices;

    bool dirty;
};

#endif
