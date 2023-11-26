#include"scene.h"


Scene::Scene()
{
};

void Scene::loadScene()
{
    InitializeCubes(cubePath);
    CalculateSceneBounds();
    m_spider = new Entity(spiderPath, glm::vec3(0.0f), glm::vec3(0.0f), glm::vec3(0.02f));
    m_ball = new Entity(ballPath, glm::vec3(0.0f), glm::vec3(0.0f), glm::vec3(1.f));
    UpdateEntityToFitScene(*m_spider);
    UpdateEntityToFitScene(*m_ball);
    m_cubemap = InitializeCubemap();

}

std::vector<Entity*> Scene::AllObjects()
{
    std::vector<Entity*> allObjects;
    allObjects.push_back(m_spider);
    allObjects.push_back(m_ball);

    return allObjects;
}
void Scene::OnUpdate(float deltaTime)
{
    glm::vec3 rotateAxis(0.0f, 0.0f, -1.0f);
    m_ball->RotateOverTime(deltaTime, rotateAxis);
    m_ball->OnMove(deltaTime);
}

void Scene::InitializeCubes(const std::string &filePath)
{
    m_cubes.clear();
    // Load the cube model from Model class
    int numRows = 20; 
    int numCols = 20; 
    float spacing = 2.5f; // Spacing between cubes

    // Create cubes arranged in a square grid
    for (int row = 0; row < numRows; row++) {
        for (int col = 0; col < numCols; col++) {
            Entity* cube_i = new Entity(filePath);
            glm::vec3 position(col * spacing, 0.0f, row * spacing);
            cube_i->SetPosition(position);
            m_cubes.push_back(cube_i);
        }
    }
}

void Scene::CalculateSceneBounds()
{
    m_sceneBounds.Reset();
    for (const Entity* cube : m_cubes) 
    {
        const BoundingBox& cubeBounds = cube->GetBoundingBox();
        m_sceneBounds.ExpandToInclude(cubeBounds);
    }
}
void Scene::UpdateEntityToFitScene(Entity& entity)
{
    // Move spider to center of scene
    glm::vec3 sceneBoundsCenter = m_sceneBounds.GetCenter();
    const BoundingBox& model_bbox = entity.GetBoundingBox();
    glm::vec3 bboxCenter = model_bbox.GetCenter();
    glm::vec3 modelMove = sceneBoundsCenter - bboxCenter;

    // Adjust the y-axis of spider to move it above the scene
    float yOffset = m_sceneBounds.GetMaxBounds().y - model_bbox.GetMinBounds().y;
    modelMove.y = yOffset;

    entity.SetPosition(modelMove);
}
void Scene::RenderObjects(Shader& shader, bool render_BBoxes)
{
    shader.activate();
    for (Entity* cube : m_cubes) 
    {
        cube->Render(shader);
    }
    m_spider->Render(shader);
    m_ball->Render(shader);


    // Render bounding boxes
    if (render_BBoxes)
    {
        for (auto& item : AllObjects())
        {
            BoundingBox bbox_item = item->GetBoundingBox();
            bbox_item.Render(shader);
        }
    }
    
}
void Scene::RenderCubeMap(Shader& shader_cubemap)
{
    shader_cubemap.activate();
    m_cubemap->Render(shader_cubemap);
}
Mesh* Scene::InitializeCubemap()
{
    // cube vertices for vertex buffer object
    std::vector<Vertex> vertices;
    /*7------ 6
     /|      /|
    / |     / |
    3------ 2 |
    | |     | |
    | 4---- |--5
    | /     | /
    |/      |/
    0------ 1   */
    glm::vec3 vertexPositions[8] =
    {
      glm::vec3(-1.0,  1.0,  1.0),
      glm::vec3(-1.0, -1.0,  1.0),
      glm::vec3(1.0, -1.0,  1.0),
      glm::vec3(1.0,  1.0,  1.0),
      glm::vec3(-1.0,  1.0, -1.0),
      glm::vec3(-1.0, -1.0, -1.0),
      glm::vec3(1.0, -1.0, -1.0),
      glm::vec3(1.0,  1.0, -1.0),
    };

    for (auto& v : vertexPositions)
    {
        Vertex vertex;
        vertex.Position = v;
        // Set other vertex attributes if needed
        vertices.push_back(vertex);
    }
    // Flip the vertex data
    for (auto& vertex : vertices) {
        vertex.Position = glm::vec3(vertex.Position.x, -vertex.Position.y, vertex.Position.z);
    }
    // cube indices for index buffer object
    std::vector<unsigned int> indices_orig =
    {
      0, 1, 2, 3, // front 
      3, 2, 6, 7, //top
      7, 6, 5, 4, //back
      4, 5, 1, 0,//bottom
      0, 3, 7, 4,//left
      1, 2, 6, 5,//right
    };
    std::vector<unsigned int> indices =
    {
        // Front
        0, 1, 2,
        0, 2, 3,
        // Back
        4, 5, 6,
        4, 6, 7,
        // Left 
        0, 4, 3,
        3, 4, 7,
        // Right
        1, 5, 2,
        2, 5, 6,
        // Top 
        3, 2, 7,
        2, 6, 7,
        // Bottom
        0, 1, 4,
        1, 5, 4
    };
    // set up the cube map texture
    string directory("source/resources/environment/skybox1/");
    const std::string right("right.png");
    const std::string left("left.png");
    const std::string top("top.png");
    const std::string bottom("bottom.png");
    const std::string front("front.png");
    const std::string back("back.png");

    std::vector<std::string> textures_faces;
    textures_faces.push_back(directory + right);
    textures_faces.push_back(directory + left);
    textures_faces.push_back(directory + top);
    textures_faces.push_back(directory + bottom);
    textures_faces.push_back(directory + back);
    textures_faces.push_back(directory + front);
    std::vector<Texture> textures_loaded = LoadCubeMapTextures(textures_faces);

    return new Mesh(vertices, indices, textures_loaded);
}


