#include <glad\glad.h>
#include <GLFW\glfw3.h>

#include <iostream>

void framebuffer_size_callback( GLFWwindow* window, int width, int height );
void processInput( GLFWwindow* window );

int main() 
{
	//initialize and set GLFW	
	glfwInit();
	glfwWindowHint( GLFW_CONTEXT_VERSION_MAJOR, 3 );
	glfwWindowHint( GLFW_CONTEXT_VERSION_MINOR, 3 );
	glfwWindowHint( GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE );

	//create window
	GLFWwindow* window = glfwCreateWindow( 800, 600, "OpenGL TESTS", NULL, NULL );
	if ( !window ) {
		std::cerr << "Falied to create GLFW window!\n";
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent( window );

	//initialize GLAD
	if ( !gladLoadGLLoader( ( GLADloadproc )glfwGetProcAddress ) ) {
		std::cerr << "Failed to initialize GLAD!\n";
		return -1;
	}
	glfwSetFramebufferSizeCallback( window, framebuffer_size_callback );

	glClearColor( 1.f, 1.f, 0.f, 1.f );
	//main loop
	while ( !glfwWindowShouldClose( window ) ) {
		//input
		processInput( window );
		
		//rendering commands
		glClear( GL_COLOR_BUFFER_BIT );

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