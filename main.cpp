#include <bits/stdc++.h>
#include <windows.h>
#include "BmpLoader.h"
#include <GL/glut.h>
#include <GL/gl.h>
#include<string>
using namespace std;

static float lookX = 0;
static float lookZ = 10;
static float eyeX = 1, eyeY = 1, eyeZ = 8;
static float carX[8] = {-6.8, -6.7, -4.2, -4.5, 4.2, 2.5, 3, 4.2}, carZ[8] = {-2, -10, -18, -30, -2, -40 , -18, -10};
static float carSpeed[8] = {0.2, 0.3, 0.35, 0.15, 0.25, 0.2, 0.25, 0.3};

unsigned int ID;

static GLfloat v_cube[8][3] =
{
    {0,0,0},
    {0,0,1},
    {0,1,0},
    {0,1,1},
    {1,0,0},
    {1,0,1},
    {1,1,0},
    {1,1,1}
};

static GLubyte c_ind[6][4] =
{
    {3,1,5,7},
    {2,0,1,3},
    {7,5,4,6},
    {2,3,7,6},
    {1,0,4,5},
    {6,4,0,2}
};


static void on_resize(int width, int height)
{
    const float rat = 1.0*width /  height;
    glViewport(0, 0, (GLsizei) width, (GLsizei) height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glFrustum(-rat, rat, -1.0, 1.0, 2.0, 2000.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity() ;
}

static void getNormal3p(GLfloat x1, GLfloat y1, GLfloat z1, GLfloat x2, GLfloat y2, GLfloat z2, GLfloat x3, GLfloat y3, GLfloat z3)
{
    GLfloat Ux, Uy, Uz, Vx, Vy, Vz, Nx, Ny, Nz;

    Ux = x2-x1;
    Uy = y2-y1;
    Uz = z2-z1;

    Vx = x3-x1;
    Vy = y3-y1;
    Vz = z3-z1;

    Nx = Uy*Vz - Uz*Vy;
    Ny = Uz*Vx - Ux*Vz;
    Nz = Ux*Vy - Uy*Vx;

    glNormal3f(Nx,Ny,Nz);
}
void LoadTexture(const char*filename, int rep = 1)
{
    glGenTextures(1, &ID);
    glBindTexture(GL_TEXTURE_2D, ID);
    glPixelStorei(GL_UNPACK_ALIGNMENT, ID);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);


    BmpLoader bl(filename);
    gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGB, bl.iWidth, bl.iHeight, GL_RGB, GL_UNSIGNED_BYTE, bl.textureData );
}
void drawCube(float colorRed, float colorGreen, float colorBlue, bool e=false)
{
    GLfloat no_mat[] = {0, 0, 0, 1.0};
    GLfloat mat_ambient[] = {colorRed,colorGreen,colorBlue,1};
    GLfloat mat_diffuse[] = {colorRed,colorGreen,colorBlue,1};
    GLfloat mat_specular[] = {1,1,1,1};
    GLfloat mat_shininess[] = {30};

    glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
    glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
    glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);



    glBegin(GL_QUADS);
    for(GLint i = 0; i<6; i++)
    {
        getNormal3p(v_cube[c_ind[i][0]][0], v_cube[c_ind[i][0]][1], v_cube[c_ind[i][0]][2],
                    v_cube[c_ind[i][1]][0], v_cube[c_ind[i][1]][1], v_cube[c_ind[i][1]][2],
                    v_cube[c_ind[i][2]][0], v_cube[c_ind[i][2]][1], v_cube[c_ind[i][2]][2]);

//        for(GLint j = 0; j<4; j++){
//            glVertex3fv(&v_cube[c_ind[i][j]][0]);
//        }

        glTexCoord2f(1,1);
        glVertex3fv(&v_cube[c_ind[i][0]][0]);
        glTexCoord2f(1,0);
        glVertex3fv(&v_cube[c_ind[i][1]][0]);
        glTexCoord2f(0,0);
        glVertex3fv(&v_cube[c_ind[i][2]][0]);
        glTexCoord2f(0,1);
        glVertex3fv(&v_cube[c_ind[i][3]][0]);

    }
    glEnd();
}


void drawText(string str,float colorRed, float colorGreen, float colorBlue,int width=1, float val=0.5,int col=0)
{
    glPushMatrix();
    glLineWidth(width);


    glScalef(.004f*val,.004f*val,10);

    glPushMatrix();
    glScalef(0,0,0);
    drawCube(colorRed, colorGreen, colorBlue);
    glPopMatrix();

    for (int c=0; c != str.size(); c++)
    {
        glutStrokeCharacter(GLUT_STROKE_MONO_ROMAN, str[c]);
    }
    glPopMatrix();

}

