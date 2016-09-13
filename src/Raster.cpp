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


#include "Raster.hpp"

#include <algorithm>
#include <iostream>
#include <cmath>
#include <chrono>

using namespace ic;
using namespace std;


Raster::Raster(int width,int height)
{

	colorBuffer=nullptr;
	depthBuffer=nullptr;
	
	projection=Mat16::Identity();
	model=Mat16::Identity();
	
	Resize(width,height);
	
		
	light=Vec4(1,1,-1,0);
	light.Normalize();
}


Raster::~Raster()
{
	delete colorBuffer;
	delete depthBuffer;
}


void Raster::Resize(int width,int height)
{

	if (colorBuffer!=nullptr) {
		delete colorBuffer;
	}

	if (depthBuffer!=nullptr) {
		delete depthBuffer;
	}

	this->width=width;
	this->height=height;

	colorBuffer=new Buffer<uint32_t>(width,height);
	depthBuffer=new Buffer<float>(width,height);

	// viewport matrix
	viewport.data[0]=width/2.0f;
	viewport.data[1]=0.0f;
	viewport.data[2]=0.0f;
	viewport.data[3]=width/2.0f;
	
	viewport.data[4]=0.0f;
	viewport.data[5]=-height/2.0f;
	viewport.data[6]=0.0f;
	viewport.data[7]=height/2.0f;
	
	viewport.data[8]=0.0f;
	viewport.data[9]=0.0f;
	viewport.data[10]=1.0f;
	viewport.data[11]=0.0f;
	
	viewport.data[12]=0.0f;
	viewport.data[13]=0.0f;
	viewport.data[14]=0.0f;
	viewport.data[15]=1.0f;
	
}


void Raster::SetMatrix(MatrixType type, Mat16 & matrix)
{
	switch (type) {
	
		case MatrixType::Viewport:
			this->viewport=matrix;
		break;
		
		case MatrixType::Projection:
			this->projection=matrix;
		break;
		
		case MatrixType::Model:
			this->model=matrix;
		break;
	}
}


void Raster::Draw(Vbo & vbo)
{

	ns_projection=0;
	ns_transform=0;
	ns_triangle=0;
	ns_t1=0;
	ns_t2=0;
	
	std::chrono::steady_clock::time_point begin,end;
	
	begin = std::chrono::steady_clock::now();
	
	Mat16 matrix=Mat16::Identity();
	
	matrix=matrix ^ model;
	matrix=matrix ^ projection;
	matrix=matrix ^ viewport;
	
	end = std::chrono::steady_clock::now();
	
	ns_projection=std::chrono::duration_cast<std::chrono::nanoseconds>(end - begin).count();
	
	switch (vbo.Type()) {
	
		case Primitive::None:
			//Nothing to do here :)
		break;
	
		case Primitive::Triangle:
			for (int n=0;n<vbo.Size();n+=3) {
				
				
				begin = std::chrono::steady_clock::now();
				
				Vec4 v1 = vbo.vertices[n] ^ matrix;
				Vec4 v2 = vbo.vertices[n+1] ^ matrix;
				Vec4 v3 = vbo.vertices[n+2] ^ matrix;
				
				Vec4 n1 = vbo.normals[n] ^ model;
				Vec4 n2 = vbo.normals[n+1] ^ model;
				Vec4 n3 = vbo.normals[n+2] ^ model;
				/*
				v1.Homogeneus();
				v2.Homogeneus();
				v3.Homogeneus();
				*/
				end = std::chrono::steady_clock::now();
				
				ns_transform+=std::chrono::duration_cast<std::chrono::nanoseconds>(end - begin).count();
				begin = std::chrono::steady_clock::now();
				
				Triangle(v1,
						n1,
						vbo.colors[n],
						vbo.uvs[n],
						v2,
						n2,
						vbo.colors[n+1],
						vbo.uvs[n+1],
						v3,
						n3,
						vbo.colors[n+2],
						vbo.uvs[n+2]);
						
				end = std::chrono::steady_clock::now();
				
				ns_triangle+=std::chrono::duration_cast<std::chrono::nanoseconds>(end - begin).count();
				
					
			}
		
		break;
		
		case Primitive::Line:
			for (int n=0;n<vbo.Size();n+=2) {
			
				Vec4 v1 = vbo.vertices[n] ^ matrix;
				Vec4 v2 = vbo.vertices[n+1] ^ matrix;
				
				
				v1.Homogeneus();
				v2.Homogeneus();
				
				Line(v1,
					vbo.normals[n],
					vbo.colors[n],
					v2,
					vbo.normals[n+1],
					vbo.colors[n+1]);
			}
		
		break;
		
		case Primitive::Point:
			for (int n=0;n<vbo.Size();n++) {
				
				Vec4 v1 = vbo.vertices[n] ^ matrix;
				
				v1.Homogeneus();
				
				Point(v1,
					vbo.normals[n],
					vbo.colors[n]);
			}
			
		break;
	
	}
}


