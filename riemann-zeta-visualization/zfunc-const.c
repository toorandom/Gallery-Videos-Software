#include <stdio.h>
#include <math.h>
#include <math.h>
#include <complex.h>
#include <GL/glut.h>

double p=2;

#define L M_PI 


double rmin=-L;
double rmax=L; 
double imin=-L;
double imax=L;
double rdelta=0.0025;
double idelta=0.0025;

#define ROJO 'r'
#define VERDE 'v'
#define AZUL 'a'
#define MORADO 'm'


int color(double complex z) {

if( (creal(z) > 0) && (cimag(z) > 0) )  {
glColor3f(1,0,0);
return ROJO;
}
if( (creal(z) < 0) && (cimag(z) > 0) ) {
glColor3f(0,1,0);
return VERDE;
}
if( (creal(z) < 0) && (cimag(z) < 0) ) {
glColor3f(0,0,1);
return AZUL;
}
if( (creal(z) > 0) && (cimag(z) < 0) ) {
glColor3f(1,0,1);
return MORADO;
}

return -1;

}


double complex _f(double complex z) {
return 1-cexp(-1*z*log(p)) ;
}

double complex f(double complex z) {
return z ;
}



void display(void) {


double r,i;
double complex w,z;

        glClear(GL_COLOR_BUFFER_BIT);
        glBegin(GL_POINTS);
	for(r=rmin;r<rmax;r+=rdelta)
		for(i=imin;i<imax;i+=0.05) {
		z = r+i*I;
		w=f(z);
		color(z);
		glVertex3f(creal(w),cimag(w),0);
	//	printf("f(%f,%f)=(%f,%f) (%c)\n",creal(z),cimag(z),creal(w),cimag(w),color(w));
	}
	for(i=imin;i<imax;i+=idelta) 
	for(r=rmin;r<rmax;r+=0.05) {
		z = r+i*I;
		w=f(z);
		color(z);
		glVertex3f(creal(w),cimag(w),0);
	//	printf("f(%f,%f)=(%f,%f) (%c)\n",creal(z),cimag(z),creal(w),cimag(w),color(w));
	}
        glEnd();
glutSwapBuffers ();

return ;

}
/*
void
display(void)
{
        double x,y,i,j;
        glColor3f(0.0, 0.0, 0.0);
        glClear(GL_COLOR_BUFFER_BIT);
        glColor3f(0.5,0.5,0.5);
        glVertex3f(1,1,0);
        glColor3f(1.0, 1.0, 1.0);
                glBegin(GL_POINTS);
                for(x=0;x<=512;x+=0.1)
                {
                        funcion(&x, &y);
                        glVertex3f(20*x, 20*y,0);
                }
                for(i=0;i<=512;i+=0.1)
                        for(j=0;j<512;j+=0.1) {
                                x=i;
                                y=j;
                                funcion(&x,&y);
                                if(y < j) {
                                        glColor3f(i,j,i+j);
                                                glVertex3f(20*i,20*j,0);

                                }
                        }

                glEnd();

}

*/

int
main(int argc, char **argv)
{
        glutInit(&argc, argv);
        glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
        glutInitWindowPosition(5, 5);
        glutInitWindowSize(1000, 1000);
        glutCreateWindow("Funcion Z de Riemann");
        gluOrtho2D(rmin*3, rmax*3, imin*3, imax*3);
        glutDisplayFunc(display);
        glutMainLoop();
        return 0;
}





