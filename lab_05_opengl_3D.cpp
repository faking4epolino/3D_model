#include <iostream>
#include <fstream> 
#include <cstdlib> // for exit(0)
#include <cmath>
#include "glut.h"

#pragma comment(lib, "opengl32.lib")
#pragma comment(lib, "glut32.lib")

const int N = 99;
float data[N][N];
float minlvl;
float maxlvl;
int last_x, last_y;
float rotation_x = 0.0;
float rotation_y = 0.0;
float rotation_z = 0.0;
float zoom = 130.0; 
float initial_rotation_x = 0.0; // the initial position of the virtual camera
float initial_rotation_y = 0.0;
float initial_zoom = 130.0; 
bool drawXPlaneFlag = false;
bool drawYPlaneFlag = false;
bool rotX = false;
bool rotY= false;
bool rotZ = false;
bool flag_cmd = false; 
int rotationSpeed = 1;

class Mesh {
public:
    // reading and writing an array from a 99x99 text file
    bool LoadFromFile(std::string fileName) {
        std::ifstream file(fileName);
        if (!file) {
            std::cerr << "Error while opening the file " << fileName << std::endl;
            return false;
        }

        for (int i = 0; i < N; ++i) {
            for (int j = 0; j < N; ++j) {
                file >> data[i][j];
            }
        }
        std::cout << "Success! File is open!\n";
        file.close();
        return true;
    }

    /* outputs information about the found maximum and minimum values
    along the Z axis to the console */
    void MinMaxValues() {

        for (int i = 0; i < N; ++i) {
            for (int j = 0; j < N; ++j) {
                if (data[i][j] < minlvl) {
                    minlvl = data[i][j];
                }
                if (data[i][j] > maxlvl) {
                    maxlvl = data[i][j];
                }
            }
        }

    }

    void Draw() {

        MinMaxValues();

        for (int i = 0; i < N-1; i++)
        {
            for (int j = 0; j < N-1; j++)
            {
                //Color interpolation from black at minimum to blue at maximum
                float high=data[i][j];
                float t = (maxlvl- high)/(maxlvl- minlvl);
                float r = 0;
                float b = 1-t;
                float g = 0;

                /*The figure is drawn in the center of the screen 
                and rotate relative to its center*/
                glBegin(GL_LINE_LOOP);
                glColor3f(r, g,b);
                glVertex3f(i-(N-1)/2, j-(N-1)/2, data[i][j]);
                glVertex3f(i - (N - 1) / 2 + 1, j - (N - 1) / 2, data[i + 1][j]);
                glVertex3f(i - (N - 1) / 2 + 1, j - (N - 1) / 2 + 1, data[i + 1][j + 1]);
                glVertex3f(i- (N - 1) / 2, j - (N - 1) / 2 + 1, data[i][j + 1]);


                glEnd();

            }
        }

    }



};

/*functions for drawing planes perpendicular to the X and Y axes 
and passing through a point (0,0,0)*/
void drawXPlane() {

    glEnable(GL_BLEND);
    glBlendFunc(GL_ONE, GL_ONE);

    glColor3f(1, 0.0, 0.0);
    glBegin(GL_POLYGON);

    glVertex3f(100.0, 0.0, 0.0);
    glVertex3f(100.0, 0.0, 100.0);
    glVertex3f(-100.0, 0.0, 100.0);
    glVertex3f(-100.0, 0.0, -100.0);
    glVertex3f(100.0, 0.0, -100.0);
    glVertex3f(100.0, 0.0, 0.0);

    glEnd();
    glDisable(GL_BLEND);
}

void drawYPlane() {

    glEnable(GL_BLEND);
    glBlendFunc(GL_ONE, GL_ONE);

    glColor3f(0.0, 1, 0.0);
    glBegin(GL_POLYGON);
    glVertex3f(0.0, 100.0, 0.0);
    glVertex3f(0.0, 100.0, 100.0);
    glVertex3f(0.0, -100.0, 100.0);
    glVertex3f(0.0, -100.0, -100.0);
    glVertex3f(0.0, 100.0, -100.0);
    glVertex3f(0.0, 100.0, 0.0);

    glEnd();
    glDisable(GL_BLEND);
}

/*the rotation function of the object, 
which takes a fixed value of the rotation angle*/
void rotationObj(int value) {

    if (rotX) {
        rotation_x += rotationSpeed;
        glutPostRedisplay();
        glutTimerFunc(1000/60, rotationObj,0);
    }
    if (rotY) {
        rotation_y += rotationSpeed;
        glutPostRedisplay();
        glutTimerFunc(1000 / 60, rotationObj, 0);

    }
    if (rotZ) {
        rotation_z += rotationSpeed;
        glutPostRedisplay();
        glutTimerFunc(1000 / 60, rotationObj, 0);
    }
}

/*the functions below catch the facts of clicking and moving the mouse. 
The click is caught by a function associated with OpenGL via glutMouseFunc. 
The movement is caught using a function that binds to OpenGL using glutMotionFunc. 
This function is called only when the movement occurs with the key pressed.*/
void mouse(int button, int state, int x, int y) {
    if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
        last_x = x;
        last_y = y;
    }
}

