#define GL_SILENCE_DEPRECATION
#include <GLUT/GLUT.h>
#include <OpenGL/OpenGL.h>
#include <math.h>
#include <iostream>
#include <fstream>
using namespace std;

/**
 Caio Brasil
 Construção de software gráfico 3D
 29/11
 */


GLfloat angleV, fAspect, an, bn, cn, an1;
GLfloat angle = 0.0f; GLfloat angle1 = 0.0f;

GLfloat colorBuffer[7][3] = {{0, 0, 0},
                             {1.0f, 0.0f, 0.0f}, //1 Vermelho
                             {0.0f, 1.0f, 0.0f}, //2 Verde
                             {0.0f, 0.0f, 1.0f}, //3 Azul
                             {1.0f, 0.6f, 0.1f}, //4 Laranja
                             {0.3f, 1.0f, 1.0f}, // 5 Azul Claro
                             {0.0f, 0.0f, 0.0f}};  // cor original do objeto
                                               

GLint lines = (sizeof colorBuffer) - 1;

GLint columns = (sizeof colorBuffer[0]) - 1;

GLint sLines = (lines / columns);

GLint colorCounter[7] = {1, 1, 1, 1, 1, 1};

GLint objectSize [7] = {0, 25, 20, 20, 20, 20, 20};

GLfloat ex1 = 0; GLfloat ex2 = 0; GLfloat ex3 = 0;
GLfloat ey1 = 0; GLfloat ey2 = 0; GLfloat ey3 = 0;
GLfloat ez1 = 0; GLfloat ez2 = 0; GLfloat ez3 = 0;

GLboolean e = false; GLboolean p = false;
GLboolean hide1 = true; GLboolean hide2 = true; GLboolean hide3 = true;
GLboolean hide4 = true; GLboolean hide5 = true; GLboolean hide6 = true;
GLint light1 = 0; GLint light2 = 0;

int numObjects;
int ObjectList[10];
int x[10], y[10], z[10];
float r[10], g[10], b[10];

int selected = 0;
char title[] = "-- Trabalho Completo OpenGL --";
float centerCoordinates [3] = {0, 0, 0};

float originalColorBuffer[7][3];


void DisplayFileRead(const char * fileName)
{
    fstream inStream;
    inStream.open(fileName, ios::in);
    if (inStream.fail())
        return;
    cout << "Abertura do arquivo com sucesso ..." << endl;
    inStream >> numObjects;
    cout << numObjects-1 << " Objetos na cena ..." << endl;
    for (int i = 1; i <= numObjects; i++) {
        inStream >>ObjectList[i]>>x[i]>>y[i]>>z[i]>>r[i]>>g[i]>>b[i];
    }
    inStream.close();
    
    for(int i = 1; i < numObjects; i++){
        originalColorBuffer[i][0] = r[i];
        originalColorBuffer[i][1] = g[i];
        originalColorBuffer[i][2] = b[i];
    }
}

void ReduceToUnit(float vector[3])
{
    float length;
    
    length = (float)sqrt((vector[0] * vector[0]) +
        (vector[1] * vector[1]) +
        (vector[2] * vector[2]));
    
    if (length == 0.0f)
        length = 1.0f;
    
    vector[0] /= length;
    vector[1] /= length;
    vector[2] /= length;
}

void calcNormal(float v[3][3], float out[3])
{
    float v1[3], v2[3];
    static const int x = 0;
    static const int y = 1;
    static const int z = 2;


    v1[x] = v[0][x] - v[1][x];
    v1[y] = v[0][y] - v[1][y];
    v1[z] = v[0][z] - v[1][z];

    v2[x] = v[1][x] - v[2][x];
    v2[y] = v[1][y] - v[2][y];
    v2[z] = v[1][z] - v[2][z];

    out[x] = v1[y] * v2[z] - v1[z] * v2[y];
    out[y] = v1[z] * v2[x] - v1[x] * v2[z];
    out[z] = v1[x] * v2[y] - v1[y] * v2[x];

    ReduceToUnit(out);
}

