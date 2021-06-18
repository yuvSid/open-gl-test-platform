#include <glad\glad.h>
#include <GLFW\glfw3.h>

#include <iostream>
#include <vector>
#include <array>
#include <fstream>
#include <string>

const char* SHADER_VERTEX_FILEPATH = "./shaders/vert.vert";
const char* SHADER_FRAGMENT_FILEPATH = "./shaders/frag.frag";
const unsigned int BUFFER_SIZE = 512;

void framebuffer_size_callback( GLFWwindow* window, int width, int height );
void processInput( GLFWwindow* window );

void initializeWindow(GLFWwindow** pWindow) noexcept(false);
void setUpWindow( GLFWwindow* window ) noexcept;
void initializeShaders() noexcept(false);

int main() 
{
	GLFWwindow* window = nullptr;	

	try {
		initializeWindow( &window );
		initializeShaders();
	}
	catch ( std::exception& e ) {
		std::cerr << e.what() << std::endl;
		std::cerr << "Class: " << typeid( e ).name() << std::endl;
		glfwTerminate();
		return -1;
	}

	setUpWindow( window );

	//hardcoded triangles
	std::array<float, 12> vertices = {
	 0.5f,  0.5f, 0.0f,  // top right
	 0.5f, -0.5f, 0.0f,  // bottom right
	-0.5f, -0.5f, 0.0f,  // bottom left
	-0.5f,  0.5f, 0.0f   // top left 
	};
	std::array<unsigned int, 6> indices = {  // note that we start from 0!
		0, 1, 3,   // first triangle
		1, 2, 3    // second triangle
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
	glVertexAttribPointer( 0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof( float ), ( void* )0 );
	glEnableVertexAttribArray( 0 );

	//main loop
	while ( !glfwWindowShouldClose( window ) ) {
		//input
		processInput( window );
		
		//rendering commands
		glClear( GL_COLOR_BUFFER_BIT );
		glBindVertexArray( VAO );
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


void cleanShadersInitialize( unsigned int vertexShader, unsigned int fragmentShader)
{
	glDeleteShader( vertexShader );
	glDeleteShader( fragmentShader );
}

void compileShader( unsigned int shader, std::vector<char> &shaderSource ) noexcept( false )
{
	int success;
	
	const char* data = shaderSource.data();
	glShaderSource( shader, 1, &data, NULL );
	glCompileShader( shader );
	glGetShaderiv( shader, GL_COMPILE_STATUS, &success );
	if ( !success )
		throw ( shader );
}

void readShader( const char *fileName, std::vector<char> &lines ) noexcept( false )
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

void initializeShaders() noexcept( false )
{
	std::vector<char> shaderSource; //dont forget to manually clean memory allocated inside
	unsigned int vertexShader = glCreateShader( GL_VERTEX_SHADER );
	unsigned int fragmentShader = glCreateShader( GL_FRAGMENT_SHADER );
	int success;
	char infoLog[BUFFER_SIZE];

	try {
		std::cout << SHADER_VERTEX_FILEPATH << " INITIALIZATION!\n";
		readShader( SHADER_VERTEX_FILEPATH, shaderSource );
		compileShader( vertexShader, shaderSource );
		std::cout << "SHADER::COMPILE::Success!\n";
		std::cout << SHADER_FRAGMENT_FILEPATH << " INITIALIZATION!\n";
		readShader( SHADER_FRAGMENT_FILEPATH, shaderSource );
		compileShader( fragmentShader, shaderSource );
		std::cout << "SHADER::COMPILE::Success!\n";

	}
	catch ( unsigned int &shader ) {
		glGetShaderInfoLog( shader, BUFFER_SIZE, NULL, infoLog );
		cleanShadersInitialize( vertexShader, fragmentShader );
		throw( std::runtime_error( std::string( "SHADER::COMPILE::Failed!\n" ) 
									+ std::string( infoLog ) ) );
	}
	catch ( std::string &description ) { 
		cleanShadersInitialize( vertexShader, fragmentShader );
		throw( std::runtime_error( description ) );
	}

	//create shader programm
	unsigned int shaderProgram;
	shaderProgram = glCreateProgram();
	glAttachShader( shaderProgram, vertexShader );
	glAttachShader( shaderProgram, fragmentShader );
	glLinkProgram( shaderProgram );
	glGetProgramiv( shaderProgram, GL_LINK_STATUS, &success );
	if ( !success ) {
		glGetProgramInfoLog( shaderProgram, BUFFER_SIZE, NULL, infoLog );
		cleanShadersInitialize( vertexShader, fragmentShader );
		throw( std::runtime_error( std::string( "SHADER::PROGRAMM::Link failed!\n" ) 
									+ std::string( infoLog ) ) );
	}
	glUseProgram( shaderProgram );
	cleanShadersInitialize( vertexShader, fragmentShader );
}


