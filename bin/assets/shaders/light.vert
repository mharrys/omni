#version 130

uniform mat4 m;   // model matrix
uniform mat4 mv;  // model-view matrix
uniform mat4 mvp; // model-view-projection matrix
uniform mat3 nm;  // normal matrix

uniform mat4 light_view; // light camera view matrix

in vec3 vertex_position;
in vec3 vertex_normal;

out vec3 position;
out vec3 position_ls; // position in light camera space
out vec3 normal;

void main()
{
    position = (mv * vec4(vertex_position, 1.0)).xyz;
    position_ls = (light_view * m * vec4(vertex_position, 1.0)).xyz;
    normal = normalize(nm * vertex_normal);

    gl_Position = mvp * vec4(vertex_position, 1.0);
}
