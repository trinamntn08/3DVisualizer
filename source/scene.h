#pragma once

#include"PhysicsEngine/Box.h"
#include"SkyBox.h"
#include"Terrain.h"
#include"SkyDome.h"

const std::string cubePath = std::string("source/resources/cube/cube.gltf");
const std::string spiderPath = std::string("source/resources/spider/spider.obj");
const std::string ballPath = std::string("source/resources/ball/ball.obj");

struct PhysicsProperties
{
    bool gravity = true;
    bool collisions = true;
    bool collisionResponse = true;
};
enum class Sky 
{
    SkyBox = 0,
    SkyDome= 1
};

class Scene 
{
public:
    Scene(Sky typeSkye= Sky::SkyDome);
    ~Scene();
    void loadScene();
    void OnUpdate(float deltaTime);

    void InitializeCubes(const std::string& filePath);

    // bounding box
    void CalculateSceneBounds();
    inline const BoundingBox& getSceneBounds() const { return m_sceneBounds;}

    void setGravity(const glm::vec3 gravity) { m_gravity = gravity; }
    glm::vec3 getGravity() const { return m_gravity; }


    // PHYSICS OBJECTS
    inline std::vector<PhysicsObject*> AllPhysicsObjects() { return m_allPhysicsObjects; };
    void UpdateAllObjectsToFitScene();
    void UpdatePhysicsObjectToFitScene(PhysicsObject& object);
    inline int numberOfObjects() { return m_allPhysicsObjects.size();};

    inline Sky typeSky() { return m_typeSky; };


    void Render();
    void RenderPhysicsObjects(Shader & shader, bool isRender_BBoxes = false);
    void RenderSkyBox(Shader& shader_skyBox);
    void RenderSkyDome(Shader& shader_skydome);
    void RenderTerrain(Shader& shader_terrain);

    void ResetScene();
    void ClearScene();


    inline BaseTerrain* getTerrain() { return m_terrain; };
    inline SkyDome* getSkyDome() { return m_skyDome; };
    inline Skybox* getSkyBox() { return m_skyBox; };

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
    Sky m_typeSky = Sky::SkyDome;

    std::vector<PhysicsObject*> m_allPhysicsObjects;
    std::vector<BoxModel*> m_grounds;
    BoundingBox m_sceneBounds;

    Skybox* m_skyBox=nullptr;
    SkyDome* m_skyDome = nullptr;

    BaseTerrain* m_terrain = nullptr;


    glm::vec3 m_gravity = glm::vec3(0.f,-3.0f,0.0f);
};