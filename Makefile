CFLAGS= -g -fsanitize=address,undefined


all: build  clean

build:
	gcc  -c  -Wextra  -ftrapv  main.c -o main.o -lreadline -lm $(CFLAGS)
	gcc -c  -Wextra -ftrapv graph.c -o stack.o $(CFLAGS)
	gcc  main.o graph.c -o gg -lreadline -lm $(CFLAGS)


clean:
	rm -rf *.o
