
APPNAME = 'numerical and temporal expression normalizer'
VERSION = '0.5.0'

def options(opt):
  opt.load('compiler_cxx')
  opt.load('unittest_gtest')

def configure(conf):
  conf.env.CXXFLAGS += ['-O2', '-Wall', '-g', '-pipe']
  conf.load('compiler_cxx')
  conf.load('unittest_gtest')
  conf.check_cfg(package = 'pficommon', args = '--cflags --libs')
  conf.check_cfg(package = 'ux', args = '--cflags --libs')
  #conf.check_cxx(lib='re2', libpath=conf.env.LIBDIR)
  #conf.check_cfg(package = 'boost', args = '--cflags --libs')
  #conf.check_cxx(lib='libname', header_name = 'header.h')
  pass

def build(bld):
  bld.recurse('src')
