#version 400

in vec3 position;
in vec2 tex_coords;

out vec2 pass_tex_coords;

uniform mat4 transformation_mat = mat4(1);

void main()
{
    gl_Position = transformation_mat * vec4(position, 1.0);
    pass_tex_coords = tex_coords;
}
