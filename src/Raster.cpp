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

using namespace IC;


Raster::Raster(int width,int height)
{

	this->width=width;
	this->height=height;

	color=new Buffer<uint32_t>(width,height);
	depth=new Buffer<float>(width,height);
}


Raster::~Raster()
{
	delete color;
	delete depth;
}


void Raster::Resize(int width,int height)
{

	delete color;
	delete depth;

	this->width=width;
	this->height=height;

	color=new Buffer<uint32_t>(width,height);
	depth=new Buffer<float>(width,height);

}


void Raster::Clear()
{
	for (int j=0;j<height;j++) {
		for (int i=0;i<width;i++) {
			if (i==j) {
				color->Set(i,j,0xff00ff00);
			}
			else {
				color->Set(i,j,0xff0000ff);
			}
		}
	}
}
