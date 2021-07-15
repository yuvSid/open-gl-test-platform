#pragma once

#include <glad\glad.h>

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

	void setValue( const std::string& name, bool value );
	void setValue( const std::string& name, int value );
	void setValue( const std::string& name, float value );

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

