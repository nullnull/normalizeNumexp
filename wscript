import os
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
  create_dic_file(bld)
  bld.recurse('src')
#  bld.install_files('${PREFIX}/include', 'src/*.hpp') #cannot install
  for dpath, dnames, fnames in os.walk("src") :
    for fname in fnames :
      if not fname.endswith(".hpp") : continue
      bld.install_files('${PREFIX}/include/normalizeNumexp/', [dpath+"/"+fname])
  for dpath, dnames, fnames in os.walk("src/dic") :
    for fname in fnames :
      if not fname.endswith(".txt") : continue
      bld.install_files('${PREFIX}/lib/normalizeNumexp/'+dpath[4:], [dpath+"/"+fname])
  

def create_dic_file(bld) :
	source = """
#include "dictionary_dirpath.hpp"
namespace dictionary_dirpath {
std::string get_dictionary_dirpath(){
			return \""""
	source += str(bld.env.PREFIX) + "/lib/normalizeNumexp/dic/\";}}"
#	source += "/home/katsuma/src/normalizeNumexp/src/dic/\";}}"
	fout = open("./src/dictionary_dirpath.cpp", "w")
	fout.write(source)
