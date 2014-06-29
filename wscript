#!/usr/bin/env python

def options(opt):
  opt.load('compiler_c')
  opt.load('man', tooldir='waftools')

def configure(cnf):
  cnf.load('compiler_c')
  cnf.load('man', tooldir='waftools')
  for package in "xmms2-client xmms2-client-glib glib-2.0".split():
    cnf.check_cfg(package=package, args=['--cflags', '--libs'], uselib_store='xmms2')

def build(bld):
  env = bld.env
  bld.program(
      source='xmms2-reseeker.c',
      target='xmms2-reseeker',
      use='xmms2')
  bld(features = 'man',
      source = 'xmms2-reseeker.1')
  bld.install_files(
      '${DATAROOTDIR}/xmms2/scripts/startup.d',
      'xmms2-reseeker.sh',
      chmod=0o755)
