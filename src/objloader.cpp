#include "objloader.hpp"

#include "mesh.hpp"

#include "lib/tinyobjloader.hpp"

#include <iostream>
#include <vector>

std::unique_ptr<WorldObject> OBJLoader::load(std::string path)
{
    std::vector<tinyobj::shape_t> shapes;
    std::string err = tinyobj::LoadObj(shapes, path.c_str());
    if (!err.empty()) {
        std::cerr << "OBJLoader::load: error loading \"" << path << "\": " << err << std::endl;
        return nullptr;
    }

    std::unique_ptr<WorldObject> root(new WorldObject());

    for (auto & shape : shapes) {
        std::vector<float> & positions = shape.mesh.positions;
        std::vector<float> & normals = shape.mesh.normals;
        std::vector<unsigned int> & indices = shape.mesh.indices;

        std::shared_ptr<Mesh> mesh(new Mesh());

        for (unsigned int i = 0; i < positions.size(); i += 3) {
            glm::vec3 position(positions[i], positions[i + 1], positions[i + 2]);
            mesh->push_position(position);
        }

        for (unsigned int i = 0; i < normals.size(); i += 3) {
            glm::vec3 normal(normals[i], normals[i + 1], normals[i + 2]);
            mesh->push_normal(normal);
        }

        for (auto index : indices) {
            mesh->push_index(index);
        }

        root->add(mesh);
    }

    return root;
}
