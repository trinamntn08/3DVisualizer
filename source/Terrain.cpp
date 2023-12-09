#include "Terrain.h"
#include"stb_image.h"
#include"Logger.h"
#include"PhysicsEngine/RigidBody.h"

BaseTerrain::BaseTerrain(glm::vec3 scale):m_scale(scale)
{ 
	m_rigidbody = new RigidBody();
	InitTerrain(); 
	ComputeBoundingBox();
}

BaseTerrain::~BaseTerrain()
{
	if (m_terrain)
	{
		delete m_terrain;
	}
}

void BaseTerrain::UpdatePhysics(glm::vec3 gravity, float timeStep)
{
	m_rigidbody->UpdatePhysics(gravity, timeStep);
}


void BaseTerrain::Render(Shader& shader)
{
	shader.activate();
	m_terrain->Render(shader);
}

void BaseTerrain::LoadHeightMapFile(const char* filePath)
{
    //int FileSize = 0;
    //unsigned char* p = (unsigned char*)ReadBinaryFile(pFilename, FileSize);

    //if (FileSize % sizeof(float) != 0) {
    //    printf("%s:%d - '%s' does not contain an whole number of floats (size %d)\n", __FILE__, __LINE__, pFilename, FileSize);
    //    exit(0);
    //}

    //m_terrainSize = (int)sqrtf((float)FileSize / (float)sizeof(float));

    //printf("Terrain size %d\n", m_terrainSize);

    //if ((m_terrainSize * m_terrainSize) != (FileSize / sizeof(float))) {
    //    printf("%s:%d - '%s' does not contain a square height map - size %d\n", __FILE__, __LINE__, pFilename, FileSize);
    //    exit(0);
    //}

    //m_heightMap.InitArray2D(m_terrainSize, m_terrainSize, (float*)p);
}

void BaseTerrain::InitTerrain()
{
	// Initialize vertices
	 std::vector<Vertex> vertices = InitVerticesWithHeightMapFromFile(heightMapFile.c_str(), m_width, m_depth);

	// Initialize indices for triangles 
	unsigned int num_indices = (m_width - 1) * (m_depth - 1) * 6;
	std::vector<unsigned int> indices(num_indices, 0);
	int Index = 0;

	for (int z = 0; z < m_depth - 1; z++)
	{
		for (int x = 0; x < m_width - 1; x++)
		{
			/*  0 ----- 1 ----- 2
				| \     | \     |
				|  \    |   \   |
				|    \  |     \ |
				3 ----- 4 ----- 5
				| \     |  \    |
				|   \   |   \   |
				|     \ |     \ |
				6 ----- 7 ----- 8 */

			unsigned int IndexBottomLeft = z * m_width + x; //0
			unsigned int IndexTopLeft = (z + 1) * m_width + x; //3
			unsigned int IndexTopRight = (z + 1) * m_width + x + 1; //4
			unsigned int IndexBottomRight = z * m_width + x + 1; //1

			// Add top left triangle
			indices[Index++] = IndexBottomLeft;
			indices[Index++] = IndexTopLeft;
			indices[Index++] = IndexTopRight;

			// Add bottom right triangle
			indices[Index++] = IndexBottomLeft;
			indices[Index++] = IndexTopRight;
			indices[Index++] = IndexBottomRight;
		}
	}

	// Update Normals for vertices
	CalculateNormals(vertices, indices);

	// textures
	std::vector<Texture> textures_terrain;

	Texture sand = LoadTerrainTextures("gTextureHeight0", path_terrain_texture + "rdiffuse.jpg");
	Texture grass = LoadTerrainTextures("gTextureHeight1", path_terrain_texture + "sand.jpg");
	Texture rdiffuse = LoadTerrainTextures("gTextureHeight2", path_terrain_texture + "snow.jpg");
	Texture snow = LoadTerrainTextures("gTextureHeight3", path_terrain_texture + "terrainTexture.jpg");
	//Texture rnormal = LoadTerrainTextures("rnormal", path_terrain_texture + "rnormal.jpg");
	//Texture terrainTexture = LoadTerrainTextures("terrainTexture", path_terrain_texture + "terrainTexture.jpg");
//	Texture SamplerTerrain = LoadTerrainTextures("SamplerTerrain", path_terrain_texture + "terrain_surface.png");
//	Texture SamplerTerrain = LoadTerrainTextures("SamplerTerrain", path_terrain_texture + "terrain_surface.png");
	//	Texture SamplerTerrain;

	textures_terrain.push_back(sand);
	textures_terrain.push_back(grass);
	textures_terrain.push_back(rdiffuse);
	textures_terrain.push_back(snow);
	//textures_terrain.push_back(rnormal);
	//textures_terrain.push_back(terrainTexture);
//	textures_terrain.push_back(SamplerTerrain);

	m_terrain = new Mesh(vertices, indices, textures_terrain);
}
void BaseTerrain::UpdateParamsForShaders()
{
	static float Height0 = 64.0f;
	static float Height1 = 128.0f;
	static float Height2 = 192.0f;
	static float Height3 = 256.0f;
}

