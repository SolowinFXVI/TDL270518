all:	eval.c
	make	compil

test:
	make	clean	&&	make	compil	&&	make	run

run	:
	./eval	'(2*((3+45)-8))+14/3'

compil:	eval.c
	gcc	-o	eval	eval.c	-Wall

clean:
	rm	-f	eval
