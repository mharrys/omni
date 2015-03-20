Omnidirectional Shadow Mapping
==============================
Demonstration on how to produce shadows for a point light. This requires six
render passes to build the shadow map, then one additional render pass to
render the scene with shadows.

+ Press `F1` to toggle shadows.
+ Press `F2` to toggle shading (shadow only mode).
+ Press `SPACE` to toggle point light movement.

How-to
------
This project depends on [Gust](https://github.com/mharrys/gust), see its
project page for details on dependencies.

Recursive clone required

    $ git clone --recursive <repository>

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
