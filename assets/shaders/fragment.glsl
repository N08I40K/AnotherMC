#version 330 core

in vec2 frag_tex_coord;

out vec4 color;

uniform sampler2D out_texture;

void main() {
    color = texture(out_texture, frag_tex_coord);
}
