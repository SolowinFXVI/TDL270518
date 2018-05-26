all:	eval.c
	make	compil

test:
	make	clean	&&	make	compil	&&	make	run

test2:
	make	clean	&&	make	compil	&&	make	run2

run:
	./eval	'(2*((3+45)-8))+14/3'

run2:
	./eval	'2*(3+1)'

compil:	eval.c
	gcc	-o	eval	eval.c	-Wall

clean:
	rm	-f	eval
