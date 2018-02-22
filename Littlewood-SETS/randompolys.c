
/*
 * Majken
 * You will need in Windows to install UNIX framework called CYGWIN with the following packages
 *
 * GCC , GSL (libgsl) , freeGLUT
 *
 * BUILD
 * gcc -lgsl -lglut -lGLU -lGL -O3 -funroll-loops randompolys.c -o randompolys
 *
 * RUN
 *
 * ./randompolys <degree of random polynomials to get roots or 0 for multiple random degrees>
 *
 * EXAMPLE
 *
 * ./randompolys 0
 *
 * ./randompolys 30
 *
 *
 * HELP
 *
 * I can help you to modify or try to run in your machine if you think is important (I dont use windows but I can try I think is easy)
 *
 * Eduardo Ruiz-Duarte
 * e.ruiz.duarte@rug.nl
 */


#include <complex.h>
#include <stdio.h>
#include <unistd.h>
#include <time.h>
#include <complex.h>
#include <gsl/gsl_poly.h>

#include <GL/glut.h>
#include <math.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>




#define MAX_DEG 26
#define MAX_POLY 100000

int j=0;
int globaldeg = 0;

typedef struct poly {
	int		X          [MAX_DEG + 1];
	int		deg;
	int		index;
	complex double	roots[MAX_DEG + 1];
}		poly_t;

int		state = 0;




void
print_poly(poly_t F)
{
	int		i;
	for (i = 0; i < F.deg; i++)
		printf("%cx^%d ", F.X[i] == 1 ? '+' : '-', i);
	printf("\n");
}




int
get_roots(poly_t * f)
{
	int		i;
	double         *a = calloc(sizeof(double), f->deg * f->deg + 1);
	double         *z = calloc(sizeof(double), f->deg * f->deg + 1);

	for (i = 0; i <= f->deg; i++) {
		a[i] = (int)f->X[i];
	}






	gsl_poly_complex_workspace *w
	= gsl_poly_complex_workspace_alloc(f->deg);

	gsl_poly_complex_solve(a, f->deg, w, z);

	gsl_poly_complex_workspace_free(w);
	for (i = 0; i < f->deg; i++) {
		f->roots[i] = z[2 * i] + I * z[2 * i + 1];
	}

	return 0;
}







int
randcoef()
{
	int		x         , r;
	srand(getpid() + time(NULL) + state);
	r = rand();
	state = r;
	x = r < 0 ? -1 * r : r;
	x %= 2;
	if (x == 1)
		return 1;
	else
		return -1;

}


poly_t
get_rand_poly(unsigned int deg)
{

	int		i;
	poly_t		f;

	for (i = 0; i <= deg; i++) {


		f.X[i] = randcoef();

	}

	f.deg = deg;

	f.X[0] = f.X[0] < 0 ? 1 : 1;

	return f;


}

void
print_roots(poly_t f)
{
	int		i;
	for (i = 0; i <= f.deg; i++) {
		printf("<%d>[%d] %f %f\n", f.index, i + 1, creal(f.roots[i]), cimag(f.roots[i]));
	}
}




void
crea_plano()
{
	gluOrtho2D(-1.5,1.5,-1.5,1.5);
}

void drawlines() {
double x;

glColor3f(0,1,0);


for(x=-10;x<10;x+=0.001) { 
	glVertex2f(-x,x);
	glVertex2f(x,x);
	glVertex2f(-1.4,x); 
	glVertex2f(1.4,x) ;
	glVertex2f(x,-1.4) ;
	glVertex2f(x,1.4) ;

		}
glColor3f(0,1,0);
}

#define INTENSITY 1000000

long dpoints[INTENSITY][INTENSITY];
double intensity(double x, double y) {

int r;
unsigned int cx,cy;

cx = (unsigned int) ceil(fabs(x+2)*150);
cy = (unsigned int) ceil(fabs(y+2)*150);

cx %= INTENSITY;
cy %= INTENSITY;

dpoints[cx][cy] += 1;

r = dpoints[cx][cy] ;

//printf("->%d-%d=>%f\n",cx,cy,1.0-(1.0/(double)r));

if(r == 0) 
	return 0;

return pow(1.0-(1.0/(double)r),5);



}

void
display(void)
{
	static poly_t	F [MAX_POLY];
	complex double	z;
	double		real   , imag;
	int		J,i,k         , r, deg;
	double x,y,sfactor;

//glClear(GL_COLOR_BUFFER_BIT);
	glBegin(GL_POINTS);

	for (J = j; J < MAX_POLY; J++) {
		srand(j + getpid());
		if(globaldeg != 0)
		deg = globaldeg;
		else
		deg = (((unsigned int)rand()) % MAX_DEG) + 2;

		F[J] = get_rand_poly(deg);
		F[J].index = j;
		get_roots(&F[J]);
	//	print_roots(F[j]);
	//	plot_roots(F[j]);
		for(k=0;k<=F[J].deg;k++)  {
		x = creal(F[J].roots[k]);
		y = cimag(F[J].roots[k]);

		sfactor = intensity(x,y);
		glColor3f(sfactor,0,0);
		glVertex2f(x,y) ;



		//printf("%f %f\n",creal(F[j].roots[k]),cimag(F[j].roots[k]));
		}
	//printf("J=%d\n",J);

		

	}


        j+=25;
	if(j == MAX_POLY)  {
		j=0;
		glClear(GL_COLOR_BUFFER_BIT);
	}
	glEnd();
	glutPostRedisplay();
	glutSwapBuffers();
}

void reshape() { }

int
main(int argc, char **argv)
{

	//memset(dpoints,0,sizeof(dpoints));
	//
	if(argv[1] != NULL) 
	globaldeg = atoi(argv[1]);
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE| GLUT_RGBA );
	glutInitWindowPosition(0, 0);
	glutInitWindowSize(650, 650);
	glutCreateWindow("Roots");
	crea_plano();
	glutDisplayFunc(display);
	glutReshapeFunc(reshape);

	glClear(GL_COLOR_BUFFER_BIT);
	glutMainLoop();
	return 0;
}
