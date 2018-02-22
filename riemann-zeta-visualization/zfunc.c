/*
Programa para dibujar pedazos de la funcion z

Eduardo Ruiz Duarte (beck)

rduarte@ciencias.unam.mx

*/

#include <stdio.h>
#include <math.h>
#include <math.h>
#include <complex.h>
#include <GL/glut.h>
#include <GL/gl.h>

/* número primo a considerar */

double p = 2;

/* Todo el cáculo lo haremos en un cuadrado de area L^2 centrado en el origen */

#define L 5


/* aquí definimos los límites del plano cartesiano para que quede como un cuadrado exacto */

double rmin = -L;
double rmax = L;
double imin = -L;
double imax = L;

/* Qué tanto incrementaremos la parte real e imag a la hora de graficar */
double rdelta = 0.00125;
double idelta = 0.00125;

/* Qué tan separados estarán cada uno de los círculos del dominio en el plano (pensando en la funcion identidad) */
double separacion = 0.01;

/* Colores.. realmente no es necesario darles un valor pero es con propositos de que el código se entienda */
#define ROJO 'r'
#define VERDE 'v'
#define AZUL 'a'
#define ROSA 'm'
#define BLANCO 'b'


/* norma del punto que se esta dibujando, lo usaremos para que 1/n defina el brillo del punto */

double n;

/* Función básica de color, ésta regresa blanco si es un eje imag o real
 * o asigna un color dependiendo del cuadrante en el que este el punto z
 * si el punto del dominio esta mas lejos del origen, este sera mas obscuro en la imagen
 */
int
color (double complex z)
{
  if ((creal (z) > 0) && (cimag (z) > 0))
    {
      glColor3f (1/n, 0, 0);
      return ROJO;
    }
  if ((creal (z) < 0) && (cimag (z) > 0))
    {
      glColor3f (0, 1/n, 0);
      return VERDE;
    }
  if ((creal (z) < 0) && (cimag (z) < 0))
    {
      glColor3f (0, 0, 1/n);
      return AZUL;
    }
  if ((creal (z) > 0) && (cimag (z) < 0))
    {
      glColor3f (1/n, 0, 1/n);
      return ROSA;
    }

  if ((creal (z) == 0) || (cimag (z) == 0))
    {
      glColor3f (1/n, 1/n, 1/n);
      return BLANCO;
    }

glColor3f(1/n,1/n,0);
  return ;

}


/* Funcion 1-e^[-zlog(p)] donde p es la variable global que se puede modificar hasta arriba */
double complex
riemann(double complex z)
{
double P[] = {2,3,5,7,11,13,17,19,23,29,31,37,41,43,47,53,59,61,67,71,

 73, 79, 83, 89, 97,101,103,107,109,113 
,127,131,137,139,149,151,157,163,167,173 
,179,181,191,193,197,199,211,223,227,229 
,233,239,241,251,257,263,269,271,277,281 
,283,293,307,311,313,317,331,337,347,349 
,353,359,367,373,379,383,389,397,401,409 
,419,421,431,433,439,443,449,457,461,463 
,467,479,487,491,499,503,509,521,523,541 
,547,557,563,569,571,577,587,593,599,601 
, 607, 613, 617, 619, 631, 641, 643, 647, 653, 659 
, 661, 673, 677, 683, 691, 701, 709, 719, 727, 733 
, 739, 743, 751, 757, 761, 769, 773, 787, 797, 809 
, 811, 821, 823, 827, 829, 839, 853, 857, 859, 863 
, 877, 881, 883, 887, 907, 911, 919, 929, 937, 941 
, 947, 953, 967, 971, 977, 983, 991, 997,1009,1013 
,1019,1021,1031,1033,1039,1049,1051,1061,1063,1069 
,1087,1091,1093,1097,1103,1109,1117,1123,1129,1151 
,1153,1163,1171,1181,1187,1193,1201,1213,1217,1223 
,1229,1231,1237,1249,1259,1277,1279,1283,1289,1291 
,1297,1301,1303,1307,1319,1321,1327,1361,1367,1373 
,1381,1399,1409,1423,1427,1429,1433,1439,1447,1451 
,1453,1459,1471,1481,1483,1487,1489,1493,1499,1511 
,1523,1531,1543,1549,1553,1559,1567,1571,1579,1583 
,1597,1601,1607,1609,1613,1619,1621,1627,1637,1657 
,1663,1667,1669,1693,1697,1699,1709,1721,1723,1733 
,1741,1747,1753,1759,1777,1783,1787,1789,1801,1811 
,1823,1831,1847,1861,1867,1871,1873,1877,1879,1889 
,1901,1907,1913,1931,1933,1949,1951,1973,1979,1987 
,1993,1997,1999,2003,2011,2017,2027,2029,2039,2053 
,2063,2069,2081,2083,2087,2089,2099,2111,2113,2129 
,2131,2137,2141,2143,2153,2161,2179,2203,2207,2213 
,2221,2237,2239,2243,2251,2267,2269,2273,2281,2287 
};

double complex w=1;
int k,s=sizeof(P)/sizeof(double);
s=2;
for(k=0;k<s;k++)
	w *= (1 - cexp (-1 * z * log (P[k])));
  return w;
}


