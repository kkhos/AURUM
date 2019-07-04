loguru_inc = [
   include_directories('inc'),
]

loguru_src = [
   files('src/loguru.cpp'),
]

loguru_deps = [
    dependency('threads'),
    meson.get_compiler('cpp').find_library('dl', required : true),
]

libloguru = static_library(
    'libloguru',
    loguru_src,
    dependencies: loguru_deps,
    include_directories: loguru_inc,
    install:false
)