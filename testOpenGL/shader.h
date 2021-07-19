#pragma once

#include <glad\glad.h>
#include <glm\glm.hpp>

#include <iostream>
#include <vector>
#include <string>
#include <fstream>

const unsigned int BUFFER_SIZE = 512;

class Shader
{
public:
	Shader( const std::string& vertexPath,
			const std::string& fragmentPath );
	~Shader();

	void use();

	void setValue( const std::string& name, int value );
	void setValue( const std::string& name, float value );
	void setValue( const std::string& name, glm::vec3& value );
	void setValue( const std::string& name, glm::mat4& value, bool isTransformNeeded = GL_FALSE );
	void setValue( const std::string& name, glm::mat3& value, bool isTransformNeeded = GL_FALSE );

	unsigned int getID() { return ID; }

protected:
	void initializeShaders( const std::string& vertexPath,
							const std::string& fragmentPath );
	void compileShader( unsigned int shader, std::vector<char>& shaderSource );
	void readShader( const std::string& fileName, std::vector<char>& lines );

	void cleanShadersInitialize( unsigned int vertexShader, unsigned int fragmentShader ) noexcept;

private:
	unsigned int ID;
};

