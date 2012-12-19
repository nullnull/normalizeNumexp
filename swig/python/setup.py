#!/usr/bin/env python

"""
setup.py file for SWIG example
"""

from distutils.core import setup, Extension
numerical_expression_extractor_module = Extension('_numerical_expression_extractor',
        sources = ['numerical_expression_extractor_wrap.cxx', 'digit_utility.cpp', 'number_normalizer.cpp', 'normalizer_utility.cpp',  'numerical_expression_normalizer.cpp', 'abstime_expression_normalizer.cpp', 'reltime_expression_normalizer.cpp', 'duration_expression_normalizer.cpp', 'numerical_expression_extractor.cpp'],
                           include_dirs=["/home/katsuma/usr/local/include"],
                           library_dirs=["/home/katsuma/usr/local/lib"],
#                           extra_compile_args=["`pkg-config --libs pficommon`"],
                           extra_link_args=["-L/home/katsuma/usr/local/lib -lpficommon -lpficommon_visualization -lpficommon_text -lpficommon_network_base -lpficommon_concurrent -lpficommon_data -lpficommon_math -lpficommon_system -lpficommon_network_http -lpficommon_lang -lpficommon_network_rpc -lpficommon_network_cgi -shared"],
                           libraries=['pficommon'],
                           language='c++',
                           )

setup (name = 'numerical_expression_extractor',
       version = '0.1',
       author      = "SWIG Docs",
       description = """Simple swig example from docs""",
       ext_modules = [numerical_expression_extractor_module],
       py_modules = ["numerical_expression_extractor"],
       )
