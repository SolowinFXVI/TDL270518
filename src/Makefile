all:	eval.c
	make	compil

test:
	make	clean	&&	make	compil	&&	make	run

run	:
	./eval

compil:	eval.c
	gcc	-o	eval	eval.c	-Wall

clean:
	rm	-f	eval
