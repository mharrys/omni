# include "mesh.hpp"

#include "program.hpp"

#include "lib/gl.hpp"

Mesh::Mesh()
    : gl(new GLVertexArray()),
      dirty(false)
{
}

Mesh::~Mesh()
{
    destroy();
}

void Mesh::draw()
{
    if (is_dirty()) {
        update_buffers();
    }

    glBindVertexArray(gl->id);
    glDrawElements(GL_TRIANGLES, gl->size, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}

void Mesh::push_position(glm::vec3 position)
{
    positions.push_back(position);
    dirty = true;
}

void Mesh::push_normal(glm::vec3 normal)
{
    normals.push_back(normal);
    dirty = true;
}

void Mesh::push_index(glm::uint index)
{
    indices.push_back(index);
    dirty = true;
}

static void set_vertex_attrib(VertexAttribIndex index, GLint size, GLsizei stride, GLsizei start)
{
    glVertexAttribPointer(
        index,
        size,
        GL_FLOAT,
        GL_FALSE,
        stride * sizeof(GLfloat),
        (const GLvoid *) (start * sizeof(GLfloat))
    );
}

void Mesh::make()
{
    glGenBuffers(1, &gl->positions_buffer);
    glGenBuffers(1, &gl->normals_buffer);
    glGenBuffers(1, &gl->indices_buffer);

    // upload positions
    glBindBuffer(GL_ARRAY_BUFFER, gl->positions_buffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * positions.size(), &positions[0], GL_STATIC_DRAW);

    // upload normals
    glBindBuffer(GL_ARRAY_BUFFER, gl->normals_buffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * normals.size(), &normals[0], GL_STATIC_DRAW);

    // upload indices
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, gl->indices_buffer);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(glm::uint) * indices.size(), &indices[0], GL_STATIC_DRAW);

    // create and setup a vertex array object for all buffers
    glGenVertexArrays(1, &gl->id);
    glBindVertexArray(gl->id);

    glEnableVertexAttribArray(VERTEX_POSITION_INDEX);
    glEnableVertexAttribArray(VERTEX_NORMAL_INDEX);

    glBindBuffer(GL_ARRAY_BUFFER, gl->positions_buffer);
    set_vertex_attrib(VERTEX_POSITION_INDEX, 3, 0, 0);

    glBindBuffer(GL_ARRAY_BUFFER, gl->normals_buffer);
    set_vertex_attrib(VERTEX_NORMAL_INDEX, 3, 0, 0);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, gl->indices_buffer);

    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    gl->size = indices.size();
}

void Mesh::destroy()
{
    if (gl->positions_buffer > 0) {
        glDeleteBuffers(1, &gl->positions_buffer);
    }

    if (gl->normals_buffer > 0) {
        glDeleteBuffers(1, &gl->normals_buffer);
    }

    if (gl->indices_buffer > 0) {
        glDeleteBuffers(1, &gl->indices_buffer);
    }

    if (gl->id > 0) {
        glDeleteVertexArrays(1, &gl->id);
    }
}


void Mesh::update_buffers()
{
    destroy();
    make();
    dirty = false;
}

bool Mesh::is_dirty()
{
    return dirty;
}
