#/* 
# * Author: David Vela Tirado
# * Software Developer
# * email:  david.vela.tirado@gmail.com
# * 
# */

PHONY: all
CFLAGS=  -Wall 
all:   practica5.1 

practica5.1: ejer5.1.c 
	gcc $(CFLAGS) ejer5.1.c  -o practica51




clean: 
	-rm -f *.o
	-rm -f practica51 
	
