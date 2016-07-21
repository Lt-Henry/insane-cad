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

#include "Core.hpp"
#include "View.hpp"
#include "Mesh.hpp"

#include <iostream>
#include <chrono>

using namespace ic;
using namespace std;


View::View()
{
	add_events(Gdk::BUTTON_PRESS_MASK | Gdk::BUTTON_RELEASE_MASK | Gdk::SCROLL_MASK);
	set_size_request(128,128);
	set_halign(Gtk::ALIGN_FILL);
	set_valign(Gtk::ALIGN_FILL);
	
	width=0;
	height=0;
	
	zoom=1.0f;
	
	raster=new Raster(32,32);

	UpdateOrtho();
	
}


View::~View()
{
	delete raster;
}

void View::UpdateOrtho()
{
	//set raster projection
	float ratio = width/(float)height;

	if (ratio>=1.0f) {
		left=-zoom*ratio;
		right=zoom*ratio;
		top=zoom;
		bottom=-zoom;
	}
	else {
		left=-zoom;
		right=zoom;
		top=zoom/ratio;
		bottom=-zoom/ratio;
	}
	
	raster->SetOrtho(left,right,top,bottom);
}


void View::Update()
{
	this->queue_draw();
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
		
		//resize raster viewport
		raster->Resize(width,height);

		UpdateOrtho();
	
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
	
	std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();
	
	raster->Clear();
	
	for (Mesh & mesh : Core::Get()->meshes) {
		raster->Draw(mesh);
	}
	
	std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
	
	cr->set_source(buffer,0,0);
	cr->paint();
	
	cr->set_source_rgb(0.39, 0.48, 0.51);
	cr->move_to(width/2.0,0);
	cr->line_to(width/2.0,height);
	cr->stroke();
	
	cr->move_to(0,height/2.0);
	cr->line_to(width,height/2.0);
	cr->stroke();
	
	
	int ms = std::chrono::duration_cast<std::chrono::milliseconds>(end - begin).count();
	ms=std::max(1,ms);
	
	int fps = 1000/ms;
	cout << "render time " << ms <<" ms ("<<fps<<") fps"<<endl;
}


bool View::on_button_press_event(GdkEventButton * button_event)
{
	float mx=button_event->x;
	float my=button_event->y;
	
	Gtk::Allocation allocation = get_allocation();
	const int w = allocation.get_width();
	const int h = allocation.get_height();
	
	cout<<"press"<<endl;
	cout<<"mouse coords: "<<mx<<","<<my<<endl;
	
	float x=mx/(float)w;
	float y=my/(float)h;
	
	float dw=(right-left);
	float dh=(top-bottom);
	
	x=x*dw - (dw/2.0f);
	y=y*dh - (dh/2.0f);
	
	cout<<"world coords: "<<x<<","<<y<<endl;
	
	cout<<"left:"<<left<<endl;
	cout<<"right:"<<right<<endl;
	
	return true;
}

bool View::on_button_release_event(GdkEventButton * button_event)
{
	cout<<"release"<<endl;
	
	return true;
}

bool View::on_scroll_event(GdkEventScroll* scroll_event)
{
	if (scroll_event->direction==GDK_SCROLL_UP) {
		zoom*=1.5f;
		
		UpdateOrtho();
		
		Update();
		
		cout<<"zoom: "<<zoom<<endl;
	}
	
	if (scroll_event->direction==GDK_SCROLL_DOWN) {
		zoom*=0.5f;
		
		if (zoom<0.1) {
			zoom=0.1f;
		}
		
		UpdateOrtho();
		
		Update();
		
		cout<<"zoom: "<<zoom<<endl;
	}

	
	return true;
}

