#include"model.h"
#include <glm/gtx/euler_angles.hpp>
Model::Model():gammaCorrection(true), directory(""), m_position(0.0f), m_rotation(0.0f), m_scale(1.0f)
{
    m_modelBounds.Reset();
}
Model::Model( string const& path, const glm::vec3& pos, const glm::vec3& rot, glm::vec3 scale, bool gamma) :
    m_position(pos), m_rotation(glm::radians(rot)), m_scale(scale), gammaCorrection(gamma)
{
    loadModel(path);
    ComputeBoundingBox();
}
Model::Model(const Model& other)
{
    // Perform a deep copy of textures_loaded
    for (const Texture& texture : other.textures_loaded) {
        textures_loaded.push_back(texture);
    }

    // Perform a deep copy of meshes
    for (const Mesh& mesh : other.meshes) {
        meshes.push_back(mesh);
    }

    // Copy other data members
    directory = other.directory;
    gammaCorrection = other.gammaCorrection;
    m_position = other.m_position;
    m_rotation = other.m_rotation;
    m_modelBounds = other.m_modelBounds;
}
Model& Model::operator=(const Model& other) {
    if (this == &other) {
        return *this; // Self-assignment guard
    }

    // Clear current data
    textures_loaded.clear();
    meshes.clear();

    // Perform a deep copy of textures_loaded
    for (const Texture& texture : other.textures_loaded) {
        textures_loaded.push_back(texture);
    }

    // Perform a deep copy of meshes
    for (const Mesh& mesh : other.meshes) {
        meshes.push_back(mesh);
    }

    // Copy other data members
    directory = other.directory;
    gammaCorrection = other.gammaCorrection;
    m_position = other.m_position;
    m_rotation = other.m_rotation;
    m_scale = other.m_scale;
    m_modelBounds = other.m_modelBounds;
    return *this;
}
void Model::Render(Shader& shader)
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

    for (unsigned int i = 0; i < meshes.size(); i++)
        meshes[i].Render(shader);
}

void Model::Rotate(Shader& shader, float angle, const glm::vec3& rotateAxis)
{
    // Create a rotation matrix
    glm::mat4 model = glm::mat4(1.0f);
    glm::mat4 rotation = glm::rotate(glm::mat4(1.0f), angle, rotateAxis);
    model = rotation * model;
    shader.setMat4("model", model);
}
void Model::RotateOverTime(float deltaTime, const glm::vec3& rotateAxis)
{
    // Accumulate the elapsed time
    static float accumulatedTime = 0.0f;
    accumulatedTime += deltaTime;

    static float rotationSpeed = 30.0f;
    // Calculate the rotation angle based on time and speed
    float rotationAngle = glm::radians(rotationSpeed * accumulatedTime);
    m_rotation = rotationAngle* rotateAxis;
}
void Model::SetPosition(const glm::vec3& position)
{
    glm::vec3 deltaPos = position-m_position;
    m_position = position;
    UpdateBoundingBox(deltaPos);
}
void Model::loadModel(string const& path)
{
    // read file via ASSIMP
    Assimp::Importer importer;
    const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_FlipUVs | aiProcess_CalcTangentSpace);
    // check for errors
    if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) // if is Not Zero
    {
        cout << "ERROR::ASSIMP:: " << importer.GetErrorString() << endl;
        return;
    }
    // retrieve the directory path of the filepath
    directory = path.substr(0, path.find_last_of('/'));

    // process ASSIMP's root node recursively
    processNode(scene->mRootNode, scene);
}

void Model::processNode(aiNode* node, const aiScene* scene)
{
    // process each mesh located at the current node
    for (unsigned int i = 0; i < node->mNumMeshes; i++)
    {
        // the node object only contains indices to index the actual objects in the scene. 
        // the scene contains all the data, node is just to keep stuff organized (like relations between nodes).
        aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
        meshes.push_back(processMesh(mesh, scene));
    }
    // after we've processed all of the meshes (if any) we then recursively process each of the children nodes
    for (unsigned int i = 0; i < node->mNumChildren; i++)
    {
        processNode(node->mChildren[i], scene);
    }

}
void Model::OnMove(float deltaTime)
{
    const float speed =0.5f;
    float horizontalMovement = speed * deltaTime;
    glm::vec3 newPos = m_position ;
    newPos.x += horizontalMovement;
    // Set the new position for the spider
    SetPosition(newPos);
}

