# SConstruct

import os

outdir = 'build'

aEnv = Environment(ENV = os.environ)

if int(ARGUMENTS.get('debug', 0)):
	aEnv.Append(CCFLAGS = '-g')
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