/* Cayley, hay que quitar el _ para que se llame f y a la otra f ponerle el _ */
double complex
f (double complex z)
{
  return (z-I)/(z+I);
}


/* IDENTIDAD, para ver el dominio, hay que quitar los __ para que se llame f y a la otra f ponerle los __ */
double complex
_f (double complex z)
{
  return z;
}


/* Funcion que dado un punto z del dominio , pinta con el color de la posicion de z al nuevo punto w=f(z) */
void
pinta (complex z)
{
  complex w;
  color (z);
  w = f (z);
  n = cabs(w);
  color(w);
  glVertex3f (creal (z), cimag (z), 0);
  return;
}

/*
Funcion que se le manda al motor de graficos la cual contiene ya 
el todos los puntos z a graficar
*/

void
display (void)
{


  double r, i, t, s[16];
  double complex w, z;
  int k = 0;

// Pendientes de rayas, omitimos la raya con pendiente cuyo angulo es pi/2
  for(k=0;k<16;k++) {
	if(k!=3)
  s[k] = tan((k+1)*M_PI/8);
	else
  s[k] = 1;
  }


/* Limpiamos pantalla */
  glClear (GL_COLOR_BUFFER_BIT|GL_ACCUM_BUFFER_BIT|GL_DEPTH_BUFFER_BIT|GL_STENCIL_BUFFER_BIT);



/* Comenzamos a dibujar */

  glBegin (GL_POINTS);
  glutSwapBuffers ();
/*
// Circulos z=re^it
  for (r = rmin; r < rmax+20; r += separacion)
    for (t = 0; t < 3 * M_PI; t += rdelta)
      {
	z = r * cexp (I * t);
	pinta (z);
      }
// Rayas (r,sr) donde s es la pendiente
  for (k = 0; k < sizeof (s) / sizeof (double); k++)
    for (r = rmin; r < rmax; r += rdelta)
      {
	z = r + s[k] * r * I;
	pinta (z);
      }
*/
  for (r = rmin; r < rmax; r += separacion)
  for (i = imin; i < imax; i += separacion) {
	z = r+i*I;
	pinta(z);
}
// Eje imag

  for (i = imin; i < imax; i += idelta)
    {
      z =I*i ;
      pinta (z);
    }

// Eje Real

  for (r = rmin; r < rmax; r += rdelta)
    {
      z = r;
      pinta (z);
    }

  glEnd ();
  glutSwapBuffers ();

  return;

}


/* funcion principal que crea las ventanas e inicializa todo el engine */

int
main (int argc, char **argv)
{
  glutInit (&argc, argv);
  glutInitDisplayMode (GLUT_DOUBLE | GLUT_RGBA | GLUT_RGB | GLUT_DEPTH | GLUT_LUMINANCE | GLUT_MULTISAMPLE);
  glEnable (GL_POINT_SMOOTH|GL_BLEND|GL_NICEST|GL_LINE_SMOOTH);

  glHint(GL_POINT_SMOOTH_HINT,GL_NICEST);
  glHint(GL_POLYGON_SMOOTH_HINT,GL_NICEST);
  glHint(GL_LINE_SMOOTH_HINT,GL_NICEST);

  glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
  glutInitWindowPosition (5, 5);
  glutInitWindowSize (700, 700);
  glutCreateWindow ("Funcion Z de Riemann");
  gluOrtho2D (rmin, rmax, imin, imax);
  glutDisplayFunc (display);
  glutMainLoop ();
  return 0;
}