void cubo(float a) {
    
    float v1[3][3] = {{-a / 2, a / 2, a / 2}, {-a / 2, -a / 2, a / 2}, {a / 2, a / 2, a / 2}}; // face direita
    float v2[3][3] = {{a / 2, -a / 2, -a / 2}, {-a / 2, a / 2, -a / 2}, {-a / 2, -a / 2, -a / 2}}; // face esquerda
    float v3[3][3] = {{-a / 2, a / 2, a / 2}, {-a / 2, a / 2, -a / 2}, {-a / 2, -a / 2, -a / 2}}; // face fundo
    float v4[3][3] = {{a / 2, a / 2, a / 2}, {a / 2, -a / 2, a / 2}, {a / 2, -a / 2, -a / 2}}; // face frontal
    float normal[3];
    
    glBegin(GL_TRIANGLE_STRIP);

    calcNormal(v1, normal);
    glNormal3fv(normal);
    //glColor3f(0.0f, 0.0f, 0.5f);
    glVertex3fv(v1[0]);
    glVertex3fv(v1[1]);
    glVertex3fv(v1[2]);
    glVertex3f(a / 2, -a / 2, a / 2);
    glVertex3f(a / 2, a / 2, -a / 2);
    calcNormal(v2, normal);
    glNormal3fv(normal);
    calcNormal(v3, normal);
    glNormal3fv(normal);
    calcNormal(v4, normal);
    glNormal3fv(normal);
    //glColor3f(0.5f, 0.1f, 0.5f);
    glVertex3fv(v2[0]); //6
    glVertex3fv(v2[1]); //7
    glVertex3fv(v2[2]); //8
    glVertex3f(-a / 2, a / 2, a / 2);
    glVertex3f(-a / 2, -a / 2, a / 2);
    glEnd();
    
    //base superior
    glBegin(GL_TRIANGLE_STRIP);
    float vbs[3][3] = {{-a / 2, a / 2, -a / 2}, {-a / 2, a / 2, a / 2}, {a / 2, a / 2, -a / 2}};
    calcNormal(vbs, normal);
    glNormal3fv(normal);
    //glColor3f(1.0f, 0.6f, 0.1f);
    glVertex3fv(vbs[0]);
    glVertex3fv(vbs[1]);
    glVertex3fv(vbs[2]);
    glVertex3f(a / 2, a / 2, a / 2);
    glEnd();
    
    //base inferior
    glBegin(GL_TRIANGLE_STRIP);
    float vbi[3][3] = {{a / 2, -a / 2, -a / 2}, {a / 2, -a / 2, a / 2}, {-a / 2, -a / 2, -a / 2}};
    calcNormal(vbi, normal);
    glNormal3fv(normal);
    //glColor3f(0.3f, 1.0f, 1.0f);
    glVertex3fv(vbi[0]);
    glVertex3fv(vbi[1]);
    glVertex3fv(vbi[2]);
    glVertex3f(-a / 2, -a / 2, a / 2);
    glEnd();
}
void setVisParam(void)
{
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(angleV, fAspect, 0.1, 500);

    glMatrixMode(GL_MODELVIEW);

    glLoadIdentity();

    gluLookAt(0, 80, 250, 0, 0, 0, 0, 1, 0);
}
// Inicializa parâmetros de rendering
void initGL()
{
    angleV = 45;
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f); // Set background color to black and opaque
    glClearDepth(1.0f);                   // Set background depth to farthest
    glEnable(GL_DEPTH_TEST);   // Enable depth testing for z-culling
    glDepthFunc(GL_LEQUAL);    // Set the type of depth-test
    glShadeModel(GL_SMOOTH);
    glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);  // Nice perspective corrections
    
    GLfloat luzAmbiente[4] = {0.7, 0.2, 0.2, 1.0};
    GLfloat luzDifusa[4] = {0.7, 0.7, 0.7, 1.0};       // "cor"
    GLfloat luzEspecular[4] = { 1.0, 1.0, 1.0, 1.0 };  // "brilho"
    GLfloat posicaoLuz[4] = { 0.0, 50.0, 50.0, 1.0 };
    
    // Capacidade de brilho do material
    GLfloat especularidade[4] = { 1.0,1.0,1.0,1.0 };
    GLint especMaterial = 60;
    
    // Especifica que a cor de fundo da janela ser· preta
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    

    
    // Define a refletância do material
    glMaterialfv(GL_FRONT, GL_SPECULAR, especularidade);
    
    // Define a concentração do brilho
    glMateriali(GL_FRONT, GL_SHININESS, especMaterial);
    
    // Ativa o uso da luz ambiente
    glLightModelfv(GL_LIGHT_MODEL_AMBIENT, luzAmbiente);
    
    // Define os par‚metros da luz de número 0
    glLightfv(GL_LIGHT0, GL_AMBIENT, luzAmbiente);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, luzDifusa);
    glLightfv(GL_LIGHT0, GL_SPECULAR, luzEspecular);
    glLightfv(GL_LIGHT0, GL_POSITION, posicaoLuz);
    
    GLfloat luzDifusa2[4] = {1.0f, 1.0f, 0.3f, 1.0f};
    GLfloat luzEspecular2[4] = {1.0, 1.0, 1.0, 1.0};
    GLfloat posicaoLuz2[4] = {0.0, 50.0, -50.0, 1.0};
    
    glLightfv(GL_LIGHT1, GL_AMBIENT, luzAmbiente);
    glLightfv(GL_LIGHT1, GL_DIFFUSE, luzDifusa2);
    glLightfv(GL_LIGHT1, GL_SPECULAR, luzEspecular2);
    glLightfv(GL_LIGHT1, GL_POSITION, posicaoLuz2);
    
    // Habilita a definição da cor do material a partir da cor corrente
    glEnable(GL_COLOR_MATERIAL);
    
    //Habilita o uso de iluminação
    glEnable(GL_LIGHTING);
    
    // Habilita a luz de número 0
    switch(light1){
        case 0:
            glDisable(GL_LIGHT0);
            break;
    
        case 1:
            glEnable(GL_LIGHT0);
            break;
    }
    switch(light2){
        case 0:
            glDisable(GL_LIGHT1);
            break;
        case 1:
            glEnable(GL_LIGHT1);
            break;
    }
}

