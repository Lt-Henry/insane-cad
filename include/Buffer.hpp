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

#ifndef _IC_BUFFER_
#define _IC_BUFFER_

#include <stdexcept>

namespace IC
{
	template <class T>
	class Buffer {
	public:
	
		int width;
		int height;
		
		T* data;
		
	
		Buffer(int width,int height)
		{
			if (width<=0 or height<=0) {
				throw std::runtime_error("Invalid buffer dimensions");
			}
	
			this->width=width;
			this->height=height;
	
			data=new T[width*height];
		}
		
		virtual ~Buffer()
		{
			delete [] data;
		}
		
		void Set(int x,int y,const T value)
		{
		
			//SLOOOOW
			if (x<0 or y<0 or x>=width or y>=height) {
				return;
			}
			
			data[x+y*width]=value;
		}
		
		T Get(int x,int y)
		{
			return data[x+y*width];
		}
	};
}

#endif
