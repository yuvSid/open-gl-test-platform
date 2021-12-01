#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;


out vec2 texCoords;
out vec3 fragPos;
out vec3 normal;

uniform mat4 transform = mat4( 1.0f );
uniform mat4 model = mat4( 1.0f );
uniform mat3 modelNormal = mat3( 1.0f );

void main()
{
	gl_Position = transform * vec4(aPos, 1.0f);
	texCoords = aTexCoords;
	fragPos = vec3( model * vec4( aPos, 1.0f ) );
	normal = modelNormal * aNormal;
}