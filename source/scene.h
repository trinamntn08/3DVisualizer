#pragma once
#include"model.h"
#include"boundingBox.h"

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
    void RenderObjects(Shader& shader);
    void RenderCubeMap(Shader& shader_cubemap);

    void InitializeCubes(const std::string& filePath);
    std::vector<Model>& getCubes(){ return m_cubes;}

    Model& getSpider() { return m_spider;}

    void UpdateModelToFitScene(Model& model);
    Model& getBall() { return m_ball; }

    Mesh InitializeCubemap();

    // bounding box
    void CalculateSceneBounds();
    const BoundingBox& getSceneBounds() const { return m_sceneBounds;}

    void handleCollision();

    std::vector<Model*> AllObjects();



private:
    std::vector<Model> m_cubes;
    Model m_spider;
    Model m_ball;
    BoundingBox m_sceneBounds;
    Mesh m_cubemap;
};