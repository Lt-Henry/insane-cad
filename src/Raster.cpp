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

#include <iostream>
#include <cmath>

using namespace ic;
using namespace std;


Raster::Raster(int width,int height)
{

	colorBuffer=nullptr;
	depthBuffer=nullptr;
	
	projection=Mat16::Identity();
	camera=Mat16::Identity();
	
	Resize(width,height);
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

void Raster::SetOrtho(float left,float right,float top,float bottom)
{
	// orthographic projection matrix
	projection.data[0]=2.0f/(right-left);
	projection.data[1]=0.0f;
	projection.data[2]=0.0f;
	projection.data[3]=-(right+left)/(right-left);

	projection.data[4]=0.0f;
	projection.data[5]=2.0f/(top-bottom);
	projection.data[6]=0.0f;
	projection.data[7]=-(top+bottom)/(top-bottom);
	
	projection.data[8]=0.0f;
	projection.data[9]=0.0f;
	projection.data[10]=1.0f;
	projection.data[11]=0.0f;
	
	projection.data[12]=0.0f;
	projection.data[13]=0.0f;
	projection.data[14]=0.0f;
	projection.data[15]=1.0f;
}


void Raster::SetCamera(Vec4 origin,Vec4 forward,Vec4 up)
{
	forward.Normalize();
	Vec4 right = forward ^ up;
	right.Normalize();
	Vec4 realUp = forward ^ right;
	
	camera.data[0]=right.data[0];
	camera.data[1]=right.data[1];
	camera.data[2]=right.data[2];
	camera.data[3]=0.0f;
	
	camera.data[4]=realUp.data[0];
	camera.data[5]=realUp.data[1];
	camera.data[6]=realUp.data[2];
	camera.data[7]=0.0f;
	
	camera.data[8]=forward.data[0];
	camera.data[9]=forward.data[1];
	camera.data[10]=forward.data[2];
	camera.data[11]=0.0f;
	
	camera.data[12]=0.0f;
	camera.data[13]=0.0f;
	camera.data[14]=0.0f;
	camera.data[15]=1.0f;
}


void Raster::Draw(Vbo & vbo)
{
	Mat16 matrix=Mat16::Identity();
	
	matrix=matrix ^ camera;
	matrix=matrix ^ projection;
	matrix=matrix ^ viewport;
	
	switch (vbo.Type()) {
	
		case Primitive::None:
			//Nothing to do here :)
		break;
	
		case Primitive::Triangle:
			for (int n=0;n<vbo.Size();n+=3) {
				Vec4 v1 = vbo.vertices[n] ^ matrix;
				Vec4 v2 = vbo.vertices[n+1] ^ matrix;
				Vec4 v3 = vbo.vertices[n+2] ^ matrix;
				
				v1.Homogeneus();
				v2.Homogeneus();
				v3.Homogeneus();
				
				Line(v1,
					vbo.normals[n],
					vbo.colors[n],
					v2,
					vbo.normals[n+1],
					vbo.colors[n+1]);
					
				Line(v1,
					vbo.normals[n],
					vbo.colors[n],
					v3,
					vbo.normals[n+2],
					vbo.colors[n+2]);
					
				Line(v2,
					vbo.normals[n+1],
					vbo.colors[n+1],
					v3,
					vbo.normals[n+2],
					vbo.colors[n+2]);
					
					
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
	for (int j=0;j<height;j++) {
		for (int i=0;i<width;i++) {
			colorBuffer->Set(i,j,0xfffdf6e3);
			depthBuffer->Set(i,j,-10000.0f);
		}
	}
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
	
	if (x<0 or y<0 or x>=colorBuffer->width or y>=colorBuffer->height) {
		return;
	}
	
	float currentZ=depthBuffer->Get(x,y);
	
	if (z>currentZ) {
		uint32_t color=c1.Pixel();
	
		colorBuffer->Set(x,y,color);
		depthBuffer->Set(x,y,z);
	}
}
