#ifndef CAMERA_H
#define CAMERA_H

#define NOMINMAX
#include<glad/glad.h>
#include<glm/glm.hpp>
#include<glm/gtc/matrix_transform.hpp>
#include<glm/gtc/type_ptr.hpp>
#include<glm/gtx/rotate_vector.hpp>
#include<glm/gtx/vector_angle.hpp>
#include<glfw3.h>
#include<vector>
#include"boundingBox.h"


enum Camera_Movement {
    FORWARD,
    BACKWARD,
    LEFT,
    RIGHT
};

enum class TypeCameraView 
{
	FirstPerson=0,
	ThirdPerson=1
};

// Default camera values
const float YAW         = -90.0f;
const float PITCH       =  0.0f;
const float TRANSLATION_SPEED =  250.0f;
const float ROTATION_SPEED = 0.8f;
const float ZOOM        =  45.0f;

class Camera
{
public:
	Camera(TypeCameraView typeView=TypeCameraView::ThirdPerson,
			float verticalFOV=45.0f, float nearClip=0.0f, float farClip=100.f);

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
	void LookAt(const glm::vec3& target);
	void SetPosition(glm::vec3&& pos);
	void SetPosition(glm::vec3& pos);

	glm::vec2 ConvertCameraPosToPixel(GLFWwindow* window);

	int GetViewPortWidth() { return m_ViewportWidth; };
	int GetViewPortHeight() { return m_ViewportHeight; };

	TypeCameraView m_typeView = TypeCameraView::FirstPerson;

	inline bool isMoving() { return m_isMoving; }

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

	bool m_isWireFrame = false;

	bool m_isMoving = false;

	bool m_isOnGroundMoving = false;
};

#endif