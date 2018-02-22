/*

Builds a spirograph based in the internal radius and iterations given in argv.
This software supports random colors or gradient colors, random colors wont work on Windows Machines
It can combine several spirographs

The opengl window will show you both discs (external and internal on the right if just one spirograph selected)

it will also show you the points of the internal radius being drawn

The software requirements are OpenGL and Glut

in linux I compile

gcc -O2 -lGL -lglut -lGLU beckgraph.c -o beckgraph

Usage:


if you want to combine 2 spirographs, use the following command:

./beckgraph 30,20 20,10  

30,20 means that the first spirograph will have 30 as its internal radius  and
beckgraph will draw 20 curves using 20 points of the internal radius

20,10 is the same, with different values, the result will be 2 overlapped
spirographs

If you want random colors (I did not like my random colors but you can try)

./beckgraph 30,20 20,10 -r

If you want to overlap more spirographs, use the same logic


#####################################################################################################################################
IMPORTANT NOTE: This source code has errors and it will crash with certain values (I have not programmed a good error handling yet)
this is open source and here is the license.
#####################################################################################################################################

License:

Copyright (c) 2011, Eduardo Ruiz Duarte
All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions
are met:
1. Redistributions of source code must retain the above copyright
   notice, this list of conditions and the following disclaimer.
2. Redistributions in binary form must reproduce the above copyright
   notice, this list of conditions and the following disclaimer in the
   documentation and/or other materials provided with the distribution.
3. All advertising materials mentioning features or use of this software
   must display the following acknowledgement:
   This product includes software developed by the University of 
   California, Berkeley and its contributors.
4. Neither the name of the University nor the names of its contributors
   may be used to endorse or promote products derived from this software
   without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY EDUARDO RUIZ DUARTE ''AS IS'' AND ANY
EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL EDUARDO RUIZ DUARTE BE LIABLE FOR ANY
DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
(INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.


*/


#include <GL/glut.h>
#include <string.h>
#include <GL/gl.h>
#include <math.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>


typedef struct pixel
{
  double r, g, b;
} pixel_t;

pixel_t colors[8192 * 2];
int randact = 0;
double a = 60, b = 40, h = 10, t, n,ng[8],bg[8];
unsigned int i,numsp=0;

void
winInit ()
{
  gluOrtho2D (-60.0, 60.0, -60.0, 60.0);
}


void
display (void)
{
  double x, y;
  double hoyitos[1024];
  pixel_t hoyoscol[1024];
  int s;
  i = 0;
  glClear (GL_COLOR_BUFFER_BIT);

  glEnable (GL_POINT_SPRITE);
  glPointSize (2);
  glBegin (GL_POINTS);
  if(numsp==1)
  for (t = 0; t < 2 * M_PI; t += 0.001)
    {
      x = a * sin (t);
      y = a * cos (t);
      glVertex3f (x, y, 0);
    }
  glEnd ();


  glPointSize (2);
  glBegin (GL_POINTS);
  glColor3f (0, 1, 1);
  b = bg[0];
  if(numsp==1)
  for (t = 0; t < 2 * M_PI; t += 0.001)
    {
      x = 60 - b + b * sin (t);
      y = b * cos (t);
      glVertex3f (x, y, 0);
    }
  glEnd ();

  glDisable (GL_POINT_SPRITE);


  glPointSize (0.1);
  glBegin (GL_POINTS);
  glColor3f (1, 0, 1);
  for(s=0;s<numsp;s++) { 
  b = bg[s];
  n = ng[s];
  for (h = 1; h < b; h += b / n)
    {
      printf ("i=%d\n", i);
      hoyitos[i] = fabs (60 - b + h);
      if (randact)
	{
	  glColor3f (colors[i].r, colors[i].g, colors[i].b);
	  hoyoscol[i] = colors[i];
	}
      else
	{
	  glColor3f (h / n, 3 * h / n, h);
	  colors[i].r = h / n;
	  colors[i].g = 3 * h / n;
	  colors[i].b = h;
	  hoyoscol[i] = colors[i];
	}
      i++;
      for (t = 0; t < 60; t += 0.001)
	{
	  x = (a - b) * cos (t) + h * cos (((a + b) / b) * t);
	  y = (a - b) * sin (t) - h * sin (((a + b) / b) * t);
	  glVertex3f (x, y, 0);
	}
    }
  }
  glEnd ();

  glEnable (GL_POINT_SPRITE);
  glPointSize (5);
  glBegin (GL_POINTS);
  glColor3f (1, 1, 0);
  if(numsp==1)
  for (i = 0; i < (int) n; i++)
    {
      glColor3f (hoyoscol[i].r, hoyoscol[i].g, hoyoscol[i].b);
      glVertex3f (hoyitos[i], 0, 0);
    }
  glEnd ();
  glDisable (GL_POINT_SPRITE);
  glEnd ();
  glutSwapBuffers ();


}


int
main (int argc, char **argv)
{
  int fd = open ("/dev/urandom", O_RDONLY),j ;
  char sp[8][16], *del = ",";
  void *handler;
  read (fd, &colors, sizeof (colors));
  close (fd);
  if(strcmp(argv[argc-1],"-r") == 0)
      randact=1;
  memset(&sp,0,sizeof(sp));
  for (j = 1; j < argc-randact; j++)
    {
      memcpy(sp[j],argv[j],strlen(argv[j]));
      if (strtok (sp[j], del) != NULL)
        {
          bg[j-1] = strtod (sp[j], NULL);
          handler = strtok (NULL, del);
          if (handler != NULL)
            ng[j-1] = strtod (handler, NULL);
          else
            exit(printf("Arg error\n"));
          numsp++;
        }
    }
  glutInit (&argc, argv);
  glutInitDisplayMode (GLUT_SINGLE | GLUT_RGBA);
  glutInitWindowPosition (5, 5);
  glutInitWindowSize (800, 800);
  glutCreateWindow ("Standard Map");

  winInit ();
  glutDisplayFunc (display);
  glutMainLoop ();

  return 0;
}
