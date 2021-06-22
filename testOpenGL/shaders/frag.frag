#version 330 core

in vec3 ourColor;
out vec4 fragColor;

uniform vec3 maskColor;

void main()
{
	vec3 resColor = mix( vec3( ourColor ), vec3( maskColor ), vec3( 0.5f, 0.5f, 0.5f ) );
	fragColor = vec4( resColor, 1.0f );
}