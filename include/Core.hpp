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

#ifndef _IC_CORE_
#define _IC_CORE_

#include "Mesh.hpp"

#include <vector>

namespace ic
{
	class Core {
	
	private:
		static Core* singleton;
		
		
	public:
	
		std::vector<Mesh> meshes;
	
		Core();
		
		static Core* Get();
		
		int Run(int argc,char* argv[]);
	};
}

#endif
