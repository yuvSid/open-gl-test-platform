#include "shader.h"

#include <iterator>
#include <glm\gtc\type_ptr.hpp>

void Shader::cleanShadersInitialize( unsigned int vertexShader, unsigned int fragmentShader ) noexcept
{
	glDeleteShader( vertexShader );
	glDeleteShader( fragmentShader );
}

void Shader::compileShader( unsigned int shader, std::vector<char>& shaderSource )
{
	int success;

	const char* data = shaderSource.data();
	glShaderSource( shader, 1, &data, NULL );
	glCompileShader( shader );
	glGetShaderiv( shader, GL_COMPILE_STATUS, &success );
	if ( !success )
		throw ( shader );
}

void Shader::readShader( const std::string& fileName, std::vector<char>& lines )
{
	lines.clear();

	std::ifstream file( fileName );
	if ( !file.is_open() )
		throw ( std::string( "SHADER::FILE::Can't open file: " ) + std::string( fileName ) );
	std::noskipws( file );
	std::copy( std::istream_iterator<char>( file ),
			   std::istream_iterator<char>(),
			   std::back_inserter( lines ) );
	if ( !lines.size() )
		throw ( std::string( "SHADER::FILE::Nothing was readen, file: " ) + std::string( fileName ) );
	else
		lines.push_back( '\0' );
}

Shader::Shader( const std::string& vertexPath, const std::string& fragmentPath )
{
	initializeShaders( vertexPath, fragmentPath );
}


Shader::~Shader()
{
	int curProgInUse = 0;
	glGetIntegerv( GL_CURRENT_PROGRAM, &curProgInUse );
	if ( curProgInUse == ( int )ID ) {
		std::cerr << "SHADER::DESTRUCTOR::Active program deleted!\n";
		glUseProgram( 0 );
	}
	glDeleteProgram( ID );
}

void Shader::use()
{
	glUseProgram( ID );
}

void Shader::setValue( const std::string& name, int value )
{
	glUniform1i( glGetUniformLocation( ID, name.c_str() ), value );
}

void Shader::setValue( const std::string& name, float value )
{
	glUniform1f( glGetUniformLocation( ID, name.c_str() ), value );

}

void Shader::setValue( const std::string& name, glm::vec3& value )
{
	glUniform3fv( glGetUniformLocation( ID, name.c_str() ), 1, glm::value_ptr( value ) );
}

void Shader::setValue( const std::string& name, glm::mat4& value, bool isTransformNeeded )
{
	glUniformMatrix4fv( glGetUniformLocation( ID, name.c_str() ), 1, isTransformNeeded, glm::value_ptr( value ) );
}

void Shader::setValue( const std::string& name, glm::mat3& value, bool isTransformNeeded )
{
	glUniformMatrix3fv( glGetUniformLocation( ID, name.c_str() ), 1, isTransformNeeded, glm::value_ptr( value ) );
}

void Shader::initializeShaders( const std::string& vertexShaderPath,
								const std::string& fragmentShaderPath )
{
	std::vector<char> shaderSource; //dont forget to manually clean memory allocated inside
	unsigned int vertexShader = glCreateShader( GL_VERTEX_SHADER );
	unsigned int fragmentShader = glCreateShader( GL_FRAGMENT_SHADER );
	int success;
	char infoLog[BUFFER_SIZE];

	try {
		std::cout << vertexShaderPath << " INITIALIZATION!\n";
		readShader( vertexShaderPath, shaderSource );
		compileShader( vertexShader, shaderSource );
		std::cout << "SHADER::COMPILE::Success!\n";
		std::cout << fragmentShaderPath << " INITIALIZATION!\n";
		readShader( fragmentShaderPath, shaderSource );
		compileShader( fragmentShader, shaderSource );
		std::cout << "SHADER::COMPILE::Success!\n";

	}
	catch ( unsigned int& shader ) {
		glGetShaderInfoLog( shader, BUFFER_SIZE, NULL, infoLog );
		cleanShadersInitialize( vertexShader, fragmentShader );
		throw( std::runtime_error( std::string( "SHADER::COMPILE::Failed!\n" )
			   + std::string( infoLog ) ) );
	}
	catch ( std::string& description ) {
		cleanShadersInitialize( vertexShader, fragmentShader );
		throw( std::runtime_error( description ) );
	}

	//create shader programm
	ID = glCreateProgram();
	glAttachShader( ID, vertexShader );
	glAttachShader( ID, fragmentShader );
	glLinkProgram( ID );
	glGetProgramiv( ID, GL_LINK_STATUS, &success );
	if ( !success ) {
		glGetProgramInfoLog( ID, BUFFER_SIZE, NULL, infoLog );
		cleanShadersInitialize( vertexShader, fragmentShader );
		throw( std::runtime_error( std::string( "SHADER::PROGRAMM::Link failed!\n" )
			   + std::string( infoLog ) ) );
	}
	glUseProgram( ID );
	cleanShadersInitialize( vertexShader, fragmentShader );
}


