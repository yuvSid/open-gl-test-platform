#version 330 core

in vec2 texCoords;

out vec4 fragColor;

uniform vec3 maskColor;
uniform sampler2D texture1;
uniform sampler2D texture2;

void main()
{
	vec4 resTexture = mix( texture(texture1, texCoords), texture(texture2, texCoords), 0.2 );	
	fragColor = resTexture;
}