void Raster::Clear()
{
	ns_clear=0;

	std::chrono::steady_clock::time_point begin,end;
				
	begin = std::chrono::steady_clock::now();
				
	for (int j=0;j<height;j++) {
		for (int i=0;i<width;i++) {
			colorBuffer->Set(i,j,0xfffdf6e3);
			depthBuffer->Set(i,j,10000.0f);
		}
	}
	
	end = std::chrono::steady_clock::now();
	
	ns_clear=std::chrono::duration_cast<std::chrono::nanoseconds>(end - begin).count();
}


void Raster::Line(Vec4 & v1,Vec4 & n1,Color & c1,Vec4 & v2,Vec4 & n2,Color & c2)
{
	float x1=v1.data[0];
	float y1=v1.data[1];
	
	float x2=v2.data[0];
	float y2=v2.data[1];
	
	
	
	uint32_t color=c1.Pixel();
	
	// Credits: https://rosettacode.org/wiki/Bitmap/Bresenham's_line_algorithm#C.2B.2B
	// Bresenham's line algorithm
	
	const bool steep = (fabs(y2 - y1) > fabs(x2 - x1));
	if (steep) {
		std::swap(x1, y1);
		std::swap(x2, y2);
	}

	if (x1 > x2) {
		std::swap(x1, x2);
		std::swap(y1, y2);
	}

	const float dx = x2 - x1;
	const float dy = fabs(y2 - y1);

	float error = dx / 2.0f;
	const int ystep = (y1 < y2) ? 1 : -1;
	int y = (int)y1;

	const int maxX = (int)x2;

	for (int x=(int)x1; x<maxX; x++) {
		if (steep) {
			colorBuffer->Set(y,x,color);
		}
		else {
			colorBuffer->Set(x,y,color);
		}

		error -= dy;
		if (error < 0) {
			y += ystep;
			error += dx;
		}
	}
}


void Raster::Point(Vec4 & v1,Vec4 & n1,Color & c1)
{
	int x=v1.data[0];
	int y=v1.data[1];
	float z=v1.data[2];
	
	if (x<0 or y<0 or x>=width or y>=height) {
		return;
	}
	
	float currentZ=depthBuffer->Get(x,y);
	
	if (z>currentZ) {
		uint32_t color=c1.Pixel();
	
		colorBuffer->Set(x,y,color);
		depthBuffer->Set(x,y,z);
	}
}


float orient(Vec4 & a,Vec4 & b,Vec4 & c)
{
	 return (b.data[0]-a.data[0])*(c.data[1]-a.data[1]) - (b.data[1]-a.data[1])*(c.data[0]-a.data[0]);
}


