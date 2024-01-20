#include"SkyDome.h"
#include"..\stb_image.h"
#include"..\model.h"


void SkyDome::Render(Shader& shader)
{
    shader.activate();
    m_sky->RenderSkyDome(shader);
}

void SkyDome::InitSkyDome(unsigned int nbr_rows, unsigned int nbr_cols, float radius)
{
    // Init vertices
    std::vector<Vertex> vertices = InitVertices(nbr_rows, nbr_cols, radius);
    // Init textures
    std::vector<Texture> textures_loaded = LoadTextures(std::vector<std::string>({ path_SkyDome + "skydome.jpg" }));

    m_sky = std::make_unique<Mesh>(vertices, textures_loaded);
}
std::vector<Vertex> SkyDome::InitVertices(int NumRows, int NumCols, float Radius)
{
    int NumVerticesTopStrip = 3 * NumCols;
    int NumVerticesRegularStrip = 6 * NumCols;
 //   m_numVertices = NumVerticesTopStrip + (NumRows - 1) * NumVerticesRegularStrip;

 //   std::vector<Vertex> Vertices(m_numVertices);
    std::vector<Vertex> Vertices;
    float PitchAngle = 90.0f / (float)NumRows;
    float HeadingAngle = 360.0f / (float)NumCols;

    glm::vec3 Apex(0.0f, Radius, 0.0f);

    float Pitch = -90.0f;
    int i = 0;

    for (float Heading = 0.0f; Heading < 360.0f; Heading += HeadingAngle) 
    {
        glm::vec3 pos = Apex;
        Vertices.push_back(CreateVertex(pos));

        glm::vec3 pos1 = InitPtsBySphericalCoords(Radius, Pitch + PitchAngle, Heading + HeadingAngle);
        Vertices.push_back(CreateVertex(pos1));

        glm::vec3 pos2 = InitPtsBySphericalCoords(Radius, Pitch + PitchAngle, Heading);
        Vertices.push_back(CreateVertex(pos2));
    }

    for (Pitch = -90.0f + PitchAngle; Pitch < 0; Pitch += PitchAngle) 
    {
        for (float Heading = 0.0f; Heading < 360.0f; Heading += HeadingAngle) 
        {
            glm::vec3 pos0 = InitPtsBySphericalCoords(Radius, Pitch, Heading);         
            glm::vec3 pos1 = InitPtsBySphericalCoords(Radius, Pitch, Heading + HeadingAngle);
            glm::vec3 pos2 = InitPtsBySphericalCoords(Radius, Pitch + PitchAngle, Heading);
            glm::vec3 pos3 = InitPtsBySphericalCoords(Radius, Pitch + PitchAngle, Heading + HeadingAngle);

            Vertices.push_back(CreateVertex(pos0));
            Vertices.push_back(CreateVertex(pos1));
            Vertices.push_back(CreateVertex(pos2));

            Vertices.push_back(CreateVertex(pos1));
            Vertices.push_back(CreateVertex(pos3));
            Vertices.push_back(CreateVertex(pos2));
        }
    }
    return Vertices;
}
Vertex SkyDome::CreateVertex(const glm::vec3& position) 
{
    Vertex vertex;
    glm::vec3 normalizedPos = glm::normalize(position);
    float u = 0.5f + atan2(normalizedPos.z, normalizedPos.x) / (2.0f * glm::pi<float>());
    float v = 0.5f - asin(normalizedPos.y) / (2.0f * glm::pi<float>());

    vertex.Position = position;
    vertex.Normal = normalizedPos;
    vertex.TexCoords = glm::vec2(u, v);
    return vertex;
}
glm::vec3 SkyDome::InitPtsBySphericalCoords(float Radius, float Pitch, float Heading)
{
    float x = Radius * cosf(glm::radians(Pitch)) * sinf(glm::radians(Heading));
    float y = -Radius * sinf(glm::radians(Pitch));
    float z = Radius * cosf(glm::radians(Pitch)) * cosf(glm::radians(Heading));
    return glm::vec3(x, y, z);
}
std::vector<unsigned int> SkyDome::InitIndices(int NumRows, int NumCols) 
{
    std::vector<unsigned int> indices;

    // Top strip indices
    for (int i = 0; i < NumCols; ++i) {
        indices.push_back(0); // Apex index
        indices.push_back(i + 1);
        indices.push_back((i + 1) % NumCols + 1); // Wrap around
    }

    // Regular strip indices
    for (int row = 1; row < NumRows; ++row) {
        for (int col = 0; col < NumCols; ++col) {
            int base = 1 + (row - 1) * NumCols + col;

            // First triangle
            indices.push_back(base);
            indices.push_back(base + NumCols);
            indices.push_back(base + NumCols + ((col + 1) % NumCols));

            // Second triangle
            indices.push_back(base);
            indices.push_back(base + NumCols + ((col + 1) % NumCols));
            indices.push_back(base + ((col + 1) % NumCols));
        }
    }


    return indices;
}

std::vector<Texture> SkyDome::LoadTextures(std::vector<std::string> textures_skydome)
{
    std::vector<Texture> textures_loaded;
    unsigned int textureID;
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);
    // Load the skydome texture
    int width, height, nrChannels;
    unsigned char* data = stbi_load(textures_skydome[0].c_str(), &width, &height, &nrChannels, 0);
    if (data)
    {
        GLenum format;
        if (nrChannels == 1)
            format = GL_RED;
        else if (nrChannels == 3)
            format = GL_RGB;
        else if (nrChannels == 4)
            format = GL_RGBA;

        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        // Set texture parameters
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_BASE_LEVEL, 0);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glGenerateMipmap(GL_TEXTURE_2D);

        if (glGetError())
        {
            std::cout << "Texture images loaded failed" << std::endl;
        }

        stbi_image_free(data);
    }
    else
    {
        std::cout << "STBI failed to load skydome texture: " << textures_skydome[0] << std::endl;
        glDeleteTextures(1, &textureID);
        stbi_image_free(data);
    }
    
    textures_loaded.push_back(Texture(textureID, "skydome", textures_skydome[0]));
 
    return textures_loaded;
}