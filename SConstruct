env = Environment(
    CC='g++',
    CCFLAGS='-std=c++11 -pedantic -Wall -Wextra -O3',
    CPPPATH='#/',
    LIBS=['SDL2', 'GL', 'GLEW']
)

source = Glob('src/*.cpp')
source += ['#/lib/tinyobjloader/tiny_obj_loader.cc']

program = env.Program(target='bin/omni', source=source);
