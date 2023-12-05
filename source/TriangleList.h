//#pragma once
//#include<vector>
//#include <glm/glm.hpp>
//#include "mesh.h"
//class BaseTerrain;
//
//class TriangleList 
//{
//public:
//	TriangleList();
//
//	void CreateTriangleList(int Width, int Depth, const BaseTerrain* pTerrain);
//
//	void Render();
//	void initTerrain(const int nbr_vertices_x=251, const int nbr_vertices_z=251, 
//		float tile_size=0.5);
//
//private:
//
//	struct VertexTerrain 
//	{
//		glm::vec3 Pos;
//
//		void InitVertex(const BaseTerrain* pTerrain, int x, int z);
//	};
//
//	void CreateGLState();
//
//	void PopulateBuffers(const BaseTerrain* pTerrain);
//	void InitVertices(const BaseTerrain* pTerrain, std::vector<VertexTerrain>& Vertices);
//	void InitIndices(std::vector<unsigned int>& Indices);
//
//	int m_width = 0;
//	int m_depth = 0;
//	unsigned int m_vao;
//	unsigned int m_vb;
//	unsigned int m_ib;
//};
