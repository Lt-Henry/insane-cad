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

#ifndef _IC_MATH_
#define _IC_MATH_

namespace IC
{
	class Vec4 {
	public:
	
		float data[4];
		
		void Homogeneus();
	};
	
	
	class Mat16 {
	public:
	
		float data[16];
	};
	
	Vec4 operator ^ (Vec4 & v,Mat16 & m);
}


#endif
