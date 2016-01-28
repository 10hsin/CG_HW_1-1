//*************************************************
// Library & Header 
//*************************************************
#include "mesh.h"
#include "glut.h"
#include "scene.h"
#include "view.h"
#include <fstream>
#include <iostream>
#include <string>
using namespace std;

//*************************************************
// Function Declaration & Global Variable
//*************************************************
#define TestScene 2

void	display();
void	keyboard(unsigned char, int, int);
void	light();
void	mouse(int, int, int, int);
void	reshape(GLsizei, GLsizei);
void	viewing();

int		obj_id = 0;
int		x_pos = -1, y_pos = -1;
int		windowSize[2];

scene	s;
view	v;

//*************************************************
// Function Implementation
//*************************************************

int main(int argc, char** argv)
{
	s.load_scene(s, TestScene);
	v.load_view(v, TestScene);

	glutInit(&argc, argv);
	glutInitWindowSize(v.viewport_width, v.viewport_height);
	glutInitWindowPosition(v.viewport_x, v.viewport_y);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
	glutCreateWindow("HW1");
	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutKeyboardFunc(keyboard);
	glutMouseFunc(mouse);

	glutMainLoop();

	return 0;
}

void light()
{
	ifstream ifs;
	string	token;

	if (TestScene == 1)
		ifs.open("Scene\\TestScene1\\light.light");
	if (TestScene == 2)
		ifs.open("Scene\\TestScene2\\scene2.light");

	glShadeModel(GL_SMOOTH);
	glEnable(GL_DEPTH_TEST);	// z buffer enable
	glEnable(GL_LIGHTING);	// enable lighting

	GLenum gl_light[8] = { GL_LIGHT0, GL_LIGHT1, GL_LIGHT2, GL_LIGHT3, GL_LIGHT4, GL_LIGHT5, GL_LIGHT6, GL_LIGHT7 };

	int i = 0;
	float temp_x, temp_y, temp_z;

	while (ifs >> token && token == "light" && i<8)
	{
		ifs >> temp_x >> temp_y >> temp_z;
		GLfloat light_position[] = { temp_x, temp_y, temp_z, 1.0f };

		ifs >> temp_x >> temp_y >> temp_z;
		GLfloat light_ambient[] = { temp_x, temp_y, temp_z, 1.0f };

		ifs >> temp_x >> temp_y >> temp_z;
		GLfloat light_diffuse[] = { temp_x, temp_y, temp_z, 1.0f };

		ifs >> temp_x >> temp_y >> temp_z;
		GLfloat light_specular[] = { temp_x, temp_y, temp_z, 1.0f };

		// set light property
		glEnable(gl_light[i]);
		glLightfv(gl_light[i], GL_POSITION, light_position);
		glLightfv(gl_light[i], GL_DIFFUSE, light_diffuse);
		glLightfv(gl_light[i], GL_SPECULAR, light_specular);
		glLightfv(gl_light[i], GL_AMBIENT, light_ambient);
		i++;
	}

	ifs >> temp_x >> temp_y >> temp_z;
	GLfloat ambient[] = { temp_x, temp_y, temp_z };
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, ambient);

	ifs.close();
}

//*************************************************
// vertex -> ModelView Matrix -> Projection Matrix 
// -> Perspective Division -> vView Transformation
//*************************************************

