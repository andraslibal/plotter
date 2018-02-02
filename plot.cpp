#include <stdio.h>
#include <stdlib.h>

#include "GL/freeglut.h"
#include "GL/gl.h"

struct particles_struct
    {
        int N;
        int *ID;
        double *x;
        double *y;
        int *color;
        double *extra;
    } particles;


void read_in_positions()
{
FILE *inputfile;
int intholder;
float floatholder;
int i;

inputfile = fopen("results.mvi","rb");
//Number of partciles
fread(&intholder,sizeof(int),1,inputfile);
printf("N = %d\n",intholder);
particles.N = intholder;
particles.ID = (int *)malloc(particles.N*sizeof(int));
particles.x = (double *)malloc(particles.N*sizeof(double));
particles.y = (double *)malloc(particles.N*sizeof(double));
particles.color = (int *)malloc(particles.N*sizeof(int));
particles.extra = (double *)malloc(particles.N*sizeof(double));

//Frame number in file
fread(&intholder,sizeof(int),1,inputfile);
printf("frame = %d\n",intholder);

for(i=0;i<particles.N;i++)
    {
    fread(&intholder,sizeof(int),1,inputfile);   
    particles.color[i] = intholder;
    fread(&intholder,sizeof(int),1,inputfile);   
    particles.ID[i] = intholder;
    fread(&floatholder,sizeof(float),1,inputfile);   
    particles.x[i] = (double)floatholder;
    fread(&floatholder,sizeof(float),1,inputfile);   
    particles.y[i] = (double)floatholder;
    fread(&floatholder,sizeof(float),1,inputfile);   
    particles.extra[i] = (double)floatholder;
    }

fclose(inputfile);    

}

void draw()
{
    int i;
    double x0,y0,xf,yf;

    glClearColor(1.0, 1.0, 1.0, 1.0);
    glClear(GL_COLOR_BUFFER_BIT);

    glOrtho(-1.0, 1.0, -1.0, 1.0, -1.0, 1.0);

    for(i=0;i<particles.N;i++)
        {
        glPushMatrix();
        glTranslatef(particles.x[i]/10.0-1.0,particles.y[i]/10.0-1.0,0);
        if (particles.color[i]==2) glColor3f(1.0, 0.0, 0.0);
        if (particles.color[i]==3) glColor3f(0.0, 0.0, 1.0);        
        glutSolidSphere(0.02,30,30);
        glPopMatrix();
        }
     
    glutSwapBuffers();
}

int main(int argc, char **argv)
{
    printf("Plotter program\n");
    read_in_positions();


    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(1200, 1200);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("Plot for BD Simulations");
    glutDisplayFunc(draw);
    glutMainLoop();
    return 0;
}



