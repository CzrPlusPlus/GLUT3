/* Ceasar Moya-Cobian | Student ID: 300274872 | CSCI 172 | Project 6 - Animation */
/* Use arrow keys to move sprite left and right */
/* Use mouse left click on window and sprite will move left or right depending on if the click was on the left side of the screen or right side */
#include <string.h>

#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <math.h>
#include "SOIL.h"
#include <time.h>

#define PI 3.14159

using namespace std;

bool WireFrame= false;
int numbOfClicks = 0;
int MAX_CLICKS = 999;
GLfloat mouseX = 0;
GLfloat mouseY = 0;

GLuint backgroundTex;
GLuint spriteTex;

GLfloat backgroundX = 0.0f;
GLfloat yMax = 1.0f;
GLfloat yMin = 0.0f;
GLfloat spriteXmax = 0.25f;
GLfloat spriteXmin = 0.0f;
GLfloat spriteYmax = 0.5f;
GLfloat spriteYmin = 0.25f;

GLfloat temp;               // used for switching direction of sprite's movement
bool walkingRight = true;   // bool to keep track of sprite orientation
bool isWalking = false;     // bool to keep track of sprite movement (used for mouse click specifically)
clock_t startTime;          // used to show sprite movement for a specified number of seconds (3 seconds in my case)


const GLfloat light_ambient[]  = { 0.0f, 0.0f, 0.0f, 1.0f };
const GLfloat light_diffuse[]  = { 1.0f, 1.0f, 1.0f, 1.0f };
const GLfloat light_specular[] = { 1.0f, 1.0f, 1.0f, 1.0f };
const GLfloat light_position[] = { 2.0f, 5.0f, 5.0f, 0.0f };

const GLfloat mat_ambient[]    = { 0.7f, 0.7f, 0.7f, 1.0f };
const GLfloat mat_diffuse[]    = { 0.8f, 0.8f, 0.8f, 1.0f };
const GLfloat mat_specular[]   = { 1.0f, 1.0f, 1.0f, 1.0f };
const GLfloat high_shininess[] = { 100.0f };


    float Wwidth,Wheight;

/* GLUT callback Handlers */

static void resize(int width, int height)
{
     double Ratio;

     Wwidth = (float)width;
     Wheight = (float)height;

     Ratio= (double)width /(double)height;

    glViewport(0,0,(GLsizei) width,(GLsizei) height);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

	gluPerspective (45.0f,Ratio,0.1f, 100.0f);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

 }

void TextureLoader(char* fileName, GLuint tex)
{
    glEnable(GL_TEXTURE_2D);

    glBindTexture(GL_TEXTURE_2D, tex);

    int width, height;
    unsigned char* image;

    image = SOIL_load_image(fileName, &width, &height, 0, SOIL_LOAD_RGBA);
    if (!image){
        cout << "Image Not Found" << endl;
    }

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
    SOIL_free_image_data(image);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
}

void loadAllTextures(){
    GLuint allTextures[2];

    glGenTextures(2, allTextures);
    backgroundTex = allTextures[0];
    spriteTex = allTextures[1];

    TextureLoader("images/bg3.png", backgroundTex);
    TextureLoader("images/sprite2.png", spriteTex);
}

void setBackground(){
    float quadHeight = 4.15f;   // hard-coded to fit the initial window size
    float quadWidth = 5.55f;
    glBindTexture(GL_TEXTURE_2D, backgroundTex);
    glBegin(GL_QUADS);
    glTexCoord2f(backgroundX, yMin);               glVertex2f(-quadWidth, -quadHeight);    // bottom left
    glTexCoord2f(backgroundX + 1.0f, yMin);        glVertex2f(quadWidth, -quadHeight);     // bottom right
    glTexCoord2f(backgroundX + 1.0f, yMax);        glVertex2f(quadWidth, quadHeight);      // top right
    glTexCoord2f(backgroundX, yMax);               glVertex2f(-quadWidth, quadHeight);     // top left
    glEnd();
}

void createSprite(){
    float spriteWidth = 1.0f;
    float spriteHeight = 2.0f;

    glPushMatrix();

    glTranslatef(0.0, -1.55, 0.0);
    glBindTexture(GL_TEXTURE_2D, spriteTex);
    glBegin(GL_QUADS);

    glTexCoord2f(spriteXmin, spriteYmin);               glVertex2f(-spriteWidth/2, -spriteHeight/2);    // bottom left
    glTexCoord2f(spriteXmax, spriteYmin);               glVertex2f(spriteWidth/2, -spriteHeight/2);     // bottom right
    glTexCoord2f(spriteXmax, spriteYmax);               glVertex2f(spriteWidth/2, spriteHeight/2);      // top right
    glTexCoord2f(spriteXmin, spriteYmax);               glVertex2f(-spriteWidth/2, spriteHeight/2);     // top left

    glEnd();

    glPopMatrix();
}

static void display(void)
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    gluLookAt(0,0,10.0,0.0,0.0,0.0,0.0,1.0,100.0);

    if(WireFrame)
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);		//Draw Our Mesh In Wireframe Mesh
	else
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);		//Toggle WIRE FRAME

 // your code here
    glDisable(GL_DEPTH_TEST);
    setBackground();
    createSprite();
    glEnable(GL_DEPTH_TEST);
    glutSwapBuffers();
}


