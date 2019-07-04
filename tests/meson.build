test_src = [
    files('ua_test.cpp'),
]

test_inc = [
    include_directories('./')
]

test_deps = [
    dependency('gtest', main:true),
    libaurum,
]

test_bin = executable('ua_test', test_src, 
                    include_directories: test_inc,
                    dependencies: test_deps,
            )

test('ua test', test_bin)