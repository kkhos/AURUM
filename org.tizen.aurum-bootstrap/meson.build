bootstrap_svr_inc = [
   libaurum_inc,
   root_inc,
   include_directories('inc'),
   include_directories('inc/Commands'),
   include_directories('inc/Runnable'),
]

bootstrap_svr_src = [
   files('src/BootstrapServer.cc'),
   files('src/AurumServiceImpl.cc'),
   files('src/ObjectMapper.cc'),
   files('src/Runnable/LaunchAppRunnable.cc'),
   files('src/Runnable/ActionAndWaitEventRunnable.cc'),
]

bootstrap_svr_src += [
   files('src/Commands/ClearCommand.cc'),
   files('src/Commands/ClickCommand.cc'),
   files('src/Commands/CloseAppCommand.cc'),
   files('src/Commands/FindElementCommand.cc'),
   files('src/Commands/FindElementsCommand.cc'),
   files('src/Commands/FlickCommand.cc'),
   files('src/Commands/GetAttributeCommand.cc'),
   files('src/Commands/GetDeviceTimeCommand.cc'),
   files('src/Commands/GetLocationCommand.cc'),
   files('src/Commands/GetSizeCommand.cc'),
   files('src/Commands/GetValueCommand.cc'),
   files('src/Commands/InstallAppCommand.cc'),
   files('src/Commands/GetAppInfoCommand.cc'),
   files('src/Commands/LaunchAppCommand.cc'),
   files('src/Commands/LongClickCommand.cc'),
   files('src/Commands/RemoveAppCommand.cc'),
   files('src/Commands/SendKeyCommand.cc'),
   files('src/Commands/SetValueCommand.cc'),
   files('src/Commands/TouchDownCommand.cc'),
   files('src/Commands/TouchMoveCommand.cc'),
   files('src/Commands/TouchUpCommand.cc'),
   files('src/Commands/KillServerCommand.cc'),
   files('src/Commands/TakeScreenshotCommand.cc'),
   files('src/Commands/Command.cc'),
   files('src/Commands/PreCommand.cc'),
   files('src/Commands/PostCommand.cc'),
   files('src/Commands/DumpObjectTreeCommand.cc'),
   files('src/Commands/GetScreenSizeCommand.cc'),
   files('src/Commands/ActionAndWaitEventCommand.cc'),
   files('src/Commands/SetFocusCommand.cc'),
]

bootstrap_svr_dep = [
   libaurum,
   grpc_deps,
]

if get_option('tizen') == true
bootstrap_svr_dep += [
   dependency('capi-appfw-app-control'),
   dependency('capi-system-device'),
   dependency('capi-appfw-app-manager'),
   dependency('capi-appfw-package-manager'),
   dependency('capi-appfw-service-application'),
   dependency('capi-system-system-settings'),
   dependency('capi-base-utils-i18n'),
   dependency('vconf'),
]
endif

bootstrap_svr_bin = executable(
   'aurum-bootstrap',
   [bootstrap_svr_src, grpc_src, grpc_pb_src],
   dependencies: bootstrap_svr_dep,
   include_directories: bootstrap_svr_inc,
   install:true,
   install_dir:  join_paths(get_option('tzapp_path'), 'org.tizen.aurum-bootstrap', 'bin'),
   pie:true,
)

install_data('org.tizen.aurum-bootstrap.xml', install_dir: get_option('tzpackage_path'))
