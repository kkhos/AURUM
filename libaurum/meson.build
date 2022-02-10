pkgconfig = import('pkgconfig')

libaurum_install_inc = [
  './inc/UiDevice.h',
  './inc/UiObject.h',
  './inc/UiSelector.h',
  './inc/Sel.h',
  './inc/Until.h',
  './inc/Waiter.h',
  './inc/Interface/ISearchable.h',
  './inc/Interface/IDevice.h',
  './inc/A11yEvent.h',
  './inc/PartialMatch.h',
  './inc/UiScrollable.h',
  './inc/A11yEvent.h',
  './inc/Comparer.h',
  './inc/Accessibility/AccessibleNode.h',
  './inc/Accessibility/AccessibleWatcher.h',
  './inc/Accessibility/IEventSource.h',
  './inc/Accessibility/AccessibleWindow.h',
  './inc/Accessibility/IEventConsumer.h',
  './inc/Accessibility/AccessibleApplication.h',
  './inc/Accessibility/Accessible.h',
  './inc/Runnable/Runnable.h',
  './inc/Runnable/Runnables.h',
  './inc/Runnable/SendKeyRunnable.h',
  './inc/Misc/bitmask.h',
  './inc/Misc/Point2D.h',
  './inc/Misc/Rect.h',
  './inc/Misc/Size2D.h',
  './inc/Aurum.h',
]

libaurum_inc = [
  include_directories('./inc'),
  include_directories('./inc/Accessibility'),
  include_directories('./inc/Impl'),
  include_directories('./inc/Impl/Accessibility'),
  include_directories('./inc/Misc'),
  include_directories('./inc/Runnable'),
  include_directories('./inc/Interface'),
  root_inc,
]

libaurum_src = []
subdir('src')

libaurum_dep = [
  dependency('atspi-2'),
  dependency('gio-2.0'),
  dependency('threads'),
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
