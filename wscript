#! /usr/bin/env python

# waf imports
import Common, Params, misc


VERSION='0.0.1'
APPNAME='manet-visualizer'

srcdir = '.'
blddir = 'build'

# exclude the following directories
# for target dist (./waf dist)
g_excludes = '.git .hg'.split()

def set_options(opt):
  opt.add_option('--cscope',
                   help=('generate an cscope index - ready to use with vim/emacs'),
                   action="store_true", default=False,
                   dest='cscope_gen')


def configure(conf):

  conf.check_tool('gcc')

  e = conf.create_library_enumerator()

  #conf.sub_config('src')

  conf.env['CFLAGS_MYPROG']='-O3 -pipe'

  conf.env['INSTALL_DIR']='/usr/bin'

  conf.env['CFLAGS'] = '-D_REENTRANT -DDBG_ENABLED -Wall -W -O0 -ggdb3 -ftemplate-depth-128'

  conf.env['LIB_M'] = 'm'

  conf.env['CXXFLAGS_X11'] = ''
  conf.env['LIBPATH_X11']  = '/usr/lib'
  conf.env['LIB_X11']      = 'X11'
  e.mandatory = 1
  e.name = 'X11'
  e.uselib_store = 'X11'
  e.paths = ['/usr/X11R6/lib','/usr/lib','/usr/local/lib']
  e.run()

  conf.env['CXXFLAGS_Xi'] = ''
  conf.env['LIBPATH_Xi']  = '/usr/lib'
  conf.env['LIB_Xi']      = 'Xi'
  e.mandatory = 1
  e.name = 'Xi'
  e.run()

  conf.env['CXXFLAGS_Xmu'] = ''
  conf.env['LIBPATH_Xmu']  = '/usr/lib'
  conf.env['LIB_Xmu']      = 'Xmu'
  e.mandatory = 1
  e.name = 'Xmu'
  e.run()

  conf.env['CXXFLAGS_glut'] = ''
  conf.env['LIBPATH_glut']  = '/usr/lib'
  conf.env['LIB_glut']      = 'glut'
  e.mandatory = 1
  e.name = 'glut'
  e.run()

  conf.env['CXXFLAGS_GL'] = ''
  conf.env['LIBPATH_GL']  = '/usr/lib'
  conf.env['LIB_GL']      = 'GL'
  e.mandatory = 1
  e.name = 'GL'
  e.run()

  conf.env['CXXFLAGS_GLU'] = ''
  conf.env['LIBPATH_GLU']  = '/usr/lib'
  conf.env['LIB_GLU']      = 'GLU'
  e.mandatory = 1
  e.name = 'GLU'
  e.run()

  conf.define('VERSION', VERSION)
  conf.env.append_value('CCFLAGS', '-DHAVE_CONFIG_H')
  conf.write_config_header('config.h')

def build(bld):

	bld.add_subdirs('src data')


def cscope_gen():

  if subprocess.Popen(['cscope', '-R', '-b']).wait():
    raise SystemExit(1)


def shutdown():
	# display the graph of header dependencies
	if 1: return
	import Params, os, types

	f = open('foo.dot', 'w')
	f.write("digraph G {\n")
	table = Params.g_build.node_deps
	for a in table:
		for x in table[a]:
			if type(table[a][x]) is types.ListType:
				for y in table[a][x]:
					f.write('"%s" -> "%s";\n' % (x, y))
			else:
				f.write('"%s" -> "%s";\n' % (x, table[a][x]))
	f.write('}\n')
	f.close()

	try:
		os.popen('dot -Tpng foo.dot -ofoo.png').read()
	except:
		print "the dot program was not found - install graphviz?"
	else:
		print "generated foo.png"
