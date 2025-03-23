#include <math.h>
#include <GL/glut.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

float angle=0.0, deltaAngle = 0.0, ratio;
float x=5.0f, y=3.0f, z=10.0f;
float lx=-0.5f, ly=-0.1f, lz=-1.0f;
int deltaMove = 0, h, w;
int moveSide = 0; 
int moveUp = 0; 
int bitmapHeight=12;

int mouseX = 0, mouseY = 0;
bool mouseLeftDown = false;
bool mouseRightDown = false;
float cameraAngleX = 0.0f;
float cameraAngleY = 0.0f;

float carX = 0.0f, carZ = 0.0f;
float carSpeed = 0.1f;
bool controllingCar = false;

void updateCamera();

void Reshape(int w1, int h1)
{
    if(h1 == 0) h1 = 1;
    w = w1;
    h = h1;
    ratio = 1.0f * w / h;
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glViewport(0, 0, w, h);
    gluPerspective(45, ratio, 0.1, 1000);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(
        x, y, z,
        x + lx, y + ly, z + lz,
        0.0f, 1.0f, 0.0f);
}

void orientMe(float ang)
{
    lx = sin(ang);
    lz = -cos(ang);
    updateCamera();
}

void updateCamera()
{
    lx = sin(cameraAngleY) * cos(cameraAngleX);
    ly = sin(cameraAngleX);
    lz = -cos(cameraAngleY) * cos(cameraAngleX);
    
    glLoadIdentity();
    gluLookAt(x, y, z,
              x + lx, y + ly, z + lz,
              0.0f, 1.0f, 0.0f);
}

void moveMeFlat(int i)
{
    x = x + i*(lx)*0.1;
    z = z + i*(lz)*0.1;
    updateCamera();
}

void moveMeSide(int i)
{
    x = x + i*(-lz)*0.1;
    z = z + i*(lx)*0.1;
    updateCamera();
}

void moveMeUpDown(int i)
{
    y = y + i*0.1;
    updateCamera();
}

void Grid() {
    double i;
    const float Z_MIN = -50, Z_MAX = 50;
    const float X_MIN = -50, X_MAX = 50;
    const float gap = 1.5;
    glColor3f(0.5, 0.5, 0.5);
    glBegin(GL_LINES);
    for(i=Z_MIN; i<Z_MAX; i+=gap)
    {
        glVertex3f(i, 0, Z_MIN);
        glVertex3f(i, 0, Z_MAX);
    }
    for(i=X_MIN; i<X_MAX; i+=gap)
    {
        glVertex3f(X_MIN, 0, i);
        glVertex3f(X_MAX, 0, i);
    }
    glEnd();
}

void BuatSisiKotak(float panjang, float lebar) {
    glBegin(GL_QUADS);
    glVertex3f(-panjang/2, -lebar/2, 0.0f);
    glVertex3f(panjang/2, -lebar/2, 0.0f);
    glVertex3f(panjang/2, lebar/2, 0.0f);
    glVertex3f(-panjang/2, lebar/2, 0.0f);
    glEnd();
}

void KotakBenar(float panjang, float lebar, float tinggi, float x, float y, float z) {
    glPushMatrix();
    glTranslatef(x, y, z);
    
    glPushMatrix();
    glTranslatef(0.0f, 0.0f, tinggi/2);
    BuatSisiKotak(panjang, lebar);
    glPopMatrix();
    
    glPushMatrix();
    glTranslatef(0.0f, 0.0f, -tinggi/2);
    glRotatef(180, 0, 1, 0);
    BuatSisiKotak(panjang, lebar);
    glPopMatrix();
    
    glPushMatrix();
    glTranslatef(0.0f, lebar/2, 0.0f);
    glRotatef(-90, 1, 0, 0);
    BuatSisiKotak(panjang, tinggi);
    glPopMatrix();
    
    glPushMatrix();
    glTranslatef(0.0f, -lebar/2, 0.0f);
    glRotatef(90, 1, 0, 0);
    BuatSisiKotak(panjang, tinggi);
    glPopMatrix();
    
    glPushMatrix();
    glTranslatef(-panjang/2, 0.0f, 0.0f);
    glRotatef(-90, 0, 1, 0);
    BuatSisiKotak(tinggi, lebar);
    glPopMatrix();
    
    glPushMatrix();
    glTranslatef(panjang/2, 0.0f, 0.0f);
    glRotatef(90, 0, 1, 0);
    BuatSisiKotak(tinggi, lebar);
    glPopMatrix();
    
    glPopMatrix();
}

