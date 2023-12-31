#pragma once
#define NOMINMAX
#include<glad/glad.h>
#include<glfw3.h>

#include<iostream>
#include<string>
#include<glm/glm.hpp>
#include<glm/gtc/matrix_transform.hpp>
#include<glm/gtc/type_ptr.hpp>

#include"Shader.h"
#include"Camera.h"
#include"Scene.h"
#include"pickingTexture.h" // Not used yet
#include"mousePicker.h" // Not used yet
#include"traceRay.h"
#include"ShadersManager.h"

#include<memory>

struct AppSpecification
{
	std::string name = "NTN";
	unsigned int width = 1600;
	unsigned int height = 900;

};

class Application
{
public:

	Application(const AppSpecification& appSpec=AppSpecification());
	~Application();

	void InitGraphicEnvironment();
	void InitShader();

	void ConfigCamera();

	void Run();

	void DisplayFPS(float currentTime);


	void MoveObjects();
	bool RayIntersectsBoundingBox(glm::vec2 &mousePos,const BoundingBox& bbox, glm::vec3& intersectPoint);
	bool RayIntersectsBoundingBox(const Ray& ray, const BoundingBox& bbox,glm::vec3& intersectPts);


private:
	AppSpecification m_spec;

	GLFWwindow* m_window=nullptr;

	bool m_running = false;

	// Time
	float m_timeStep = 0.0f; // For FPS
	float m_frameTime = 0.0f; // For Camera
	float m_lastFrameTime = 0.f;

	ShadersManager m_shadersManager;

	std::unique_ptr<Camera> m_camera;
	std::unique_ptr<Scene> m_scene;

	// Mouse
	MouseHandler m_mouseHandler;

};

// Called from entrypoint
Application* CreateApplication();