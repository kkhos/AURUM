
libaurum_src += [
  files('Sel.cc'),
  files('UiDevice.cc'),
  files('UiObject.cc'),
  files('UiSelector.cc'),
  files('Comparer.cc'),
  files('Until.cc'),
  files('Waiter.cc'),
  files('PartialMatch.cc'),
  files('A11yEvent.cc'),
  files('A11yEventHandler.cc'),
  files('AurumXML.cc'),
]

if get_option('mqtt_enabled') == true
  libaurum_src += [
    files('SaObject.cc'),
  ]
endif

subdir('Accessibility')
if get_option('mqtt_enabled') == true
  subdir('ScreenAnalyzer')
endif
subdir('Impl')
subdir('Runnable')
