The repository contains two C files and a makefile for their compilation.
The files were created in a virtual machine Ubuntu OS- Ubuntu 14.04.5 LTS with a gcc compiler and using the OpenGL library.
make -f Makefile.mk for compilation.


In draw_line_ellipse file:
  - Create a window for drawing.
  - Right click for options (Draw Line, Draw ellipse, Clear screen).
  - Draw line: Select 2 points and a line is created using the Bresenham algorithm.
  - Draw ellipse: Select three points, center, a value, b value of ellipse, the ellipse is created using the Bresenham algorithm.
  - Clear screen: Return the window to it's original state.


In create fill polygon:
  - Select number of edges (3-10).
  - Choose color (Red, Green, Blue).
  - Select the points.
  - First, the lines are drawn.
  - Second, the polygon created by the lines are filled with color.
  - User has the option to clear the window.
   
In polygon_cut:
  - The program performs the "selection" of a specific area of a polygon, using a rectangle for selection with the help of Sutherland-Hodgman algorithm.
  - A grey window is created.
  - Right click to choose number of edges in polygon.
  - Right click to choose color.
  - Right click to fill the polygon with color.
  - Press "f" to create a rectangle.
  - Press "c" to create the intersection between the rectangle and the polygon.
  - Press "space" to show the polygon that was created from the intersection of the original polygon and the rectangle.
  - Right click to select clear screen to obtain the original window.
