#version 330

layout (location = 0) in vec3 position;
layout (location = 1) in vec2 tex_coord;

out vec2 tex_coord_out;

void main()
{
    gl_Position = vec4(position, 1.0);
    tex_coord_out = vec2(tex_coord.x, 1.0f - tex_coord.y);
}
