# Viriditas
Mars Colony Game

Current state:

generates a plasma fractal during runtime

fps counter is in the upper left corner


controls:

space generates a new layer to the plasma fractal

shift-space deletes the current plasma fractal and creates a new one

esc quits the program

mouseover and hover generates a tooltip which shows the floating point value of the grid cell under the mouse

mouseover textures the grid ell under the mouse



How to build:

acquire the freeglut and freetype libraries

freeglut binaries for VC++ can be found here:

http://www.transmissionzero.co.uk/software/freeglut-devel/

freetype binaries can be found here:

http://www.freetype.org/download.html

The visual studio project configuration files should be set up to link to each of these libraries at the full paths:

C:\mylib\freeglut

C:\Program Files (x86)\GnuWin32  -- (the default install location for the freetype installer)

to link each of these in visual studio add "C:\mylib\freeglut\include;C:\Program Files (x86)\GnuWin32\include;C:\Program Files (x86)\GnuWin32\include\freetype2;" to the end of the "Project\Properties\Configuration Properties\VC++ Directories\Include Directories" field
and 
"C:\mylib\freeglut\lib\freeglut.lib;C:\Program Files (x86)\GnuWin32\lib\freetype.lib;" to the end of the "Project\Properties\Configuration Properties\Linker\Input\Additional Dependencies" field, immediately before "%(AdditionalDependencies)"

opengl distributions come, as far as I know, by default with microsoft visual studio or windows, contained in already existing directory paths under the directory GL/
