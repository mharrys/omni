#include "shader.hpp"

#include "lib/gl.hpp"

#include <fstream>
#include <iostream>
#include <sstream>

static GLenum convert_type(ShaderType type)
{
    switch (type) {
    case ShaderType::VERTEX:
        return GL_VERTEX_SHADER;
    case ShaderType::GEOMETRY:
        return GL_GEOMETRY_SHADER;
    case ShaderType::FRAGMENT:
        return GL_FRAGMENT_SHADER;
    default:
        return 0;
    }
}

Shader::Shader(ShaderType type, std::string path)
    : gl(new GLShader()),
      type(type),
      path(path),
      last_load_modified(0)
{
    gl->type = convert_type(type);
    load();
}

Shader::~Shader()
{
    destroy();
}

void Shader::reload()
{
    destroy();
    load();
}

void Shader::attach_to(GLProgram * p)
{
    glAttachShader(p->program, gl->shader);
}

void Shader::detach_from(GLProgram * p)
{
    glDetachShader(p->program, gl->shader);
}

bool Shader::is_modified() const
{
    return (last_modified() > last_load_modified);
}

bool Shader::is_valid() const
{
    return gl->shader > 0;
}

ShaderType Shader::get_type() const
{
    return type;
}

static bool read_source(std::string path, std::string & source)
{
    std::ifstream f(path.c_str(), std::ifstream::in | std::ifstream::binary);

    if (f.is_open()) {
        std::stringstream buffer;
        buffer << f.rdbuf();
        source = buffer.str();
        return true;
    } else {
        std::cerr << "Shader::read_source: could not read source \"" << path << "\"." << std::endl;
        source = "void main() {}";
        return false;
    }
}

void Shader::load()
{
    std::string source;
    if (!read_source(path, source)) {
        return;
    }

    make(source);

    if (is_valid()) {
        last_load_modified = last_modified();
    } else {
        last_load_modified = 0;
    }
}

static void print_shader_error(GLuint shader, std::string path)
{
    GLint log_len;
    glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &log_len);

    GLchar * log_info = new GLchar[log_len];
    glGetShaderInfoLog(shader, log_len, NULL, log_info);

    std::cerr << "Shader::make: could not compile \"" << path << "\": " << log_info << std::endl;

    delete[] log_info;
}

void Shader::make(std::string & source)
{
    const char * shader_source = source.c_str();

    gl->shader = glCreateShader(gl->type);
    glShaderSource(gl->shader, 1, &shader_source, NULL);
    glCompileShader(gl->shader);

    GLint status;
    glGetShaderiv(gl->shader, GL_COMPILE_STATUS, &status);
    if (status ==  GL_FALSE) {
        print_shader_error(gl->shader, path);
        gl->shader = 0;
    }
}

void Shader::destroy()
{
    if (gl->shader > 0) {
        glDeleteShader(gl->shader);
        gl->shader = 0;
        last_load_modified = 0;
    }
}

time_t Shader::last_modified() const
{
   struct stat buf;

   if (stat(path.c_str(), &buf) == 0) {
       return buf.st_mtime;
   } else {
       std::cerr << "Shader:last_modified: could not get file status for \"" << path << "\"." << std::endl;
       return 0;
   }
}
