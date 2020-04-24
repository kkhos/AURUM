test_src = [
    files('ua_test.cpp'),
]

test_inc = [
    include_directories('./')
]

test_deps = [
    dependency('gmock', main:true),
    libaurum,
]

test_bin = executable('gtest_aurum', test_src,
                    include_directories: test_inc,
                    dependencies: test_deps,
                    pie:true,
                    install:get_option('tizen_gcov'),
            )

test('gtest_aurum', test_bin)