void BuatRoda(float radius, float width, int slices, float x, float y, float z, int orientation) {
    glPushMatrix();
    glTranslatef(x, y, z);
    
    if (orientation == 1) {
        glRotatef(90, 0, 1, 0);
    } else {
        glRotatef(-90, 0, 1, 0);
    }
    
    glBegin(GL_POLYGON);
    for (int i = 0; i < slices; i++) {
        float angle = i * 2.0f * M_PI / slices;
        glVertex3f(0.0f, radius * cos(angle), radius * sin(angle));
    }
    glEnd();
    
    glBegin(GL_POLYGON);
    for (int i = 0; i < slices; i++) {
        float angle = i * 2.0f * M_PI / slices;
        glVertex3f(width, radius * cos(angle), radius * sin(angle));
    }
    glEnd();
    
    glBegin(GL_QUAD_STRIP);
    for (int i = 0; i <= slices; i++) {
        float angle = i * 2.0f * M_PI / slices;
        glVertex3f(0.0f, radius * cos(angle), radius * sin(angle));
        glVertex3f(width, radius * cos(angle), radius * sin(angle));
    }
    glEnd();
    
    glPopMatrix();
}

void Mobil3D() {
    glPushMatrix();
    glTranslatef(carX, 0.0f, carZ); 
    
    glColor3f(1.0f, 0.0f, 0.0f);
    KotakBenar(5.0f, 1.5f, 2.0f, 0.0f, 1.0f, 0.0f);
    
    glColor3f(1.0f, 1.0f, 1.0f);
    KotakBenar(3.0f, 1.2f, 1.8f, 0.0f, 2.2f, 0.0f);
    
    glColor3f(1.0f, 0.0f, 0.0f);
    KotakBenar(1.5f, 1.0f, 1.8f, 2.0f, 1.2f, 0.0f);
    
    glColor3f(1.0f, 1.0f, 1.0f);
    KotakBenar(1.0f, 1.0f, 1.8f, -2.2f, 1.2f, 0.0f);
    
    glColor3f(0.7f, 0.7f, 1.0f);
    KotakBenar(0.1f, 1.0f, 1.7f, 1.4f, 1.8f, 0.0f);
    
    KotakBenar(0.1f, 1.0f, 1.7f, -1.4f, 1.8f, 0.0f);
    
    glColor3f(0.2f, 0.2f, 0.2f);
    
    BuatRoda(0.6f, 0.5f, 20, 1.8f, 0.6f, 1.0f, 0);
    
    BuatRoda(0.6f, 0.5f, 20, 1.8f, 0.6f, -1.0f, 1);
    
    BuatRoda(0.6f, 0.5f, 20, -1.8f, 0.6f, 1.0f, 0);
    
    BuatRoda(0.6f, 0.5f, 20, -1.8f, 0.6f, -1.0f, 1);
    
    glColor3f(1.0f, 1.0f, 0.0f);
    KotakBenar(0.2f, 0.2f, 0.2f, 2.6f, 1.0f, 0.6f);
    KotakBenar(0.2f, 0.2f, 0.2f, 2.6f, 1.0f, -0.6f);
    
    glColor3f(1.0f, 0.0f, 0.0f);
    KotakBenar(0.2f, 0.2f, 0.4f, -2.6f, 1.0f, 0.6f);
    KotakBenar(0.2f, 0.2f, 0.4f, -2.6f, 1.0f, -0.6f);
    
    glPopMatrix();
}

void display() {
    if (deltaMove)
        moveMeFlat(deltaMove);
    if (moveSide)
        moveMeSide(moveSide);
    if (moveUp)
        moveMeUpDown(moveUp);
    if (deltaAngle) {
        angle += deltaAngle;
        orientMe(angle);
    }
    
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    Grid();
    
    Mobil3D();
    
    glutSwapBuffers();
    glFlush();
}

void pressKey(int key, int x, int y) {
    switch (key) {
        case GLUT_KEY_LEFT : deltaAngle = -0.01f; break;
        case GLUT_KEY_RIGHT : deltaAngle = 0.01f; break;
        case GLUT_KEY_UP : deltaMove = 1; break;
        case GLUT_KEY_DOWN : deltaMove = -1; break;
    }
}

void releaseKey(int key, int x, int y) {
    switch (key) {
        case GLUT_KEY_LEFT :
            if (deltaAngle < 0.0f)
                deltaAngle = 0.0f;
            break;
        case GLUT_KEY_RIGHT : 
            if (deltaAngle > 0.0f)
                deltaAngle = 0.0f;
            break;
        case GLUT_KEY_UP : 
            if (deltaMove > 0)
                deltaMove = 0;
            break;
        case GLUT_KEY_DOWN : 
            if (deltaMove < 0)
                deltaMove = 0;
            break;
    }
}

