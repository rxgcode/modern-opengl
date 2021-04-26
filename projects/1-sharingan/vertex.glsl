#version 430 core

layout (location = 0) in vec2 position;

out vec4 the_color;

uniform float u_time;
uniform vec2 u_resolution;
uniform mat4 mv_matrix;
uniform vec4 color;

void main(void) {
    gl_Position = mv_matrix * vec4(position, 0.0, 1.0);
    the_color = color;
}
