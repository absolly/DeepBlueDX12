The CodeBlocks MinGW_extended bundle was constructed for ease of use in the Saxion C++ 3D Rendering course.
See the MicroEngine CodeBlocks readme for more info on how to use this bundle.

For details on how this bundle was constructed in case you do NOT (Why god why?) want to use the bundle or want to upgrade/downgrade to a different version of a library, see below.

Bundle construction:
--------------------

1. MinGW
--------------------------------------
Download https://sourceforge.net/projects/codeblocks/files/Binaries/16.01/Windows/codeblocks-16.01mingw-setup.exe/download
Copy the complete MinGW folder to MinGW_extended

2. Overlay Mingw-to-string patch
--------------------------------------
Download http://tehsausage.com/mingw-to-string:
Copy the following files:
- include\* to include\*
- os-defines.h to lib\gcc\mingw32\4.9.2\include\c++\mingw32\bits

3. Overlay GLM
--------------------------------------
Download GLM from https://github.com/g-truc/glm/releases/download/0.9.8.2/glm-0.9.8.2.zip
Copy the GLM folder at glm-0.9.8.2\glm\glm (so the deepest one) to MinGW_extended\include

4. Overlay GLEW
--------------------------------------
To overlay GLEW, I rebuilt Glew from the source (see readme_2_compiling_libraries.txt) and
installed all the resulting .dll, .lib and .a files into the correct MinGW_extended folders.
This means you can use GLEW in debug/release mode, shared and static mode.

5. Overlay SFML
---------------
Download http://www.sfml-dev.org/files/SFML-2.4.0-windows-gcc-4.9.2-tdm-32-bit.zip
Copy include/bin/lib to the corresponding folders with the same name in the MinGW_extended folder

6. Dll folder
---------------
The DLLs folder is simply a combination of all SFML and GLEW dll's for easy copying to your exe folder.
These are only required in CodeBlocks when you want to release a dynamically build program.