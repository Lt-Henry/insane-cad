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

#include <cstdint>


namespace IC
{
	class Raster {
	public:
	
		Mat16 viewport;
	
		int width;
		int height;
	
		Buffer<uint32_t>* colorBuffer;
		Buffer<float>* depthBuffer;
	
		Raster(int width,int height);
		virtual ~ Raster();
		
		void Resize(int width,int height);
		void Clear();
		
		void Line(Vec4 & a,Vec4 & b);
		
	};
}


#endif
