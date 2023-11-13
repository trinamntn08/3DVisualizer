#include "Application.h"


Application* CreateApplication()
{
	AppSpecification spec;
	Application* app = new Application(spec);
	return app;
}

Application::Application(const AppSpecification& appSpec):
    m_spec(appSpec),m_camera(Camera(45.0f, 0.1f, 100.0f))
{
    Init();
    InitShader();
    // draw in wireframe
   // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
}

Application::~Application()
{
}

void Application::Init()
{    
    // glfw setting 
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    // glfw window
    m_window = glfwCreateWindow(m_spec.width, m_spec.height, m_spec.name.c_str(), NULL, NULL);
    if (m_window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return;
    }
    // Set the user pointer to the instance of the Application class
    glfwSetWindowUserPointer(m_window, this);

    //Update screen's size
    glfwSetFramebufferSizeCallback(m_window, [](GLFWwindow* window, int width, int height)
    {
        glViewport(0, 0, width, height);
        Application* instance = static_cast<Application*>(glfwGetWindowUserPointer(window));
        instance->m_spec.height = height;
        instance->m_spec.width = width;
        
    });

    glfwSetScrollCallback(m_window, [](GLFWwindow* window, double xoffset, double yoffset) 
    {
        Application* instance = static_cast<Application*>(glfwGetWindowUserPointer(window));
        instance->m_camera.ProcessMouseScroll(static_cast<float>(yoffset));
    });

    glfwSetMouseButtonCallback(m_window, [](GLFWwindow* window, int button, int action, int mods)
    {
        Application* instance = static_cast<Application*>(glfwGetWindowUserPointer(window));
        if (button == GLFW_MOUSE_BUTTON_LEFT)
        {
            double xpos, ypos;
            glfwGetCursorPos(window, &xpos, &ypos);

            if (action == GLFW_PRESS)
            {
                instance->m_mouseHandler.onLeftMouseDown(static_cast<int>(xpos), static_cast<int>(ypos));
            }
            else if (action == GLFW_RELEASE)
            {
                instance->m_mouseHandler.onLeftMouseUp(static_cast<int>(xpos), static_cast<int>(ypos));
            }
        }
    });
    // capture mouse
    glfwSetInputMode(m_window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);


    glfwMakeContextCurrent(m_window);
    // glad: load all OpenGL function pointers
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return;
    }
}

void Application::InitShader()
{
    // tell stb_image.h to flip loaded texture's on the y-axis (before loading model).
    stbi_set_flip_vertically_on_load(true);

    m_shader_scene = Shader("source/shaders/vertex_core.glsl", "source/shaders/fragment_core.glsl");
    m_shader_cubemap = Shader("source/shaders/skybox_vertex.glsl", "source/shaders/skybox_fragment.glsl");
}

