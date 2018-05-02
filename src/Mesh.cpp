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

#include <Mesh.hpp>

#include <blaster/vector.h>
#include <blaster/color.h>

#include <iostream>
#include <fstream>
#include <vector>
#include <cmath>

using namespace ic;
using namespace std;

Mesh::Mesh()
{
}

vector<string> split(string line)
{
    vector<string> tokens;
    
    bool knee=false;
    string tmp;

    for (char c : line) {

        if (c==' ') {
            if (knee==true) {
                tokens.push_back(tmp);
                knee=false;
                tmp="";
            }
        }
        else {
            tmp=tmp+c;
            knee=true;
        }
    }

    if (tmp!="") {
        tokens.push_back(tmp);
    }
    
    return tokens;
}

Mesh* Mesh::load_ply(string filename)
{
    fstream fs;

    setlocale(LC_NUMERIC,"C");

    fs.open(filename,fstream::in);

    string chunk="header";

    vector<string> tokens;
    string line;

    int num_vertices=0;
    int num_faces=0;
    
    Mesh* mesh = new Mesh();

    mesh->vbo=nullptr;

    while (!fs.eof()) {
        getline(fs,line);

        tokens=split(line);


        //faces section
        if (chunk=="faces") {
            if (tokens[0]=="3") {
                int i1,i2,i3;

                i1=stoi(tokens[1]);
                i2=stoi(tokens[2]);
                i3=stoi(tokens[3]);

                Triangle triangle;

                triangle.vertices[0]=i1;
                triangle.vertices[1]=i2;
                triangle.vertices[2]=i3;

                mesh->triangles.push_back(triangle);

                num_faces--;

                if (num_faces==0) {
                    chunk="end";
                }
            }
        }


        //vertices section
        if (chunk=="vertices") {
            float x,y,z;
            float nx,ny,nz;

            x=stof(tokens[0]);
            y=stof(tokens[1]);
            z=stof(tokens[2]);

            nx=stof(tokens[3]);
            ny=stof(tokens[4]);
            nz=stof(tokens[5]);

            Vertex vx;

            vx.position[0]=x;
            vx.position[1]=y;
            vx.position[2]=z;
            vx.position[3]=1.0;
            
            
            mesh->vertices.push_back(vx);

            num_vertices--;

            if (num_vertices==0) {
                chunk="faces";
            }
        }

        //header
        if (chunk=="header") {
            if (tokens[0]=="element") {
                if (tokens[1]=="vertex") {
                    num_vertices=stoi(tokens[2]);
                }

                if (tokens[1]=="face") {
                    num_faces=stoi(tokens[2]);
                }
            }

            if (tokens[0]=="end_header") {
                chunk="vertices";
            }
        }
        tokens.clear();
    }

    clog<<"vertices: "<<num_vertices<<endl;
    clog<<"faces: "<<num_faces<<endl;


    fs.close();
    
    return mesh;
}

Mesh* Mesh::load_obj(string filename)
{
    fstream fs;

    setlocale(LC_NUMERIC,"C");

    fs.open(filename,fstream::in);
    
    Mesh* mesh = new Mesh();
    mesh->vbo=nullptr;
    
    vector<string> tokens;
    
    
    
    
    while (!fs.eof()) {
        string line;
        getline(fs,line);

        //clog<<line<<endl;
        tokens=split(line);
        
        if (tokens.size()==0) {
            continue;
        }
        
        if (tokens[0]=="o") {
            clog<<"object: "<<tokens[1]<<endl;
        }
        
        if (tokens[0]=="v") {
            Vertex vx;
            
            vx.position[0]=stof(tokens[1]);
            vx.position[1]=stof(tokens[2]);
            vx.position[2]=stof(tokens[3]);
            vx.position[3]=1.0f;
            
            mesh->vertices.push_back(vx);
        }
        
        
    }
    
    
    fs.close();
    
    return mesh;
}

void Mesh::build_vbo()
{
    if (vbo!=nullptr) {
        bl_vbo_delete(vbo);
        vbo=nullptr;
    }
    
    struct point_t {
        bl_vector_t pos;
        bl_color_t color;
    };

    //vbo=bl_vbo_new(this->vertices.size(),sizeof(struct point_t));
    vbo=bl_vbo_new(this->triangles.size()*6,sizeof(struct point_t));
    int index=0;
    
    for (int n=0;n<triangles.size();n++) {
        for (int m=0;m<3;m++) {
            int i1=m;
            int i2=(m+1)%3;
            Vertex & v1 = vertices[triangles[n].vertices[i1]];
            Vertex & v2 = vertices[triangles[n].vertices[i2]];
            
            struct point_t point;
            
            point.color.r=0.0f;
            point.color.g=0.0f;
            point.color.b=1.0f;
            point.color.a=1.0f;
            
            for (int k=0;k<4;k++) {
                point.pos.data[k]=v1.position[k];
            }
            
            bl_vbo_set(vbo,index,&point);
            index++;
            
            for (int k=0;k<4;k++) {
                point.pos.data[k]=v2.position[k];
            }
            
            bl_vbo_set(vbo,index,&point);
            index++;
        }
    }
    /*
    for (int n=0;n<vertices.size();n++) {
    
        struct point_t point;
        
        point.pos.x=vertices[n].position[0];
        point.pos.y=vertices[n].position[1];
        point.pos.z=vertices[n].position[2];
        point.pos.w=vertices[n].position[3];
        
        point.color.r=0.0f;
        point.color.g=0.0f;
        point.color.b=1.0f;
        point.color.a=1.0f;
        
        bl_vbo_set(vbo,n,&point);
        
        
    }
    */

}
