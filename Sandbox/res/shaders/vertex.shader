#version 400

in vec3 position;
in vec2 tex_coords;

out vec2 pass_tex_coords;


void main()
{
    gl_Position = vec4(position, 1.0);
    pass_tex_coords = tex_coords;
}
