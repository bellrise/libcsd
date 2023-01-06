project('csd', 'cpp', version: '0.1', default_options: ['cpp_std=c++2a'])
fs = import('fs')

sources = [
  'src/str.cc',
  'src/print.cc',
  'src/error.cc'
]

includes = include_directories('../incl')
lib = static_library('csd', sources, include_directories: includes,
  cpp_args: ['-O3'], prelink: true, install: true)

# For testing
if fs.is_file('main.cc')
  executable('main', 'main.cc', cpp_args: ['-O3'], link_with: lib, include_directories: includes,
    link_args: ['-fsanitize=address'])
endif
