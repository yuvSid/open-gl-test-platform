#include <glad\glad.h>
#include <GLFW\glfw3.h>
#define STB_IMAGE_IMPLEMENTATION
#include "stb/stb_image.h"
#include <glm\glm.hpp>
#include <glm\gtc\matrix_transform.hpp>
#include <glm\gtc\type_ptr.hpp>

#include <iostream>
#include <vector>
#include <array>
#include <string>

#include "shader.h"

const char* SHADER_VERTEX_FILEPATH = "./shaders/vert.vert";
const char* SHADER_FRAGMENT_FILEPATH = "./shaders/frag.frag";
const std::vector<std::string> TEXTURES_FILENAMES{
	"./resources/textures/container.jpg",
	"./resources/textures/awesomeface.png"
};

void framebuffer_size_callback( GLFWwindow* window, int width, int height );
void processInput( GLFWwindow* window );

void initializeWindow( GLFWwindow** pWindow ) noexcept( false );
void initializeTextures( std::vector<unsigned int>& textures ) noexcept( false );
void setUpWindow( GLFWwindow* window ) noexcept;

void bindTextures( std::vector<unsigned int>& textures ) noexcept;


int main()
{
	GLFWwindow* window = nullptr;
	std::unique_ptr<Shader> defaultShader;
	std::vector<unsigned int> textures;

	try {
		initializeWindow( &window );
		defaultShader.reset( new Shader( SHADER_VERTEX_FILEPATH, SHADER_FRAGMENT_FILEPATH ) );
		initializeTextures( textures );
	}
	catch ( std::exception& e ) {
		std::cerr << e.what() << std::endl;
		std::cerr << "Class: " << typeid( e ).name() << std::endl;
		glfwTerminate();
		return -1;
	}

	setUpWindow( window );
	glEnable( GL_DEPTH_TEST );

	//hardcoded triangles
	std::array vertices{
		-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
		 0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
		 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
		 0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
		 0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
		-0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

		-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
		-0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

		 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
		 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		 0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		 0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		 0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		 0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
		 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
		 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

		-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
		 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
		 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
		-0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
		-0.5f,  0.5f, -0.5f,  0.0f, 1.0f
	};

	std::array positions{
		glm::vec3( 0.0f,  0.0f,  0.0f ),
		glm::vec3( 2.0f,  5.0f, -15.0f ),
		glm::vec3( -1.5f, -2.2f, -2.5f ),
		glm::vec3( -3.8f, -2.0f, -12.3f ),
		glm::vec3( 2.4f, -0.4f, -3.5f ),
		glm::vec3( -1.7f,  3.0f, -7.5f ),
		glm::vec3( 1.3f, -2.0f, -2.5f ),
		glm::vec3( 1.5f,  2.0f, -2.5f ),
		glm::vec3( 1.5f,  0.2f, -1.5f ),
		glm::vec3( -1.3f,  1.0f, -1.5f )
	};

	unsigned int VBO;
	unsigned int VAO;
	unsigned int EBO;
	glGenVertexArrays( 1, &VAO );
	glBindVertexArray( VAO );
	//set vertexes and copy to VRAM
	glGenBuffers( 1, &VBO );
	glBindBuffer( GL_ARRAY_BUFFER, VBO );
	glBufferData( GL_ARRAY_BUFFER, sizeof( vertices ), vertices.data(), GL_STATIC_DRAW );
	//set vertex atributes pointers
	glVertexAttribPointer( 0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof( float ), ( void* )0 );
	glEnableVertexAttribArray( 0 );
	glVertexAttribPointer( 1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof( float ), ( void* )( 3 * sizeof( float ) ) );
	glEnableVertexAttribArray( 1 );

	bindTextures( textures );
	defaultShader->use();
	glUniform1i( glGetUniformLocation( defaultShader->getID(), "texture1" ), 0 );
	glUniform1i( glGetUniformLocation( defaultShader->getID(), "texture2" ), 1 );

	int transformMatLocation = glGetUniformLocation( defaultShader->getID(), "transform" );

	//main loop
	while ( !glfwWindowShouldClose( window ) ) {
		//input
		processInput( window );

		//rendering commands
		glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
		glBindVertexArray( VAO );
		for ( auto& eachPossition : positions ) {
			glm::mat4 trans = glm::mat4( 1.0f );
			glm::mat4 model = glm::translate( glm::mat4( 1.f ), eachPossition );
			model = glm::rotate( model,
								 glm::length( eachPossition ) * ( float )glfwGetTime() * glm::radians( 50.f ),
								 glm::vec3( 0.5f, 1.f, 0.f ) );
			glm::mat4 view = glm::translate( glm::mat4( 1.f ), glm::vec3( 0.f, 0.f, -3.f ) );
			glm::mat4 projection = glm::perspective( glm::radians( 45.f ), 800.f / 600.f, 0.1f, 100.f );
			trans = projection * view * model;

			glUniformMatrix4fv( transformMatLocation, 1, GL_FALSE, glm::value_ptr( trans ) );
			glDrawArrays( GL_TRIANGLES, 0, vertices.size() );
		}

		//check and call events and swap the buffers
		glfwPollEvents();
		glfwSwapBuffers( window );
	}

	glfwTerminate();
	return 0;
}

