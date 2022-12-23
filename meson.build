project('mint', 'cpp', version: '0.1', default_options: ['cpp_std=c++2a'])
fs = import('fs')

sources = [
  'src/str.cc',
  'src/print.cc'
]

includes = include_directories('../incl')
lib = static_library('mint', sources, include_directories: includes,
  cpp_args: ['-ggdb'])

# For testing
if fs.is_file('main.cc')
  executable('main', 'main.cc', link_with: lib, include_directories: includes)
endif