static void key(unsigned char key, int x, int y)
{
    switch (key)
    {
        case 27 :
        case 'q':
            exit(0);
            break;

	  case 'w':
		WireFrame =!WireFrame;
	       break;
    }
}

void Specialkeys(int key, int x, int y)
{
    switch(key)
    {
    case GLUT_KEY_RIGHT:
        if(!walkingRight){  // if right key is pressed but sprite is not facing right, flip the sprite
            temp = spriteXmax;
            spriteXmax = spriteXmin;
            spriteXmin = temp;
            walkingRight = true;
        }
        spriteXmax += 0.25f;
        spriteXmin += 0.25f;
        backgroundX += 0.01f;
    break;

     case GLUT_KEY_LEFT:
         if(walkingRight){  // if left key is pressed but sprite is not facing left, flip the sprite
            temp = spriteXmax;
            spriteXmax = spriteXmin;
            spriteXmin = temp;
            walkingRight = false;
         }
         spriteXmax += 0.25f;
         spriteXmin += 0.25f;
         backgroundX -= 0.01f;
    break;
   }

  glutPostRedisplay();
}


static void idle(void)
{
    // Use parametric equation with t increment for xpos and y pos
    // Don't need a loop
    if(isWalking){  // Is only true if mouse has been clicked (left click only)
        if ((clock() - startTime) / CLOCKS_PER_SEC < 3) {   // sprite movement will occur for 3 seconds
            if (walkingRight) {
                spriteXmax += 0.25f;
                spriteXmin += 0.25f;
                backgroundX += 0.0001f;
            } else {
                spriteXmax += 0.25f;
                spriteXmin += 0.25f;
                backgroundX -= 0.0001f;
            }
        }
        else{   // after 3 seconds, set isWalking to false
            isWalking = false;
        }
    }

     glutPostRedisplay();
}


void mouse(int btn, int state, int x, int y){

    float scale = 54*(Wwidth/Wheight);

   //  printf(" original %d %d \n", x,y);
    switch(btn){
        case GLUT_LEFT_BUTTON:

        if(state==GLUT_DOWN){

               // get new mouse coordinates for x,y
               // use scale to match right
              if(numbOfClicks <MAX_CLICKS)
                {
                    mouseX = (x - (Wwidth/2))/scale; // Get the mouse x and y coordinate, and translate based on origin.
                    mouseY = ((Wheight/2) - y)/scale;

                    if(x < (Wwidth/2)){         // check if mouse click was on left side of screen
                        if(walkingRight){       // check if sprite is facing right so we can flip
                            temp = spriteXmax;
                            spriteXmax = spriteXmin;
                            spriteXmin = temp;
                            walkingRight = false;
                        }
                    }
                    else{                       // mouse click must be on right side of screen
                        if(!walkingRight){      // check if sprite is facing left side so we can flip
                            temp = spriteXmax;
                            spriteXmax = spriteXmin;
                            spriteXmin = temp;
                            walkingRight = true;
                        }
                    }
                    startTime = clock();        // start startTime
                    isWalking = true;           // set isWalking to true so the movement can be done through the idle function
             }
        }
            break;

        case GLUT_MIDDLE_BUTTON:
        break;

        case GLUT_RIGHT_BUTTON:
        break;
    }
  glutPostRedisplay();
};



static void init(void)
{
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);

    glEnable(GL_NORMALIZE);
    glEnable(GL_COLOR_MATERIAL);

    glEnable(GL_DEPTH_TEST);
    glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
    glShadeModel(GL_SMOOTH);

    glLightfv(GL_LIGHT0, GL_AMBIENT,  light_ambient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE,  light_diffuse);
    glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
    glLightfv(GL_LIGHT0, GL_POSITION, light_position);

    glMaterialfv(GL_FRONT, GL_AMBIENT,   mat_ambient);
    glMaterialfv(GL_FRONT, GL_DIFFUSE,   mat_diffuse);
    glMaterialfv(GL_FRONT, GL_SPECULAR,  mat_specular);
    glMaterialfv(GL_FRONT, GL_SHININESS, high_shininess);

    glEnable(GL_LIGHT0);
    glEnable(GL_NORMALIZE);
    glEnable(GL_LIGHTING);

    loadAllTextures();
    glEnable(GL_BLEND); // used for PNGs since sprite sheets have transparent backgrounds
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

/* Program entry point */

int main(int argc, char *argv[])
{
    glutInit(&argc, argv);

    glutInitWindowSize(800,600);
    glutInitWindowPosition(0,0);
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);

    glutCreateWindow("Project 6 - Animation");
    init();
    glutReshapeFunc(resize);
    glutDisplayFunc(display);
    glutMouseFunc(mouse);
    glutKeyboardFunc(key);
    glutSpecialFunc(Specialkeys);

    glutIdleFunc(idle);

    glutMainLoop();

    return EXIT_SUCCESS;
}
