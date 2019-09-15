#version 400

in vec2 pass_tex_coords;
in vec3 surface_normal;
in vec3 to_light_vector;
out vec4 out_colour; 

uniform sampler2D tex_sampler;
uniform vec3 light_colour;

void main()
{
	vec3 u_normal = normalize(surface_normal);
	vec3 u_light = normalize(to_light_vector);
	float vecs_dot = dot(u_normal, u_light); 
	float brightness = max(vecs_dot, 0.0);
	vec3 diffuse = brightness * light_colour;

    out_colour = vec4(diffuse, 1.0f) * texture(tex_sampler, pass_tex_coords);
}
