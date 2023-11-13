#pragma once
#include<glad/glad.h>
#include<glfw3.h>
#include<iostream>
#include<string>
#include<glm/glm.hpp>
#include<glm/gtc/matrix_transform.hpp>
#include<glm/gtc/type_ptr.hpp>

#include"shader.h"
#include"camera.h"
#include"model.h"
#include"scene.h"
#include"pickingTexture.h" // Not used yet
#include"mousePicker.h" // Not used yet
#include"traceRay.h"

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

	void Init();
	void InitShader();


	void Run();

	void MoveObject();
	bool RayIntersectsBoundingBox(glm::vec2 &mousePos, glm::vec3& intersectPoint);
	bool RayIntersectsBoundingBox(const Ray& ray, const BoundingBox& bbox,glm::vec3& intersectPts);



private:
	AppSpecification m_spec;

	GLFWwindow* m_window=nullptr;

	bool m_running = false;

	// Time
	float m_timeStep = 0.0f; // For FPS
	float m_frameTime = 0.0f; // For Camera
	float m_lastFrameTime = 0.f;

	//Shaders
	Shader m_shader_scene;
	Shader m_shader_cubemap;
	//Camera
	Camera m_camera;

	// Scene
	Scene m_scene;

	// Mouse
//	MouseButton m_mouse;
	MouseHandler m_mouseHandler;

};

// Called from entrypoint
Application* CreateApplication();