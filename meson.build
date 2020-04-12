project('aurum', ['cpp'],
  version: '0.0.1',
  default_options : ['buildtype=debugoptimized', 'cpp_std=c++17'],
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

root_inc = include_directories('./')

subdir('protocol')
subdir('libloguru')
subdir('libaurum')
subdir('org.tizen.aurum-bootstrap')
subdir('tests')

configure_file(
  output: 'config.h',
  configuration: config_h
)