void keyboard(unsigned char key, int x, int y) {
    switch (key) {
        case 'w':
        case 'W':
            deltaMove = 1;
            break;
        case 's':
        case 'S':
            deltaMove = -1;
            break;
        case 'a':
        case 'A':
            moveSide = -1;
            break;
        case 'd':
        case 'D':
            moveSide = 1;
            break;
        case ' ':
            moveUp = 1;
            break;
        case 'x':
        case 'X': 
            moveUp = -1;
            break;
        case 'm': 
        case 'M':
            controllingCar = !controllingCar;
            printf("Mode kontrol mobil: %s\n", controllingCar ? "AKTIF" : "NONAKTIF");
            break;
        case 27: 
            exit(0);
            break;
    }
}

void keyboardUp(unsigned char key, int x, int y) {
    switch (key) {
        case 'w':
        case 'W':
            if (deltaMove > 0)
                deltaMove = 0;
            break;
        case 's':
        case 'S':
            if (deltaMove < 0)
                deltaMove = 0;
            break;
        case 'a':
        case 'A':
            if (moveSide < 0)
                moveSide = 0;
            break;
        case 'd':
        case 'D':
            if (moveSide > 0)
                moveSide = 0;
            break;
        case ' ': 
            if (moveUp > 0)
                moveUp = 0;
            break;
        case 'x':
        case 'X': 
            if (moveUp < 0)
                moveUp = 0;
            break;
    }
}

void mouse(int button, int state, int x, int y) {
    mouseX = x;
    mouseY = y;
    
    if (button == GLUT_LEFT_BUTTON) {
        if (state == GLUT_DOWN) {
            mouseLeftDown = true;
        }
        else if (state == GLUT_UP) {
            mouseLeftDown = false;
        }
    }
    
    if (button == GLUT_RIGHT_BUTTON) {
        if (state == GLUT_DOWN) {
            mouseRightDown = true;
            controllingCar = !controllingCar;
            printf("Mode kontrol mobil: %s\n", controllingCar ? "AKTIF" : "NONAKTIF");
        }
        else if (state == GLUT_UP) {
            mouseRightDown = false;
        }
    }
}

void mouseMotion(int x, int y) {
    float dx = x - mouseX;
    float dy = y - mouseY;
    
    if (mouseLeftDown) {
        if (controllingCar) {
            carX += dx * 0.05f;
            carZ += dy * 0.05f;
        } else {

            float mouseSensitivity = 0.003f;
            
            cameraAngleY += dx * mouseSensitivity;
            cameraAngleX += dy * mouseSensitivity;
            
            if (cameraAngleX > 1.5f) cameraAngleX = 1.5f;
            else if (cameraAngleX < -1.5f) cameraAngleX = -1.5f;
            
            updateCamera();
        }
    }
    
    mouseX = x;
    mouseY = y;
}

const GLfloat light_ambient[] = { 0.5f, 0.5f, 0.5f, 0.0f };
const GLfloat light_diffuse[] = { 1.0f, 1.0f, 1.0f, 1.0f };
const GLfloat light_specular[] = { 1.0f, 1.0f, 1.0f, 1.0f };
const GLfloat light_position[] = { 0.0f, 20.0f, 10.0f, 1.0f };
const GLfloat mat_ambient[] = { 0.7f, 0.7f, 0.7f, 1.0f };
const GLfloat mat_diffuse[] = { 0.8f, 0.8f, 0.8f, 1.0f };
const GLfloat mat_specular[] = { 1.0f, 1.0f, 1.0f, 1.0f };
const GLfloat high_shininess[] = { 100.0f };

void lighting(){
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
    glEnable(GL_LIGHT0);
    glEnable(GL_NORMALIZE);
    glEnable(GL_COLOR_MATERIAL);
    glEnable(GL_LIGHTING);
    glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
    glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
    glLightfv(GL_LIGHT0, GL_POSITION, light_position);
    glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
    glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
    glMaterialfv(GL_FRONT, GL_SHININESS, high_shininess);
}

void init(void)
{
    glEnable(GL_DEPTH_TEST);
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}

int main(int argc, char **argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
    glutInitWindowPosition(100,100);
    glutInitWindowSize(640,480);
    glutCreateWindow("Mobil 3D");
    glutIgnoreKeyRepeat(1);
    
    glutSpecialFunc(pressKey);
    glutSpecialUpFunc(releaseKey);
    
    glutKeyboardFunc(keyboard);
    glutKeyboardUpFunc(keyboardUp);
    
    glutMouseFunc(mouse);
    glutMotionFunc(mouseMotion);
    
    glutDisplayFunc(display);
    glutIdleFunc(display);
    glutReshapeFunc(Reshape);
    lighting();
    init();
    
    printf("Controls:\n");
    printf("WASD/Arrow Keys: Move camera\n");
    printf("Space/X: Move camera up/down\n");
    printf("M or Right-click: Toggle car control mode\n");
    printf("Left-click and drag: Rotate camera (default) or move car (in car control mode)\n");
    printf("ESC: Exit\n");
    
    glutMainLoop();
    return(0);
}
