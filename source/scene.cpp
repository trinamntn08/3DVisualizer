#include"scene.h"

const std::string cubePath = std::string("source/resources/cube/cube.gltf");
const std::string spiderPath = std::string("source/resources/spider/spider.obj");


Scene::Scene(): m_spider(Model(spiderPath,glm::vec3(0.05f)))
{
    m_sceneBounds = BoundingBox();
    InitializeCubes(cubePath);
    CalculateSceneBounds();
    UpdateSpiderPosition();
};

void Scene::InitializeCubes(const std::string &filePath)
{
    m_cubes.clear();
    // Load the cube model from Model class
    Model cubeModel(filePath);
    int numRows = 20; // Number of rows in the square grid
    int numCols = 20; // Number of columns in the square grid
    float spacing = 3.0f; // Spacing between cubes

    // Create 1000 cubes arranged in a square grid
    for (int row = 0; row < numRows; row++) {
        for (int col = 0; col < numCols; col++) {
            Model cube_i = cubeModel;
            // Set the position of the cube
            glm::vec3 position(col * spacing, 0.0f, row * spacing);
            cube_i.SetPosition(position);
            m_cubes.push_back(cube_i);
        }
    }
}

void Scene::CalculateSceneBounds()
{
    // Initialize the scene bounds to an invalid state
    m_sceneBounds.Reset();

    // Iterate through all the cubes and update the scene's bounding box
    for (const Model& cube : m_cubes) 
    {
        const BoundingBox& cubeBounds = cube.GetBoundingBox();
        m_sceneBounds.ExpandToInclude(cubeBounds);
    }
}

void Scene::UpdateSpiderPosition()
{
    // Move spider to center of scene
    glm::vec3 sceneBoundsCenter = m_sceneBounds.GetCenter();
    BoundingBox spider_bbox = m_spider.GetBoundingBox();
    glm::vec3 spiderBoundsCenter = spider_bbox.GetCenter();
    glm::vec3 spiderMove = sceneBoundsCenter - spiderBoundsCenter;

    // Adjust the y-axis of spider to move it above the scene
    float yOffset = m_sceneBounds.GetMaxBounds().y - spider_bbox.GetMinBounds().y;
    spiderMove.y = yOffset;

    m_spider.SetPosition(spiderMove);
}

void Scene::Render(Shader& shader)
{
    // Render cubes
    for (Model& cube : m_cubes) 
    {
        cube.Render(shader);
    }
  //  m_sceneBounds.Render(shader);

    // Render spider and its boundingBox
    m_spider.Render(shader);

    /*BoundingBox spider_Bounds = m_spider.GetBoundingBox();
    spider_Bounds.Render(shader);*/
}
