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

#include "View.hpp"

#include <iostream>

using namespace IC;
using namespace std;

View::View()
{
	width=0;
	height=0;
	
	raster=new Raster(32,32);
}


View::~View()
{
	delete raster;
}


bool View::on_draw(const Cairo::RefPtr<Cairo::Context>& cr)
{

	Gtk::Allocation allocation = get_allocation();
	const int w = allocation.get_width();
	const int h = allocation.get_height();

	//resize
	if (w!=width or h!=height) {
	
		
		width=w;
		height=h;
		
		raster->Resize(width,height);
	
	}
	
	Cairo::RefPtr<Cairo::ImageSurface> buffer;
	
	buffer=Cairo::ImageSurface::create(
		(unsigned char*)(raster->colorBuffer->data),
		Cairo::Format::FORMAT_ARGB32,
		width,
		height,
		width*4
		);

	//253 246 227 base3
	//101 123 131 base00
	
	
	raster->Clear();
	
	cr->set_source(buffer,0,0);
	cr->paint();
	
	cr->set_source_rgb(0.39, 0.48, 0.51);
	cr->move_to(width/2.0,0);
	cr->line_to(width/2.0,height);
	cr->stroke();
	
	cr->move_to(0,height/2.0);
	cr->line_to(width,height/2.0);
	cr->stroke();
	

}