void light()
{
    GLfloat no_light[] = {0, 0, 0, 1.0};
    GLfloat light_ambient[] = {0.1, 0.1, 0.1, 1.0};
    GLfloat light_diffuse[] = {1,1,1,1};
    GLfloat light_specular[] = {0.2,0.2,0.2,1};
    GLfloat light_pos[] = {0,3,0,1.0};

    glEnable(GL_LIGHT0);
    glLightfv(GL_LIGHT0,GL_AMBIENT,light_ambient);
    glLightfv(GL_LIGHT0,GL_DIFFUSE,light_diffuse);
    glLightfv(GL_LIGHT0,GL_SPECULAR,light_specular);
    glLightfv(GL_LIGHT0, GL_POSITION, light_pos);

}


void axis(void)
{

    glBegin(GL_LINES);
    glColor3f (1.0, 0.0, 0.0); ///red is X
    glVertex3f(0.0, 0.0, 0.0);
    glVertex3f(50.0, 0.0, 0.0);

    glColor3f (0.0, 1.0, 0.0);
    glVertex3f(0.0, 0.0, 0.0);
    glVertex3f(0.0, 50.0, 0.0); /// green is Y

    glColor3f (0.0, 0.0, 1.0);
    glVertex3f(0.0, 0.0, 0.0);
    glVertex3f(0.0, 0.0, 50.0); ///blue is Z
    glEnd();

}

void Floor(void)
{
    for(int i=0; i<15; i++)
    {
        glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, 5);
        glPushMatrix();
        glTranslatef(-100,-3,i*-100);
        glScaled(200,1,110);
        drawCube(.35,.75,.5);
        glDisable(GL_TEXTURE_2D);
        glPopMatrix();
    }
}

void footPath()
{
    for(int i=1; i<=300;i++){
        glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, 3);
        ///Left FootPath
        glPushMatrix();
        glTranslatef(-12.5, -2.7, -5*i);
        glScalef(5.0, 1.0, 5.0);
        drawCube(1,1,1);
        glPopMatrix();

        ///Right FootPath
        glPushMatrix();
        glTranslatef(7.5, -2.7, -5*i);
        glScalef(5.0, 1.0, 5.0);
        drawCube(1,1,1);
        glPopMatrix();

        glDisable(GL_TEXTURE_2D);
    }
}

void road()
{
    ///road
    for(int i=1; i<=150; i++){

        glPushMatrix();
        glTranslatef(-7.5,-2.9,-10*i);
        glScalef(15.0,1.0,10.0);
        drawCube(1,1,1);
        glPopMatrix();
    }
}
void pool(){
    float length = 20,width = 12;

    glPushMatrix();
    ///water
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, 4);
    glBindTexture(GL_TEXTURE_2D, 4);
    glBindTexture(GL_TEXTURE_2D, 4);

    glPushMatrix();
    glScalef(width, 0.0, length);
    drawCube(1, 1, 1);
    glPopMatrix();
    glDisable(GL_TEXTURE_2D);

