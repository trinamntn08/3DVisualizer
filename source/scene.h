#pragma once
#include"Entity.h"

const std::string environementPath = std::string("source/resources/cube/cube.gltf");
const std::string spiderPath = std::string("source/resources/spider/spider.obj");
const std::string ballPath = std::string("source/resources/ball/ball.obj");
std::vector<Texture> LoadCubeMapTextures(std::vector<std::string> textures_faces);

class Scene 
{
public:
    Scene();

    void loadScene();
    void OnUpdate(float deltaTime);

    // Render scene
    void RenderObjects(Shader& shader,bool render_BBoxes=false);
    void RenderEnvironment(Shader& shader_environement);

    void InitializeCubes(const std::string& filePath);
    std::vector<Entity*>& getCubes(){ return m_cubes;}

    Entity* getSpider() { return m_spider;}
    Entity* getBall() { return m_ball1; }
    Mesh* InitializeEnvironement();

    void UpdateEntityToFitScene(Entity& entity);
    // bounding box
    void CalculateSceneBounds();
    const BoundingBox& getSceneBounds() const { return m_sceneBounds;}

    void handleCollision();

    std::vector<Entity*> AllObjects();

private:
    std::vector<Entity*> m_cubes;
    Entity* m_ball1;
    Entity* m_ball2;
    Entity* m_spider;
    BoundingBox m_sceneBounds;
    Mesh* m_environement;
};