# SConstruct

import os

outdir = 'build'

aEnv = Environment(ENV = os.environ)

defines = ['DEBUG_OUTPUT', 'LALR1_SHIFT_FIRST'] #['COMBINE_REGEX_MACHINES']
if int(ARGUMENTS.get('noreduce', 0)) != 0:
	defines.append('NOTCALL_REDUCE_FUNC')
debug_ccflags = ''
if os.sys.platform.lower() == 'win32':
	aEnv['boost_inc'] = r'D:\Work\libs\boost_1_50_0'
	aEnv['boost_libpath'] = r'D:\Work\libs\boost_1_50_0\stage\lib'
	aEnv.Append(CCFLAGS = '/D WIN32')
	aEnv.Append(CCFLAGS = '/D UNICODE /D VISUAL_STDIO /EHsc /MTd /D DEBUG_OUTPUT')
	aEnv.Append(CCFLAGS = ' '.join(['/D ' + x for x in defines]))
	debug_ccflags = '/D _DEBUG /D DEBUG'
else:
	aEnv['boost_inc'] = ''
	aEnv['boost_libpath'] = ''
	debug_ccflags = '-g -rdynamic -D_DEBUG'
	aEnv.Append(CCFLAGS = ' '.join(['-D' + x for x in defines]))

if int(ARGUMENTS.get('debug', 0)):
	aEnv.Append(CCFLAGS = debug_ccflags)
	aEnv.VariantDir(os.path.join(outdir, 'debug/'), '.', duplicate = 0)
	aEnv['builddir'] = os.path.join(outdir, 'debug/')
else:
	aEnv.Append(CCFLAGS = '-g')
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
aEnv.SConscript('cprogramlanguage/SConscript', variant_dir = varf('cpl'), duplicate = 0)
aEnv.SConscript('cexpressiontest/SConscript', variant_dir = varf('cexp'), duplicate = 0)
