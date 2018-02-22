/*
 * Teoricamente los numeros primos en la recta real (roja) se encuentran en donde intersectan solo dos curvas periodicas
 * estas curvas solo son la parte de arriba del circulo y la parte de abajo del circulo recorrida donde termina la primera parte
 *
 * sera?
 *
 * Necesitas:
 * freeglut, opengl 
 *
 * Compila:
 * gcc -lGL -lGLU -lglut -lm -funroll-all-loops -fomit-frame-pointer -O3 primos-funcs.c -o primos-funcs
 * 
 * Corre:
 * ./primos-funcs
 * rduarte@ciencias.unam.mx
 *
 */

#include <GL/glut.h>
#include <string.h>
#include <GL/gl.h>
#include <math.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define NUM_FUNCIONES 65
#define FRECUENCIA 61
#define DELTA 0.001

void
winInit ()
{
  gluOrtho2D (-60.0, 60.0, -60.0, 60.0);
  glEnable (GL_LINE_SMOOTH);   
  glEnable (GL_POINT_SMOOTH);
  glHint(GL_POINT_SMOOTH_HINT,GL_NICEST);
  glHint(GL_POLYGON_SMOOTH_HINT,GL_NICEST);
  glHint(GL_LINE_SMOOTH_HINT,GL_NICEST);
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
}

void
display (void)
{
  double x, y, k, t, n,r;
  glClear (GL_COLOR_BUFFER_BIT|GL_ACCUM_BUFFER_BIT|GL_DEPTH_BUFFER_BIT|GL_STENCIL_BUFFER_BIT);
  srand(getpid()+getppid());
  glBegin (GL_POINTS);
  for (n = 1; n < NUM_FUNCIONES; n++) {
          r = rand();
  glutSwapBuffers ();
    for (k = 1; k < (FRECUENCIA/n)+1; k += 1)
      for (t = -1*M_PI; t < 0; t += DELTA)
	{
          /* dibujamos el pedazo de arriba del circulo y el pedazo de abajo recorrido a la derecha */
	  x = (cos (t) * pow (-1, k)) + 2 * k;
	  y = (sin (t) * pow (-1, k));
          /* escalamos el radio para que vayan chocando en la recta real en los enteros n */
	  x = (x * n / 2) - n / 2;
	  y = (y * n / 2);
          // Aqui solo es para acomodar en GL el origen y la escala
          x -=30;
          x *= 2;
          y *= 2;
	  y -=15;
          // asignamos colores aleatorios a cada curva periodica excepto a la de diametro 1 que sera roja para identificar enteros
         if(n==1)
         glColor3f(1,0,0);
	 else
         glColor3f(sin(n),0.2+fabs(cos(r+n)),0.2+fabs(sin(cos(r))));
          glVertex2f(x,y);
          // dibujamos el eje x en verde
          glColor3f(0,1,0);
          glVertex2f(x,-15);
	}
  }

  glEnd ();

  glutSwapBuffers ();

}


int init()
{
	return 1;
}

int
main (int argc, char **argv)
{
  glutInit (&argc, argv);
  glutInitDisplayMode (GLUT_DOUBLE | GLUT_RGBA);
  glutInitWindowPosition (0, 0);
  glutInitWindowSize (1024, 1024);
  glutCreateWindow ("Curvas periodicas como arcos de circunferencia");
  glClear (GL_COLOR_BUFFER_BIT|GL_ACCUM_BUFFER_BIT|GL_DEPTH_BUFFER_BIT|GL_STENCIL_BUFFER_BIT);
  winInit ();
  glutDisplayFunc (display);
  glutMainLoop ();
  return 0;
}
