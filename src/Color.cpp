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


#include "Color.hpp"


using namespace std;
using namespace ic;


Color::Color()
{
}


Color::Color(float r,float g,float b,float a)
{
	Set(r,g,b,a);
}


void Color::Set(float r,float g,float b,float a)
{
	data[0]=a;
	data[1]=r;
	data[2]=g;
	data[3]=b;
}


uint32_t Color::Pixel()
{
	uint32_t px;
	
	int ia=data[0]*255;
	int ir=data[1]*255;
	int ig=data[2]*255;
	int ib=data[3]*255;
	
	px=(ia<<24) | (ir<<16) | (ig<<8) | ib;
	
	return px;
}
