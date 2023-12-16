#pragma once

#include"camera.h"

#include"PhysicsEngine/Box.h"
#include"PhysicsEngine/Plane.h"

#include"SkyBox.h"
#include"Terrain.h"
#include"SkyDome.h"

#include"ShadersManager.h"

#include<memory>

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

    void InitializeBalls(const std::string& filePath);

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

    void Render(ShadersManager& shadersManager, const std::unique_ptr<Camera>& camera);
    void RenderPhysicsObjects(Shader & shader,const std::unique_ptr<Camera>& camera, bool isRender_BBoxes = false);
    void RenderSkyBox(Shader& shader_skyBox, const std::unique_ptr<Camera>& camera);
    void RenderSkyDome(Shader& shader_skydome, const std::unique_ptr<Camera>& camera);
    void RenderPlane(Shader& shader_plane,const std::unique_ptr<Camera>& camera);
    void RenderTerrain(Shader& shader_terrain, const std::unique_ptr<Camera>& camera);
    void RenderTerrainTesselation(Shader& shader_terrain, const std::unique_ptr<Camera>& camera);


    void ResetScene();
    void ClearScene();


    inline std::unique_ptr <BaseTerrain>& getTerrain() { return m_terrain; };
    inline std::unique_ptr <SkyDome>& getSkyDome() { return m_skyDome; };
    inline std::unique_ptr <Skybox>& getSkyBox() { return m_skyBox; };
    inline std::unique_ptr <PlaneModel>& getPlane() { return m_plane; }

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
    PhysicsProperties m_properties= PhysicsProperties(true,true,true);


private:
    Sky m_typeSky = Sky::SkyDome;

    std::unique_ptr<Skybox> m_skyBox=nullptr;
    std::unique_ptr<SkyDome> m_skyDome = nullptr;
    std::unique_ptr<BaseTerrain> m_terrain = nullptr;
    std::unique_ptr <PlaneModel> m_plane = nullptr;

    std::vector<PhysicsObject*> m_allPhysicsObjects;
    std::vector<BoxModel*> m_cubes;

    BoundingBox m_sceneBounds;

    glm::vec3 m_gravity = glm::vec3(0.f,-2.0f,0.0f);

};