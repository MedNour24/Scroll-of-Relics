
CC = gcc
CFLAGS = -Wall -lSDL -lSDL_ttf -lSDL_mixer -lSDL_image
OBJS = main.o enigme.o
TARGET = prog

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(OBJS) -o $(TARGET) $(CFLAGS)

main.o: main.c enigme.h
	$(CC) -c main.c -o main.o $(CFLAGS)

enigme.o: enigme.c enigme.h
	$(CC) -c enigme.c -o enigme.o $(CFLAGS)

clean:
	rm -f $(OBJS) $(TARGET)

