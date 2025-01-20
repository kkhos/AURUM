test_src = [
    ['test_misc', files('Test_Misc.cc')],
    ['test_sel', files('Test_Sel.cc')],
    ['test_uidevice', files('Test_UiDevice.cc')],
    ['test_uiobject', files('Test_UiObject.cc')],
    ['test_uiselector', files('Test_UiSelector.cc')],
]

if get_option('build_c_bindings') == true
    test_src += [['Test_CBindings', files('Test_CBindings.cc')]]
endif

test_inc = [
    include_directories('./')
]

test_deps = [
    dependency('gmock', main:true),
    libaurum,
]

foreach atest : test_src
    test_bin = executable(atest[0], atest[1],
                    include_directories: test_inc,
                    dependencies: test_deps,
                    pie:true,
                    install:get_option('tizen_gcov'),
            )

    test(atest[0], test_bin)
endforeach
