libaurum_inc = [
  include_directories('./inc'),
  root_inc,
  loguru_inc,
]

libaurum_src = [
  files('src/Sel.cc'),
  files('src/UiDevice.cc'),
  files('src/UiObject.cc'),
  files('src/UiSelector.cc'),
  files('src/AccessibleWatcher.cc'),
  files('src/AccessibleNode.cc'),
  files('src/AccessibleUtils.cc'),
  files('src/Comparer.cc'),
  files('src/Until.cc'),
  files('src/Waiter.cc'),
  files('src/PartialMatch.cc'),
]

if get_option('tizen') == true
libaurum_src +=[
  files('src/DeviceImpl/TizenImpl.cc'),
]
endif

libaurum_dep = [
  dependency('capi-system-info'),
  dependency('libtdm'),
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
