
#ifndef _IC_MESH_
#define _IC_MESH_

#include "Math.hpp"

#include <string>
#include <vector>

namespace IC
{
	class Vertex {
	public:
		Vec4 position;
		Vec4 normal;
	};
	
	
	class Triangle {
	public:
		Vertex vertices[3];
	};
	
	
	class Mesh {
	public:
		std::string name;
		std::vector<Triangle> triangles;
		
		Mesh();
		Mesh(std::string filename);
	};
}

#endif
