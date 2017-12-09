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

#include <Core.hpp>
#include <View.hpp>

#include <blaster/constants.h>
#include <blaster/vbo.h>

#include <iostream>
#include <chrono>
#include <random>

using namespace ic;
using namespace std;

bl_vbo_t* points;

View::View()
{
    add_events(Gdk::BUTTON_PRESS_MASK | Gdk::BUTTON_RELEASE_MASK | Gdk::SCROLL_MASK | Gdk::POINTER_MOTION_MASK);
    set_size_request(128,128);
    set_halign(Gtk::ALIGN_FILL);
    set_valign(Gtk::ALIGN_FILL);

    width=32;
    height=32;


    zoom=10.0f;
    rx=0.0f;
    ry=0.0f;
    rz=0.0f;

    button_status=ButtonStatus::Released;

    raster=bl_raster_new(width,height);
    
    // generate some random points
    const int num_points=320000;
    points=bl_vbo_new(num_points,8);
    
    std::random_device rd;
    std::mt19937 mt(rd());
    std::uniform_real_distribution<float> dist(-5, 5);
    
    for (int n=0;n<num_points;n++) {
        float x=dist(mt);
        float y=dist(mt);
        float z=dist(mt);
        
        bl_vbo_add(points,x,y,z,1.0f,0.0f,0.0f,0.0f,1.0f);
    }
    
    
    update_ortho();
}


View::~View()
{
    bl_raster_delete(raster);
}


void View::update_ortho()
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
    bl_matrix_stack_load_identity(raster->projection);
    bl_matrix_stack_frustum(raster->projection,
        left,right,top,bottom,10,100);
    //Mat16 projection = Mat16::Frustum(left,right,top,bottom,1.0f,1000.0f);


}


void View::update()
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

        bl_raster_resize(raster,width,height);
        update_ortho();

    }

    auto start = chrono::steady_clock::now();

    float clear_color[4] = {1.0,0.9,0.9,0.9};
    
    bl_raster_set_clear_color(raster,clear_color);
    bl_raster_clear(raster);
    
    bl_matrix_stack_load_identity(raster->modelview);
    bl_matrix_stack_translate(raster->modelview,0.0f,0.0f,-zoom);
    bl_matrix_stack_rotate_x(raster->modelview,this->ry);
    bl_matrix_stack_rotate_y(raster->modelview,this->rx);
    
    bl_raster_draw(raster,points);

    bl_raster_update(raster);

    auto end = chrono::steady_clock::now();
    
    auto diff = end - start;
    
    double raster_time = chrono::duration <double, milli> (diff).count();
    int fps = 1000/raster_time;
    
    clog<<"raster time: "<<raster_time<<" ms fps:"<<fps<<"\n";
    
    Cairo::RefPtr<Cairo::ImageSurface> imageBuffer;

    imageBuffer=Cairo::ImageSurface::create(
    (unsigned char*)(this->raster->color_buffer->data),
    Cairo::Format::FORMAT_ARGB32,
    width,
    height,
    width*4
    );
    
    //253 246 227 base3
    //101 123 131 base00




    //raster->Draw(this->axis);
    
    cr->set_source(imageBuffer,0,0);
    cr->paint();
    
    cr->set_source_rgb(0.39, 0.48, 0.51);
    cr->move_to(width/2.0,0);
    cr->line_to(width/2.0,height);
    cr->stroke();

    cr->move_to(0,height/2.0);
    cr->line_to(width,height/2.0);
    cr->stroke();

    /*
    int ns=(raster->ns_clear+raster->ns_projection+raster->ns_transform+raster->ns_triangle);

    int fps = 1000000000/ns;
    cout <<"* time " << ns/1000000 <<" ms ("<<fps<<") fps"<<endl;
    cout<<"* clear: "<<raster->ns_clear/1000000<<" ms"<<endl;
    cout<<"* projection: "<<raster->ns_projection/1000000<<" ms"<<endl;
    cout<<"* transform: "<<raster->ns_transform/1000000<<" ms"<<endl;
    cout<<"* triangle: "<<raster->ns_triangle/1000000<<" ms"<<endl;
    cout<<"* t1: "<<raster->ns_t1/1000000<<" ms"<<endl;
    cout<<"* t2: "<<raster->ns_t2/1000000<<" ms"<<endl;
    */
    
    return true;
}


bool View::on_button_press_event(GdkEventButton * button_event)
{
    float mx=button_event->x;
    float my=button_event->y;

    //Gtk::Allocation allocation = get_allocation();
    //const int w = allocation.get_width();
    //const int h = allocation.get_height();

    clog<<"press"<<endl;
    clog<<"mouse coords: "<<mx<<","<<my<<endl;



    button_status=ButtonStatus::Pressed;
    this->press_x=mx;
    this->press_y=my;

    return true;
}


bool View::on_button_release_event(GdkEventButton * button_event)
{
    clog<<"release"<<endl;
    button_status=ButtonStatus::Released;

    return true;
}


bool View::on_scroll_event(GdkEventScroll* scroll_event)
{
    if (scroll_event->direction==GDK_SCROLL_UP) {
        zoom+=1.0f;

        //UpdateOrtho();

        update();

        clog<<"zoom: "<<zoom<<endl;
    }

    if (scroll_event->direction==GDK_SCROLL_DOWN) {
        zoom-=1.0f;

        if (zoom<1.0f) {
            zoom=1.0f;
        }

        //UpdateOrtho();

        update();

        clog<<"zoom: "<<zoom<<endl;
    }


    return true;
}


bool View::on_motion_notify_event(GdkEventMotion* motion_event)
{

    float mx=motion_event->x;
    float my=motion_event->y;

    if (button_status==ButtonStatus::Pressed) {
        float deltax=mx-this->press_x;
        float deltay=this->press_y-my;

        this->rx+=(deltax/8.0f);
        this->ry-=(deltay/8.0f);

        this->press_x=mx;
        this->press_y=my;

        update();
    }

    return true;
}
