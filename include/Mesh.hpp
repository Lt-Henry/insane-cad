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

#ifndef IC_MESH
#define IC_MESH

#include <blaster/constants.h>
#include <blaster/vbo.h>

#include <string>
#include <vector>

namespace ic
{
    class Vertex {
    public:
        float position [4];
        float normal [4];
        float color[4];
        float uv[2];
    };


    class Triangle {
    public:
        unsigned int vertices[3];
    };


    class Mesh {
    public:

        bl_vbo_t* vbo;

        std::string name;
        std::vector<Vertex> vertices;
        std::vector<Triangle> triangles;

        Mesh();
        
        static Mesh* load_ply(std::string filename);
        static Mesh* load_obj(std::string filename);
        
        void build_vbo();
    };
}

#endif
