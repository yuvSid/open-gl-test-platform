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
	Shader( const char* vertexPath, const char* fragmentPath );

	void use();

	void setValue( const std::string& name, bool value );
	void setValue( const std::string& name, int value );
	void setValue( const std::string& name, float value );

	unsigned int getID() { return ID; }

protected:
	void initializeShaders( const char* vertexShader, 
							const char* fragmentShader ) noexcept( false );

	void cleanShadersInitialize( unsigned int vertexShader, unsigned int fragmentShader );
	void compileShader( unsigned int shader, std::vector<char>& shaderSource ) noexcept( false );
	void readShader( const char* fileName, std::vector<char>& lines ) noexcept( false );

private:
	unsigned int ID;
};

