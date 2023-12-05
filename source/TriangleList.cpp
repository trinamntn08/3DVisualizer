//#include "TriangleList.h"
//
//#include <stdio.h>
//#include <vector>
//#include "Terrain.h"
//
//
//TriangleList::TriangleList()
//{
//}
//
//void TriangleList::CreateTriangleList(int Width, int Depth, const BaseTerrain* pTerrain)
//{
//	m_width = Width;
//	m_depth = Depth;
//
//	CreateGLState();
//
//	PopulateBuffers(pTerrain);
//
//	glBindVertexArray(0);
//	glBindBuffer(GL_ARRAY_BUFFER, 0);
//	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
//}
//
//
//void TriangleList::CreateGLState()
//{
//	glGenVertexArrays(1, &m_vao);
//
//	glBindVertexArray(m_vao);
//
//	glGenBuffers(1, &m_vb);
//
//	glBindBuffer(GL_ARRAY_BUFFER, m_vb);
//
//	glGenBuffers(1, &m_ib);
//	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ib);
//
//	int POS_LOC = 0;
//
//	size_t NumFloats = 0;
//
//	glEnableVertexAttribArray(POS_LOC);
//	glVertexAttribPointer(POS_LOC, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const void*)(NumFloats * sizeof(float)));
//	NumFloats += 3;
//}
//
//
//void TriangleList::PopulateBuffers(const BaseTerrain* pTerrain)
//{
//	std::vector<VertexTerrain> Vertices;
//	Vertices.resize(m_width * m_depth);
//
//	InitVertices(pTerrain, Vertices);
//
//	std::vector<unsigned int> Indices;
//	int NumQuads = (m_width - 1) * (m_depth - 1);
//	Indices.resize(NumQuads * 6);
//	InitIndices(Indices);
//
//	glBufferData(GL_ARRAY_BUFFER, sizeof(Vertices[0]) * Vertices.size(), &Vertices[0], GL_STATIC_DRAW);
//
//	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(Indices[0]) * Indices.size(), &Indices[0], GL_STATIC_DRAW);
//}
//
//
//void TriangleList::VertexTerrain::InitVertex(const BaseTerrain* pTerrain, int x, int z)
//{
////	float y = pTerrain->GetHeight(x, z);
//
//	float WorldScale = pTerrain->GetWorldScale();
//
////	Pos = glm::vec3(x * WorldScale, y, z * WorldScale);
//	Pos = glm::vec3(x * WorldScale, 0.0f, z * WorldScale);
//}
//
//
//void TriangleList::InitVertices(const BaseTerrain* pTerrain, std::vector<VertexTerrain>& Vertices)
//{
//	int Index = 0;
//
//	for (int z = 0; z < m_depth; z++) {
//		for (int x = 0; x < m_width; x++) {
//			assert(Index < Vertices.size());
//			Vertices[Index].InitVertex(pTerrain, x, z);
//			Index++;
//		}
//	}
//
//	assert(Index == Vertices.size());
//}
//
//
//void TriangleList::InitIndices(std::vector<unsigned int>& Indices)
//{
//	int Index = 0;
//
//	for (int z = 0; z < m_depth - 1; z++) {
//		for (int x = 0; x < m_width - 1; x++) {
//			unsigned int IndexBottomLeft = z * m_width + x;
//			unsigned int IndexTopLeft = (z + 1) * m_width + x;
//			unsigned int IndexTopRight = (z + 1) * m_width + x + 1;
//			unsigned int IndexBottomRight = z * m_width + x + 1;
//
//			// Add top left triangle
//			assert(Index < Indices.size());
//			Indices[Index++] = IndexBottomLeft;
//			assert(Index < Indices.size());
//			Indices[Index++] = IndexTopLeft;
//			assert(Index < Indices.size());
//			Indices[Index++] = IndexTopRight;
//
//			// Add bottom right triangle
//			assert(Index < Indices.size());
//			Indices[Index++] = IndexBottomLeft;
//			assert(Index < Indices.size());
//			Indices[Index++] = IndexTopRight;
//			assert(Index < Indices.size());
//			Indices[Index++] = IndexBottomRight;
//		}
//	}
//
//	assert(Index == Indices.size());
//}
//
//
//void TriangleList::Render()
//{
//	glBindVertexArray(m_vao);
//
//	glDrawElements(GL_TRIANGLES, (m_depth - 1) * (m_width - 1) * 6, GL_UNSIGNED_INT, NULL);
//
//	glBindVertexArray(0);
//}
//
//void TriangleList::initTerrain(const int nbr_vertices_x, const int nbr_vertices_z,
//	float tile_size)
//{
//	// Initialize vertices
//	std::vector<Vertex> vertices;
//	for (int pos_x = 0; pos_x < nbr_vertices_x; ++pos_x)
//	{
//		for (int pos_z = 0; pos_z < nbr_vertices_z; ++pos_z)
//		{
//			//add position
//			float x = pos_x* tile_size;
//			float y = 0.0;
//			float z = -pos_z*tile_size;
//
//
//			//add normal
//			float x_n = 0.0;
//			float y_n = 1.0;
//			float z_n = 0.0;
//
//			//vertices[(i + j * res) * 8] = x; //8 = 3 + 3 + 2, float per point
//			//vertices[(i + j * res) * 8 + 1] = y;
//			//vertices[(i + j * res) * 8 + 2] = z;
//			// 
//			//vertices[(i + j * res) * 8 + 3] = x_n;
//			//vertices[(i + j * res) * 8 + 4] = y_n;
//			//vertices[(i + j * res) * 8 + 5] = z_n;
//			//add texcoords
//			//vertices[(i + j * res) * 8 + 6] = (float)j / (res - 1);
//			//vertices[(i + j * res) * 8 + 7] = (float)(res - i - 1) / (res - 1);
//
//			// NTN
//			Vertex vertex;
//			vertex.Position = glm::vec3(x, y, z);
//			vertex.Normal = glm::vec3(x_n, y_n, z_n);
//	//		vertex.TexCoords = glm::vec2((float)j / (res - 1), (float)(res - i - 1) / (res - 1));
//
//			vertices.push_back(vertex);
//		}
//	}
//
//	// Initialize indices for triangles 
//	// RAW triangles
////	unsigned int num_indices = (nbr_vertices_x - 1) * (nbr_vertices_z * 2) + (nbr_vertices_x - 2) + (nbr_vertices_z - 2);
//	
//	unsigned int num_indices = (nbr_vertices_x - 1) * (nbr_vertices_z - 1) * 6;
//	std::vector<unsigned int> indices(num_indices, 0);
//	int Index = 0;
//
//	for (int z = 0; z < nbr_vertices_z - 1; z++) 
//	{
//		for (int x = 0; x < nbr_vertices_x - 1; x++) 
//		{
//			/*  0 ----- 1 ----- 2
//				| \     | \     |
//				|  \    |   \   |
//				|    \  |     \ |
//				3 ----- 4 ----- 5
//				| \     |  \    |
//				|   \   |   \   |
//				|     \ |     \ |
//				6 ----- 7 ----- 8 */
//
//			unsigned int IndexBottomLeft = z * nbr_vertices_x + x; //0
//			unsigned int IndexTopLeft = (z + 1) * nbr_vertices_x + x; //3
//			unsigned int IndexTopRight = (z + 1) * nbr_vertices_x + x + 1; //4
//			unsigned int IndexBottomRight = z * nbr_vertices_x + x + 1; //1
//
//			// Add top left triangle
//			indices[Index++] = IndexBottomLeft;
//			indices[Index++] = IndexTopLeft;
//			indices[Index++] = IndexTopRight;
//
//			// Add bottom right triangle
//			indices[Index++] = IndexBottomLeft;
//			indices[Index++] = IndexTopRight;
//			indices[Index++] = IndexBottomRight;
//		}
//	}
//
//	std::vector<Texture> textures_loaded;
//	m_terrain = new Mesh(vertices, indices, textures_loaded);
//
//	/*
//	const int ntris = (res - 1) * (res - 1) * 2;
//	int* trisindices = new int[ntris * 3];
//
//	for (int i = 0; i < ntris; i++) 
//	{
//		int trisperrow = 2 * (res - 1);
//		for (int j = 0; j < trisperrow; j++) 
//		{
//			if (!(i % 2)) { //upper triangle
//				int k = i * 3;
//				int triindex = i % trisperrow;
//
//				int row = i / trisperrow;
//				int col = triindex / 2;
//				trisindices[k] = row * res + col;
//				trisindices[k + 1] = ++row * res + col;
//				trisindices[k + 2] = --row * res + ++col;
//			}
//			else {
//				int k = i * 3;
//				int triindex = i % trisperrow;
//
//				int row = i / trisperrow;
//				int col = triindex / 2;
//				trisindices[k] = row * res + ++col;
//				trisindices[k + 1] = ++row * res + --col;
//				trisindices[k + 2] = row * res + ++col;
//			}
//		}
//	}
//	glgenvertexarrays(1, planevao);
//	glgenbuffers(1, planevbo);
//	glgenbuffers(1, planeebo);
//
//	glbindvertexarray(*planevao);
//
//	glbindbuffer(gl_array_buffer, *planevbo);
//	glbufferdata(gl_array_buffer, size * sizeof(float), vertices, gl_static_draw);
//
//	glbindbuffer(gl_element_array_buffer, *planeebo);
//	glbufferdata(gl_element_array_buffer, ntris * 3 * sizeof(unsigned int), trisindices, gl_static_draw);
//
//	glvertexattribpointer(0, 3, gl_float, gl_false, 8 * sizeof(float), (void*)0);
//	glenablevertexattribarray(0);
//	glvertexattribpointer(1, 3, gl_float, gl_false, 8 * sizeof(float), (void*)(3 * sizeof(float)));
//	glenablevertexattribarray(1);
//	glvertexattribpointer(2, 2, gl_float, gl_false, 8 * sizeof(float), (void*)(6 * sizeof(float)));
//	glenablevertexattribarray(2);
//	glbindvertexarray(0);
//
//	delete[] vertices; */
//}
//
