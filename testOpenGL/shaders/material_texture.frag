//FRAGMENT SHADER
#version 330 core

struct Light {
	vec3 position;

	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};

struct Material {
	sampler2D diffuse;
	sampler2D specular;
	float shininess;
};

in vec2 texCoords;
in vec3 fragPos;
in vec3 normal;

uniform Light light;
uniform Material material;
uniform vec3 viewPos;

out vec4 fragColor;

void main()
{
	vec3 norm = normalize( normal );
	vec3 lightDir = normalize( light.position - fragPos );
	float diff = max( dot( norm, lightDir ), 0.0 );

	vec3 viewDir = normalize( viewPos - fragPos );
	vec3 reflectDir = reflect( -lightDir, norm );
	float spec = pow( max( dot( viewDir, reflectDir ), 0.0 ), material.shininess );

	vec3 ambient = light.ambient * texture( material.diffuse, texCoords ).rgb;
	vec3 diffuse = light.diffuse * diff * texture( material.diffuse, texCoords ).rgb;
	vec3 specular = light.specular * spec * texture( material.specular, texCoords ).rgb;

	vec3 result = ambient + diffuse + specular;
	fragColor = vec4( result, 1.0 );
}