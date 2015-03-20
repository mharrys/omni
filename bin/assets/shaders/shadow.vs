#version 130

uniform mat4 mvp;

in vec4 vertex_position;

void main()
{
    gl_Position = mvp * vertex_position;
}