void framebuffer_size_callback( GLFWwindow* window, int width, int height )
{
	glViewport( 0, 0, width, height );
}

void processInput( GLFWwindow* window )
{
	if ( glfwGetKey( window, GLFW_KEY_ESCAPE ) == GLFW_PRESS )
		glfwSetWindowShouldClose( window, true );
}

void initializeWindow( GLFWwindow** pWindow ) noexcept( false )
{
	//initialize and set GLFW	
	glfwInit();
	glfwWindowHint( GLFW_CONTEXT_VERSION_MAJOR, 3 );
	glfwWindowHint( GLFW_CONTEXT_VERSION_MINOR, 3 );
	glfwWindowHint( GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE );

	//create window
	*pWindow = glfwCreateWindow( 800, 600, "OpenGL TESTS", NULL, NULL );
	if ( !*pWindow )
		throw ( std::runtime_error( std::string( "INIT::GLFW::Falied to create GLFW window!" ) ) );
	glfwMakeContextCurrent( *pWindow );

	//initialize GLAD
	if ( !gladLoadGLLoader( ( GLADloadproc )glfwGetProcAddress ) )
		throw ( std::runtime_error( std::string( "INIT::GLAD::Failed to initialize GLAD!" ) ) );

}

void setUpWindow( GLFWwindow* window ) noexcept
{
	glfwSetFramebufferSizeCallback( window, framebuffer_size_callback );
	glClearColor( 0.5f, 0.5f, 0.5f, 1.f );
}

void processEmptyLoadedTextureData( unsigned char* data )
{
	data = new unsigned char[3];
	std::fill_n( data, 3, 255 );
	glTexImage2D( GL_TEXTURE_2D, 0, GL_RGB, 1, 1, 0, GL_RGB, GL_UNSIGNED_BYTE, data );
	delete[] data;
	data = nullptr;
}

void processCorrectlyLoadedTexureData( const std::string& loadedFileName,
									   int& width, int& height, unsigned char* data )
{
	if ( loadedFileName == TEXTURES_FILENAMES[0] )
		glTexImage2D( GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data );
	else if ( loadedFileName == TEXTURES_FILENAMES[1] )
		glTexImage2D( GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data );
	else
		throw( std::runtime_error( "INIT::TEXTURE::Failes to recognize how to load texture into openGL!" ) );
}

void initializeTextures( std::vector<unsigned int>& textures ) noexcept( false )
{
	const unsigned short textures_count = TEXTURES_FILENAMES.size();

	textures.clear();
	textures.resize( textures_count );
	glGenTextures( textures_count, textures.data() );

	int width, height, nrChannels;
	unsigned char* data;
	stbi_set_flip_vertically_on_load( true );
	for ( size_t i = 0; i < textures_count; i++ ) {
		glBindTexture( GL_TEXTURE_2D, textures[i] );
		glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT );
		glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT );
		glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
		glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );

		data = nullptr;
		try {
			data = stbi_load( TEXTURES_FILENAMES[i].c_str(),
							  &width, &height, &nrChannels, 0 );
			if ( !data )
				throw ( TEXTURES_FILENAMES[i] );
			processCorrectlyLoadedTexureData( TEXTURES_FILENAMES[i], width, height, data );
			glGenerateMipmap( GL_TEXTURE_2D );
			stbi_image_free( data );
		}
		catch ( const std::string& fileName ) {
			std::cerr << "INIT::TEXTURE::Undable to load texture from file: " << fileName << std::endl;
			processEmptyLoadedTextureData( data );
		}
	}
}

void bindTextures( std::vector<unsigned int>& textures ) noexcept
{
	for ( size_t i = 0; i < textures.size(); i++ ) {
		glActiveTexture( GL_TEXTURE0 + i );
		glBindTexture( GL_TEXTURE_2D, textures[i] );
	}
}







