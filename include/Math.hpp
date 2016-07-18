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

namespace ic
{
	class Vec4 {
	public:
	
		float data[4];
		
		
		void Set(float x,float y,float z,float w=0.0f);
		void Homogeneus();
	};
	
	
	class Mat16 {
	public:
	
		float data[16];
		
		static Mat16 Identity();
		static Mat16 RotationX(float phi);
		static Mat16 RotationY(float phi);
		static Mat16 RotationZ(float phi);
		static Mat16 Translation(float x,float y,float z);
		static Mat16 Scale(float x,float y,float z);
	};
	
	Vec4 operator ^ (Vec4 & v,Mat16 & m);
	Mat16 operator ^ (Mat16 & a,Mat16 & b);
}


#endif
