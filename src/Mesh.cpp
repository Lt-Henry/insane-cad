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

#include "Mesh.hpp"

#include <iostream>
#include <fstream>
#include <vector>

using namespace ic;
using namespace std;

Mesh::Mesh()
{
}


Mesh::Mesh(string filename)
{
	fstream fs;
	
	setlocale(LC_NUMERIC,"C");
	
	fs.open(filename,fstream::in);
	
	
	string chunk="header";
	
	vector<string> tokens;
	string line;
	
	int numVertices=0;
	int numFaces=0;
	
	
	while (!fs.eof()) {
		getline(fs,line);
		
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
				
				this->triangles.push_back(triangle);
				
				numFaces--;
				
				if (numFaces==0) {
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
			vx.position.Set(x,y,z,1.0f);
			vx.normal.Set(nx,ny,nz,0.0f);
			vx.color.Set(101,123,131);
			
			this->vertices.push_back(vx);
			
			numVertices--;
			
			if (numVertices==0) {
				chunk="faces";
			}
		}
		
		//header
		if (chunk=="header") {
			if (tokens[0]=="element") {
				if (tokens[1]=="vertex") {
					numVertices=stoi(tokens[2]);
				}
			
				if (tokens[1]=="face") {
					numFaces=stoi(tokens[2]);
				}
			}
			
			if (tokens[0]=="end_header") {
				chunk="vertices";
			}
		
		}
		
		
		
		
		tokens.clear();
	
	}
	
	
	cout<<"vertices: "<<numVertices<<endl;
	cout<<"faces: "<<numFaces<<endl;

	
	fs.close();
}

