#version 400

in vec3 position;
in vec2 tex_coords;
in vec3 normal;

out vec2 pass_tex_coords;
out vec3 surface_normal;
out vec3 to_light_vector;

uniform mat4 transformation_mat = mat4(1);
uniform mat4 projection_mat = mat4(1);
uniform mat4 view_mat = mat4(1);
uniform vec3 light_pos;

void main()
{
	vec4 world_position = transformation_mat * vec4(position, 1.0);
    gl_Position = projection_mat * view_mat * world_position;
    pass_tex_coords = tex_coords;

	surface_normal = (transformation_mat * vec4(normal, 0.0)).xyz;
	to_light_vector = light_pos - world_position.xyz;
}
