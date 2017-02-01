GLEW
----------------------------------------
Download GLEW src from the GLEW site.
Download Codeblocks with MinGW

Install Codeblocks in a folder WITHOUT SPACES.
Open glew-x.x.x\build\vc6\glew.dsw in codeblocks

Select use defaults and import all on the questions you get.
Make sure the selected compiler is installed in a path WITHOUT SPACES.

Remove from Linker settings / other linker options:
--dll 
--image-base 0x62AA0000

Add this to Linker settings / other linker options:
-nostdlib

Update the output for the shared projects so that one filename (release) reads libglew32s and the other one (debug) libglew32sd
Build glew_shared/glew_static in both debug and release mode.
Copy all dlls (2) to the mingw\bin folder and the a/def files (6) to the lib folder.

Set your project settings correctly:
debug shared		lib: glew32d
debug static		lib: glew32sd	define: GLEW_STATIC
release shared		lib: glew32
release static		lib: glew32s	define: GLEW_STATIC

Build and enjoy! 

SFML
----
I haven't tested the building of SFML lately, your best bet would be a process similar to the above or using CMAKE.
If you need help, let me know, we'll have a go at it together.

