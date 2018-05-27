all:
	make	test

test:
	make	clean	&&	make	compil	&&	make	runtest

runtest:
	./eval	'(2*('	'(3+'	'45)-8))+14'	'/3'
	./eval  '(50+4)'	'((0))'	'/3'
	./eval	'6-7'
	./eval	'67898*('	'345/(2*(4+3))'	')'	'+7'
	./eval	')2*(3+45)-8)'	')+14/3'

compil:	eval.c
	gcc	-o	eval	eval.c	-Wall

clean:
	rm	-f	eval