std::vector<Texture> LoadCubeMapTextures(std::vector<std::string> textures_faces)
{
    std::vector<Texture> textures_loaded;
    unsigned int textureID;
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);
    // Load the cubemap texture
    for (int i = 0; i < 6; i++)
    {
        int width, height, nrChannels;
        unsigned char* data = stbi_load(textures_faces[i].c_str(), &width, &height, &nrChannels, 0);
        if (data)
        {
            GLenum format;
            if (nrChannels == 1)
                format = GL_RED;
            else if (nrChannels == 3)
                format = GL_RGB;
            else if (nrChannels == 4)
                format = GL_RGBA;

            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
                0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data
            );
            if (glGetError())
            {
                std::cout << "Texture images loaded failed" << std::endl;
            }

            stbi_image_free(data);
        }
        else
        {
            std::cout << "STBI failed to load cubemap texture: " << textures_faces[i] << std::endl;
            stbi_image_free(data);
        }
    }
    textures_loaded.push_back(Texture(textureID, "cubemap", textures_faces[0]));
    // Set texture parameters
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
    return textures_loaded;
}

void Scene::handleCollision()
{
    const BoundingBox& bbox_spider = m_spider->GetBoundingBox();
    // Calculate the displacement vector needed to push the object back
    glm::vec3 displacement = glm::vec3(0.0f);

    // Adjust the displacement based on the axis of collision
    if (bbox_spider.GetMinBounds().x > m_sceneBounds.GetMinBounds().x && 
        bbox_spider.GetMinBounds().x < m_sceneBounds.GetMaxBounds().x) 
    {
        displacement.x = (m_sceneBounds.GetMaxBounds().x - bbox_spider.GetMinBounds().x) > (m_sceneBounds.GetMaxBounds().x - bbox_spider.GetMinBounds().x) ?
                          (m_sceneBounds.GetMaxBounds().x - m_sceneBounds.GetMinBounds().x) : 
                         -(m_sceneBounds.GetMaxBounds().x - bbox_spider.GetMinBounds().x);
    }

    if (bbox_spider.GetMinBounds().y > m_sceneBounds.GetMinBounds().y &&
        bbox_spider.GetMinBounds().y < m_sceneBounds.GetMaxBounds().y)
    {
        displacement.y = (m_sceneBounds.GetMaxBounds().y - bbox_spider.GetMinBounds().y) > (m_sceneBounds.GetMaxBounds().y - bbox_spider.GetMinBounds().y) ?
                         (m_sceneBounds.GetMaxBounds().y - m_sceneBounds.GetMinBounds().y) :
                        -(m_sceneBounds.GetMaxBounds().y - bbox_spider.GetMinBounds().y);
    }

    if (bbox_spider.GetMinBounds().z > m_sceneBounds.GetMinBounds().z &&
        bbox_spider.GetMinBounds().z < m_sceneBounds.GetMaxBounds().z)
    {
        displacement.z = (m_sceneBounds.GetMaxBounds().z - bbox_spider.GetMinBounds().z) > (m_sceneBounds.GetMaxBounds().z - bbox_spider.GetMinBounds().z) ?
            (m_sceneBounds.GetMaxBounds().z - m_sceneBounds.GetMinBounds().z) :
            -(m_sceneBounds.GetMaxBounds().z - bbox_spider.GetMinBounds().z);
    }

    // Apply the displacement to push the object back
    glm::vec3 newPos = m_spider->GetPosition() + displacement;
    m_spider->SetPosition(newPos);

    std::cout << "Collision detected! Object pushed back." << std::endl;
}