void Application::Run()
{
	m_running = true;
    m_scene.loadScene();
    m_camera.LookAtBoundingBox(m_scene.getSceneBounds());

    static int counter = 0;
    // render loop
    while (!glfwWindowShouldClose(m_window))
    {
        MoveObject();
        // per-frame time logic
        float currentFrame = static_cast<float>(glfwGetTime());
        m_frameTime = currentFrame - m_lastFrameTime;
        m_timeStep += m_frameTime;
        m_lastFrameTime = currentFrame;
        counter++; //nbr of frames

        // Update title every second
        if (m_timeStep >= 1.0f)
        {
            // Avoid division by zero
            if (counter > 0)
            {
                // Calculate FPS and milliseconds per frame
                float fps = counter / m_timeStep;
                float msPerFrame = m_timeStep / (counter) * 1000.0f;

                // Creates new title
                std::string FPS = std::to_string(static_cast<int>(fps));
                std::string ms = std::to_string((msPerFrame));
                std::string newTitle = "3D Visualizer - " + FPS + "FPS / " + ms + "ms";

                glfwSetWindowTitle(m_window, newTitle.c_str());
            }

            // Reset counters and timer
            m_timeStep = 0.0f;
            counter = 0;
        }

        m_camera.OnUpdate(m_window, m_frameTime);

        // render
        glClearColor(0.08f, 0.16f, 0.18f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glEnable(GL_DEPTH_TEST);

        // enable shader before setting uniforms
        m_shader_scene.activate();

        // view/projection matrix
        glm::mat4 model = glm::mat4(1.0f);
        glm::mat4 view = m_camera.GetViewMatrix();
        glm::mat4 projection = m_camera.GetProjectionMatrix();
        m_shader_scene.setMat_MVP(model, view, projection);

        // Render Scene's Objects
        m_scene.RenderObjects(m_shader_scene);

        // Render Cubemap
        glDepthFunc(GL_LEQUAL);
        m_shader_cubemap.activate();
        // view/projection transformations
        glm::mat4 model_cubemap = glm::mat4(1.0f);
        glm::mat4 view_cubemap = m_camera.GetViewMatrix();
        glm::mat4 projection_cubemap = m_camera.GetProjectionMatrix();
    //    view_cubemap = glm::mat4(glm::mat3(glm::lookAt(m_camera.GetPosition(), m_camera.GetPosition() + m_camera.GetDirection(), glm::vec3(0, 1, 0))));
    //    projection_cubemap = glm::perspective(glm::radians(45.0f), (float)m_spec.width / m_spec.height, 0.1f, 100.0f);

        m_shader_cubemap.setMat_MVP(model_cubemap, view_cubemap, projection_cubemap);
        m_scene.RenderCubeMap(m_shader_cubemap);

        glDepthFunc(GL_LESS);
        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        // -------------------------------------------------------------------------------
        glfwSwapBuffers(m_window);
        glfwPollEvents();
    }
}

bool Application::RayIntersectsBoundingBox(glm::vec2& mousePos, glm::vec3& intersectPoint)
{
    // Convert mouse position to normalized device coordinates (NDC)
    float ndcX = (2.0f * mousePos.x) / m_camera.GetViewPortWidth() - 1.0f;
    float ndcY = 1.0f - (2.0f * mousePos.y) / m_camera.GetViewPortHeight();

    // Construct the near and far points in clip space
    glm::vec4 nearPoint = glm::vec4(ndcX, ndcY, -1.0f, 1.0f);
    glm::vec4 farPoint = glm::vec4(ndcX, ndcY, 1.0f, 1.0f);

    // Convert the near and far points to view space
    glm::mat4 inverseProjection = m_camera.GetInverseProjectionMatrix();
    glm::mat4 inverseView = m_camera.GetInverseViewMatrix();

    glm::vec4 nearPointView = inverseProjection * nearPoint;
    nearPointView /= nearPointView.w;
    nearPointView = inverseView * nearPointView;

    glm::vec4 farPointView = inverseProjection * farPoint;
    farPointView /= farPointView.w;
    farPointView = inverseView * farPointView;

    // Create the ray
    Ray ray;
    ray.Origin = glm::vec3(nearPointView);
    ray.Direction = glm::normalize(glm::vec3(farPointView - nearPointView));

    BoundingBox bbox = m_scene.getSpider().GetBoundingBox();
    bool hit =  RayIntersectsBoundingBox(ray, bbox, intersectPoint);
    return hit;
}

bool Application::RayIntersectsBoundingBox(const Ray& ray, const BoundingBox& bbox, glm::vec3& intersectPts)
{
    // Perform ray-box intersection tests
    float txmin, txmax, tymin, tymax, tzmin, tzmax;

    // Initialize tmin and tmax to the minimum and maximum possible values
    txmin = (bbox.GetMinBounds().x - ray.Origin.x) / ray.Direction.x;
    txmax = (bbox.GetMaxBounds().x - ray.Origin.x) / ray.Direction.x;
    if (txmin > txmax)
    {
       std::swap(txmin, txmax);
    }

    tymin = (bbox.GetMinBounds().y - ray.Origin.y) / ray.Direction.y;
    tymax = (bbox.GetMaxBounds().y - ray.Origin.y) / ray.Direction.y;
    if (tymin > tymax)
    {
        std::swap(tymin, tymax);
    }

    tzmin = (bbox.GetMinBounds().z - ray.Origin.z) / ray.Direction.z;
    tzmax = (bbox.GetMaxBounds().z - ray.Origin.z) / ray.Direction.z;
    if (tzmin > tzmax)
    {
        std::swap(tzmin, tzmax);
    }

    float tmin = std::max(txmin, std::max(tymin, tzmin));
    float tmax = std::min(txmax, std::min(tymax, tzmax));

    if (txmin > tymax || tymin > txmax)
        return false;
    if (txmin > tzmax || tzmin > txmax)
        return false;

    // Calculate the intersection point
    intersectPts = ray.Origin + tmin * ray.Direction;

    return true;
}

void Application::MoveObject()
{
    static bool isObjectGrabbed = false;
    static glm::vec3 grabOffset;
    // Click on objects
    if (m_mouseHandler.leftButton.isLeftPressed)
    {
        // Get mouse cursor position
        double mouseX, mouseY;
        glfwGetCursorPos(m_window, &mouseX, &mouseY);
        glm::vec2 mousePosition((float)mouseX, (float)mouseY);
        glm::vec3 intersectPoint;
        bool hit = RayIntersectsBoundingBox(mousePosition, intersectPoint);

        if (hit)
        {
            std::cout << "Bounding Box Clicked! " << std::endl;
            if (!isObjectGrabbed)
            {
                // Grab the object on the first press
                isObjectGrabbed = true;
                grabOffset = intersectPoint - m_scene.getSpider().m_position;
            }

            if (isObjectGrabbed)
            {
                // Move the object only if it's grabbed
                glm::vec3 movePos(intersectPoint.x - grabOffset.x, intersectPoint.y - grabOffset.y, intersectPoint.z - grabOffset.z);
                m_scene.getSpider().SetPosition(movePos);
            }

        }
    }
    else
    {
        // Reset isObjectGrabbed when the left mouse button is released
        isObjectGrabbed = false;
    }
}

