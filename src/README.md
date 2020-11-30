# Directory structure

* src: Source files (.cpp)

* include: Header files (.hpp)

* images: Images and fonts

* sound: Audio files

* xml: XML files (for tracks)

* cmake_modules: FindSFML.cmake script which locates SFML

* SFML: SFML headers and libraries for Windows and Visual Studio 2015.
 	Not compatible with other compilers than Visual C++.

* CMakeLists.txt: For generating Makefile and other building tools. 

***

Do not build the project in this directory.
Instead, create a separate building directory and run `cmake ..` there to generate Makefiles. 





