#include"Entity.h"

Entity::Entity() : m_position(0.0f, 0.0f, 0.0f),
                   m_rotation(0.0f, 0.0f, 0.0f),
                   m_scale(1.0f, 1.0f, 1.0f), 
                   m_model(nullptr) {}

Entity::Entity(const std::string& pathToModel, const glm::vec3& position,
                const glm::vec3& rotation, const glm::vec3& scale) :
                m_position(position), m_rotation(rotation),
                m_scale(scale)
{
    m_model = new Model(pathToModel);
    ComputeBoundingBox();
}

Entity::Entity(Model* model,const glm::vec3& position, 
               const glm::vec3& rotation, const glm::vec3& scale) :
               m_model(model),m_position(position), 
               m_rotation(rotation), m_scale(scale)  {}

void Entity::Render(Shader& shader)
{
    // Update the model matrix based on the position
    glm::mat4 modelMatrix = glm::mat4(1.0f);

    //Apply transtion 
    modelMatrix = glm::translate(modelMatrix, m_position);
    // Apply rotation
    modelMatrix = glm::rotate(modelMatrix, m_rotation.z, glm::vec3(0, 0, 1));
    modelMatrix = glm::rotate(modelMatrix, m_rotation.y, glm::vec3(0, 1, 0));
    modelMatrix = glm::rotate(modelMatrix, m_rotation.x, glm::vec3(1, 0, 0));
    // Apply scaling 
    modelMatrix = glm::scale(modelMatrix, m_scale);

    shader.setMat4("model", modelMatrix);
    m_model->Render(shader);
}
void Entity::Rotate(Shader& shader, float angle, const glm::vec3& rotateAxis)
{
    // Create a rotation matrix
    glm::mat4 model = glm::mat4(1.0f);
    glm::mat4 rotation = glm::rotate(glm::mat4(1.0f), angle, rotateAxis);
    model = rotation * model;
    shader.setMat4("model", model);
}
void Entity::RotateOverTime(float deltaTime, const glm::vec3& rotateAxis)
{
    // Accumulate the elapsed time
    static float accumulatedTime = 0.0f;
    accumulatedTime += deltaTime;

    static float rotationSpeed = 30.0f;
    // Calculate the rotation angle based on time and speed
    float rotationAngle = glm::radians(rotationSpeed * accumulatedTime);
    m_rotation = rotationAngle * rotateAxis;
}
//void Entity::SetPosition(const glm::vec3& position)
//{
//    glm::vec3 deltaPos = position - m_position;
//    m_position = position;
//    UpdateBoundingBox(deltaPos);
//}
void Entity::OnMove(float deltaTime)
{
    const float speed = 0.5f;
    float horizontalMovement = speed * deltaTime;
    glm::vec3 newPos = m_position;
    newPos.x += horizontalMovement;
    // Set the new position for the spider
    SetPosition(newPos);
}

void Entity::ComputeBoundingBox()
{
    if (m_model->meshes.empty())
    {
        return;
    }
    m_modelBounds.Reset();
    // Initialize temporary bounds with the position of the first vertex
    glm::vec3 minBound_temp = m_model->meshes[0].vertices[0].Position + m_position;
    glm::vec3 maxBound_temp = minBound_temp;

    // Iterate over each mesh within the model
    for (const Mesh& mesh : m_model->meshes)
    {
        // Iterate over each vertex within the mesh
        for (const Vertex& vertex : mesh.vertices)
        {
            // Apply the model's position to the vertex
            glm::vec3 vertexPosition = vertex.Position + m_position;

            // Update the temporary bounds more efficiently
            minBound_temp = glm::min(minBound_temp, vertexPosition);
            maxBound_temp = glm::max(maxBound_temp, vertexPosition);
        }
    }

    // Update the model's bounding box with scale
    minBound_temp *= m_scale;
    maxBound_temp *= m_scale;

    m_modelBounds.setMinBound(minBound_temp);
    m_modelBounds.setMaxBound(maxBound_temp);
}
void Entity::UpdateBoundingBox(glm::vec3 deltaPos)
{
    m_modelBounds.Move(deltaPos);
}
const BoundingBox& Entity::GetBoundingBox() const
{
    return m_modelBounds;
}