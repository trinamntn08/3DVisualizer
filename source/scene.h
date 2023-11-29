#pragma once
#include"Entity.h"

const std::string environmentPath = std::string("source/resources/cube/cube.gltf");
const std::string spiderPath = std::string("source/resources/spider/spider.obj");
const std::string ballPath = std::string("source/resources/ball/ball.obj");
std::vector<Texture> LoadCubeMapTextures(std::vector<std::string> textures_faces);

struct PhysicsProperties
{
    bool gravity = false;
    bool collisions = true;
    bool collisionResponse = true;
};


class Ball;
class Scene 
{
public:
    Scene() {};
    ~Scene();
    void loadScene();
    void OnUpdate(float deltaTime);

    // Render scene
    void RenderObjects(Shader& shader,bool isRender_BBoxes =false);
    void RenderEnvironment(Shader& shader_environement);

    void InitializeCubes(const std::string& filePath);
    inline std::vector<Entity*>& getCubes(){ return m_cubes;}

    inline Entity* getSpider() { return m_spider;}
    inline Entity* getBall() { return m_ball1; }
    Mesh* InitializeEnvironment();

    void UpdateEntityToFitScene(Entity& entity);

    // bounding box
    void CalculateSceneBounds();
    inline const BoundingBox& getSceneBounds() const { return m_sceneBounds;}

    inline std::vector<Entity*> AllObjects() { return m_allObjects;};


    // PHYSICS OBJECTS
    inline std::vector<PhysicsObject*> AllPhysicsObjects() { return m_allPhysicsObjects; };
    inline void UpdatePhysicsObjectToFitScene(PhysicsObject& entity);
    inline int numberOfObjects() { return m_allPhysicsObjects.size();};

    void RenderPhysicsObjects(Shader & shader, bool isRender_BBoxes = false);

    void ResetScene();
    void ClearScene();


    /**************     COLLISIONS  ****************/
    void checkCollisions();
    // plane
    static bool planeToPlane(PhysicsObject * planeA, PhysicsObject * planeB);
    static bool planeToSphere(PhysicsObject * plane, PhysicsObject * sphere);
    static bool planeToBox(PhysicsObject * plane, PhysicsObject * box);
    // sphere
    static bool sphereToSphere(PhysicsObject * sphereA, PhysicsObject * sphereB);
    static bool sphereToPlane(PhysicsObject * sphere, PhysicsObject * plane);
    static bool sphereToBox(PhysicsObject * sphere, PhysicsObject * box);
    // box
    static bool boxToSphere(PhysicsObject * box, PhysicsObject * sphere);
    static bool boxToPlane(PhysicsObject * box, PhysicsObject * plane);
    static bool boxToBox(PhysicsObject * boxA, PhysicsObject * boxB);
    /************************************************/

    // scene properties
    PhysicsProperties m_properties;

private:
    std::vector<Entity*> m_cubes;
    Entity* m_ball1=nullptr;
    Entity* m_ball2 = nullptr;
    Entity* m_spider = nullptr;
    std::vector<Entity*> m_allObjects;

    BoundingBox m_sceneBounds;
    Mesh* m_environment = nullptr;

    Ball* a_ball1 = nullptr;
    Ball* a_ball2 = nullptr;
    std::vector<PhysicsObject*> m_allPhysicsObjects;
};