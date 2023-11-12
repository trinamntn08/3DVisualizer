//#include <glad/glad.h>
//#include <glfw3.h>
//#include <iostream>
//#include <glm/glm.hpp>
//#include <glm/gtc/matrix_transform.hpp>
//#include <glm/gtc/type_ptr.hpp>
//
//#include "shader.h"
//#include "camera.h"
//#include "model.h"
//#include "scene.h"
//#include "pickingTexture.h"
//#include "mousePicker.h"
//
//
//void framebuffer_size_callback(GLFWwindow* window, int width, int height);
//void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
//
//
//// screen
//const unsigned int SCR_WIDTH = 1600;
//const unsigned int SCR_HEIGHT = 1200;
//
//// camera
//Camera camera(45.0f, 0.1f, 100.0f);
//float lastX = SCR_WIDTH / 2.0f;
//float lastY = SCR_HEIGHT / 2.0f;
//bool leftMouseClicked = false;
//float lastFrame_camera = 0.0f;
//
////Mouse Picker
//MousePicker mousePicker(camera);
//
//
//// timing
//float deltaTime = 0.0f;
//float lastFrame = 0.0f;
//int counter = 0;
//
//
//
//int main()
//{
//
//
//
//
//
//    glfwMakeContextCurrent(m_window);
//    glfwSetFramebufferSizeCallback(m_window, framebuffer_size_callback);
//    glfwSetScrollCallback(m_window, scroll_callback);
//    // tell GLFW to capture our mouse
//    glfwSetInputMode(m_window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
//    // tell stb_image.h to flip loaded texture's on the y-axis (before loading model).
//    stbi_set_flip_vertically_on_load(true);
//
//
//    // build and compile shaders
//    // -------------------------
//    Shader ourShader("source/shaders/vertex_core.glsl", "source/shaders/fragment_core.glsl");
//    Shader skyShader("source/shaders/skybox_vertex.glsl", "source/shaders/skybox_fragment.glsl");
//
//    // Load Scene
//    Scene scene;
//
//     camera.LookAtBoundingBox(scene.getSceneBounds());
//
//    // draw in wireframe
//    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
//
//    // render loop
//    // -----------
//    while (!glfwWindowShouldClose(window))
//    {
//        // per-frame time logic
//        // --------------------
//        float currentFrame = static_cast<float>(glfwGetTime());
//        deltaTime = currentFrame - lastFrame; 
//
//        counter++; //nbr of frames
//
//        if (deltaTime >= 1.0f)
//        {
//            // Creates new title
//            std::string FPS = std::to_string(counter);
//            std::string ms = std::to_string((deltaTime / counter) * 1000);
//            std::string newTitle = "3D Visualizer - " + FPS + "FPS / " + ms + "ms";
//            glfwSetWindowTitle(window, newTitle.c_str());
//
//            // Resets times and counter
//            lastFrame = currentFrame;
//            counter = 0;
//        }
//        
//        // Process Input
//        deltaTime = currentFrame - lastFrame_camera;
//        lastFrame_camera = currentFrame;
//        camera.OnUpdate(window, deltaTime);
//        
//
//        // process move object
//        if(glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS)
//        {
//            double mouseX;
//            double mouseY;
//            glfwGetCursorPos(window, &mouseX, &mouseY);
//            mousePicker.setMousePosition(mouseX, mouseY);
//            mousePicker.update();
//            mousePicker.rayIntersectsBoundingBox(scene.getSceneBounds());
//        }
//
//        // render
//        glClearColor(0.08f, 0.16f, 0.18f, 1.0f);
//        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
//
//        // enable shader before setting uniforms
//        glEnable(GL_DEPTH_TEST);
//
//        ourShader.activate();
//
//        // view/projection transformations
//        glm::mat4 model = glm::mat4(1.0f);
//        glm::mat4 view = camera.GetViewMatrix();
//        glm::mat4 projection = camera.GetProjectionMatrix();
//        // Scale object
//        glm::mat4 scaleMatrix = glm::scale(model, glm::vec3(1.0f));
//        model = scaleMatrix * model;
//
//        ourShader.setMat_MVP(model, view, projection);
//
//        //Rotate model
//        //float currentTime = static_cast<float>(glfwGetTime());
//        //float rotationSpeed = 45.0f;  // Adjust the rotation speed as needed
//        //float rotationAngle = glm::radians(rotationSpeed * currentTime);
//        //ourModel.Rotate(ourShader, rotationAngle, glm::vec3(0.0f, 1.0f, 0.0f));
//
//
//        // Render Scene's Objects
//         scene.RenderObjects(ourShader);
//
//         // Render CubeMap
//         glDepthFunc(GL_LEQUAL);
//         skyShader.activate();
//         // view/projection transformations
//         glm::mat4 model_cubemap = glm::mat4(1.0f);
//         glm::mat4 view_cubemap = camera.GetViewMatrix();
//         glm::mat4 projection_cubemap = glm::mat4(1.0f);
//         view_cubemap = glm::mat4(glm::mat3(glm::lookAt(camera.GetPosition(), camera.GetPosition() + camera.GetDirection(), glm::vec3(0, 1, 0))));
//         projection_cubemap = glm::perspective(glm::radians(45.0f), (float)SCR_WIDTH / SCR_HEIGHT, 0.1f, 100.0f);
//         // Scale object
//    //     glm::mat4 scaleMatrix_cubemap = glm::scale(model_cubemap, glm::vec3(1.0f));
//    //     model_cubemap = scaleMatrix_cubemap * model_cubemap;
//         skyShader.setMat_MVP(model_cubemap, view_cubemap, projection_cubemap);
//         scene.RenderCubeMap(skyShader);
//
//        glDepthFunc(GL_LESS);
//        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
//        // -------------------------------------------------------------------------------
//        glfwSwapBuffers(window);
//        glfwPollEvents();
//    }
//
//    // glfw: terminate, clearing all previously allocated GLFW resources.
//    // ------------------------------------------------------------------
//    glfwTerminate();
//    return 0;
//}
//// glfw: whenever the window size changed (by OS or user resize) this callback function executes
//// ---------------------------------------------------------------------------------------------
//void framebuffer_size_callback(GLFWwindow* window, int width, int height)
//{
//    glViewport(0, 0, width, height);
//}
//
//// glfw: whenever the mouse scroll wheel scrolls, this callback is called
//// ----------------------------------------------------------------------
//void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
//{
//    camera.ProcessMouseScroll(static_cast<float>(yoffset));
//}