//    ///Left edge
//    glPushMatrix();
//    glTranslatef(-1.5, 0, 0);
//    glScalef(1.5, 0.1, length);
//    drawCube(0.001, 0.001, 1);
//    glPopMatrix();
//
//    ///Right edge
//    glPushMatrix();
//    glTranslatef(width, 0.0, 0);
//    glScalef(1.5, 0.1, length);
//    drawCube(0.001, 0.001, 1);
//    glPopMatrix();
//
//    ///Front edge
//    glPushMatrix();
//    glTranslatef(-1.5, 0, length);
//    glScalef(width + 3, 0.1, 1.5);
//    drawCube(0.001, 0.001, 1);
//    glPopMatrix();
//
//    ///Back edge
//    glPushMatrix();
//    glTranslatef(-1.5, 0, -1.5);
//    glScalef(width + 3, 0.1, 1.5);
//    drawCube(0.001, 0.001, 1);
//    glPopMatrix();

    glPopMatrix();
}
void building(void)
{
    int height1 = 18,height2,height3,height4;
    int width = 13 ,length = 15;

    for(int j=0;j<8;j++){
        int space = j*165;
        for(int i=0;i<4;i++){
            glPushMatrix();
            ///House
            glEnable(GL_TEXTURE_2D);

            if(i&1)
                glBindTexture(GL_TEXTURE_2D,9);
            else
                glBindTexture(GL_TEXTURE_2D, 12);

            int dst = i*10;

            glPushMatrix();
            glTranslatef(-20- width, -2, -100 -i*length - dst - space);
            glScalef(width, height1, length);
            drawCube(1,1,1);
            glPopMatrix();

            glPushMatrix();
            glTranslatef(-20- width, -2+height1, -100 -i*length - dst - space);
            glScalef(width, height1, length);
            drawCube(1,1,1);
            glPopMatrix();

            glDisable(GL_TEXTURE_2D);

            ///HouseDoor
            glPushMatrix();
            glTranslatef(-20, -2, -96.5 - i*length - dst -space);
            glScalef(.1, 9, 7);
            drawCube(.8,.8,.8);
            glPopMatrix();

            glEnable(GL_TEXTURE_2D);
            glBindTexture(GL_TEXTURE_2D, 10);

            glPushMatrix();
            glTranslatef(-19.5, -1, -95 - i*length -dst - space);
            glScalef(.01, 5, 5.3);
            drawCube(1,1,1);
            glPopMatrix();

            glDisable(GL_TEXTURE_2D);
            ///Stair
            glPushMatrix();
            glTranslatef(-19.5, -2, -95 - i*length - dst - space);
            glScalef(1, 1, 5.3);
            drawCube(0.5, 0.5, 0.5);
            glPopMatrix();
            glPushMatrix();
            glTranslatef(-18.5, -2, -95 - i*length - dst - space);
            glScalef(1, 0.5, 5.3);
            drawCube(0.5, 0.5, 0.5);
            glPopMatrix();
            glPopMatrix();
        }
    }
}

void bench()
{
    float length = 10, height = 2.0, width = 1.0;

    for(int i=0;i<10;i++){
        glPushMatrix();
        glTranslatef( -15, -1.7, -50 - i*165);

        glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D,2);
        glPushMatrix();
        glTranslatef(-0.5, 0, -0.9);
        glScalef(0.001, height * 2.3, length + 2);
        drawCube(1, 1, 1);
        glPopMatrix();

        glPushMatrix();
        glTranslatef(-0.5, height * 2.3 + 0.001, -0.9);
        glScalef(width * 3.2 , 0.001, length + 2);
        drawCube(1, 1, 1);
        glPopMatrix();
        glDisable(GL_TEXTURE_2D);

        ///base
        glPushMatrix();
        glTranslatef(0, height/2, 0);
        glScalef(width, 0.05, length);
        drawCube(0.980, 0.922, 0.843);
        glPopMatrix();

        ///Left back leg
        glPushMatrix();
        glTranslatef(0, 0, 0);
        glScalef(0.1, height, 0.1);
        drawCube(0.980, 0.922, 0.843);
        glPopMatrix();

        ///Left front leg
        glPushMatrix();
        glTranslatef(width - 0.1, 0, 0);
        glScalef(0.1, height/2, 0.1);
        drawCube(0.980, 0.922, 0.843);
        glPopMatrix();

        ///Right back leg
        glPushMatrix();
        glTranslatef(0, 0, length-0.1);
        glScalef(0.1, height, 0.1);
        drawCube(0.980, 0.922, 0.843);
        glPopMatrix();

        ///Right front leg
        glPushMatrix();
        glTranslatef(width - 0.1, 0 , length-0.1);
        glScalef(0.1, height/2, 0.1);
        drawCube(0.980, 0.922, 0.843);
        glPopMatrix();

        ///Lean Back
        for(int i=1;i<10;i+=2){
            glPushMatrix();
            glTranslatef(0, height-0.1*i , 0.1);
            glScalef(0.1, 0.1, length-0.2);
            drawCube(0.980, 0.922, 0.843);
            glPopMatrix();
        }

        glPopMatrix();
    }

}
void shop()
{
    float width = 5, height = 5 , length = 8;

    for(int j=0;j<10;j++){
        int dst = j*165;
        for(int i=0;i<2;i++){
            glPushMatrix();
            ///MainShop
            glEnable(GL_TEXTURE_2D);
            glBindTexture(GL_TEXTURE_2D, 6);
            glPushMatrix();
            glTranslatef(-19, -1.9, -20 - i*length - i*1 - dst);
            glScalef(width, height, length);
            drawCube(0.5, 0.5, 0.5);
            glPopMatrix();

            ///Stair
            glPushMatrix();
            glTranslatef(-19 + width, -1.9, -20 - i*length - i*1 -dst);
            glScalef(0.5, 0.6, length);
            drawCube(0.5, 0.5, 0.5);
            glPopMatrix();

            glPushMatrix();
            glTranslatef(-19 + width + 0.5, -1.9, -20 - i*length - i*1 -dst);
            glScalef(0.5, 0.3, length);
            drawCube(0.5, 0.5, 0.5);
            glPopMatrix();
            ///Small RoofTop
            glPushMatrix();
            glTranslatef(-19 + width, height-2.7, -20 - i*length - i*1 -dst);
            glScalef(1, 0.2, length);
            drawCube(0.5, 0.5 ,0.5);
            glPopMatrix();
            glDisable(GL_TEXTURE_2D);

            ///Store Front
            glEnable(GL_TEXTURE_2D);
            if(!(i&1))
                glBindTexture(GL_TEXTURE_2D, 7);
            else
                glBindTexture(GL_TEXTURE_2D, 8);

            glPushMatrix();
            glTranslatef(-19 + width, -1.9, -20 - i*length - i*1 -dst);
            glScalef(0.01, height, length);
            drawCube(1,1,1);
            glPopMatrix();
            glDisable(GL_TEXTURE_2D);
            glPopMatrix();
        }
    }
}

