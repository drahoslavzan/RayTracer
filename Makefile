CC=g++
CFLAGS=-W -Wall -Wextra -Wno-long-long -fopenmp -O2 -DNDEBUG -I/usr/include/eigen2
LFLAGS=-lm -lglut -lGL -lgomp -lnoise
PROG=csgrt


all: $(PROG)

$(PROG): main.o material.o materials.o object.o ray.o scene.o shape.o fps.o
	$(CC) -o $@ $^ $(LFLAGS)


main.o: main.cc
	$(CC) $(CFLAGS) -c -o $@ $<

material.o: material.cc material.h
	$(CC) $(CFLAGS) -c -o $@ $<

materials.o: materials.cc materials.h
	$(CC) $(CFLAGS) -c -o $@ $<

object.o: object.cc object.h
	$(CC) $(CFLAGS) -c -o $@ $<

ray.o: ray.cc ray.h
	$(CC) $(CFLAGS) -c -o $@ $<

scene.o: scene.cc scene.h
	$(CC) $(CFLAGS) -c -o $@ $<

shape.o: shape.cc shape.h
	$(CC) $(CFLAGS) -c -o $@ $<

fps.o: fps.cc fps.h
	$(CC) $(CFLAGS) -c -o $@ $<


clean:
	rm -f *.o

