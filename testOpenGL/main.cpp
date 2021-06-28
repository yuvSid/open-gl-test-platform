#include <glad\glad.h>
#include <GLFW\glfw3.h>
#define STB_IMAGE_IMPLEMENTATION
#include "stb/stb_image.h"

#include <iostream>
#include <vector>
#include <array>
#include <string>

#include "shader.h"

const char* SHADER_VERTEX_FILEPATH = "./shaders/vert.vert";
const char* SHADER_FRAGMENT_FILEPATH = "./shaders/frag.frag";
const std::string WOODEN_CONTAINER_FILEPATH( "./resources/textures/container.jpg" );

void framebuffer_size_callback( GLFWwindow* window, int width, int height );
void processInput( GLFWwindow* window );

void initializeWindow(GLFWwindow** pWindow) noexcept(false);
void initializeTexture();
void setUpWindow( GLFWwindow* window ) noexcept;


int main() 
{
	GLFWwindow* window = nullptr;
	std::unique_ptr<Shader> defaultShader;

	try {
		initializeWindow( &window );
		defaultShader.reset( new Shader( SHADER_VERTEX_FILEPATH, SHADER_FRAGMENT_FILEPATH ) );
	}
	catch ( std::exception& e ) {
		std::cerr << e.what() << std::endl;
		std::cerr << "Class: " << typeid( e ).name() << std::endl;
		glfwTerminate();
		return -1;
	}

	setUpWindow( window );
	initializeTexture();

	//hardcoded triangles
	std::array vertices {
		// positions          // colors           // texture coords
		 0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f,   // top right
		 0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f,   // bottom right
		-0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f,   // bottom left
		-0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 1.0f,   0.0f, 1.0f    // top left
	};
	std::array indices {  // note that we start from 0!
		0, 1, 2,
		0, 2, 3
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
	//set index array and copy to VRAM
	glGenBuffers( 1, &EBO );
	glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, EBO );
	glBufferData( GL_ELEMENT_ARRAY_BUFFER, sizeof( indices ), indices.data(), GL_STATIC_DRAW );
	//set vertex atributes pointers
	glVertexAttribPointer( 0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof( float ), ( void* )0 );
	glEnableVertexAttribArray( 0 );
	glVertexAttribPointer( 1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof( float ), ( void* )( 3 * sizeof( float ) ) );
	glEnableVertexAttribArray( 1 );
	glVertexAttribPointer( 2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof( float ), ( void* )( 6 * sizeof( float ) ) );
	glEnableVertexAttribArray( 2 );

	float prevUpdate = glfwGetTime();
	//main loop
	while ( !glfwWindowShouldClose( window ) ) {
		//input
		processInput( window );
		
		float timeValue = glfwGetTime();
		float redValue = ( sin( timeValue ) / 2.0f ) + 0.5f;
		float greenValue = ( sin( timeValue/2.f ) / 2.0f ) + 0.5f;
		float blueValue = ( sin( timeValue /4.f ) / 2.0f ) + 0.5f;
		int vertexColorLocation = glGetUniformLocation(defaultShader->getID(), "maskColor");
		glUniform3f(vertexColorLocation, redValue, greenValue, blueValue);
		
		//rendering commands
		glClear( GL_COLOR_BUFFER_BIT );
		glBindVertexArray( VAO );
		defaultShader->use();
		glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0 );

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
		throw ( std::runtime_error( std::string( "INIT::GLFW::Falied to create GLFW window!") ) );
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

void initializeTexture()
{
	unsigned int texture;
	glGenTextures( 1, &texture );
	glBindTexture( GL_TEXTURE_2D, texture );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );

	int width, height, nrChannels;
	unsigned char* data = stbi_load( WOODEN_CONTAINER_FILEPATH.c_str(), &width, &height,
									 &nrChannels, 0 );
	if ( !data ) {
		std::cerr << "INIT::TEXTURE::Undable to load texture from file: " << WOODEN_CONTAINER_FILEPATH << std::endl;
		data = new unsigned char[3];
		std::fill_n( data, 3, 255 );
		width = height = 1;
	}

	glTexImage2D( GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data );
	glGenerateMipmap( GL_TEXTURE_2D );
	//TODO clean memory
}







