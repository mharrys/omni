#version 130

uniform mat4 model_view;
uniform mat4 projection;
uniform mat3 nm; // normal matrix
uniform mat4 ls; // light space

in vec4 vertex_position;
in vec3 vertex_normal;

out vec3 position;
out vec3 position_ls; // position in light camera space
out vec3 normal;

void main()
{
    position = (model_view * vertex_position).xyz;
    position_ls = (ls * vertex_position).xyz;
    normal = normalize(nm * vertex_normal);

    gl_Position = projection * model_view * vertex_position;
}
