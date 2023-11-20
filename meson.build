project(
    'Louvre Example',
    'c','cpp',
    version : '0.1.0',
    meson_version: '>= 0.56.0',
    default_options: [
        'warning_level=2',
        'cpp_std=c++11',
    ]
)
 
cpp = meson.get_compiler('cpp')
 
include_directories = [
    '/usr/include/pixman-1',
    '/usr/include/drm',
    '/usr/include/libdrm',
    '/usr/include/Louvre'
]
 
include_directories_filtered = []
 
# Exclude non-existent directories to prevent meson from throwing errors
foreach d : include_directories
    if run_command('[', '-d', d, ']', check : false).returncode() == 0
        include_directories_filtered += [include_directories(d)]
    endif
endforeach
 
louvre_dep = cpp.find_library('Louvre')
 
# List C/C++ source files stored in ./src
sources = run_command('find', './src', '-type', 'f', '-name', '*[.c,.cpp,.h,.hpp]', check : false).stdout().strip().split('\n')
 
executable(
    'louvre-example',
    sources : sources,
    include_directories : include_directories_filtered,
    dependencies : [
        louvre_dep,
])
