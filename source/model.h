#ifndef MODEL_H
#define MODEL_H

#include <glad/glad.h> 

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "stb_image.h"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "mesh.h"
#include "shader.h"
#include"boundingBox.h"

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <map>
#include <vector>

unsigned int TextureFromFile(const char* path, const string& directory, bool gamma=true);
unsigned int TextureForSkyFromFile(const char* path, const string& directory, bool gamma);

class Model 
{
public:
    // model data 
    std::vector<Texture> textures_loaded;	// stores all the textures loaded so far, optimization to make sure textures aren't loaded more than once.
    std::vector<Mesh>    meshes;
    string directory;
    glm::vec3 m_position= glm::vec3(0.0f);
    glm::vec3 m_rotation = glm::vec3(0.0f);
    glm::vec3 m_scale = glm::vec3(1.0f);
    bool gammaCorrection=true;
    BoundingBox m_modelBounds;
    Model();
    // constructor, expects a filepath to a 3D model.
    Model(string const& path, const glm::vec3& pos = glm::vec3(0.0f), const glm::vec3& rot = glm::vec3(0.0f),
           glm::vec3 scale = glm::vec3(1.0f), bool gamma = true);
    // Copy constructor
    Model(const Model& other);
    // Copy assignment operator
    Model& operator=(const Model& other);

    // draws the model, and thus all its meshes
    void Render(Shader& shader);
    void Rotate(Shader& shader, float angle, const glm::vec3& axis);
    // Set the position of the model
    void SetPosition(const glm::vec3& position);

    void SetScale(glm::vec3 scale);

    const glm::vec3& GetScale() const;

    void ComputeBoundingBox(); // initialization
    void UpdateBoundingBox(glm::vec3 deltaPos); // update when event changed

    const BoundingBox& GetBoundingBox() const ;

    void Move(glm::vec3& newPos);

private:
    // loads a model with supported ASSIMP extensions from file and stores the resulting meshes in the meshes vector.
    void loadModel(string const& path);

    // processes a node in a recursive fashion. Processes each individual mesh located at the node and repeats this process on its children nodes (if any).
    void processNode(aiNode* node, const aiScene* scene);

    Mesh processMesh(aiMesh* mesh, const aiScene* scene);

    // checks all material textures of a given type and loads the textures if they're not loaded yet.
    // the required info is returned as a Texture struct.
    vector<Texture> loadMaterialTextures(aiMaterial* mat, aiTextureType type, string typeName);

 
};

#endif