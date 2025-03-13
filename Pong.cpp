#include <GL/glut.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define PI 3.1415926535898 

// Variables de la pelota
double posicion_pelota_x = 80, posicion_pelota_y = 60;
double direccion_pelota_x = 1.5, direccion_pelota_y = 1.5; // 🔹 Velocidad fija
const float velocidad_pelota = 1.0;  // 🔹 Se mantiene constante
const float radio_pelota = 5.0;

// Variables de las paletas
float posicion_paleta1_y = 50, posicion_paleta2_y = 50;
const float alto_paleta = 20, ancho_paleta = 5;
const float velocidad_paleta = 10.0;

// Puntajes
int puntaje_jugador1 = 0, puntaje_jugador2 = 0;

// Variables para el color de la pelota
float color_pelota_r = 1.0, color_pelota_g = 1.0, color_pelota_b = 1.0;

// Inicializar la semilla aleatoria
void iniciar_juego() {
    glClearColor(0.0, 0.0, 0.0, 1.0);
    srand(time(NULL));
}

// Cambiar el color de la pelota cuando alguien anota
void cambiar_color_pelota() {
    color_pelota_r = (rand() % 256) / 255.0;
    color_pelota_g = (rand() % 256) / 255.0;
    color_pelota_b = (rand() % 256) / 255.0;
}

// Dibujar la pelota
void dibujar_pelota() {
    glColor3f(color_pelota_r, color_pelota_g, color_pelota_b);
    glBegin(GL_POLYGON);
    for (int i = 0; i < 100; i++) {
        double angulo = 2 * PI * i / 100;
        glVertex2f(posicion_pelota_x + radio_pelota * cos(angulo), 
                   posicion_pelota_y + radio_pelota * sin(angulo));
    }
    glEnd();
}

// Dibujar las paletas
void dibujar_paletas() {
    glColor3f(1.0, 1.0, 1.0);
    glRectf(10, posicion_paleta1_y, 15, posicion_paleta1_y + alto_paleta);
    glRectf(145, posicion_paleta2_y, 150, posicion_paleta2_y + alto_paleta);
}

// Mostrar el puntaje en pantalla
void mostrar_puntaje() {
    char texto[20];
    glColor3f(1.0, 1.0, 1.0);
    
    sprintf(texto, "Jugador 1: %d", puntaje_jugador1);
    glRasterPos2f(50, 110);
    for (int i = 0; texto[i] != '\0'; i++) {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, texto[i]);
    }
    
    sprintf(texto, "Jugador 2: %d", puntaje_jugador2);
    glRasterPos2f(90, 110);
    for (int i = 0; texto[i] != '\0'; i++) {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, texto[i]);
    }
}

// Manejo del teclado (paletas)
void manejar_teclas_especiales(int tecla, int x, int y) {
    if (tecla == GLUT_KEY_UP && posicion_paleta2_y + alto_paleta < 120)
        posicion_paleta2_y += velocidad_paleta;
    if (tecla == GLUT_KEY_DOWN && posicion_paleta2_y > 0)
        posicion_paleta2_y -= velocidad_paleta;
    glutPostRedisplay();
}

void manejar_teclas(unsigned char tecla, int x, int y) {
    if (tecla == 'w' && posicion_paleta1_y + alto_paleta < 120)
        posicion_paleta1_y += velocidad_paleta;
    if (tecla == 's' && posicion_paleta1_y > 0)
        posicion_paleta1_y -= velocidad_paleta;
    glutPostRedisplay();
}

// Detectar colisión de la pelota con las paletas
void detectar_colision() {
    if (posicion_pelota_x - radio_pelota <= 15 && 
        posicion_pelota_y >= posicion_paleta1_y && 
        posicion_pelota_y <= posicion_paleta1_y + alto_paleta) {
        direccion_pelota_x = -direccion_pelota_x;
    }

    if (posicion_pelota_x + radio_pelota >= 145 && 
        posicion_pelota_y >= posicion_paleta2_y && 
        posicion_pelota_y <= posicion_paleta2_y + alto_paleta) {
        direccion_pelota_x = -direccion_pelota_x;
    }
}

// Reiniciar la pelota tras un punto
void reiniciar_pelota() {
    posicion_pelota_x = 80;  // 🔹 Asegurar que siempre inicie en el centro
    posicion_pelota_y = 60;  
    direccion_pelota_x = (rand() % 2 == 0) ? velocidad_pelota : -velocidad_pelota;
    direccion_pelota_y = (rand() % 2 == 0) ? velocidad_pelota : -velocidad_pelota;
    cambiar_color_pelota();
}

// Actualizar la lógica del juego
void actualizar(int valor) {
    posicion_pelota_x += direccion_pelota_x;
    posicion_pelota_y += direccion_pelota_y;

    detectar_colision();

    if (posicion_pelota_y + radio_pelota >= 120 || posicion_pelota_y - radio_pelota <= 0) {
        direccion_pelota_y = -direccion_pelota_y;
    }

    if (posicion_pelota_x - radio_pelota <= 0) {
        puntaje_jugador2++;
        reiniciar_pelota();
    }

    if (posicion_pelota_x + radio_pelota >= 160) {
        puntaje_jugador1++;
        reiniciar_pelota();
    }

    glutPostRedisplay();
    glutTimerFunc(16, actualizar, 0);
}

// Renderizar la escena
void mostrar_escena() {
    glClear(GL_COLOR_BUFFER_BIT);
    dibujar_paletas();
    dibujar_pelota();
    mostrar_puntaje();
    glutSwapBuffers();
}

// Configurar la ventana
void cambiar_tamano_ventana(int ancho, int alto) {
    glViewport(0, 0, ancho, alto);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0, 160, 0, 120);
    glMatrixMode(GL_MODELVIEW);
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(840, 680);
    glutCreateWindow("Juego Pong");

    iniciar_juego();
    glutDisplayFunc(mostrar_escena);
    glutReshapeFunc(cambiar_tamano_ventana);
    glutTimerFunc(16, actualizar, 0);
    glutSpecialFunc(manejar_teclas_especiales);
    glutKeyboardFunc(manejar_teclas);
    glutMainLoop();
    return 0;
}
