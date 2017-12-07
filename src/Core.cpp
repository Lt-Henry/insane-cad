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
#include <Window.hpp>

#include <gtkmm.h>
#include <stdexcept>

using namespace ic;
using namespace std;

Core* Core::singleton=nullptr;

Core::Core()
{
    if (Core::singleton==nullptr) {
        Core::singleton=this;
    }
    else {
        throw runtime_error("There is already a Core instance");
    }
}

Core* Core::get()
{
    return Core::singleton;
}


int Core::run(int argc,char* argv[])
{
    auto app = Gtk::Application::create(argc, argv, "es.opencircus.insane-cad");

    Window mw;

    return app->run(mw);
}
