CC = gcc

CFLAGS = -g #-DGL_GLEXT_PROTOTYPES 
INCFLAGS = -I/usr/X11R6/include -I/sw/include \
		-I/usr/sww/include -I/usr/sww/pkg/Mesa/include
LDFLAGS = -L/usr/X11R6/lib -L/sw/lib -L/usr/sww/lib \
		-L/usr/sww/bin -L/usr/sww/pkg/Mesa/lib -lGLEW -lglut -lGLU -lGL -lX11 -lm

RM = /bin/rm -f
all: askisi2 askisi4
askisi2: askisi2.o 
	$(CC) $(CFLAGS) -o askisi2 askisi2.o $(INCFLAGS) $(LDFLAGS)
askisi2.o: askisi2.c 
	$(CC) $(CFLAGS) $(INCFLAGS) -c askisi2.c 
askisi4: askisi4.o 
	$(CC) $(CFLAGS) -o askisi4 askisi4.o $(INCFLAGS) $(LDFLAGS)
askisi4.o: askisi4.c 
	$(CC) $(CFLAGS) $(INCFLAGS) -c askisi4.c
clean: 
	$(RM) *.o askisi2 askisi4
 

