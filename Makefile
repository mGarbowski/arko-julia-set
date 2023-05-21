LIB = -I/usr/include/x86_64-linux-gnu -lallegro_font -lallegro

all: julia.c
	gcc julia.c -o julia $(LIB)

clean:
	rm julia
