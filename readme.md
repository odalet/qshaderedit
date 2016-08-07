This is a fork of [https://github.com/magicafe/qshaderedit](https://github.com/magicafe/qshaderedit)
The differences are:

* Only targets Windows (x86/x64) and Qt 5.7. As such,
	* CMake support was dropped;
	* Linux / OSX support is not tested nor guaranteed anymore.
* Code layout was modified and many files were deleted.
* Currently build with Visual Studio 2015 with Qt5 Plugin.
* Depends on Qt 5.7 (built against VS 2015 x86 runtime).

# Dependencies

* Visual Studio 2015
* Qt 5.7 (for VS2015 x86 and/or VS2015 x64)
* Qt5 Plugin for VS2015. Download [here](https://visualstudiogallery.msdn.microsoft.com/c89ff880-8509-47a4-a262-e4fa07168408).
* Glew 2.0: [http://glew.sourceforge.net/](http://glew.sourceforge.net/)

----
Original README:

## INTRODUCTION

QShaderEdit is a simple multiplatform shader editor inspired by Apple's OpenGL Shader 
Builder.

It supports ARB programs, GLSL shaders and CgFx effects.


## REQUIREMENTS

You will need at least:

* Qt 4.4, recommended Qt4.5
* GLEW 1.3
* cmake 2.4

Optionally, if the Cg (1.4 or above) runtime is found, support for CgFx effects will be 
available.


## DOWNLOAD

You can find the latest release of QShaderEdit at: 

http://castano.ludicon.com/page.php?page_id=118


## INSTALLATION

I use cmake as the build system. To build the sources, run cmake on the source directory, 
that will generate native makefiles or projects. For example, on unix platforms you have
to run make.

$ cmake && make

or

1.qmake
2.make
3.sudo make install

For more info about cmake, look at cmake's documentation.


## DEVELOPMENT

Latest version of the source code can be obtained at:

svn co https://svn.sourceforge.net/svnroot/castano-stuff/qshaderedit qshaderedit

Development is discussed in the following mailing list:

https://lists.sourceforge.net/lists/listinfo/castano-stuff-qshaderedit

The mailing list and the subversion repository are hosted on the following sourceforge project:

http://sourceforge.net/projects/castano-stuff/

