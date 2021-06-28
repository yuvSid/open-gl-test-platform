#version 330 core

in vec3 ourColor;
in vec2 texCoords;

out vec4 fragColor;

uniform vec3 maskColor;
uniform sampler2D defTexture;

void main()
{
	vec3 resColor = mix( vec3( ourColor ), vec3( maskColor ), 0.5f );
		
	fragColor = vec4( resColor, 1.f ) * texture( defTexture, texCoords );
}