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
#include"pickingTexture.h"
#include"mousePicker.h"
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

	bool RayIntersectsBoundingBox(glm::vec2 &mousePos);

	bool RayIntersectsBoundingBox(const Ray& ray, const BoundingBox& bbox);



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

};
// Called from entrypoint
Application* CreateApplication();