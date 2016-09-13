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

#ifndef _IC_RASTER_
#define _IC_RASTER_

#include "Buffer.hpp"
#include "Math.hpp"
#include "Vbo.hpp"

#include <cstdint>


namespace ic
{

	enum class MatrixType {
		Viewport,
		Projection,
		Model
	};

	class Raster {
	private:
	
		Buffer<uint32_t>* colorBuffer;
		Buffer<float>* depthBuffer;

		Mat16 viewport;
		Mat16 projection;
		Mat16 model;

	public:
	
	
		int width;
		int height;
		
		int ns_clear;
		int ns_projection;
		int ns_transform;
		int ns_triangle;
		int ns_t1;
		int ns_t2;
		
		Vec4 light;

	
		Raster(int width,int height);
		virtual ~Raster();
		
		void Resize(int width,int height);
		
		void SetMatrix(MatrixType type, Mat16 & matrix);
		
		void Draw(Vbo & vbo);
		void Clear();
		
		void Line(Vec4 & v1,Vec4 & n1,Color & c1,Vec4 & v2,Vec4 & n2,Color & c2);
		void Point(Vec4 & v1,Vec4 & n1,Color & c1);
		void Triangle(Vec4 & v1,Vec4 & n1,Color & c1,Vec2 & uv1,Vec4 & v2,Vec4 & n2,Color & c2,Vec2 & uv2, Vec4 & v3,Vec4 & n3,Color & c3,Vec2 & uv3);
		
	};
}


#endif