// Função usada para especificar o volume de visualização

// Função callback chamada quando o tamanho da janela é alterado
void reshape(GLsizei w, GLsizei h)
{
    if (h == 0) h = 1;
    glViewport(0, 0, w, h);
    fAspect = (GLfloat)w / (GLfloat)h;
    setVisParam();
}

// Função callback chamada para gerenciar eventos do mouse
void mouse(int button, int state, int x, int y)
{
    if (button == GLUT_LEFT_BUTTON)
        if (state == GLUT_DOWN) {  // Zoom-in
            if (angleV >= 10) angleV -= 5;
        }
    if (button == GLUT_RIGHT_BUTTON)
        if (state == GLUT_DOWN) {  // Zoom-out
            if (angleV <= 130) angleV += 5;
        }
    setVisParam();
    glutPostRedisplay();  // redraw
}

void render(void)
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // limpa a tela e o buffer de profundidade
    glPushMatrix();
        glRotatef(angle, 0.0f, 1.0f, 0.0f);
        glRotatef(angle1, 1.0f, 0.0f, 0.0f);

            for (int i = 1; i < numObjects; i++) {  // percorre todo o display-file
                switch (ObjectList[i]) {
                case 1: // cubo
                        if(hide1){
                            glPushMatrix();
                            glColor3f(r[i], g[i], b[i]);
                            glRotatef(an, 1.0f, 0.0f, 0.0f);
                            glRotatef(an1, 0.0f, 1.0f, 0.0f);
                            glTranslatef(x[i], y[i], z[i]);
                            cubo(objectSize[i]);
                            glPopMatrix();
                        }
                    break;
                case 2: // esfera
                        if(hide2){
                            glPushMatrix();
                            glColor3f(r[i], g[i], b[i]);
                            glRotatef(bn, ex2, ey2, ez2);
                            glTranslatef(x[i], y[i], z[i]);
                            glutSolidSphere(objectSize[i], 24, 24);
                            glPopMatrix();
                        }
                    break;
                case 3: // cone
                        if(hide3){
                            glPushMatrix();
                            glColor3f(r[i], g[i], b[i]);
                            glRotatef(cn, ex3, ey3, ez3);
                            glTranslatef(x[i], y[i], z[i]);
                            glutSolidCone(objectSize[i], 20, 24, 24);
                            glPopMatrix();
                        }
                    break;
                case 4:
                        if(hide4){
                            glPushMatrix();
                            glColor3f(r[i], g[i], b[i]);
                            glTranslatef(x[i], y[i], z[i]);
                            glutSolidCube(objectSize[i]);
                            glPopMatrix();
                        }
                        break;
                case 5:
                        if(hide5){
                            glPushMatrix();
                            glColor3f(r[i], g[i], b[i]);
                            glTranslatef(x[i], y[i], z[i]);
                            glutSolidSphere(objectSize[i], 24, 24);
                            glPopMatrix();
                        }
                        break;
                case 6:
                        if(hide6){
                            glPushMatrix();
                            glColor3f(r[i], g[i], b[i]);
                            glTranslatef(x[i], y[i], z[i]);
                            glutSolidCone(objectSize[i], 20, 24, 24);
                            glPopMatrix();
                        }
                        break;
                }
            }
        
    glPopMatrix();
    glutSwapBuffers();
}

