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

using namespace IC;
using namespace std;


Raster::Raster(int width,int height)
{

	colorBuffer=nullptr;
	depthBuffer=nullptr;
	
	projection=Mat16::Identity();
	
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

void Raster::Draw(Mesh & mesh)
{
	static float phi=0.0f;

	phi=phi+0.1f;
	
	Mat16 rot=Mat16::RotationX(phi);
	
	// line triangles
	/*
	for (Triangle triangle : mesh.triangles) {
	
		Vec4 a,b,c;
		
		a=mesh.vertices[triangle.vertices[0]].position;
		b=mesh.vertices[triangle.vertices[1]].position;
		c=mesh.vertices[triangle.vertices[2]].position;
		
		a=a ^ rot;
		b=b ^ rot;
		c=c ^ rot;
		
		a=a ^ projection;
		b=b ^ projection;
		c=c ^ projection;
		
		a=a ^ viewport;
		b=b ^ viewport;
		c=c ^ viewport;
		
		a.Homogeneus();
		b.Homogeneus();
		c.Homogeneus();
		
		Line(a,b);
		Line(b,c);
		Line(c,a);

		
	}
	*/
	
	// dots
	for (Vertex & vx : mesh.vertices) {

		Vec4 p = vx.position ^ rot;
		Mat16 m = projection ^ viewport;
		
		p=p ^ m;
		
		p.Homogeneus();
		
		colorBuffer->Set(p.data[0],p.data[1],0xffff0000);
	}
}

void Raster::Clear()
{
	for (int j=0;j<height;j++) {
		for (int i=0;i<width;i++) {
			colorBuffer->Set(i,j,0xfffdf6e3);
		}
	}
}


void Raster::Line(Vec4 & a,Vec4 & b)
{
	float x1=a.data[0];
	float y1=a.data[1];
	
	float x2=b.data[0];
	float y2=b.data[1];
	
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
			colorBuffer->Set(y,x,0xff000000);
		}
		else {
			colorBuffer->Set(x,y,0xff000000);
		}

		error -= dy;
		if (error < 0) {
			y += ystep;
			error += dx;
		}
	}
}