Texture BaseTerrain::LoadTerrainTextures(std::string name_texture,std::string pathFile_texture)
{
	Texture texture_loaded;
	unsigned int textureID;
	glGenTextures(1, &textureID);

	int width, height, nrComponents;
	unsigned char* data = stbi_load(pathFile_texture.c_str(), &width, &height, &nrComponents, 0);
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
		std::cout << "Texture failed to load at path: " << pathFile_texture << std::endl;
		stbi_image_free(data);
	}

	texture_loaded.id = textureID;
	texture_loaded.type = name_texture;
	texture_loaded.path = pathFile_texture.c_str();

	return texture_loaded;

	/*
	// Load the skybox texture
	for (int i = 0; i < 6; i++)
	{
		int width, height, nrChannels;
		unsigned char* data = stbi_load(textures_terrain[i].c_str(), &width, &height, &nrChannels, 0);
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
			std::cout << "STBI failed to load skyBox texture: " << textures_faces[i] << std::endl;
			stbi_image_free(data);
		}
	}
	textures_loaded.push_back(Texture(textureID, "skyBox", textures_faces[0]));

	return textures_loaded;
	*/
}

std::vector<Vertex> BaseTerrain::InitVerticesWithHeightMapFromFile(const char* imagePath, unsigned int&width, unsigned int& height)
{
	// Initialize vertices
	std::vector<Vertex> vertices;
	int nChannels; 
	unsigned char* data = stbi_load(imagePath, (int*)(&width), (int*)&height, &nChannels, 0);

	if (!data) 
	{
		Log::error("Can not read Height Texture image");
		return {};
	}
	
	static float yScale = 170.0f / 256.0f, yShift = 10.0f;  // apply a scale+shift to the height data
	
	if (nChannels == 1 || nChannels == 3 || nChannels == 4) //RGB || RGBA
	{
		for (int i = 0; i < height; ++i)
		{
			for (int j = 0; j < width; ++j)
			{
				// retrieve texel for (i,j) tex coord
				unsigned char* texel = data + (j + width * i) * nChannels;
				// raw height at coordinate
				unsigned char h = texel[0];

				float x = -(int)height / 2.0f + i;
				float z = -(int)width / 2.0f + j;
				Vertex vertex;
				vertex.Position = glm::vec3(x, (int)h * yScale - yShift, z);
				//Normals will be computed later
				
				float x_n = 0.0;
				float y_n = 1.0;
				float z_n = 0.0;
				vertex.Normal = glm::vec3(x_n, y_n, z_n);
				

				// Add texture coordinates
				float u = (float)j / (width - 1);
				float v = 1.0 - (float)i / (height - 1);
				vertex.TexCoords = glm::vec2(u,v);

				vertices.push_back(vertex);
			}
		}
	}
	stbi_image_free(data);
	return vertices;
}
void BaseTerrain::CalculateNormals(std::vector<Vertex>& Vertices, std::vector<unsigned int>& Indices)
{
	unsigned int Index = 0;

	// Accumulate each triangle normal into each of the triangle vertices
	for (unsigned int i = 0; i < Indices.size(); i += 3) {
		unsigned int Index0 = Indices[i];
		unsigned int Index1 = Indices[i + 1];
		unsigned int Index2 = Indices[i + 2];
		glm::vec3 v1 = Vertices[Index1].Position - Vertices[Index0].Position;
		glm::vec3 v2 = Vertices[Index2].Position - Vertices[Index0].Position;
		glm::vec3 Normal = glm::cross( v1,v2);
		Normal = glm::normalize(Normal);

		Vertices[Index0].Normal += Normal;
		Vertices[Index1].Normal += Normal;
		Vertices[Index2].Normal += Normal;
	}

	// Normalize all the vertex normals
	for (unsigned int i = 0; i < Vertices.size(); i++) 
	{
		Vertices[i].Normal = glm::normalize(Vertices[i].Normal);
	}
}
void BaseTerrain::SetPosition(const glm::vec3& newPosition)
{
	glm::vec3 pos = GetPosition();
	glm::vec3 deltaPos = newPosition - pos;
	PhysicsObject::SetPosition(newPosition);
	UpdateBoundingBox(deltaPos);
}

void BaseTerrain::ComputeBoundingBox()
{
	if (!m_terrain)
	{
		return;
	}
	m_bbox.Reset();
	// Initialize temporary bounds with the position of the first vertex
	glm::vec3 pos = GetPosition();
	glm::vec3 minBound_temp = m_terrain->vertices[0].Position + pos;
	glm::vec3 maxBound_temp = minBound_temp;

	// Iterate over each vertex within the mesh
	for (const Vertex& vertex : m_terrain->vertices)
	{
		// Apply the model's position to the vertex
		glm::vec3 vertexPosition = vertex.Position + pos;

		// Update the temporary bounds more efficiently
		minBound_temp = glm::min(minBound_temp, vertexPosition);
		maxBound_temp = glm::max(maxBound_temp, vertexPosition);
	}

	// Update the model's bounding box with scale
	minBound_temp *= m_scale;
	maxBound_temp *= m_scale;

	m_bbox.setMinBound(minBound_temp);
	m_bbox.setMaxBound(maxBound_temp);
}

void BaseTerrain::UpdateBoundingBox(glm::vec3 deltaPos)
{
	m_bbox.Move(deltaPos);
}


float BaseTerrain::GetHeightForPos(unsigned int x, unsigned int z, std::vector<std::vector<float>> heightMap)
{
	// Check if the height map is not empty
	assert(!heightMap.empty() && !heightMap[0].empty());

	int img_x = x;
	int img_z = z;
	// Check boundaries to avoid accessing out-of-bounds indices
	if (img_z < heightMap.size() && img_x < heightMap[0].size())
	{
		return heightMap[img_z][img_x];
	}
	else
	{
		return 0.0f;
	}
}