void hideObject(){
    if(selected == 1) hide1 = !hide1;
    if(selected == 2) hide2 = !hide2;
    if(selected == 3) hide3 = !hide3;
    if(selected == 4) hide4 = !hide4;
    if(selected == 5) hide5 = !hide5;
    if(selected == 6) hide6 = !hide6;
}

// Keyboard ...
void specialKeyHandler(int key, int xx, int yy) {
    switch (key) {;
    case GLUT_KEY_LEFT:
            if (selected == 1) x[1] -= 2;
            if (selected == 2) x[2] -= 2;
            if (selected == 3) x[3] -= 2;
            if (selected == 4) x[4] -= 2;
            if (selected == 5) x[5] -= 2;
            if (selected == 6) x[6] -= 2;
        break;
    case GLUT_KEY_RIGHT:
            if (selected == 1) x[1] += 2;
            if (selected == 2) x[2] += 2;
            if (selected == 3) x[3] += 2;
            if (selected == 4) x[4] += 2;
            if (selected == 5) x[5] += 2;
            if (selected == 6) x[6] += 2;
            
        break;
    case GLUT_KEY_UP:
            if (selected == 1) y[1]+=2;
            if (selected == 2) y[2]+=2;
            if (selected == 3) y[3]+=2;
            if (selected == 4) y[4]+=2;
            if (selected == 5) y[5]+=2;
            if (selected == 6) y[6]+=2;
            
        break;
    case GLUT_KEY_DOWN:
            if (selected == 1) y[1] -= 2;
            if (selected == 2) y[2] -= 2;
            if (selected == 3) y[3] -= 2;
            if (selected == 4) y[4] -= 2;
            if (selected == 5) y[5] -= 2;
            if (selected == 6) y[6] -= 2;

        break;
    }
    
    glutPostRedisplay();
}

void changeObjectColor(int pos){
        if(colorCounter[pos] == 1){
            colorBuffer[6][0] = originalColorBuffer[pos][0];
            colorBuffer[6][1] = originalColorBuffer[pos][1];
            colorBuffer[6][2] = originalColorBuffer[pos][2];
        }
        r[pos] = colorBuffer[colorCounter[pos]][0];
        g[pos] = colorBuffer[colorCounter[pos]][1];
        b[pos] = colorBuffer[colorCounter[pos]][2];
    
        if(colorCounter[pos] < (sLines - 1)){
            colorCounter[pos]++;

        }
        else
            colorCounter[pos] = 1;
    }

void increaseObjectSize(int size){
    if(objectSize[size] < 70){
        objectSize[size] += 5;
    }
}

void decreaseObjectSize(int size){
    if(objectSize[size] > 5){
        objectSize[size] -= 5;
    }
}

