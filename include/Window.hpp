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

#ifndef _IC_WINDOW_
#define _IC_WINDOW_


#include "View.hpp"

#include <gtkmm.h>


namespace IC
{

	class Window : public Gtk::Window {
	
	protected:
	
		Gtk::Box box;
		View view;
		
		Gtk::ScrolledWindow scroll;
		Gtk::TextView console;
		Gtk::Entry prompt;
		
		void Print(std::string text);
	
	public:
	
		Window();
		virtual ~Window();
		
		bool OnPromptKey(GdkEventKey* event);
	
	};

}


#endif
