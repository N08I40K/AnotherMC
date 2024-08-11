#version 330 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec4 color;
layout (location = 2) in vec2 tex_coord;

out vec4 frag_color;
out vec2 frag_tex_coord;

uniform uniform_data {
    mat4 transform;
};

void main() {
    gl_Position = transform * vec4(position, 1.0);

    frag_color = color;
    frag_tex_coord = tex_coord;
}