Mesh Model::processMesh(aiMesh* mesh, const aiScene* scene)
{
    // data to fill
    vector<Vertex> vertices;
    vector<unsigned int> indices;
    vector<Texture> textures;

    // walk through each of the mesh's vertices
    for (unsigned int i = 0; i < mesh->mNumVertices; i++)
    {
        Vertex vertex;
        glm::vec3 vector; // we declare a placeholder vector since assimp uses its own vector class 
        //that doesn't directly convert to glm's vec3 class so we transfer the data to this placeholder glm::vec3 first.
        // positions
        vector.x = mesh->mVertices[i].x;
        vector.y = mesh->mVertices[i].y;
        vector.z = mesh->mVertices[i].z;
        vertex.Position = vector;
        // normals
        if (mesh->HasNormals())
        {
            vector.x = mesh->mNormals[i].x;
            vector.y = mesh->mNormals[i].y;
            vector.z = mesh->mNormals[i].z;
            vertex.Normal = vector;
        }
        // texture coordinates
        if (mesh->mTextureCoords[0]) // does the mesh contain texture coordinates?
        {
            glm::vec2 vec;
            // a vertex can contain up to 8 different texture coordinates. We thus make the assumption that we won't 
            // use models where a vertex can have multiple texture coordinates so we always take the first set (0).
            vec.x = mesh->mTextureCoords[0][i].x;
            vec.y = mesh->mTextureCoords[0][i].y;
            vertex.TexCoords = vec;
            // tangent
            vector.x = mesh->mTangents[i].x;
            vector.y = mesh->mTangents[i].y;
            vector.z = mesh->mTangents[i].z;
            vertex.Tangent = vector;
            // bitangent
            vector.x = mesh->mBitangents[i].x;
            vector.y = mesh->mBitangents[i].y;
            vector.z = mesh->mBitangents[i].z;
            vertex.Bitangent = vector;
        }
        else
            vertex.TexCoords = glm::vec2(0.0f, 0.0f);

        vertices.push_back(vertex);
    }
    // now wak through each of the mesh's faces (a face is a mesh its triangle) and retrieve the corresponding vertex indices.
    for (unsigned int i = 0; i < mesh->mNumFaces; i++)
    {
        aiFace face = mesh->mFaces[i];
        // retrieve all indices of the face and store them in the indices vector
        for (unsigned int j = 0; j < face.mNumIndices; j++)
            indices.push_back(face.mIndices[j]);
    }
    // process materials
    for (unsigned int i = 0; i < scene->mNumMaterials; i++)
    {
       // aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
        aiMaterial* material = scene->mMaterials[i];
        // we assume a convention for sampler names in the shaders. Each diffuse texture should be named
        // as 'texture_diffuseN' where N is a sequential number ranging from 1 to MAX_SAMPLER_NUMBER. 
        // Same applies to other texture as the following list summarizes:
        // diffuse: texture_diffuseN
        // specular: texture_specularN
        // normal: texture_normalN

        // 1. diffuse maps
        vector<Texture> diffuseMaps = loadMaterialTextures(material, aiTextureType_DIFFUSE, "texture_diffuse");
        textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());
        // 2. specular maps
        vector<Texture> specularMaps = loadMaterialTextures(material, aiTextureType_SPECULAR, "texture_specular");
        textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());
        // 3. normal maps
        std::vector<Texture> normalMaps = loadMaterialTextures(material, aiTextureType_HEIGHT, "texture_normal");
        textures.insert(textures.end(), normalMaps.begin(), normalMaps.end());
        // 4. height maps
        std::vector<Texture> heightMaps = loadMaterialTextures(material, aiTextureType_AMBIENT, "texture_height");
        textures.insert(textures.end(), heightMaps.begin(), heightMaps.end());
    }
   
    // return a mesh object created from the extracted mesh data
    return Mesh(vertices, indices, textures);
}

