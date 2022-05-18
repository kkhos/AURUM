pkgconfig = import('pkgconfig')

libpugixml_lib = library(
    'pugixml',
    ['pugixml.cpp'],
    gnu_symbol_visibility : 'hidden',
    version : meson.project_version(),
    install : true,
)

libpugixml_dep = declare_dependency(
    link_with : libpugixml_lib,
    include_directories : include_directories('.'),
)

install_headers('pugixml.hpp',
                'pugixml.cpp',
                'pugiconfig.hpp')

pkgconfig.generate(
    libpugixml_lib,
    description : 'simple, small, C++ XML parser',
)
