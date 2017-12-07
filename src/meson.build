
gtkmm=dependency('gtkmm-3.0')
blaster_dep=blaster.get_variable('blaster')

executable('insane-cad', ['main.cpp','Core.cpp','Window.cpp','View.cpp','Mesh.cpp'],
    cpp_args:'-std=c++11',
    include_directories:ic_headers,
    dependencies:[gtkmm,blaster_dep]
    )
