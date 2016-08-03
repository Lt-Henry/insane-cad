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
#include "Window.hpp"
#include <iostream>
#include <sstream>

using namespace ic;
using namespace std;


Window::Window()
{

	box=Gtk::Box(Gtk::ORIENTATION_VERTICAL,4);
	box.set_margin_top(6);
	box.set_margin_bottom(6);
	box.set_margin_left(6);
	box.set_margin_right(6);
	box.set_homogeneous(false);
	add(box);

	box.pack_start(view,true,true,0);
	
	
	console=Gtk::TextView();
	console.set_editable(false);
	console.set_can_focus(false);
	
	scroll=Gtk::ScrolledWindow();
	scroll.add(console);
	scroll.set_size_request(-1,100);
	box.pack_start(scroll,false,true,0);
	
	prompt=Gtk::Entry();
	prompt.set_valign(Gtk::ALIGN_END);
	box.pack_start(prompt,false,true,0);
	
	prompt.signal_key_release_event().connect(sigc::mem_fun(*this,&Window::OnPromptKey));
	
	show_all();
	
	Print("*** insane cad ***");
	Print("*** Copyright Enrique M.G. 2016 ***");
}


Window::~Window()
{

}

void Window::Print(string text)
{

	console.get_buffer()->insert_at_cursor(text+"\n");
	auto mark=console.get_buffer()->get_insert();
	
	console.scroll_to(mark);
	
}

bool Window::OnPromptKey(GdkEventKey* event)
{
	if (event->keyval==GDK_KEY_Return) {
		string line=prompt.get_text();
		
		//ToDo: run?
		
		Print(line);
		prompt.set_text("");
		
		istringstream iss(line);
		
		vector<string> tokens;
		
		while (iss) {
		
			string sub;
			iss >> sub;
			tokens.push_back(sub);
		}
		
		
		if (tokens[0]=="top") {
			view.raster->SetCamera(Vec4(0,0,0,1),Vec4(0,1,0,0),Vec4(1,0,0,0));
			view.Update();
		}
		if (tokens[0]=="front") {
			view.raster->SetCamera(Vec4(0,0,0,1),Vec4(0,0,1,0),Vec4(0,1,0,0));
			view.Update();
		}
		
		if (tokens[0]=="load") {
			if (tokens.size() > 1 ) {
				Print("Loading: "+tokens[1]);
				Mesh mesh(tokens[1]);
				//mesh.Select(Vec4(23,0,0,1),10.0f);
				mesh.Select(Vec4(23,0,0,1),Vec4(1,0,0),5.0f);
				
				mesh.BuildVbo();
				Core::Get()->meshes.push_back(mesh);
				Print("done");
				view.Update();
			}
		}

		
	}
}
