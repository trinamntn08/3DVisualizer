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

			m_isMoving = true;
			
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
			m_isMoving = true;
		}
		else if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		{
			m_Position.x -= m_ForwardDirection.x * velocity / smoothFactor;
			m_Position.z -= m_ForwardDirection.z * velocity / smoothFactor;
			m_isMoving = true;
		}
		
		if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		{
			m_Position.x -= rightDirection.x * velocity / smoothFactor;
			m_Position.z -= rightDirection.z * velocity / smoothFactor;
			m_isMoving = true;
		}
		else if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		{
			m_Position.x += rightDirection.x * velocity / smoothFactor;
			m_Position.z += rightDirection.z * velocity / smoothFactor;
			m_isMoving = true;
		}
		
	}
		break;
	case TypeCameraView::ThirdPerson:
	{
		if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		{
			m_Position += m_ForwardDirection * velocity;
			m_isMoving = true;
		}
		else if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		{
			m_Position -= m_ForwardDirection * velocity;
			m_isMoving = true;
		}

		if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		{
			m_Position -= rightDirection * velocity;
			m_isMoving = true;
		}
		else if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		{
			m_Position += rightDirection * velocity;
			m_isMoving = true;
		}

		if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
		{
			m_Position -= upDirection * velocity;
			m_isMoving = true;
		}
		else if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
		{
			m_Position += upDirection * velocity;
			m_isMoving = true;
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
	
	if (glfwGetKey(window, GLFW_KEY_ENTER) == GLFW_PRESS)
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

	if (m_isMoving)
	{
		RecalculateView();
	}
	return m_isMoving;
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

void printMat4(const glm::mat4& matrix) {
	for (int i = 0; i < 4; ++i) {
		for (int j = 0; j < 4; ++j) {
			std::cout << matrix[i][j] << " ";
		}
		std::cout << std::endl;
	}
}
void printVec4(const glm::vec4& vector) 
{
	std::cout << "(" << vector.x << ", " << vector.y << ", " << vector.z << ", " << vector.w << ")" << std::endl;
}
// Simple 4x4 matrix multiplication function
void matrixVectorMultiply(const float matrix[16], const float vector[4], float result[4]) {
	for (int i = 0; i < 4; ++i) {
		result[i] = 0.0f;
		for (int j = 0; j < 4; ++j) {
			result[i] += matrix[i * 4 + j] * vector[j];
		}
	}
}
void mat4ToArray(const glm::mat4& matrix, float array[16]) {
	// Use glm::value_ptr to get the raw array representation of the matrix
	const float* ptr = glm::value_ptr(matrix);

	// Copy the values to the destination array
	for (int i = 0; i < 16; ++i) {
		array[i] = ptr[i];
	}
}
glm::vec2 Camera::ConvertCameraPosToPixel(GLFWwindow* window) 
{
	glm::mat4 modelMatrix(1.0f);
	glm::mat4 mvpMatrix = m_Projection * m_View * modelMatrix;

	float matrix_MVP[16];
	mat4ToArray(mvpMatrix, matrix_MVP);
	float pos[4] = { m_Position.x, m_Position.y, m_Position.z, 1.0f };
	float result[4];
	matrixVectorMultiply(matrix_MVP, pos, result);
	glm::vec4 clipSpacePosition(result[0], result[1], result[2], result[3]);

	// Perspective divide to get normalized device coordinates (NDC)
	glm::vec3 ndcPosition = glm::vec3(clipSpacePosition) / clipSpacePosition.w;

	// Map NDC coordinates to pixel coordinates
	glm::vec2 pixelCoordinates;
	int viewportWidth, viewportHeight;
	glfwGetFramebufferSize(window, &viewportWidth, &viewportHeight);
	pixelCoordinates.x = 0.5f * (ndcPosition.x + 1.0f) * viewportWidth;
	pixelCoordinates.y = 0.5f * (1.0f - ndcPosition.y) * viewportHeight;

	return pixelCoordinates;
}



