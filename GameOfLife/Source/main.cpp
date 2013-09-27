#include <iostream>

#include <random>
#include <windows.h>
#include <GL\freeglut.h>

int **pixelmap;
int **temppixelmap;

std::random_device seeder;
std::mt19937 engine(seeder());
std::uniform_int_distribution<int> dist(0, 1);

int adjacentLife[3][3] = {{1, 1, 1},
						  {1, 0, 1},
						  {1, 1, 1}};

int t = 0;
int gridSize;


void init() {
	srand(1);


	do {
	std::cout << "Please enter a grid size greater than 0:" << std::endl;
	std::cin >> gridSize;
	} while (gridSize <= 0);


	pixelmap = new int*[gridSize + 2];
	for (int i = 0; i < (gridSize + 2); i++)
	{
		pixelmap[i] = new int[gridSize + 2]();
	}

	temppixelmap = new int*[gridSize + 2];
	for (int i = 0; i < (gridSize + 2); i++)
	{
		temppixelmap[i] = new int[gridSize + 2]();
	}

	for (int i = 1; i <= (gridSize + 1); i++)
	{
		for (int j = 1; j <= (gridSize + 1); j++)
		{
			pixelmap[i][j] = dist(engine);
		}
	}
}

void display()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	for (int i = 1; i <= (gridSize + 1); i++)
	{
		for (int j = 1; j <= (gridSize + 1); j++)
		{
			if (pixelmap[i][j])
			{
				glPushMatrix();
					glTranslatef(((5.0*i)-5.0), ((5.0*j)-5.0), 0.0);
					glBegin(GL_QUADS);
					glColor3f(rand(),rand(),rand());
					glVertex3f(5.0, 0.0, 0.0);
					glColor3f(rand(),rand(),rand());
					glVertex3f(5.0, 5.0, 0.0);
					glColor3f(rand(),rand(),rand());
					glVertex3f(0.0, 5.0, 0.0);
					glColor3f(rand(),rand(),rand());
					glVertex3f(0.0, 0.0, 0.0);
					glEnd();
				glPopMatrix();
			}
		}
	}

	glutSwapBuffers();
}

void orthogonal(int h, int w) {
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();

	glLoadIdentity();
	gluOrtho2D(0, w, 0, h);

	glMatrixMode(GL_MODELVIEW);
}

void reshape(int w, int h)
{
	orthogonal(w, h);
}

int countLife(int i, int j)
{
	int sum = 0;
	for (int y = 0; y < 3; y++)
	{
		for (int x = 0; x < 3; x++)
		{
			sum = sum + (pixelmap[i+(x-1)][j+(y-1)] * adjacentLife[y][x]);
		}
	}
	return(sum);
}




void calcNewLife()
{
	

	for (int i = 1; i < (gridSize+1); i++)
	{
		for (int j = 1; j < (gridSize+1); j++)
		{
			int cellLife = countLife(i, j);
			if (pixelmap[i][j]) 
			{
				if (cellLife < 2 || cellLife > 3) 
				{
					temppixelmap[i][j] = 0;
				}
				else
				{
					temppixelmap[i][j] = 1;
				}
			}
			else
			{
				if (cellLife == 3)
				{
					temppixelmap[i][j] = 1;
				}
				else
				{
					temppixelmap[i][j] = 0;
				}
			}
		}
	}

	for (int i = 1; i <= (gridSize+1); i++)
	{
		for (int j = 1; j <= (gridSize+1); j++)
		{
			pixelmap[i][j] = temppixelmap[i][j];
		}
	}
}


void idle()
{
	Sleep(100);

	calcNewLife();

	t++;
	glutPostRedisplay();
}

int main(int argc, char **argv) {
	glutInit(&argc, argv);
	init();

	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowPosition(10,10);
	glutInitWindowSize((5 * gridSize), (5 * gridSize));
	glutCreateWindow("Game of Life");
	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutIdleFunc(idle);

	

	glutMainLoop();
}