void Raster::Triangle(Vec4 & v1,Vec4 & n1,Color & c1,Vec2 & uv1,Vec4 & v2,Vec4 & n2,Color & c2,Vec2 & uv2, Vec4 & v3,Vec4 & n3,Color & c3,Vec2 & uv3)
{

	std::chrono::steady_clock::time_point begin,end;
	
	v1.data[0]/=v1.data[3];
	v1.data[1]/=v1.data[3];
	
	v2.data[0]/=v2.data[3];
	v2.data[1]/=v2.data[3];
	
	v3.data[0]/=v3.data[3];
	v3.data[1]/=v3.data[3];
	
	//credits:
	//https://fgiesen.wordpress.com/2013/02/08/triangle-rasterization-in-practice/
	//http://www.scratchapixel.com/code.php?id=26&origin=/lessons/3d-basic-rendering/rasterization-practical-implementation&src=0
	
	//triangle bound
	int minX = std::min({v1.data[0],v2.data[0],v3.data[0]});
	int minY = std::min({v1.data[1],v2.data[1],v3.data[1]});
	
	int maxX = std::max({v1.data[0],v2.data[0],v3.data[0]});
	int maxY = std::max({v1.data[1],v2.data[1],v3.data[1]});
	
	//screen clip
	
	minX = std::max(minX,0);
	minY = std::max(minY,0);
	
	maxX = std::min(maxX,width-1);
	maxY = std::min(maxY,height-1);
	
	//reciprocal
	
	float rz1=1.0f/v1.data[3];
	float rz2=1.0f/v2.data[3];
	float rz3=1.0f/v3.data[3];
	
	float s1=uv1.data[0]*rz1;
	float t1=uv1.data[1]*rz1;
	
	float s2=uv2.data[0]*rz2;
	float t2=uv2.data[1]*rz2;
	
	float s3=uv3.data[0]*rz3;
	float t3=uv3.data[1]*rz3;

	
	
	float area = orient(v1,v2,v3);
	
	for (int j=minY;j<=maxY;j++) {
		for (int i=minX;i<=maxX;i++) {
			
			//barycentric coords
			Vec4 c(i,j,0,0);
			
			float w1=orient(v2,v3,c);
			float w2=orient(v3,v1,c);
			float w3=orient(v1,v2,c);
			
			//float area=w1+w2+w3;
			
			w1/=area;
			w2/=area;
			w3/=area;
			
			
			if (w1 >= 0 and w2 >= 0 and w3 >= 0) {
				
				float oneOverZ = w1*rz1 + w2*rz2 + w3*rz3;
				float z=1.0f/oneOverZ;

				
				float currentZ=depthBuffer->Get(i,j);
	
				if (z<currentZ) {
					begin = std::chrono::steady_clock::now();
					float cosAlpha=0.2f+(light*n1);
					cosAlpha=std::max(0.0f,cosAlpha);
					cosAlpha=std::min(1.0f,cosAlpha);
					
					
					Color c=(c1*(w1*rz1*cosAlpha)) + (c2*(w2*rz2*cosAlpha)) + (c3*(w3*rz3*cosAlpha));
					c=c*z;
					c.data[0]=1.0f;
					
					float u=s1*w1 + s2*w2 + s3*w3;
					float v=t1*w1 + t2*w2 + t3*w3;
					
					u*=z;
					v*=z;
					
					uint32_t color=c.Pixel();
					//uint32_t color=Color(u,v,0).Pixel();
					
					//uint32_t color=0;
					const int M=8;
					float p = (fmod(u * M, 1.0) > 0.5) ^ (fmod(v * M, 1.0) < 0.5);
					//uint32_t color=Color(p,p,p).Pixel();
					
					/*
					uint32_t color=Color(1,0,0).Pixel();
					if (u>0.5 xor v>0.5) {
						color=Color(0,1,0).Pixel();
					}
					*/

					//Color zc(oneOverZ,oneOverZ,oneOverZ);
					
					//Color zc(w1,w2,w3);
					//Color zc(z,z,z);
					//uint32_t color=zc.Pixel();
					
					colorBuffer->Set(i,j,color);
					depthBuffer->Set(i,j,z);
					end = std::chrono::steady_clock::now();
					ns_t2+=std::chrono::duration_cast<std::chrono::nanoseconds>(end - begin).count();
				}
			}
		}
	}
	
	
}