void car(void)
{
    glPushMatrix();

    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D,6);
    ///lowerPart
    glPushMatrix();
    glTranslatef(0, -0.8, 0 - 2);
    glScalef(2, 0.8, 5);
    drawCube(1, 1, 1);
    glPopMatrix();

    ///upperPart
    glPushMatrix();
    glTranslatef(0, 0, 0 - 0.4);
    glScalef(2, 0.55, 2);
    drawCube(1, 1, 1);
    glPopMatrix();

    glDisable(GL_TEXTURE_2D);

    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, 11);
    ///backGlass
    glPushMatrix();
    glTranslatef(0, 0, 0 + 1.95);
    glRotatef(-35,1,0,0);
    glScalef(2, 0.65, 0.01);
    drawCube(1, 1, 1);
    glPopMatrix();
    ///FrontGlass
    glPushMatrix();
    glTranslatef(0, 0, 0 - 1);
    glRotatef(45, 1,0,0);
    glScalef(2,0.8, 0.01);
    drawCube(1,1,1);
    glPopMatrix();

    glDisable(GL_TEXTURE_2D);

    ///Signal Light Left
    glPushMatrix();
    glTranslatef(0, -0.4, 0 + 3.1);
    glScalef(.3, .15, .01);
    drawCube(1, 0, 0);
    glPopMatrix();

    ///Signal Light Right
    glPushMatrix();
    glTranslatef(0 + 2 - 0.3 , -0.4, 0 + 3.1);
    glScalef(.3, .15, .01);
    drawCube(1, 0, 0);
    glPopMatrix();

    glPopMatrix();
}

static void display(void)
{

    glClearColor(0.686, 0.933, 0.933, 1.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();

    gluLookAt(lookX + eyeX, eyeY, eyeZ + lookZ, lookX, 0, lookZ-5, 0, 1, 0);

    //axis();

    ///Start text Section
    stringstream ss;
    ss<<lookX;
    string temp(ss.str());
    string s1 = "LookX = " + temp;
    ss.str(string());
    ss<<lookZ;
    temp = ss.str();
    string s2 = " LookZ = " + temp;
    s1 = s1 + s2;

    glPushMatrix();
    glTranslatef(lookX-2.3 , 2.7 , lookZ);
    glRotatef(-2,0,0,1);
    drawText(s1,0, 0, 1  ,2);
    glPopMatrix();
    ///End text section

    glPushMatrix();
    glTranslatef(lookX-2.3 , 10 , lookZ-5);
    glScalef(1,1,200);
    light();
    glPopMatrix();

    shop();

    bench();

    building();

    footPath();
    Floor();

    glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D,1);
        road();
    glDisable(GL_TEXTURE_2D);


    for(int i=0;i<8;i++){
        glPushMatrix();
        glTranslatef(carX[i], -0.5, carZ[i]);
        car();
        glPopMatrix();

    }

    glPushMatrix();
    glTranslatef(lookX, 0, lookZ);
    car();
    glPopMatrix();

    glutSwapBuffers();

}

