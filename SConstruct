env = Environment(
    CC='g++',
    CCFLAGS='-std=c++11 -pedantic -Wall -Wextra -O3',
)

SConscript('lib/gust/SConscript', 'env', variant_dir='.gust', duplicate=0)
env.Append(LIBS='gust')
env.Append(LIBPATH='.gust/build')
env.Append(CPPPATH=['lib/gust/lib', 'lib/gust/src'])

env.Program(target='bin/omni', source=Glob('src/*.cpp'))
