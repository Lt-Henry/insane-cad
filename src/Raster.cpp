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

using namespace IC;
using namespace std;


Raster::Raster(int width,int height)
{

	colorBuffer=nullptr;
	depthBuffer=nullptr;
	
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


void Raster::Clear()
{
	for (int j=0;j<height;j++) {
		for (int i=0;i<width;i++) {
			colorBuffer->Set(i,j,0xfffdf6e3);
		}
	}
	
	Vec4 p;
	
	
	p.data[0]=0.0f;
	p.data[1]=0.0f;
	p.data[2]=0.0f;
	p.data[3]=1.0f;
	
	for (int n=0;n<10;n++) {
		
		
		Vec4 s = p ^ viewport;
		s.Homogeneus();
		cout<<p.data[0]<<","<<p.data[1]<<endl;
		cout<<s.data[0]<<","<<s.data[1]<<endl<<endl;
		
		colorBuffer->Set(s.data[0],s.data[1],0xff000000);
		
		p.data[0]+=0.1f;
		p.data[1]+=0.1f;
	}
}
