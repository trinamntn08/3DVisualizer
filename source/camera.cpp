#include"camera.h"
#include"Logger.h"

Camera::Camera(TypeCameraView typeView,float verticalFOV, float nearClip, float farClip)
				:m_typeView(typeView), m_VerticalFOV(verticalFOV), 
				 m_NearClip(nearClip), m_FarClip(farClip)
{
	m_ForwardDirection = glm::vec3(0, 0, -1);
	m_Position = glm::vec3(0, 0.0f, 10.0f);
	RecalculateProjection();
	RecalculateView();
}
void Camera::OnResize(int viewport_Width, int viewport_Height)
{ 
	if (m_ViewportWidth != viewport_Width || m_ViewportHeight != viewport_Height)
	{
		m_ViewportWidth = viewport_Width;
		m_ViewportHeight = viewport_Height;
		RecalculateProjection();
	}
}
bool Camera::OnUpdate(GLFWwindow* window,float deltaTime)
{
	int viewport_Width, viewport_Height;
	glfwGetFramebufferSize(window, &viewport_Width, &viewport_Height);
	OnResize(viewport_Width, viewport_Height);
//	bool moved = false;
	constexpr glm::vec3 upDirection(0.0f, 1.0f, 0.0f);
	glm::vec3 rightDirection = glm::cross(m_ForwardDirection, upDirection);

	float velocity = TRANSLATION_SPEED * deltaTime;
	//Mouse
	if(glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS)
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

			}

			m_isMoved = true;
			
		}
	}
	else if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_RELEASE)
	{
		// Unhides cursor since camera is not looking around anymore
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
		// Makes sure the next time the camera looks around it doesn't jump
		firstClick = true;
	}

	// Keyboard 
	switch (m_typeView)
	{
	case TypeCameraView::FirstPerson:
	{
		static const float smoothFactor = 7.0f;
		if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		{
			m_Position.x += m_ForwardDirection.x * velocity/ smoothFactor;
			m_Position.z += m_ForwardDirection.z * velocity / smoothFactor;
			m_isMoved = true;
		}
		else if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		{
			m_Position.x -= m_ForwardDirection.x * velocity / smoothFactor;
			m_Position.z -= m_ForwardDirection.z * velocity / smoothFactor;
			m_isMoved = true;
		}
		if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		{
			m_Position.x -= rightDirection.x * velocity / smoothFactor;
			m_Position.z -= rightDirection.z * velocity / smoothFactor;
			m_isMoved = true;
		}
		if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		{
			m_Position.x += rightDirection.x * velocity / smoothFactor;
			m_Position.z += rightDirection.z * velocity / smoothFactor;
			m_isMoved = true;
		}
	}
		break;
	case TypeCameraView::ThirdPerson:
	{
		if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		{
			m_Position += m_ForwardDirection * velocity;
			m_isMoved = true;
		}
		else if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		{
			m_Position -= m_ForwardDirection * velocity;
			m_isMoved = true;
		}
		if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		{
			m_Position -= rightDirection * velocity;
			m_isMoved = true;
		}
		if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		{
			m_Position += rightDirection * velocity;
			m_isMoved = true;
		}
		if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
		{
			m_Position -= upDirection * velocity;
			m_isMoved = true;
		}
		else if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
		{
			m_Position += upDirection * velocity;
			m_isMoved = true;
		}
	}
		break;
	default:
		break;
	}
	// Swith type of camera View 
	if (glfwGetKey(window, GLFW_KEY_KP_1) == GLFW_PRESS)
	{
		m_typeView = TypeCameraView::FirstPerson;
	}
	else if (glfwGetKey(window, GLFW_KEY_KP_3) == GLFW_PRESS)
	{
		m_typeView = TypeCameraView::ThirdPerson;
	}
	
	if (glfwGetKey(window, GLFW_KEY_C) == GLFW_PRESS)
	{
		m_isWireFrame = !m_isWireFrame;
		if (m_isWireFrame)
		{
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		}
		else
		{
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		}
		
	}
	else if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window, true);
		return false;
	}

	if (m_isMoved)
	{
		RecalculateView();
	}
	return m_isMoved;
}

float Camera::GetRotationSpeed()
{
	return ROTATION_SPEED;
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

void Camera::LookAtBoundingBox(const BoundingBox& boundingBox) 
{
	// Calculate camera position and target to fit the bounding box
	glm::vec3 boundingBoxCenter = boundingBox.GetCenter();
	float boundingBoxRadius = boundingBox.GetBoundingBoxRadius();
	// Raise the camera position along the y-axis to view the object from above
	m_Position = boundingBoxCenter + glm::vec3(0.0f, 0.0f, 1.5* boundingBoxRadius);
	m_ForwardDirection = glm::normalize(boundingBoxCenter - m_Position);

	RecalculateView();
}

void Camera::LookAt(const glm::vec3& target)
{
	// Calculate the new forward direction
	m_ForwardDirection = glm::normalize(target - m_Position);
	RecalculateView();
}

void Camera::SetPosition(glm::vec3&& pos)
{
	m_Position = pos;
	RecalculateView();
}
void Camera::SetPosition(glm::vec3& pos)
{
	m_Position = pos;
	RecalculateView();
}



