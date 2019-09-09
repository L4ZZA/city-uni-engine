#type vertex
#version 430

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec2 texcoord;

out vec3 vFragPos;
out vec3 vNormal;
out vec2 vTexCoords;

uniform mat4 model;
uniform mat4 mvp;

void main()
{
	gl_Position = mvp * vec4(position, 1.0);

  vFragPos = vec3(model * vec4(position, 1.0));
  vNormal = mat3(transpose(inverse(model))) * normal;
  vTexCoords = texcoord;

}

#type fragment
#version 430

layout(location = 0) out vec4 FragColor;

in vec3 vFragPos;
in vec3 vNormal;
in vec2 vTexCoords;

struct Material {
  sampler2D diffuse1;
  sampler2D specular1;
  float shininess;
};

struct Light {
    vec3 position;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

uniform vec3 viewPos;
uniform Material material;
uniform Light light;

void main()
{
    // ambient
    vec3 ambient = light.ambient * texture(material.diffuse1, vTexCoords).rgb;
  	
    // diffuse 
    vec3 norm = normalize(vNormal);
    vec3 lightDir = normalize(light.position - vFragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = light.diffuse * diff * texture(material.diffuse1, vTexCoords).rgb;  
    //vec3 diffuse = texture(material.diffuse1, vTexCoords).rgb; // Temporary

    // specular
    vec3 viewDir = normalize(viewPos - vFragPos);
    vec3 reflectDir = reflect(-lightDir, norm);  
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    vec3 specular = light.specular * spec * texture(material.specular1, vTexCoords).rgb;  
        
    vec3 result = ambient + diffuse + specular;
    FragColor = vec4(result, 1.0);
}
