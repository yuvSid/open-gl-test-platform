#include <glad\glad.h>
#include <GLFW\glfw3.h>

#include <iostream>
#include <vector>
#include <array>
#include <string>

#include "shader.h"

const char* SHADER_VERTEX_FILEPATH = "./shaders/vert.vert";
const char* SHADER_FRAGMENT_FILEPATH = "./shaders/frag.frag";

void framebuffer_size_callback( GLFWwindow* window, int width, int height );
void processInput( GLFWwindow* window );

void initializeWindow(GLFWwindow** pWindow) noexcept(false);
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

	//hardcoded triangles
	std::array<float, 18> vertices = {
			// positions         // colors
		0.5f, -0.5f, 0.0f,  1.0f, 0.0f, 0.0f,   // bottom right
	   -0.5f, -0.5f, 0.0f,  0.0f, 1.0f, 0.0f,   // bottom left
		0.0f,  0.5f, 0.0f,  0.0f, 0.0f, 1.0f    // top
	};
	std::array<unsigned int, 6> indices = {  // note that we start from 0!
		0, 1, 2
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
	glVertexAttribPointer( 0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof( float ), ( void* )0 );
	glEnableVertexAttribArray( 0 );
	glVertexAttribPointer( 1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof( float ), ( void* )( 3 * sizeof( float ) ) );
	glEnableVertexAttribArray( 1 );

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







