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

#ifndef _IC_VIEW_
#define _IC_VIEW_

#include "Raster.hpp"

#include <gtkmm/drawingarea.h>


namespace ic
{
	class View : public Gtk::DrawingArea {
	
	protected:
	
		int width;
		int height;
	
		float zoom;
	
		Raster* raster;
		
		void UpdateOrtho();
	
	public:
	
		View();
		virtual ~View();
		
		//overload draw method
		bool on_draw(const Cairo::RefPtr<Cairo::Context>& cr) override;
		bool on_button_press_event(GdkEventButton * button_event) override;
		bool on_button_release_event(GdkEventButton * button_event) override;
		bool on_scroll_event(GdkEventScroll* scroll_event) override;


	};
}


#endif
