LIB = -I/usr/include/x86_64-linux-gnu -lallegro_font -lallegro

all: hello.c
	gcc hello.c -o hello $(LIB)

clean:
	rm hello
