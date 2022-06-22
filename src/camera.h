#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <vector>

// Default camera values
const float YAW = -90.0f;
const float PITCH = 0.0f;
const float SPEED = 2.5f;
const float SENSITIVITY = 0.0001f;
const float ZOOM = 45.0f;


// An abstract camera class that processes input and calculates the corresponding Euler Angles, Vectors and Matrices for use in OpenGL
class Camera
{
public:
	enum Camera_Movement
	{
		FORWARD,
		BACKWARD,
		LEFT,
		RIGHT
	};
	Camera( glm::vec3 position = glm::vec3( 0.0f, 0.0f, 0.0f ),
			glm::vec3 up = glm::vec3( 0.0f, 1.0f, 0.0f ),
			float yaw = YAW, float pitch = PITCH );

	Camera( float posX, float posY, float posZ, float upX,
			float upY, float upZ, float yaw, float pitch );

	glm::mat4 getViewMatrix() const;
	float getFov() const { return glm::radians( Zoom ); }
	glm::vec3 getPosition() const { return Position; }

	void ProcessKeyboard( Camera_Movement direction, short FPS );
	void ProcessMouseMovement( float xpos, float ypos, bool constrainPitch = true );
	void ProcessMouseScroll( float yoffset );

private:
	void updateCameraVectors();

private:
	// camera Attributes
	glm::vec3 Position;
	glm::vec3 Front;
	glm::vec3 Up;
	glm::vec3 Right;
	glm::vec3 WorldUp;
	// euler Angles
	float Yaw;
	float Pitch;
	// camera options
	float MovementSpeed;
	float MouseSensitivity;
	float Zoom;

	// mouse input
	float lastX;
	float lastY;
	bool firstMouseInput = true;
};
