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


#ifndef _IC_VBO_
#define _IC_VBO_

#include "Math.hpp"
#include "Color.hpp"

#include <vector>


namespace ic
{
	enum class Primitive {
		Triangles,
		Lines,
		Points
	};
	
	class Vbo {
	
	public:
	
		Primitive type;
	
		std::vector<Vec4> vertices;
		std::vector<Color> colors;
		std::vector<Vec4> normals;
		
		Vbo();
		Vbo(Primitive type);
		
		void Push();
		void Clear();
	
	};
}

#endif
