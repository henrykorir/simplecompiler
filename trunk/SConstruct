# SConstruct

import os

outdir = 'build'

aEnv = Environment(ENV = os.environ)

debug_ccflags = ''
if os.sys.platform.lower() == 'win32':
	aEnv.Append(CCFLAGS = '/D WIN32')
	aEnv.Append(CCFLAGS = '/D UNICODE /D VISUAL_STDIO /EHsc /MT /D DEBUG_OUTPUT')
	debug_ccflags = '/D _DEBUG'
else:
	debug_ccflags = '-g -rdynamic'

if int(ARGUMENTS.get('debug', 0)):
	aEnv.Append(CCFLAGS = debug_ccflags)
	aEnv.VariantDir(os.path.join(outdir, 'debug/'), '.', duplicate = 0)
	aEnv['builddir'] = os.path.join(outdir, 'debug/')
else:
	aEnv.VariantDir(os.path.join(outdir, 'release/'), '.', duplicate = 0)
	aEnv['builddir'] = os.path.join(outdir, 'release/')

Export('aEnv')

def varf(o):
	return aEnv['builddir'] + o

aEnv.SConscript('third_party/SConscript', variant_dir = varf('third_party'), duplicate = 0)
#aEnv.SConscript('symbol/SConscript', variant_dir = varf('symbol'), duplicate = 0)
aEnv.SConscript('syntax/SConscript', variant_dir = varf('syntax'), duplicate = 0)
aEnv.SConscript('galgorithm/SConscript', variant_dir = varf('galgorithm'), duplicate = 0)
aEnv.SConscript('compiler/SConscript', variant_dir = varf('compiler'), duplicate = 0)
aEnv.SConscript('makecompiler/SConscript', variant_dir = varf('mcompiler'), duplicate = 0)
aEnv.SConscript('test/SConscript', variant_dir = varf('test'), duplicate = 0)
