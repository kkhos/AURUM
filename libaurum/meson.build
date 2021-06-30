pkgconfig = import('pkgconfig')

libaurum_install_inc = [
  './inc/UiDevice.h',
  './inc/UiObject.h',
  './inc/UiSelector.h',
  './inc/Sel.h',
  './inc/Until.h',
  './inc/Waiter.h',
  './inc/ISearchable.h',
  './inc/IDevice.h',
  './inc/A11yEvent.h',
  './inc/bitmask.h',
  './inc/Accessibility/AccessibleNode.h',
  './inc/Accessibility/AccessibleUtils.h',
  './inc/Accessibility/AccessibleWatcher.h',
  './inc/Accessibility/AccessibleApplication.h',
  './inc/Accessibility/AccessibleWindow.h',
  './inc/Accessibility/IEventConsumer.h',
  './inc/Accessibility/IEventSource.h',
  './inc/Runnable/Runnable.h',
  './inc/Misc/Point2D.h',
  './inc/Misc/Rect.h',
  './inc/Aurum.h',
]

libaurum_inc = [
  include_directories('./inc'),
  include_directories('./inc/Accessibility'),
  include_directories('./inc/Impl'),
  include_directories('./inc/Impl/Accessibility'),
  include_directories('./inc/Misc'),
  include_directories('./inc/Runnable/'),
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
    dependency('dlog'),
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

pkgconfig.generate(filebase : 'libaurum',
                   name : 'libaurum',
                   description : 'Autumation library',
                   version : meson.project_version(),
                   libraries : libaurum_lib,
)

libaurum =  declare_dependency(link_with: libaurum_lib,
                               dependencies: libaurum_dep,
                               include_directories: libaurum_inc,
            )

install_headers(libaurum_install_inc,
)