void motion(int x, int y) {
    rotation_x += (y - last_y);
    rotation_y += (x - last_x);
    last_x = x;
    last_y = y;
    glutPostRedisplay();
}

/*the function of drawing small axes
showing the rotation of the shape.*/
void axes() {

        glLoadIdentity();
        gluLookAt(0, 0, 20.0, 13.0, 7.5, 0, 0, 1, 0);
        

        glRotatef(rotation_x, 1, 0, 0);
        glRotatef(rotation_y, 0, 1, 0);
        glRotatef(rotation_z, 0, 0, 1);

        glBegin(GL_LINES);

        // X (red)
        glColor3f(1.0, 0.0, 0.0);
        glVertex3f(0, 0, 0);
        glVertex3f(1, 0, 0);

        // Y (green)
        glColor3f(0.0, 1.0, 0.0);
        glVertex3f(0, 0, 0);
        glVertex3f(0, 1, 0);

        //Z (blue)
        glColor3f(0.0, 0.0, 1.0);
        glVertex3f(0, 0, 0);
        glVertex3f(0, 0, 1);
        glEnd();

        glFlush();
}

//function of reading function keys
void read_special_kb(int key, int, int) {
    switch (key) {
    case GLUT_KEY_HOME:
        rotation_x = initial_rotation_x;
        rotation_y = initial_rotation_y;
        zoom = initial_zoom;
        glutPostRedisplay();
        break;
    case GLUT_KEY_F6:
        if (!drawXPlaneFlag && !drawYPlaneFlag) {
            drawXPlaneFlag = true; 
        }
        else if (drawXPlaneFlag && !drawYPlaneFlag) {
            drawYPlaneFlag = true; 
        }
        else {

            drawXPlaneFlag = false;
            drawYPlaneFlag = false;
            glutPostRedisplay();
        }
        break;
    case GLUT_KEY_F7: {

        if (!rotX && !rotY && !rotZ) {

            rotX = true;
            glutTimerFunc(1000 / 60, rotationObj, 0);

        }
        else if (rotX && !rotY && !rotZ) {

            rotX = false;
            rotY = true;
            glutTimerFunc(1000 / 60, rotationObj, 0);
        }
        else if (!rotX && rotY && !rotZ) {

            rotY = false;
            rotZ = true;
            glutTimerFunc(1000 / 60, rotationObj, 0);
        }
        else if (!rotX && !rotY && rotZ) {

            rotZ = false;
        }
        break;
        glutPostRedisplay();
    }


    }
}


void keyboard(unsigned char key, int x, int y) {
    switch (key) {
        case '+':
            zoom -= 1.0; // reducing the zoom value to get closer to the shape
            break;
        case '-':
            zoom += 1.0; // increasing the zoom value to move away from the shape
            break;
        case 27:
            exit(0); // the Esc key to exit the program
            break;
        default:
            break;
    }

    glutPostRedisplay();
}

Mesh myZ;

void RenderScene() 
{
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
  
  glTranslatef(0, 0, -zoom);
  glRotatef(rotation_x, 1, 0, 0);
  glRotatef(rotation_y, 0, 1, 0);
  glRotatef(rotation_z, 0, 0, 1);

  myZ.Draw();
  
  if (drawXPlaneFlag) {
      drawXPlane(); 
  }
  if (drawYPlaneFlag) {
      drawYPlane();
  }
  if (flag_cmd){

  axes();

  }
  
  glutSwapBuffers();
}

void ReshapeWindow(GLsizei width, GLsizei height) 
{
  if (height == 0)
    height = 1;
  GLfloat aspect = (GLfloat)width / (GLfloat)height;
  glViewport(0, 0, width, height);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluPerspective(45.0f, aspect, 0.1f, 1000.0f);
}

int main(int argc, char *argv[]) {

    std::string command;
    std::string command_2;
    if (argc == 3) {
        command = argv[1];
        command_2 = argv[2];
        if (command_2 == "axes") {

            flag_cmd = true;

        }
        else {
            std::cerr << "The argument for drawing small axes is set incorrectly!\n\n";
        }
    }
    else {
        std::cout << "False!\nThe arguments were not set correctly\n";
        glLoadIdentity();
    }
   
 
 std::string fileName = command;
 myZ.LoadFromFile(fileName);
 myZ.MinMaxValues();
 std::cout << "\nMinimum value on the Z axis: " << minlvl << std::endl;
 std::cout << "Maximum value on the Z axis: " << maxlvl << std::endl;

  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
  glutInitWindowPosition(100, 100);
  glutInitWindowSize(1920, 1080);
  glutCreateWindow("3D MODEL");
  glEnable(GL_DEPTH_TEST);
  glDepthFunc(GL_LEQUAL);
  glShadeModel(GL_SMOOTH);
  glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
  glClearColor(0.55, 0.55, 0.55, 1); 

  glutDisplayFunc(RenderScene);
  glutReshapeFunc(ReshapeWindow);
  glutMouseFunc(mouse);
  glutMotionFunc(motion);
  glutKeyboardFunc(keyboard);
  glutSpecialFunc(read_special_kb);
  


  glutMainLoop();

  return 0;
}

