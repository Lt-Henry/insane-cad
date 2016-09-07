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

namespace ic
{

	Vec2::Vec2()
	{
	}

	Vec2::Vec2(float x,float y)
	{
		data[0]=x;
		data[1]=y;
	}

	Vec4::Vec4()
	{
	
	}


	Vec4::Vec4(float x,float y,float z,float w)
	{
		Set(x,y,z,w);
	}


	void Vec4::Set(float x,float y,float z,float w)
	{
		data[0]=x;
		data[1]=y;
		data[2]=z;
		data[3]=w;
	}


	void Vec4::Homogeneus()
	{
		/*
		data[0]=data[0]/data[3];
		data[1]=data[1]/data[3];
		data[2]=data[2]/data[3];
		data[3]=1.0f;
		*/
		
		float w=1.0f/data[3];
		
		data[0]*=w;
		data[1]*=w;
		data[2]*=w;
		data[3]*=w;
	}


	float Vec4::Norm()
	{
		float tmp=(data[0]*data[0])+(data[1]*data[1])+(data[2]*data[2])+(data[3]*data[3]);
		
		return sqrt(tmp);
	}


	void Vec4::Normalize()
	{
		float norm=Norm();
		data[0]=data[0]/norm;
		data[1]=data[1]/norm;
		data[2]=data[2]/norm;
		data[3]=data[3]/norm;
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
	

	Mat16 Mat16::Translation(float x,float y,float z)
	{
		Mat16 ret;
		
		ret.data[0]=1.0f;
		ret.data[1]=0.0f;
		ret.data[2]=0.0f;
		ret.data[3]=x;
		
		ret.data[4]=0.0f;
		ret.data[5]=1.0f;
		ret.data[6]=0.0f;
		ret.data[7]=y;
		
		ret.data[8]=0.0f;
		ret.data[9]=0.0f;
		ret.data[10]=1.0f;
		ret.data[11]=z;

		ret.data[12]=0.0f;
		ret.data[13]=0.0f;
		ret.data[14]=0.0f;
		ret.data[15]=1.0f;
		
		return ret;
	}
	

	Mat16 Mat16::Scale(float x,float y,float z)
	{
		Mat16 ret;
		
		ret.data[0]=x;
		ret.data[1]=0.0f;
		ret.data[2]=0.0f;
		ret.data[3]=0.0f;
		
		ret.data[4]=0.0f;
		ret.data[5]=y;
		ret.data[6]=0.0f;
		ret.data[7]=0.0f;
		
		ret.data[8]=0.0f;
		ret.data[9]=0.0f;
		ret.data[10]=z;
		ret.data[11]=0.0f;

		ret.data[12]=0.0f;
		ret.data[13]=0.0f;
		ret.data[14]=0.0f;
		ret.data[15]=1.0f;
		
		return ret;
	}


	Vec4 operator - (Vec4 & a, Vec4 & b)
	{
		Vec4 ret;
		
		ret.data[0]=a.data[0]-b.data[0];
		ret.data[1]=a.data[1]-b.data[1];
		ret.data[2]=a.data[2]-b.data[2];
		ret.data[3]=a.data[3]-b.data[3];
		
		return ret;
	}


	Vec4 operator ^ (Vec4 & a,Vec4 & b)
	{
		Vec4 ret;
		
		ret.data[0] = a.data[1]*b.data[2] - a.data[2]*b.data[1];
		ret.data[1] = a.data[2]*b.data[0] - a.data[0]*b.data[2];
		ret.data[2] = a.data[0]*b.data[1] - a.data[1]*b.data[0];
		ret.data[3] = 0.0f;
		
		return ret;
	}


	float operator * (Vec4 & a,Vec4 & b)
	{
		float ret;
		
		ret=(a.data[0]*b.data[0]) + (a.data[1]*b.data[1]) + (a.data[2]*b.data[2]) + (a.data[3]*b.data[3]);
	
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
/*		
		Vec4 ret;
		
		ret.data[0]=m.data[4*0+0]*v.data[0] + m.data[4*0+1]*v.data[1] + m.data[4*0+2]*v.data[2] + m.data[4*0+3]*v.data[3];
		
		ret.data[1]=m.data[4*1+0]*v.data[0] + m.data[4*1+1]*v.data[1] + m.data[4*1+2]*v.data[2] + m.data[4*1+3]*v.data[3];
		
		ret.data[2]=m.data[4*2+0]*v.data[0] + m.data[4*2+1]*v.data[1] + m.data[4*2+2]*v.data[2] + m.data[4*2+3]*v.data[3];
		
		ret.data[3]=m.data[4*3+0]*v.data[0] + m.data[4*3+1]*v.data[1] + m.data[4*3+2]*v.data[2] + m.data[4*3+3]*v.data[3];
		*/
		
		
		return ret;
	}
	
	Mat16 operator ^ (Mat16 & a,Mat16 & b)
	{
		Mat16 ret;
		
			for (int i=0;i<16;i+=4) {
				for (int j=0;j<4;j++) {
					ret.data[i + j] = (b.data[i + 0] * a.data[j +  0])
					+ (b.data[i + 1] * a.data[j +  4])
					+ (b.data[i + 2] * a.data[j +  8])
					+ (b.data[i + 3] * a.data[j + 12]);
				}
			}
		
		return ret;
	}
}