bool* keyStates = new bool[256];
static bool running = false, start = false;
static void keyPressed(unsigned char key, int x, int y)
{
    keyStates[key] = true;

    if(keyStates['e']){
        running = true;
        start = true;
        lookZ = lookZ - 1;
        if(keyStates['f']){
            if ( lookX < 4.5)
                lookX = lookX + 0.15;
        }
        else if(keyStates['a']){
            if ( lookX > -6.5)
                lookX = lookX - 0.15;
        }
    }
    else if(keyStates['s']){
        start = true;
        running = false;
        lookZ = lookZ + 0.3;

        if(keyStates['f']){
            if ( lookX < 4.5)
                lookX = lookX + 0.15;
        }
        else if(keyStates['a']){
            if ( lookX > -6.5)
                lookX = lookX - 0.15;
        }
    }
    else if(keyStates['a']){
        if ( lookX > -6.2)
                lookX = lookX - 0.2;
    }
    else if(keyStates['f']){
        if ( lookX < 4.5)
                lookX = lookX + 0.15;
    }
    else if(keyStates['z']){
            eyeZ-=0.5;

    }
    else if(keyStates['Z']){
        eyeZ+=0.5;
    }
    else if(keyStates['x']){
        eyeX-=0.5;
    }
    else if(keyStates['X']){
        eyeX+=0.5;
    }
    else if(keyStates['y']){
        eyeY-=0.5;
    }
    else if(keyStates['Y']){
        eyeY+=0.5;
    }
    else if(keyStates['q']){
        exit(0);
    }

    glutPostRedisplay();
}

static void keyUp(unsigned char key, int x, int y){
    keyStates[key] = false;

}

static void idle(void)
{
    if(lookZ<-1300)
        lookZ = -80;
    else if(lookZ>3){
        lookZ=3;
    }
    if(start){
        for(int i=0;i<8;i++){
            if(carZ[i]<-1300)
                carZ[i] = -80 + i*5;
            else{
                carZ[i] -= carSpeed[i];
            }
        }
    }

    glutPostRedisplay();
}

int main(int argc, char *argv[])
{
    glutInit(&argc, argv);
    glutInitWindowSize(620,480);
    glutInitWindowPosition(10,10);
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
    glutInitWindowPosition (100, 100);


    glutCreateWindow("Car Ride");

    LoadTexture("C:\\running-project\\graphics\\Car Ride\\images\\road.bmp",1);
    LoadTexture("C:\\running-project\\graphics\\Car Ride\\images\\tin.bmp",2);
    LoadTexture("C:\\running-project\\graphics\\Car Ride\\images\\footpath.bmp",3);
    LoadTexture("C:\\running-project\\graphics\\Car Ride\\images\\pool.bmp",4);
    LoadTexture("C:\\running-project\\graphics\\Car Ride\\images\\grass.bmp",5);
    LoadTexture("C:\\running-project\\graphics\\Car Ride\\images\\wall.bmp",6);
    LoadTexture("C:\\running-project\\graphics\\Car Ride\\images\\shopFront.bmp",7);
    LoadTexture("C:\\running-project\\graphics\\Car Ride\\images\\shopFront2.bmp",8);
    LoadTexture("C:\\running-project\\graphics\\Car Ride\\images\\houseWall.bmp",9);
    LoadTexture("C:\\running-project\\graphics\\Car Ride\\images\\gateDoor.bmp",10);
    LoadTexture("C:\\running-project\\graphics\\Car Ride\\images\\car.bmp",11);
    LoadTexture("C:\\running-project\\graphics\\Car Ride\\images\\houseWall1.bmp",12);

    glutReshapeFunc(on_resize);
    glutDisplayFunc(display);
    glutKeyboardFunc(keyPressed);
    glutKeyboardUpFunc(keyUp);
    glutIdleFunc(idle);




    glShadeModel( GL_SMOOTH );
    glEnable( GL_DEPTH_TEST );
    glEnable(GL_NORMALIZE);
    glEnable(GL_BLEND);
    glEnable(GL_LIGHTING);

    printf("########################################################################################\n");
    printf("########################################################################################\n");
    printf("##############                                                         #################\n");
    printf("##############           PLEASE FOLLOW THE BELOW INSTRUCTIONS          #################\n");
    printf("##############                                                         #################\n");
    printf("########################################################################################\n");
    printf("########################################################################################\n\n\n");
    printf("Press  'E' for Forward\n");
    printf("Press  'S' for Backward\n");
    printf("Press  'A' for Left\n");
    printf("Press  'F' for Right\n");
    printf("Press  'Q' to close the game\n");

    glutMainLoop();

    return EXIT_SUCCESS;
}
