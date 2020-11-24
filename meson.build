project('aurum', ['cpp'],
  version: '0.1.1',
  default_options : ['buildtype=debugoptimized', 'cpp_std=gnu++17'],
  meson_version : '>=0.47'
)

config_h = configuration_data()
if get_option('tizen') == true
  config_h.set10('GBS_BUILD', true)
  config_h.set10('GBSBUILD', true)
  config_h.set10('TIZEN', true)
else
  message('')
endif
if get_option('tizen_gcov') == true
  config_h.set10('TIZEN_GCOV', true)
endif

root_inc = include_directories('./')

subdir('protocol')
subdir('libloguru')
subdir('libaurum')
if get_option('tizen') == true
  subdir('org.tizen.aurum-bootstrap')
endif
subdir('tests')

configure_file(
  output: 'config.h',
  configuration: config_h
)

dconf = configuration_data()
dconf.set('VERSION', meson.project_version())
if find_program('dot', required : false).found()
   dconf.set('HAVE_DOT', 'YES')
else
   dconf.set('HAVE_DOT', 'NO')
endif

if get_option('enable_documentation') == true
  doxygen = find_program('doxygen', required: false)
  if not doxygen.found()
    error('doxygen not found')
  endif
  subdir('doc')
endif
