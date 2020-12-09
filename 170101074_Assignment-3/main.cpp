#include<GL/glut.h>
#include<bits/stdc++.h>
#include <SDL2/SDL_image.h>

using namespace std;

bool mouseDown = false;

float rotation_x = 0.0f;
float rotation_y = 0.0f;
float translation_z = 0.0f;

float difference_x = 0.0f;
float difference_y = 0.0f;

SDL_Surface *surface;
GLuint texture; 

GLfloat mat_specular[] = { 0.0, 0.0, 0.0, 1 };
GLfloat no_shininess_plastic[] = { 0 };

GLfloat low_shininess[] = { 128 };

GLfloat diffuse[] = { 1, 1, 1, 1 };
GLfloat specular[] = { 1.0, 1.0, 1.0, 1.0 };

int LoadGLTextures( ) 
{
    int Status = 0;
    SDL_Surface *TextureImage[2]; 
    if ((TextureImage[0] = IMG_Load("texture-glass.jpg"))) 
    {
	    Status = 1;
	    glGenTextures( 1, &texture );
	    glBindTexture( GL_TEXTURE_2D, texture );
	    glTexImage2D( GL_TEXTURE_2D, 0, 3, TextureImage[0]->w,TextureImage[0]->h, 0, GL_RGB,
			  GL_UNSIGNED_BYTE, TextureImage[0]->pixels );
	    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
	    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
    }

    if ( TextureImage[0] )
	    SDL_FreeSurface( TextureImage[0] );
    return Status;
}

void init()
{
	if(!LoadGLTextures())
		cout<<"Error Loading Texture"<<endl;

	GLfloat light_position[] = { 0.0, 0.0, 0.0, 1 };
	glClearColor (0.0, 0.0, 0.0, 0.0);
	glShadeModel (GL_SMOOTH);

	glLightfv(GL_LIGHT0, GL_POSITION, light_position);

	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_COLOR_MATERIAL);

	glBindTexture( GL_TEXTURE_2D, texture );
}

void drawWalls()
{
	// glPushMatrix();
	glBegin(GL_QUADS);
		glNormal3f(0,1,0);
		glColor3f(0.7,0.7,0.7);
	    glVertex3f(-7,-5,0);
		glVertex3f(7,-5,0);
		glVertex3f(7,-5,-3);
		glVertex3f(-7,-5,-3);

		glNormal3f(1,0,0);
		glColor3f(0.99,0,0);
		glVertex3f(-7,-5,0);
		glVertex3f(-7,-5,-3);
		glVertex3f(-7,5,-3);
		glVertex3f(-7,5,0);

		glNormal3f(-1,0,0);
		glColor3f(0,0,0.999);
		glVertex3f(7,-5,-3);
		glVertex3f(7,-5,0);
		glVertex3f(7,5,0);
		glVertex3f(7,5,-3);

		glNormal3f(0,0,1);
		glColor3f(0.9,0.9,0.9);
		glVertex3f(-7,-5,-3);
		glVertex3f(7,-5,-3);
		glVertex3f(7,5,-3);
		glVertex3f(-7,5,-3);

		glNormal3f(0,-1,0);
		glColor3f(0.9,0.9,0.9);
		glVertex3f(-7,5,-3);
		glVertex3f(7,5,-3);
		glVertex3f(7,5,0);
		glVertex3f(-7,5,0);
	glEnd();
	glBegin(GL_POINTS);
	glVertex3f(0.0, 0.0, -1.0);
	glEnd();
}

void drawTexturedSphere()
{
	glPushMatrix();
		glTranslatef(-4,-4,-1);
		glScalef(1,1,0.1);
		GLUquadricObj *quadricObj = gluNewQuadric();
		gluQuadricDrawStyle(quadricObj, GLU_FILL);
		gluQuadricNormals(quadricObj, GLU_SMOOTH);
		gluQuadricTexture(quadricObj, GL_TRUE);
		gluSphere(quadricObj, 1.5, 100, 100);
	glPopMatrix();
}

void drawPlasticSphere()
{
	glPushMatrix();
		glColor3f(0.0,0.5,0.0);
		glPushMatrix();
			glLightfv(GL_LIGHT0, GL_SPECULAR, mat_specular);
			glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
			glMaterialfv(GL_FRONT, GL_SHININESS, no_shininess_plastic);
		glPopMatrix();
		glTranslatef(0,-4,-1.5);
		glScalef(1,1,0.1);
		glutSolidSphere(2.2,100,100);
	glPopMatrix();
}

void drawGlossySphere()
{
	glPushMatrix();
		glColor3f(0.4,0.3,0.0);
		glPushMatrix();
			glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuse);
			glLightfv(GL_LIGHT0, GL_SPECULAR, specular);
			glMaterialfv(GL_FRONT, GL_SHININESS, low_shininess);
		glPopMatrix();		
		glTranslatef(4,-4,-1);
		glScalef(1,1,0.1);
		glutSolidSphere(1.5,100,100);
	glPopMatrix();
}

void display()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();
	gluLookAt (0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);

	glTranslatef(0,0,translation_z);
	glRotatef(rotation_x, 1.0f, 0.0f, 0.0f);
	glRotatef(rotation_y, 0.0f, 1.0f, 0.0f);

	glEnable( GL_TEXTURE_2D );
	drawTexturedSphere();
	glDisable(GL_TEXTURE_2D);
	drawPlasticSphere();
	drawGlossySphere();
	drawWalls();
	glutSwapBuffers();  
}

void reshape(int w, int h)
{
	glViewport(0,0,w,h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(-10,10,-10,10);
	gluPerspective( 60, (float)w/(float)h, 0, 20);
	glMatrixMode( GL_MODELVIEW );
	glLoadIdentity();
}

void mouseCallback(int button, int state, int x, int y)
{
	if(state == GLUT_DOWN)
	{
		if(button == GLUT_LEFT_BUTTON)
		{
			mouseDown = true;

			difference_x = x - rotation_y;
			difference_y = -y + rotation_x;
		}
		if(button == 3)
		{
			translation_z += 0.2;
			glutPostRedisplay();
		}
		if(button == 4)
		{
			translation_z -= 0.2;
			glutPostRedisplay();
		}
	}
	else
		mouseDown = false;
}

void mouseDragCallback(int x, int y)
{
	if (mouseDown)
	{
		rotation_y = x - difference_x;
		rotation_x = y + difference_y;

		glutPostRedisplay();
	}
}

int main(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowPosition(20,20);
	glutInitWindowSize(900,900);
	glutCreateWindow("3D Spheres");
	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutMouseFunc(mouseCallback);
	glutMotionFunc(mouseDragCallback);
	init();
	glutMainLoop();
	return 0;
}