void normalKeyHandler(unsigned char key, int xx, int yy){
    switch(key){
        case 'a':
            angle+=2;
            break;
        case 'd':
            angle-=2;
            break;
        case 'w':
            angle1--;
            break;
        case 's':
            angle1++;
            break;
            
        case 'h': //hide object
            hideObject();
            break;
            
        case 'k': //color
            if(selected == 1) changeObjectColor(1);
            if(selected == 2) changeObjectColor(2);
            if(selected == 3) changeObjectColor(3);
            if(selected == 4) changeObjectColor(4);
            if(selected == 5) changeObjectColor(5);
            if(selected == 6) changeObjectColor(6);
            break;
            //select figures
            
        case 'm':
            if(selected == 1) increaseObjectSize(1);
            if(selected == 2) increaseObjectSize(2);
            if(selected == 3) increaseObjectSize(3);
            if(selected == 4) increaseObjectSize(4);
            if(selected == 5) increaseObjectSize(5);
            if(selected == 6) increaseObjectSize(6);
            break;
        case 'n':
            if(selected == 1) decreaseObjectSize(1);
            if(selected == 2) decreaseObjectSize(2);
            if(selected == 3) decreaseObjectSize(3);
            if(selected == 4) decreaseObjectSize(4);
            if(selected == 5) decreaseObjectSize(5);
            if(selected == 6) decreaseObjectSize(6);
            break;
        case '1':
            selected = 1;
            cout << "CUBO Selecionado" << endl;
            break;
        case '2':
            selected = 2;
            cout << "ESFERA Selecionada" << endl;
            break;
        case '3':
            selected = 3;
            cout << "CONE Selecionado" << endl;
            break;
        case '4':
            selected = 4;
            cout << "CUBO 2 Selecionado" << endl;
            break;
        case '5':
            selected = 5;
            cout << "ESFERA 2 Selecionado" << endl;
            break;
        case '6':
            selected = 6;
            cout << "CONE 2 Selecionado" << endl;
            break;
        case '9': //turn on/off light 1
            if(light1 == 0){
                light1 = 1;
                initGL();
                cout << "Luz 1 acesa" << endl;
            }
            else{
                light1 = 0;
                initGL();
                cout << "Luz 1 apagada" << endl;
            }
            break;
        case '0': // turn on/off light2
            if(light2 == 0){
                light2 = 1;
                initGL();
                cout << "Luz 2 acesa" << endl;
            }
            else{
                light2 = 0;
                initGL();
                cout << "Luz 2 apagada" << endl;
            }
            break;
    }
    glutPostRedisplay();
}


int main(int argc, char** argv) {
    DisplayFileRead("/Users/Caio/GitHub Files/Project OpenGL/Project OpenGL/Objects.txt");
    cout << "9 - Desligar luz 1" << endl;
    cout << "0 - Desligar luz 2" << endl;
    cout << "Setas para mover o objeto selecionado" << endl;
    cout << "A D para navegar dentro da cena" << endl;
    cout << "W S para rotacionar os objetos" << endl;
    cout << "1-6 Seleciona objetos" << endl;
    cout << "H - Esconder objeto selecionado" << endl;
    cout << "K - Trocar cor do objeto selecionado" << endl;
    cout << "M para aumentar o tamanho do objeto" << endl;
    cout << "N para diminuir o tamanho do objeto" << endl;
    cout << "\n" << endl;
    glutInit(&argc, argv);                    // Initialize GLUT
    glutInitDisplayMode(GLUT_DOUBLE);        // Enable double buffered mode
    glutInitWindowSize(640, 480);            // Set the window's initial width & height
    glutInitWindowPosition(50, 50);            // Position the window's initial top-left corner
    glutCreateWindow(title);

    glutDisplayFunc(render);                // Regster callback for render function
    glutSpecialFunc(specialKeyHandler);        // Register callback handler for arrow keys (keyborad)
    glutKeyboardFunc(normalKeyHandler);
    glutReshapeFunc(reshape);                // Register callback for window resize event
    glutMouseFunc(mouse);                    // Register callback for mouse event
    initGL();                                // Initialization function
    glutMainLoop();                            // event loop
    return 0;
}
