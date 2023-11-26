#pragma once
#include"Entity.h"

const std::string cubePath = std::string("source/resources/cube/cube.gltf");
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
    void RenderCubeMap(Shader& shader_cubemap);

    void InitializeCubes(const std::string& filePath);
    std::vector<Entity*>& getCubes(){ return m_cubes;}

    Entity* getSpider() { return m_spider;}

    void UpdateEntityToFitScene(Entity& entity);
    Entity* getBall() { return m_ball; }

    Mesh* InitializeCubemap();

    // bounding box
    void CalculateSceneBounds();
    const BoundingBox& getSceneBounds() const { return m_sceneBounds;}

    void handleCollision();

    std::vector<Entity*> AllObjects();



private:
    std::vector<Entity*> m_cubes;
    Entity* m_spider;
    Entity* m_ball;
    BoundingBox m_sceneBounds;
    Mesh* m_cubemap;
};