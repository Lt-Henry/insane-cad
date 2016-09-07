/*
	Insane cad
	
	Copyright (C) 2016  Enrique Medina Gremaldos <quiqueiii@gmail.com>

	This program is free software: you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation, either version 3 of the License, or
	(at your option) any later version.

	This program is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef _IC_MESH_
#define _IC_MESH_

#include "Math.hpp"
#include "Color.hpp"
#include "Vbo.hpp"

#include <string>
#include <vector>

namespace ic
{
	class Vertex {
	public:
		Vec4 position;
		Vec4 normal;
		Color color;
		Vec2 uv;
	};
	
	
	class Triangle {
	public:
		unsigned int vertices[3];
	};
	
	
	class Mesh {
	public:
		Vbo vbo;
	
		std::string name;
		std::vector<Vertex> vertices;
		std::vector<Triangle> triangles;
		
		Mesh();
		Mesh(std::string filename);
		
		void Select(Vec4 center,float radius);
		void Select(Vec4 point,Vec4 normal,float distance);
		
		void BuildVbo();
	};
}

#endif