void display()
{
	//setup
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);				//清除用color
	glClearDepth(1.0f);									// Depth Buffer (就是z buffer) Setup
	glEnable(GL_DEPTH_TEST);							// Enables Depth Testing
	glDepthFunc(GL_LEQUAL);								// The Type Of Depth Test To Do
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);	//這行把畫面清成黑色並且清除z buffer

	// viewport transformation
	glViewport(v.viewport_x, v.viewport_y, windowSize[0], windowSize[1]);

	// projection transformation
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(v.fovy, (GLfloat)windowSize[0] / (GLfloat)windowSize[1], v.dnear, v.dfar);

	// viewing and modeling transformation
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(	v.eye.x, v.eye.y, v.eye.z,	// eye
				v.vat.x, v.vat.y, v.vat.z,  // center
				v.vup.x, v.vup.y, v.vup.z); // up						
	light();								// should after gluLookAt

	for (int k = 0; k < s.obj_num; k++)
	{
		glPushMatrix();					//put the current matrix into stack (modelview)
		glTranslatef(s.transfer_vector[k].x, s.transfer_vector[k].y, s.transfer_vector[k].z);
		glRotatef(s.angle[k], s.rotation_axis_vector[k].x, s.rotation_axis_vector[k].y, s.rotation_axis_vector[k].z);
		glScalef(s.scale_value[k].x, s.scale_value[k].y, s.scale_value[k].z);
		
		
		int lastMaterial = -1;
		for (size_t i = 0; i < s.object[k]->fTotal; ++i)
		{
			// set material property if this face used different material
			if (lastMaterial != s.object[k]->faceList[i].m)
			{
				lastMaterial = (int)s.object[k]->faceList[i].m;
				glMaterialfv(GL_FRONT, GL_AMBIENT, s.object[k]->mList[lastMaterial].Ka);
				glMaterialfv(GL_FRONT, GL_DIFFUSE, s.object[k]->mList[lastMaterial].Kd);
				glMaterialfv(GL_FRONT, GL_SPECULAR, s.object[k]->mList[lastMaterial].Ks);
				glMaterialfv(GL_FRONT, GL_SHININESS, &s.object[k]->mList[lastMaterial].Ns);
			}

			glBegin(GL_TRIANGLES);
			for (size_t j = 0; j<3; ++j)
			{
				glNormal3fv(s.object[k]->nList[s.object[k]->faceList[i][j].n].ptr);
				glVertex3fv(s.object[k]->vList[s.object[k]->faceList[i][j].v].ptr);
			}
			glEnd();
		}

		glPopMatrix();//拿stack中的矩陣到指定矩陣(modelview),會回到 I * ModelView
	}

	glutSwapBuffers();
}

void reshape(GLsizei w, GLsizei h)
{
	windowSize[0] = w;
	windowSize[1] = h;
}

//*************************************************
// rotate matrix along y
// cos 0 -sin 0
// 0   1   0  0
// sin 0   cos 0
// 0   0   0   1
//*************************************************

void keyboard(unsigned char key, int x, int y)
{
	float x_rotate, z_rotate;
	switch (key)
	{
	case 'w':
		v.fovy += 1;
		break;

	case 's':
		v.fovy -= 1;
		break;

	case 'd':
		x_rotate = v.eye.x - v.vat.x;
		z_rotate = v.eye.z - v.vat.z;
		v.eye.x = x_rotate*cos(-0.5) + z_rotate*sin(-0.5);
		v.eye.z = z_rotate*cos(-0.5) - x_rotate*sin(-0.5);
		break;

	case 'a':
		x_rotate = v.eye.x - v.vat.x;
		z_rotate = v.eye.z - v.vat.z;
		v.eye.x = x_rotate*cos(0.5) + z_rotate*sin(0.5);
		v.eye.z = z_rotate*cos(0.5) - x_rotate*sin(0.5);
		break;

	case'1':
		if (s.obj_num >= 1)
			obj_id = 1;
		else
			obj_id = 0;
		break;

	case'2':
		if (s.obj_num >= 2)
			obj_id = 2;
		else
			obj_id = 0;
		break;

	case'3':
		if (s.obj_num >= 3)
			obj_id = 3;
		else
			obj_id = 0;
		break;

	case'4':
		if (s.obj_num >= 4)
			obj_id = 4;
		else
			obj_id = 0;
		break;

	case'5':
		if (s.obj_num >= 5)
			obj_id = 5;
		else
			obj_id = 0;
		break;

	case'6':
		if (s.obj_num >= 6)
			obj_id = 6;
		else
			obj_id = 0;
		break;

	case'7':
		if (s.obj_num >= 7)
			obj_id = 7;
		else
			obj_id = 0;
		break;

	case'8':
		if (s.obj_num >= 8)
			obj_id = 8;
		else
			obj_id = 0;
		break;

	case'9':
		if (s.obj_num >= 9)
			obj_id = 9;
		else
			obj_id = 0;
		break;

	default:
		obj_id = 0;
		break;
	}

	glutPostRedisplay();
}

void mouse(int button, int state, int x, int y)
{
	if (button == GLUT_LEFT_BUTTON && obj_id != 0)
	{
		if (state == GLUT_DOWN)
		{
			x_pos = x;
			y_pos = y;
		}
		else if (state == GLUT_UP)
		{
			s.transfer_vector[obj_id - 1].x += (x - x_pos) / 50;
			s.transfer_vector[obj_id - 1].y -= (y - y_pos) / 50;
			glutPostRedisplay();
		}
	}
}

