#version 330

layout (location = 0) in vec3 position;
layout (location = 1) in vec2 tex_coord;

out vec2 tex_coord_out;

uniform mat4 M;
uniform mat4 V;
uniform mat4 P;
uniform mat4 R;

void main()
{
    gl_Position = P * V * R * M * vec4(position, 1.0);
    tex_coord_out = vec2(tex_coord.x, 1.0f - tex_coord.y);
}
