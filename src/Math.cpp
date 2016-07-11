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

#include <cmath>
using namespace std;

namespace IC
{

	void Vec4::Set(float x,float y,float z,float w)
	{
		data[0]=x;
		data[1]=y;
		data[2]=z;
		data[3]=w;
	}

	void Vec4::Homogeneus()
	{
		data[0]=data[0]/data[3];
		data[1]=data[1]/data[3];
		data[2]=data[2]/data[3];
		data[3]=1.0f;
	}

	Mat16 Mat16::Identity()
	{
		Mat16 ret;
		
		ret.data[0]=1.0f;
		ret.data[1]=0.0f;
		ret.data[2]=0.0f;
		ret.data[3]=0.0f;
		
		ret.data[4]=0.0f;
		ret.data[5]=1.0f;
		ret.data[6]=0.0f;
		ret.data[7]=0.0f;
		
		ret.data[8]=0.0f;
		ret.data[9]=0.0f;
		ret.data[10]=1.0f;
		ret.data[11]=0.0f;

		ret.data[12]=0.0f;
		ret.data[13]=0.0f;
		ret.data[14]=0.0f;
		ret.data[15]=1.0f;
		
		return ret;
	}
	
	Mat16 Mat16::RotationX(float phi)
	{
		Mat16 ret;
		
		ret.data[0]=1.0f;
		ret.data[1]=0.0f;
		ret.data[2]=0.0f;
		ret.data[3]=0.0f;
		
		ret.data[4]=0.0f;
		ret.data[5]=cos(phi);
		ret.data[6]=-sin(phi);
		ret.data[7]=0.0f;
		
		ret.data[8]=0.0f;
		ret.data[9]=sin(phi);
		ret.data[10]=cos(phi);
		ret.data[11]=0.0f;

		ret.data[12]=0.0f;
		ret.data[13]=0.0f;
		ret.data[14]=0.0f;
		ret.data[15]=1.0f;
		
		return ret;
	}

	Mat16 Mat16::RotationY(float phi)
	{
		Mat16 ret;
		
		ret.data[0]=cos(phi);
		ret.data[1]=0.0f;
		ret.data[2]=sin(phi);
		ret.data[3]=0.0f;
		
		ret.data[4]=0.0f;
		ret.data[5]=1.0f;
		ret.data[6]=0.0f;
		ret.data[7]=0.0f;
		
		ret.data[8]=-sin(phi);
		ret.data[9]=0.0f;
		ret.data[10]=cos(phi);
		ret.data[11]=0.0f;

		ret.data[12]=0.0f;
		ret.data[13]=0.0f;
		ret.data[14]=0.0f;
		ret.data[15]=1.0f;
		
		return ret;
	}
	
	Mat16 Mat16::RotationZ(float phi)
	{
		Mat16 ret;
		
		ret.data[0]=cos(phi);
		ret.data[1]=-sin(phi);
		ret.data[2]=0.0f;
		ret.data[3]=0.0f;
		
		ret.data[4]=sin(phi);
		ret.data[5]=cos(phi);
		ret.data[6]=0.0f;
		ret.data[7]=0.0f;
		
		ret.data[8]=0.0f;
		ret.data[9]=0.0f;
		ret.data[10]=1.0f;
		ret.data[11]=0.0f;

		ret.data[12]=0.0f;
		ret.data[13]=0.0f;
		ret.data[14]=0.0f;
		ret.data[15]=1.0f;
		
		return ret;
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
