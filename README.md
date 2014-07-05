Omnidirectional Shadow Mapping
==============================

Simple demonstration on producing shadows for a point light using OpenGL and
GLSL. This was created as my final project in the computer graphics course
[DH2323](https://www.kth.se/student/kurser/kurs/DH2323?l=en).

The project make use of the C++11 language standard, GLM, GLEW, SDL2 and
tinyobjloader.

License
-------
Licensed under GNU GPL v3.0.

How-to
------
Build and run

    $ scons
    $ cd bin
    $ ./omni
    
Cleanup

    $ scons -c

References
----------
1. Lance Williams. Casting curved shadows on curved surfaces. SIGGRAPH’78 Proceedings of the 5th annual conference on Computer graphics and interactive techniques, pages 270–274, 1978.
2. Dave Shreiner; Graham Sellers; John M. Kessenich; Bill M. Licea-Kane. OpenGL Programming Guide: The Official Guide to Learning OpenGL. Addison-Wesley Professional, 8th edition, 2013.
3. Philipp S. Gerasimov. Chapter 12. omnidirectional shadow mapping. c2004 [downloaded 2014 May 13]. Available from http://http.developer.nvidia.com/GPUGems/gpugems_ch12.html.
4. Specification for texture cube map. c1999 [downloaded 2014 May 20]. Available from https://www.opengl.org/registry/specs/ARB/texture_cube_map.txt.
