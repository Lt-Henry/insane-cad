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

#ifndef _IC_COLOR_
#define _IC_COLOR_

#include <cstdint>

namespace ic
{
	class Color {
	public:
	
		float data[4];
		
		
		Color();
		Color(float r,float g,float b,float a=1.0f);
		
		void Set(float r,float g,float b,float a=1.0f);
		
		uint32_t Pixel();
	};
}


#endif
