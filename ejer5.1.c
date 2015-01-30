 /* 
# * Author: David Vela Tirado
# * Software Developer
# * email:  david.vela.tirado@gmail.com
# * 
  */
#include <errno.h>
#include <stdio.h>
#include <fcntl.h>
#include <math.h>
#include <time.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>


#define LONG_MAX_MENSAJE 127
#define MAX_INTENTOS 10
#define ACK 0


void mostrarErrorSalir (char *mensaje) {
   perror(mensaje);
   exit (-1);
}

int main(int argc,char *argv[])   //programa ./archivo.txt
 {
   int p1_p2[2];
   int p2_p3[2];
   int pipe_ret = 0;
   int pid1, pid2,pid3;
int status;
 

	if(argc != 2)
	{
		printf("el numero de argumentos es incorrecto\n");
		printf("programa ./archivo.txt\n");
		exit(0);

	}
	printf("el numero de argumentos es correcto\n");

   if ((pipe_ret = pipe(p1_p2)) == -1)
      mostrarErrorSalir ("Error en la tuberia p1_p2\n");

   if ((pipe_ret = pipe(p2_p3)) == -1)
      mostrarErrorSalir ("Error en la tuberia p2_p3\n");


  

   if ((pid1 = fork()) == 0) { /* Proceso1; lee fichero y envia a p2 */
      
	char buffer[1000];	
	//char mensaje;
        //int longMensaje, 
	int descriptor,bytesleidos;

	close(p2_p3[0]); close(p2_p3[1]);
	close (p1_p2[0]); 
		

		descriptor= open ( argv[1],  O_RDONLY);
		if (descriptor==-1) 	//compruebo quela apertura es correcta
		printf("error de apertura de fichero\n");
	
	//while( (bytesleidos=read(descriptor,&mensaje,1)) )     
	while( (bytesleidos=read(descriptor,buffer,1)) ) 	

	{  
	   
         //longMensaje = 1;
	if(write (p1_p2[1], buffer, strlen(buffer)+1)== -1) 
        // if (write (p1_p2[1], &longMensaje, sizeof (longMensaje)) == -1)
               mostrarErrorSalir ("Error al escribir en p1_p2[1]\n");

        } 

      close(p1_p2[1]);  close(descriptor); 
      exit (0); 

   } /* Fin Proceso p1 */

    if ((pid2=fork()) == 0)  /* Proceso 2, recibe el mensaje de p1, selecciona las vocales y las manda a p3 */
 {
     char mensaje;
      int  nbytes,resp;

   
      close (0); 
              	
	close(p1_p2[1]); close(p2_p3[0]);
	

     
      while ( (  nbytes = read (p1_p2[0], &mensaje, sizeof (mensaje) ) ) )
 {
 
if(( (mensaje=='a')||(mensaje=='e')||(mensaje=='i')||(mensaje=='o')||(mensaje=='u') ) )
	{
	printf("vocal: %c  ",mensaje);

         if (write (p2_p3[1], &resp, sizeof (resp)) == -1)
            mostrarErrorSalir ("Error al escribir en p2_p3[1]\n");

	}

if(( (mensaje=='A')||(mensaje=='E')||(mensaje=='I')||(mensaje=='O')||(mensaje=='U') ) )
	{
	printf("vocal: %c  ",mensaje);

         if (write (p2_p3[1], &resp, sizeof (resp)) == -1)
            mostrarErrorSalir ("Error al escribir en p2_p3[1]\n");

	}
 } 

       	close(p1_p2[0]); close(p2_p3[1]);
	exit (0);

   } /* Fin Proceso 2 */


   else if (  (pid3=fork()) == 0) { /* Proceso 3, recibe el mensaje de p2, y cuenta */
      char mensaje;
      int longMensaje, nbytes, contador;
	contador=0;
   
      close (0); 
              	
	close(p1_p2[1]); close(p2_p3[1]);close(p1_p2[0]);

	 while (( nbytes = read (p2_p3[0], &mensaje, sizeof (longMensaje) ) ))		
		contador ++;

  printf("\nEl numero de vocales suma un numero de %d \n",contador);

	close(p2_p3[0]); 

	exit (0);

   } /* Fin Proceso 3 */


//espero a que finalicen los hijos....
 	
wait(&status);
 
close(p1_p2[0]);
close(p1_p2[1]);
close(p2_p3[0]); 
close(p2_p3[1]);

exit (0);


}
