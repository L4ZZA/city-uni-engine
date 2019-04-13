#version 400

in vec3 colour;
in vec2 pass_tex_coords;
out vec4 out_colour;

uniform sampler2D tex_sampler;

void main()
{
    out_colour = texture(tex_sampler, pass_tex_coords);
}
