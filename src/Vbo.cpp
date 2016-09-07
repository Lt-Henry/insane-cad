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


#include "Vbo.hpp"

using namespace ic;
using namespace std;


Vbo::Vbo()
{
	this->type=Primitive::None;
}


Vbo::Vbo(Primitive type)
{
	this->type=type;
	this->size=0;
}


Primitive Vbo::Type()
{
	return this->type;
}


int Vbo::Size()
{
	return this->size;
}


void Vbo::Load(Vec4 vertex,Vec4 normal,Color color,Vec2 uv)
{
	this->vertex=vertex;
	this->normal=normal;
	this->color=color;
	this->uv=uv;
}


void Vbo::Push()
{
	vertices.push_back(this->vertex);
	normals.push_back(this->normal);
	colors.push_back(this->color);
	uvs.push_back(this->uv);
	
	this->size++;
}


void Vbo::Clear()
{
	vertices.clear();
	normals.clear();
	colors.clear();
	uvs.clear();
	
	this->size=0;
}
