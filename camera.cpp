#include"source/camera.h"

Camera::Camera(float verticalFOV, float nearClip, float farClip)
	: m_VerticalFOV(verticalFOV), m_NearClip(nearClip), m_FarClip(farClip)
{
	m_ForwardDirection = glm::vec3(0, 0, -1);
	m_Position = glm::vec3(0, 0, 6);
	RecalculateProjection();
	RecalculateView();
}

bool Camera::OnUpdate(GLFWwindow* window,float deltaTime)
{
	glfwGetFramebufferSize(window, &m_ViewportWidth, &m_ViewportHeight);
	
	bool moved = false;
	constexpr glm::vec3 upDirection(0.0f, 1.0f, 0.0f);
	glm::vec3 rightDirection = glm::cross(m_ForwardDirection, upDirection);

	float speed = 5.f;
	float velocity = speed * deltaTime;
	//Mouse
	if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS)
	{
		// Hides mouse cursor
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
		// Stores the coordinates of the cursor
		double mouseX;
		double mouseY;
		glfwGetCursorPos(window, &mouseX, &mouseY);
		glm::vec2 mousePos(mouseX, mouseY);

		// Prevents camera from jumping on the first click
		if (firstClick)
		{
			glfwSetCursorPos(window, (m_ViewportWidth / 2), (m_ViewportHeight / 2));
			m_LastMousePosition = glm::vec2(m_ViewportWidth / 2, m_ViewportHeight / 2);
			firstClick = false;
			return false;
		}
		else
		{
			glm::vec2 delta = (mousePos - m_LastMousePosition) * 0.002f;
			m_LastMousePosition = mousePos;

			// Rotation
			if (delta.x != 0.0f || delta.y != 0.0f)
			{
				float pitchDelta = delta.y * GetRotationSpeed();
				float yawDelta = delta.x * GetRotationSpeed();

				glm::quat q = glm::normalize(glm::cross(glm::angleAxis(-pitchDelta, rightDirection),
					glm::angleAxis(-yawDelta, glm::vec3(0.f, 1.0f, 0.0f))));
				m_ForwardDirection = glm::rotate(q, m_ForwardDirection);

				moved = true;
			}
		}
	}
	else if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_RELEASE)
	{
		// Unhides cursor since camera is not looking around anymore
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
		// Makes sure the next time the camera looks around it doesn't jump
		firstClick = true;
	}

	// Keyboard 
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
	{
		m_Position += m_ForwardDirection * velocity;
		moved = true;
	}
	else if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
	{
		m_Position -= m_ForwardDirection * velocity;
		moved = true;
	}
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
	{
		m_Position -= rightDirection * velocity;
		moved = true;
	}
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
	{
		m_Position += rightDirection * velocity;
		moved = true;
	}
	if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
	{
		m_Position -= upDirection * velocity;
		moved = true;
	}
	else if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
	{
		m_Position += upDirection * velocity;
		moved = true;
	}
	else if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window, true);
		return false;
	}

	if (moved)
	{
		RecalculateView();
	}
	return moved;
}

void Camera::OnResize(uint32_t width, uint32_t height)
{
	if (width == m_ViewportWidth && height == m_ViewportHeight)
		return;

	m_ViewportWidth = width;
	m_ViewportHeight = height;

	RecalculateProjection();
}

float Camera::GetRotationSpeed()
{
	return 0.3f;
}

void Camera::RecalculateProjection()
{
	m_Projection = glm::perspectiveFov(glm::radians(m_VerticalFOV), (float)m_ViewportWidth, (float)m_ViewportHeight, m_NearClip, m_FarClip);
	m_InverseProjection = glm::inverse(m_Projection);
}

void Camera::RecalculateView()
{
	m_View = glm::lookAt(m_Position, m_Position + m_ForwardDirection, glm::vec3(0, 1, 0));
	m_InverseView = glm::inverse(m_View);
}


void Camera::ProcessMouseScroll(float yOffSet)
{
	m_VerticalFOV -= (float)yOffSet;
	if (m_VerticalFOV < 1.0f)
		m_VerticalFOV = 1.0f;
	if (m_VerticalFOV > 90.0f)
		m_VerticalFOV = 90.0f;
	RecalculateProjection();
}