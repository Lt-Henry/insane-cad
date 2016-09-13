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
	add_events(Gdk::BUTTON_PRESS_MASK | Gdk::BUTTON_RELEASE_MASK | Gdk::SCROLL_MASK | Gdk::POINTER_MOTION_MASK);
	set_size_request(128,128);
	set_halign(Gtk::ALIGN_FILL);
	set_valign(Gtk::ALIGN_FILL);
	
	width=0;
	height=0;
	
	zoom=10.0f;
	rX=0.0f;
	rY=0.0f;
	rZ=0.0f;
	
	buttonStatus=ButtonStatus::Released;
	
	raster=new Raster(32,32);
	
	//build axis vbo
	axis=Vbo(Primitive::Line);
	
	axis.Load(Vec4(0,0,0,1),Vec4(1,0,0),Color(1,0,0),Vec2(0,0));
	axis.Push();
	
	axis.Load(Vec4(10,0,0,1),Vec4(1,0,0),Color(1,0,0),Vec2(0,0));
	axis.Push();
	
	axis.Load(Vec4(0,0,0,1),Vec4(0,1,0),Color(0,1,0),Vec2(0,0));
	axis.Push();
	
	axis.Load(Vec4(0,10,0,1),Vec4(0,1,0),Color(0,1,0),Vec2(0,0));
	axis.Push();
	
	axis.Load(Vec4(0,0,0,1),Vec4(0,0,1),Color(0,0,1),Vec2(0,0));
	axis.Push();
	
	axis.Load(Vec4(0,0,10,1),Vec4(0,0,1),Color(0,0,1),Vec2(0,0));
	axis.Push();

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
		left=-1.0f*ratio;
		right=1.0f*ratio;
		top=1.0f;
		bottom=-1.0f;
	}
	else {
		left=-1.0f;
		right=1.0f;
		top=1.0f/ratio;
		bottom=-1.0f/ratio;
	}
	
	//raster->SetOrtho(left,right,top,bottom,0.01,1000.0);
	
	Mat16 projection = Mat16::Frustum(left,right,top,bottom,1.0f,1000.0f);
	
	raster->SetMatrix(MatrixType::Projection,projection);
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
	
	Mat16 modelMatrix = Mat16::Identity();
	Mat16 tM = Mat16::Translation(0,0,-zoom);
	Mat16 rxM = Mat16::RotationX(rX);
	Mat16 ryM = Mat16::RotationY(rY);
	
	modelMatrix = modelMatrix ^ ryM;
	modelMatrix = modelMatrix ^ rxM;
	modelMatrix = modelMatrix ^ tM;
	
	raster->SetMatrix(MatrixType::Model,modelMatrix);
	
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
	
	for (Mesh & mesh : Core::Get()->meshes) {
		raster->Draw(mesh.vbo);
	}
	
	//raster->Draw(this->axis);
	
	cr->set_source(buffer,0,0);
	cr->paint();
	/*
	cr->set_source_rgb(0.39, 0.48, 0.51);
	cr->move_to(width/2.0,0);
	cr->line_to(width/2.0,height);
	cr->stroke();
	
	cr->move_to(0,height/2.0);
	cr->line_to(width,height/2.0);
	cr->stroke();
	*/
	
	int ns=(raster->ns_clear+raster->ns_projection+raster->ns_transform+raster->ns_triangle);
	
	int fps = 1000000000/ns;
	cout <<"* time " << ns/1000000 <<" ms ("<<fps<<") fps"<<endl;
	cout<<"* clear: "<<raster->ns_clear/1000000<<" ms"<<endl;
	cout<<"* projection: "<<raster->ns_projection/1000000<<" ms"<<endl;
	cout<<"* transform: "<<raster->ns_transform/1000000<<" ms"<<endl;
	cout<<"* triangle: "<<raster->ns_triangle/1000000<<" ms"<<endl;
	cout<<"* t1: "<<raster->ns_t1/1000000<<" ms"<<endl;
	cout<<"* t2: "<<raster->ns_t2/1000000<<" ms"<<endl;
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
	
	
	
	buttonStatus=ButtonStatus::Pressed;
	this->pressX=mx;
	this->pressY=my;
	
	return true;
}


bool View::on_button_release_event(GdkEventButton * button_event)
{
	cout<<"release"<<endl;
	buttonStatus=ButtonStatus::Released;
	
	
	return true;
}


bool View::on_scroll_event(GdkEventScroll* scroll_event)
{
	if (scroll_event->direction==GDK_SCROLL_UP) {
		zoom+=1.0f;
		
		//UpdateOrtho();
		
		Update();
		
		cout<<"zoom: "<<zoom<<endl;
	}
	
	if (scroll_event->direction==GDK_SCROLL_DOWN) {
		zoom-=1.0f;
		
		if (zoom<1.0f) {
			zoom=1.0f;
		}
		
		//UpdateOrtho();
		
		Update();
		
		cout<<"zoom: "<<zoom<<endl;
	}

	
	return true;
}


bool View::on_motion_notify_event(GdkEventMotion* motion_event)
{

	float mx=motion_event->x;
	float my=motion_event->y;
	

	
	if (buttonStatus==ButtonStatus::Pressed) {
			float deltax=mx-this->pressX;
			float deltay=this->pressY-my;
			
			this->rX+=deltax*0.01f;
			this->rY+=deltay*0.01f;
			
			Update();
	}
	
	return true;
}
