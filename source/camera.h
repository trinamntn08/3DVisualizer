#ifndef CAMERA_H
#define CAMERA_H

#include<glad/glad.h>
#include<glm/glm.hpp>
#include<glm/gtc/matrix_transform.hpp>
#include<glm/gtc/type_ptr.hpp>
#include<glm/gtx/rotate_vector.hpp>
#include<glm/gtx/vector_angle.hpp>
#include<glfw3.h>
#include<vector>
#include"boundingBox.h"

// Defines several possible options for camera movement. Used as abstraction to stay away from window-system specific input methods
enum Camera_Movement {
    FORWARD,
    BACKWARD,
    LEFT,
    RIGHT
};

// Default camera values
const float YAW         = -90.0f;
const float PITCH       =  0.0f;
const float SPEED       =  20.0f;
const float SENSITIVITY =  200.0f;
const float ZOOM        =  45.0f;

class Camera
{
public:
	Camera(float verticalFOV, float nearClip, float farClip);

	bool OnUpdate(GLFWwindow* window, float deltaTime =0.01);
	void OnResize(int viewport_Width, int viewport_Height);

	const glm::mat4& GetProjectionMatrix() const { return m_Projection; }
	const glm::mat4& GetInverseProjectionMatrix() const { return m_InverseProjection; }
	const glm::mat4& GetViewMatrix() const { return m_View; }
	const glm::mat4& GetInverseViewMatrix() const { return m_InverseView; }

	const glm::vec3& GetPosition() const { return m_Position; }
	const glm::vec3& GetDirection() const { return m_ForwardDirection; }

	float GetRotationSpeed();
	void ProcessMouseScroll(float yOffSet);

	void LookAtBoundingBox(const BoundingBox& boundingBox);

	void SetPosition(glm::vec3&& pos);

private:
	void RecalculateProjection();
	void RecalculateView();

private:
	glm::mat4 m_Projection{ 1.0f };
	glm::mat4 m_View{ 1.0f };
	glm::mat4 m_InverseProjection{ 1.0f };
	glm::mat4 m_InverseView{ 1.0f };

	float m_VerticalFOV = 45.0f;
	float m_NearClip = 0.1f;
	float m_FarClip = 100.0f;

	glm::vec3 m_Position{ 0.0f, 0.0f, 0.0f };
	glm::vec3 m_ForwardDirection{ 0.0f, 0.0f, 0.0f };


	glm::vec2 m_LastMousePosition{ 0.0f, 0.0f };
	bool firstClick = true;

	int m_ViewportWidth = 800, m_ViewportHeight = 600;
};

#endif