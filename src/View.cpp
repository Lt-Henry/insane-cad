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
#include <blaster/color.h>
#include <blaster/vector.h>


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
    
    pre_rx=0;
    pre_ry=0;
    

    button_status=ButtonStatus::Released;

    raster=bl_raster_new(width,height);
    
    // generate some random points
    const int num_points=3200000;
    struct point_t {
        bl_vector_t pos;
        bl_color_t color;
    };
    
    
                            
    points=bl_vbo_new(num_points,sizeof(struct point_t));
    
    std::random_device rd;
    std::mt19937 mt(rd());
    std::uniform_real_distribution<float> dist(-5, 5);
    
    for (int n=0;n<num_points;n++) {
    
        struct point_t point;
        
        point.pos.x=dist(mt);
        point.pos.y=dist(mt);
        point.pos.z=dist(mt);
        point.pos.w=1.0f;
        
        point.color.r=(point.pos.x+5.0f)/10.0f;
        point.color.g=(point.pos.y+5.0f)/10.0f;
        point.color.b=(point.pos.z+5.0f)/10.0f;
        point.color.a=1.0f;
        
        bl_vbo_set(points,n,&point);
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
        left,right,top,bottom,1,1000);
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

    bl_color_t clear_color;
    bl_color_set(&clear_color,0.9,0.9,0.9,1.0);
    
    bl_raster_set_clear_color(raster,&clear_color);
    bl_raster_clear(raster);
    
    auto p0 = chrono::steady_clock::now();
    
    bl_matrix_stack_load_identity(raster->modelview);
    bl_matrix_stack_translate(raster->modelview,0.0f,0.0f,-zoom);

    bl_matrix_stack_rotate_x(raster->modelview,pre_ry+ry);
    bl_matrix_stack_rotate_y(raster->modelview,pre_rx+rx);


    
    bl_raster_draw_points(raster,points);
    
    for (Mesh* mesh: Core::get()->models) {
        bl_raster_draw_points(raster,mesh->vbo);
    }

    auto p1 = chrono::steady_clock::now();
    
    bl_raster_update(raster);

    auto end = chrono::steady_clock::now();
    
    auto diff = end - start;
    auto diff1 = p0 - start;
    auto diff2 = p1 - p0;
    auto diff3 = end - p1;
    
    double full_time = chrono::duration <double, milli> (diff).count();
    double clear_time = chrono::duration <double, milli> (diff1).count();
    double raster_time = chrono::duration <double, milli> (diff2).count();
    double update_time = chrono::duration <double, milli> (diff3).count();
    
    int fps = 1000/full_time;
    
    clog<<"time: "<<full_time<<" ms - fps:"<<fps<<" ["<<clear_time<<","<<raster_time<<","<<update_time<<"]"<<"\n";
    
    
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
    /*
    cr->set_source_rgb(0.39, 0.48, 0.51);
    cr->move_to(width/2.0,0);
    cr->line_to(width/2.0,height);
    cr->stroke();

    cr->move_to(0,height/2.0);
    cr->line_to(width,height/2.0);
    cr->stroke();
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
    
    uint16_t* zbuffer;
    uint32_t* cbuffer;
    
    zbuffer = (uint16_t*)raster->depth_buffer->data;
    cbuffer = (uint32_t*)raster->color_buffer->data;
    
    int ix = mx;
    int iy = my;
    
    uint16_t z = zbuffer[ix+iy*(raster->screen_width)];
    uint32_t pix = cbuffer[ix+iy*(raster->screen_width)];
    
    clog<<"zbuffer on ["<<ix<<","<<iy<<"]="<<hex<<z<<endl;
    clog<<"pixel on ["<<ix<<","<<iy<<"]="<<hex<<pix<<dec<<endl;
    

    return true;
}


bool View::on_button_release_event(GdkEventButton * button_event)
{
    clog<<"release"<<endl;
    button_status=ButtonStatus::Released;
    
    pre_rx+=rx;
    pre_ry+=ry;
    
    rx=0;
    ry=0;

    return true;
}


bool View::on_scroll_event(GdkEventScroll* scroll_event)
{
    if (scroll_event->direction==GDK_SCROLL_DOWN) {
        zoom+=1.0f;

        //UpdateOrtho();

        update();

        clog<<"zoom: "<<zoom<<endl;
    }

    if (scroll_event->direction==GDK_SCROLL_UP) {
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
        float deltax=mx-press_x;
        float deltay=press_y-my;

        rx=(deltax*0.01745338f);
        ry=(deltay*0.01745338f);

        //this->press_x=mx;
        //this->press_y=my;

        update();
    }

    return true;
}
