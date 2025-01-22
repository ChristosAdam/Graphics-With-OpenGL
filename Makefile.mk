CC = gcc

CFLAGS = -g #-DGL_GLEXT_PROTOTYPES 
INCFLAGS = -I/usr/X11R6/include -I/sw/include \
		-I/usr/sww/include -I/usr/sww/pkg/Mesa/include
LDFLAGS = -L/usr/X11R6/lib -L/sw/lib -L/usr/sww/lib \
		-L/usr/sww/bin -L/usr/sww/pkg/Mesa/lib -lGLEW -lglut -lGLU -lGL -lX11 -lm

RM = /bin/rm -f
all: draw_line_ellipse create_fill_polygon
draw_line_ellipse:  draw_line_ellipse.o 
	$(CC) $(CFLAGS) -o  draw_line_ellipse  draw_line_ellipse.o $(INCFLAGS) $(LDFLAGS)
draw_line_ellipse.o:  draw_line_ellipse.c 
	$(CC) $(CFLAGS) $(INCFLAGS) -c  draw_line_ellipse.c 
create_fill_polygon: create_fill_polygon.o 
	$(CC) $(CFLAGS) -o create_fill_polygon create_fill_polygon.o $(INCFLAGS) $(LDFLAGS)
create_fill_polygon.o: create_fill_polygon.c 
	$(CC) $(CFLAGS) $(INCFLAGS) -c create_fill_polygon.c
clean: 
	$(RM) *.o  draw_line_ellipse create_fill_polygon
 

