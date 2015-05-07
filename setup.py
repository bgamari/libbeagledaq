#!/usr/bin/env python

from distutils.core import setup

setup(name='beagledaq',
      version='0.1',
      description='Library for interfacing with beagle-daq',
      author='Kieran Ramos',
      author_email='kramos@physics.umass.edu',
      url='https://www.github.com/krrk/libbeagledaq',
      py_modules=['beagledaq'],
      scripts=['tools/adc-acquire.py','tools/dac-output.py']
      )
