dconf.set('TOP_SRCDIR', meson.source_root())
dconf.set('TOP_BUILDDIR', meson.build_root())

doxyfile = configure_file(input: 'Doxyfile.in',
                          output: 'Doxyfile',               
                          configuration: dconf,
                          install: false)                   
                                                            
datadir = join_paths(get_option('datadir'), 'doc', 'aurum') 
                                                            
html_target = custom_target('aurum-docs',                   
                            input: doxyfile,                
                            output: 'html',                 
                            command: [doxygen, doxyfile],   
                            install: true,                  
                            install_dir: datadir)           

