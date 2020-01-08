bootstrap_svr_inc = [
   libaurum_inc,
   root_inc,
   include_directories('inc'),
   include_directories('inc/Commands'),
   loguru_inc,
]

bootstrap_svr_src = [
   files('src/BootstrapServer.cc'),
   files('src/AurumServiceImpl.cc'),
   files('src/ObjectMapper.cc'),
]

bootstrap_svr_src += [
   files('src/Commands/ClearCommand.cc'),
   files('src/Commands/ClickCommand.cc'),
   files('src/Commands/CloseAppCommand.cc'),
   files('src/Commands/FindElementCommand.cc'),
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
   files('src/Commands/SyncCommand.cc'),
   files('src/Commands/KillServerCommand.cc'),
   files('src/Commands/Command.cc'),
   files('src/Commands/PreCommand.cc'),
   files('src/Commands/PostCommand.cc'),
]

bootstrap_svr_dep = [
   libaurum,
   grpc_deps,
   loguru_deps,
]

if get_option('tizen') == true
bootstrap_svr_dep += [
   dependency('capi-appfw-app-control'),
   dependency('capi-appfw-app-manager'),
   dependency('capi-appfw-package-manager'),
]
endif

bootstrap_svr_bin = executable(
   'aurum_bootstrap',
   [bootstrap_svr_src, grpc_src, grpc_pb_src],
   dependencies: bootstrap_svr_dep,
   include_directories: bootstrap_svr_inc,
   link_with: libloguru,
   install:true,
   pie:true,
)

system_unit_dir = ''
user_unit_dir = ''

systemd = dependency('systemd', required: false)
if systemd.found()
   user_unit_dir = systemd.get_pkgconfig_variable('systemduserunitdir')
endif

install_data('aurum-bootstrap.service', install_dir:user_unit_dir)
