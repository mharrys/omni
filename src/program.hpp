#ifndef PROGRAM_HPP_INCLUDED
#define PROGRAM_HPP_INCLUDED

#include "shader.hpp"

#include "lib/glm.hpp"

#include <string>

class Shader;

class GLProgram;

class Program {
public:
    Program(std::string vertex_shader_path, std::string fragment_shader_path);
    ~Program();

    void use();
    void reload();

    bool is_modified() const;
    bool is_valid() const;

    void set_uniform(std::string name, int value);
    void set_uniform(std::string name, bool value);
    void set_uniform(std::string name, float value);
    void set_uniform(std::string name, glm::mat3 const & value);
    void set_uniform(std::string name, glm::mat4 const & value);
    void set_uniform(std::string name, glm::vec2 const & value);
    void set_uniform(std::string name, glm::vec3 const & value);
    void set_uniform(std::string name, glm::vec4 const & value);
private:
    void load();
    void make();
    void destroy();
private:
    std::unique_ptr<GLProgram> gl;

    Shader vert;
    Shader frag;
};

#endif
