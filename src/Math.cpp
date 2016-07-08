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

#include "Math.hpp"


namespace IC
{
	void Vec4::Homogeneus()
	{
		data[0]=data[0]/data[3];
		data[1]=data[1]/data[3];
		data[2]=data[2]/data[3];
		data[3]=1.0f;
	}

	Vec4 operator ^ (Vec4 & v,Mat16 & m)
	{
		Vec4 ret;
		
		ret.data[0]=0.0f;
		ret.data[1]=0.0f;
		ret.data[2]=0.0f;
		ret.data[3]=0.0f;
		
		for (int i=0;i<4;i++) {
			ret.data[0]= ret.data[0]+m.data[4*0+i]*v.data[i];
			ret.data[1]= ret.data[1]+m.data[4*1+i]*v.data[i];
			ret.data[2]= ret.data[2]+m.data[4*2+i]*v.data[i];
			ret.data[3]= ret.data[3]+m.data[4*3+i]*v.data[i];
		}
		return ret;
	}
}
