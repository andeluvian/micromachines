# ELEC-A7150 C++ Programming

Programming project: Micro machines

Authors: Miika Karsimus, Hannes Pitkänen, Martin Vidjeskog, Vili Karilas

Directory structure:

  * `plan/` -- Here is the project plan (pdf).

  * `doc/` -- Here is the documentation (pdf).

  * `src/` -- Source files, header files, xml files, building tools etc. are stored here in subfolders..


See also src/README.md

***

Building instructions. SFML 2.3 must be installed in the default path. CMake version 3.1 or higher is required.
Works on Linux and Mac. 

`cd src`

`mkdir build`

`cd build`

`cmake ..`

`make`

And in order to run the executable, type

`make run` or just `./app`

***

You can also compile the program with an IDE. First, create IDE project files by typing for example

`cmake -G "CodeBlocks - Unix Makefiles" ..` or `cmake -G "Xcode" ..`

Then open the created project with IDE. This should also work with Windows and Visual Studio but in this case,
you probably have to link SFML libraries manually to the project. Headers and libraries for SFML 2.4 and Visual Studio 2015 are provided in src/SFML but the libraries may be incompatible with your compiler version.

Type `cmake --help` to see more building options.

***

If you want to generate Doxygen documentation, type `make docs`. It stores the HTML documentation in doc/html directory (in project root).
Of course, Doxygen must be installed before this. When you have run the command, navigate to doc/html and open index.html with your
web browser. After that, click Classes on the toolbar to see the classes and member documentation. Note that the classes under tinyxml2
namespace belong TinyXML2 library and are not implemented by us. 








