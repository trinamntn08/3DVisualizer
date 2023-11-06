#include "boundingBox.h"
#include"mesh.h"
//#include"model.h"

BoundingBox::BoundingBox()
{
    m_minBounds = glm::vec3(std::numeric_limits<float>::max());
    m_maxBounds = glm::vec3(std::numeric_limits<float>::lowest());
}

//void BoundingBox::ExpandToFit(const Model& model)
//{
//    // Initialize the bounds using the first vertex of the first mesh
//    if (model.meshes.empty() || model.meshes[0].vertices.empty()) {
//        return;
//    }
//
//    // Iterate over each mesh within the model
//    for (const Mesh& mesh : model.meshes) {
//        // Iterate over each vertex within the mesh
//        for (const Vertex& vertex : mesh.vertices) {
//            // Apply the model's position to the vertex
//            glm::vec3 vertexPosition = vertex.Position + model.m_position;
//
//            // Expand the bounds to fit the transformed vertex
//            m_minBounds = glm::min(m_minBounds, vertexPosition);
//            m_maxBounds = glm::max(m_maxBounds, vertexPosition);
//        }
//    }
//}

glm::vec3 BoundingBox::GetCenter() const
{
    return 0.5f * (m_minBounds + m_maxBounds);
}

glm::vec3 BoundingBox::GetDimensions() const
{
    return m_maxBounds - m_minBounds;
}
float BoundingBox::GetBoundingBoxRadius() const
{
    return glm::distance(m_maxBounds, m_minBounds) * 0.5f;
} 


Mesh BoundingBox::toMesh()
{
    // Define the vertices of the bounding box as glm::vec3
    std::vector<Vertex> vertices;
    glm::vec3 vertexPositions[8] = {
         /*7----- 6
          /|      /|
         / |     / |
        3------ 2  |
        |  |    |  |
        |  4----|--5
        | /     | /
        |/      |/
        0------ 1   */
        glm::vec3(m_minBounds.x, m_minBounds.y, m_minBounds.z), //0
        glm::vec3(m_maxBounds.x, m_minBounds.y, m_minBounds.z), //1
        glm::vec3(m_maxBounds.x, m_maxBounds.y, m_minBounds.z), //2
        glm::vec3(m_minBounds.x, m_maxBounds.y, m_minBounds.z), //3
        glm::vec3(m_minBounds.x, m_minBounds.y, m_maxBounds.z), //4
        glm::vec3(m_maxBounds.x, m_minBounds.y, m_maxBounds.z), //5
        glm::vec3(m_maxBounds.x, m_maxBounds.y, m_maxBounds.z), //6
        glm::vec3(m_minBounds.x, m_maxBounds.y, m_maxBounds.z)  //7
    };

    for (auto& v : vertexPositions) {
        Vertex vertex;
        vertex.Position = v;
        // Set other vertex attributes if needed
        vertices.push_back(vertex);
    }

    // Define the indices for drawing the lines of the bounding box
    std::vector<unsigned int> indices =
    {
         /*7------ 6
          /|      /|
         / |     / |
        3------ 2  |
        |  |    |  |
        |  4----|--5
        | /     | /
        |/      |/
        0------ 1   */

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

    // Create a Mesh instance using the vertices and indices
    return Mesh(vertices, indices, std::vector<Texture>());
}

void BoundingBox::Render(Shader& shader)
{
    Mesh bBox_mesh = toMesh();
    // Update the model matrix based on the position
    glm::mat4 modelMatrix = glm::mat4(1.0f);
    shader.setMat4("model", modelMatrix);
    bBox_mesh.Render(shader);
}


void BoundingBox::ExpandToInclude(const BoundingBox& bbox)
{
    // Update the minimum and maximum bounds of the current bounding box
    m_minBounds.x = std::min(m_minBounds.x, bbox.m_minBounds.x);
    m_minBounds.y = std::min(m_minBounds.y, bbox.m_minBounds.y);
    m_minBounds.z = std::min(m_minBounds.z, bbox.m_minBounds.z);

    m_maxBounds.x = std::max(m_maxBounds.x, bbox.m_maxBounds.x);
    m_maxBounds.y = std::max(m_maxBounds.y, bbox.m_maxBounds.y);
    m_maxBounds.z = std::max(m_maxBounds.z, bbox.m_maxBounds.z);
}

void BoundingBox::Reset()
{
    m_minBounds = glm::vec3(std::numeric_limits<float>::max());
    m_maxBounds = glm::vec3(std::numeric_limits<float>::lowest());
}

void BoundingBox::UpdateScale(float scale)
{
}
void BoundingBox::Move(const glm::vec3& movePos)
{
    m_minBounds += movePos;
    m_maxBounds += movePos;
}