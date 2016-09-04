
#include "Vbo.hpp"
#include "Raster.hpp"

#include <iostream>
#include <cstdlib>

using namespace ic;
using namespace std;

float frand()
{
	float f=(float)RAND_MAX/(float)rand();
}

int main(int argc,char* argv[])
{
	Raster raster(1280,720);
	
	Vbo mesh(Primitive::Triangle);
	
	srand(34);
	
	for (int n=0;n<10000000;n++) {
		
		for (int m=0;m<3;m++) {
			float x,y,z;
			
			x=1.0 - (frand()*2.0f);
			y=1.0 - (frand()*2.0f);
			z=1.0 - (frand()*2.0f);
			
			float c;
			
			c=frand();
			
			mesh.Load(Vec4(x,y,z,1),Vec4(0,1,0,0),Color(c,c,c));
			mesh.Push();
		}
	}
	
	raster.SetOrtho(-1.0,1.0,-1.0,1.0);
	
	raster.Draw(mesh);
	
	
	int ns=(raster.ns_clear+raster.ns_projection+raster.ns_transform+raster.ns_triangle);
	
	int fps = 1000000000/ns;
	cout <<"* time " << ns/1000000 <<" ms ("<<fps<<") fps"<<endl;
	cout<<"* clear: "<<raster.ns_clear/1000000<<" ms"<<endl;
	cout<<"* projection: "<<raster.ns_projection/1000000<<" ms"<<endl;
	cout<<"* transform: "<<raster.ns_transform/1000000<<" ms"<<endl;
	cout<<"* triangle: "<<raster.ns_triangle/1000000<<" ms"<<endl;
	
	return 0;
}
