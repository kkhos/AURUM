libaurum_inc = [
  include_directories('./inc'),
  include_directories('./inc/Accessibility'),
  include_directories('./inc/Impl'),
  include_directories('./inc/Impl/Accessibility'),
  include_directories('./inc/Misc'),
  root_inc,
  loguru_inc,
]

libaurum_src = []
subdir('src')

libaurum_dep = [
  dependency('atspi-2'),
  dependency('gio-2.0'),
  dependency('threads'),
  loguru_deps,
]

if get_option('tizen') == true
  libaurum_dep += [
    dependency('capi-system-info'),
    dependency('capi-ui-efl-util'),
    dependency('elementary'),
    dependency('libtdm'),
  ]
endif

libaurum_lib = library('aurum', libaurum_src,
                        dependencies: libaurum_dep,
                        include_directories: libaurum_inc,
                        link_with: libloguru,
                        install: true,
                        version: meson.project_version(),
               )

libaurum =  declare_dependency(link_with: libaurum_lib,
                               dependencies: libaurum_dep,
                               include_directories: libaurum_inc,
            )
