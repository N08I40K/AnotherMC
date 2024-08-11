#version 330 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec2 tex_coord;

layout (row_major) uniform uniform_data {
    mat4 transform;
    mat4 projection;
};

out vec2 frag_tex_coord;

void main() {
    gl_Position = projection * (transform * vec4(position, 1.0));

    frag_tex_coord = tex_coord;
}