vector<Texture> Model::loadMaterialTextures(aiMaterial* mat, aiTextureType type, string typeName)
{
    vector<Texture> textures;
    for (unsigned int i = 0; i < mat->GetTextureCount(type); i++)
    {
        aiString str;
        mat->GetTexture(type, i, &str);
        // check if texture was loaded before and if so, continue to next iteration: skip loading a new texture
        bool skip = false;
        for (unsigned int j = 0; j < textures_loaded.size(); j++)
        {
            if (std::strcmp(textures_loaded[j].path.data(), str.C_Str()) == 0)
            {
                textures.push_back(textures_loaded[j]);
                skip = true; // a texture with the same filepath has already been loaded, continue to next one. (optimization)
                break;
            }
        }
        if (!skip)
        {   // if texture hasn't been loaded already, load it
            Texture texture;
            texture.id = TextureFromFile(str.C_Str(), this->directory);
            texture.type = typeName;
            texture.path = str.C_Str();
            textures.push_back(texture);
            textures_loaded.push_back(texture);  // store it as texture loaded for entire model, to ensure we won't unnecessary load duplicate textures.
        }
    }
    return textures;
}


unsigned int TextureFromFile(const char* path, const string& directory, bool gamma)
{
    string filename = string(path);
    filename = directory + '/' + filename;

    unsigned int textureID;
    glGenTextures(1, &textureID);

    int width, height, nrComponents;
    unsigned char* data = stbi_load(filename.c_str(), &width, &height, &nrComponents, 0);
    if (data)
    {
        GLenum format;
        if (nrComponents == 1)
            format = GL_RED;
        else if (nrComponents == 3)
            format = GL_RGB;
        else if (nrComponents == 4)
            format = GL_RGBA;

        glBindTexture(GL_TEXTURE_2D, textureID);
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        stbi_image_free(data);
    }
    else
    {
        std::cout << "Texture failed to load at path: " << path << std::endl;
        stbi_image_free(data);
    }

    return textureID;
}

unsigned int TextureForSkyFromFile(const char* path, const string& directory, bool gamma)
{
    string filename = string(path);
    filename = directory + '/' + filename;

    unsigned int textureID;
    glGenTextures(1, &textureID);

    int width, height, nrComponents;
    unsigned char* data = stbi_load(filename.c_str(), &width, &height, &nrComponents, 0);
    if (data)
    {
        GLenum format;
        if (nrComponents == 1)
            format = GL_RED;
        else if (nrComponents == 3)
            format = GL_RGB;
        else if (nrComponents == 4)
            format = GL_RGBA;

        glBindTexture(GL_TEXTURE_2D, textureID);
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        stbi_image_free(data);
    }
    else
    {
        std::cout << "Texture failed to load at path: " << path << std::endl;
        stbi_image_free(data);
    }

    return textureID;
}
void Model::ComputeBoundingBox()
{
    if (meshes.empty()) 
    {
        return;
    }
    m_modelBounds.Reset();
    // Initialize temporary bounds with the position of the first vertex
    glm::vec3 minBound_temp = meshes[0].vertices[0].Position + m_position;
    glm::vec3 maxBound_temp = minBound_temp;

    // Iterate over each mesh within the model
    for (const Mesh& mesh : meshes) 
    {
        // Iterate over each vertex within the mesh
        for (const Vertex& vertex : mesh.vertices) 
        {
            // Apply the model's position to the vertex
            glm::vec3 vertexPosition = vertex.Position  + m_position;

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

void Model::UpdateBoundingBox(glm::vec3 deltaPos)
{
    m_modelBounds.Move(deltaPos);
}

void Model::SetScale(glm::vec3 scale)
{
    m_scale = scale;
}
const glm::vec3& Model::GetScale() const
{
    return m_scale;
}
const BoundingBox& Model::GetBoundingBox() const
{
    return m_modelBounds;
}