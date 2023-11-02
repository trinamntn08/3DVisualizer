#include"scene.h"

Scene::Scene(const std::string& filePath) 
{
    m_sceneBounds = BoundingBox();
    InitializeCubes(filePath);
};


void Scene::InitializeCubes(const std::string &filePath)
{
    m_cubes.clear();
    // Load the cube model from Model class
    Model cubeModel(filePath);
    int numRows = 20; // Number of rows in the square grid
    int numCols = 100; // Number of columns in the square grid
    float spacing = 2.0f; // Spacing between cubes

    // Create 1000 cubes arranged in a square grid
    for (int row = 0; row < numRows; row++) {
        for (int col = 0; col < numCols; col++) {
            Model cube_i = cubeModel;
            // Set the position of the cube
            glm::vec3 position(col * spacing, 0.0f, row * spacing);
            cube_i.SetPosition(position);
            m_sceneBounds.ExpandToFit(cube_i);
            m_cubes.push_back(cube_i);
        }
    }
}

void Scene::Render(Shader& shader)
{
    for (Model& cube : m_cubes) {
        cube.Render(shader);
    }
    // TO FIX: boundingbox is not correct yet
   // m_sceneBounds.Render(shader);
}
