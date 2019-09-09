#type vertex
#version 430

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec2 texcoord;

uniform mat4 mvp;

void main()
{
	gl_Position = mvp * vec4(position, 1.0);
}
#endshader

#type fragment
#version 430

layout(location = 0) out vec4 outColor;

uniform vec3 color;

void main()
{
  outColor = vec4(color.xyz, 1.0);
}
