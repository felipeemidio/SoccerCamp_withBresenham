#ifndef OPENGLWIDGET_H
#define OPENGLWIDGET_H

#include <iostream>
#include <cmath>
#include <QtOpenGL/QGL>
#include <QOpenGLWidget>
#include <QTimer>
#include <QMouseEvent>


class OpenGLWidget: public QOpenGLWidget
{
    Q_OBJECT

    //CONSTANTES GLOBAIS
    const float color[5][3] = {{1.0, 1.0, 1.0}, //branco
                               {1.0, 0.0, 0.0}, //vermelho
                               {0.0, 1.0, 0.0}, //verde
                               {0.0, 0.0, 1.0}, //azul
                               {1.0, 1.0, 0.0} }; // amarelo

    int MouseAux = 0;
    QPoint MousePoint;
    QVector<QRect> rects;

public:
    OpenGLWidget(QWidget *parent = NULL) : QOpenGLWidget(parent)
    {}

    /*
     * Desenha um Ponto
     */
    void drawPoint(float x, float y) {
        //Desenhando um Ponto
        glBegin(GL_POINTS);
            glVertex2i(x , y);
        glEnd();
    }

    /*
     * Desenha um reta com a equação da reta.
     */
    void lineEquation(int startx, int starty, int endx, int endy) {
        int t, distance;
        int xerr=0, yerr=0, delta_x, delta_y;
        int incx, incy;

        /* compute the distances in both directions */
        delta_x=endx-startx;
        delta_y=endy-starty;

        /* Compute the direction of the increment,
           an increment of 0 means either a horizontal or vertical
           line.
        */
        if(delta_x>0) incx=1;
        else if(delta_x==0) incx=0;
        else incx=-1;

        if(delta_y>0) incy=1;
        else if(delta_y==0) incy=0;
        else incy=-1;

        /* determine which distance is greater */
        delta_x=abs(delta_x);
        delta_y=abs(delta_y);
        if(delta_x>delta_y) distance=delta_x;
        else distance=delta_y;

        /* draw the line */
        for(t=0; t<=distance+1; t++) {
            drawPoint(startx, starty);

            xerr+=delta_x;
            yerr+=delta_y;
            if(xerr>distance) {
                xerr-=distance;
                startx+=incx;
            }
            if(yerr>distance) {
                yerr-=distance;
                starty+=incy;
            }
        }
    }

    /*
     * Função que usa a Equação do circulo para desenhar uma circunferencia
     */
    void circleEquation(int cx, int cy, int r, int octant) {
        float thetaMin, thetaMax, x, y;

        switch(octant){
            case 1:
                thetaMin = 0;
                thetaMax = 90;
                break;
            case 2:
                thetaMin = 90;
                thetaMax = 180;
                break;
            case 3:
                thetaMin = 180;
                thetaMax = 270;
                break;
            case 4:
                thetaMin = 270;
                thetaMax = 360;
                break;
            default:
                thetaMin = 0;
                thetaMax = 360;
        }

        float theta, rad;
        const float  PI = 3.14159265358979f;
        for(theta = thetaMin; theta < thetaMax; ++theta){
            rad = (theta* PI) / 180;
            x = cx + r*cos(rad);
            y = cy + r*sin(rad);
            drawPoint(x, y);
        }
    }

    /*
     * Algoritmo de Bresenham (ou Algoritmo do ponto médio) para desenhar uma reta.
     */
    void bresenhamLine(int x1, int y1, int x2, int y2){

        int dx, dy, incE, incNE, d, x, y;
        int slope;

        if (x1 == x2){
            x = x1;
            if (y1 == y2){
                drawPoint(x1, y1);
            }
            else if( y1 <= y2 ) {
                for(y = y1; y < y2; ++y){
                    drawPoint(x, y);
                }
            }
            else {
                for(y = y2; y < y1; ++y){
                    drawPoint(x, y);
                }
            }
            return;
        }
        // Onde inverte a linha x1 > x2
        if (x1 > x2){
            bresenhamLine(x2, y2, x1, y1);
            return;
        }
        //a diferença entre os pontos.
        dx = x2 - x1;
        dy = y2 - y1;

        //se dy > 0 o ponto vai pra NE, se não vai pra E
        if (dy < 0){
            slope = -1;
            dy = -dy;
        }
        else{
            slope = 1;
        }
        // Constante de Bresenham
        incE = 2 * dy;
        incNE = 2 * dy - 2 * dx;
        d = 2 * dy - dx;
        y = y1;

        for (x = x1; x <= x2; x++){
            //printf("%d %d\n", x1, x2);
            drawPoint(x, y);
            if (d <= 0) {
                d += incE;
            } else {
                d += incNE;
                y += slope;
            }
        }
    }

    /*
     * Algoritmo de Bresenham (ou Algoritmo do ponto médio) para desenhar uma circulo.
     */
    void bresenhamCircle(int x0, int y0, int radius, int octant)
    {
      int x = radius;
      int y = 0;
      int decisionOver2 = 1 - x;   // Decision criterion divided by 2 evaluated at x=r, y=0

      while( y <= x )
      {
        switch(octant){
            case 1:
                drawPoint( x + x0,  y + y0); // Octant 1
                drawPoint( y + x0,  x + y0); // Octant 2
                break;
            case 2:
                drawPoint(-x + x0,  y + y0); // Octant 4
                drawPoint(-y + x0,  x + y0); // Octant 3
                break;
            case 3:
                drawPoint(-x + x0, -y + y0); // Octant 5
                drawPoint(-y + x0, -x + y0); // Octant 6
                break;
            case 4:
                drawPoint( x + x0, -y + y0); // Octant 7
                drawPoint( y + x0, -x + y0); // Octant 8
                break;
            default:
                drawPoint( x + x0,  y + y0); // Octant 1
                drawPoint( y + x0,  x + y0); // Octant 2
                drawPoint(-x + x0,  y + y0); // Octant 4
                drawPoint(-y + x0,  x + y0); // Octant 3
                drawPoint(-x + x0, -y + y0); // Octant 5
                drawPoint(-y + x0, -x + y0); // Octant 6
                drawPoint( x + x0, -y + y0); // Octant 7
                drawPoint( y + x0, -x + y0); // Octant 8
        }
        y++;
        if (decisionOver2<=0)
        {
          decisionOver2 += 2 * y + 1;   // Change in decision criterion for y -> y+1
        }
        else
        {
          x--;
          decisionOver2 += 2 * (y - x) + 1;   // Change for y -> y+1, x -> x-1
        }
      }
    }

    /*
     * Funçao que desenha um quadrado de acordo com os cliques na tela
     */
    void mouseReleaseEvent(QMouseEvent *event) override
    {
        int x = event->x(), y = event->y();
        if (event->button() != Qt::LeftButton) {
            return;
        }

        if(MouseAux%2 == 0) {
            MousePoint = QPoint(x, 500-y);
        } else {
            QRect r(MousePoint, event->pos());
            rects.push_back(r);
        }
        ++MouseAux;
        update();
    }

    /*
     * Desenha um campo de futebol usando 2 algoritimos diferentes
     */
    void drawCamp() {
        // Especifica que a cor do campo é verde.
        glColor3f(0.0f, 0.8f, 0.0f);
        // Desenha o Quadrado do campo
        glBegin(GL_QUADS);
            glVertex2i(100, 400);
            glVertex2i(100, 100);
            glVertex2i(600, 100);
            glVertex2i(600, 400);
        glEnd();

        //Definindo o tamanho do ponto
        glPointSize(lineSize);
        //Seta a cor do contorno do campo como branco
        glColor3f(color[colorChosed][0], color[colorChosed][1], color[colorChosed][2]);

        if (algorithm){
            //Contorno
            bresenhamLine(100, 400, 100, 100);
            bresenhamLine(100, 100, 600, 100);
            bresenhamLine(600, 100, 600, 400);
            bresenhamLine(600, 400, 100, 400);
            //Linha do Meio
            bresenhamLine(350, 100, 350, 400);
            //Trave A
            bresenhamLine(100, 330, 200, 330);
            bresenhamLine(100, 170, 200, 170);
            bresenhamLine(200, 330, 200, 170);
            //Trave B
            bresenhamLine(500, 330, 600, 330);
            bresenhamLine(500, 170, 600, 170);
            bresenhamLine(500, 330, 500, 170);

            //desenha os pontos do penalti
            bresenhamCircle( 150, 250, 2, 0);
            bresenhamCircle( 550, 250, 2, 0);
            //Desenha o circulos do meio
            bresenhamCircle( 350, 250, 5, 0);
            bresenhamCircle( 350, 250, 50, 0);
            //Desenha o semi-circulo da trave A
            bresenhamCircle( 200, 250, 30, 4);
            bresenhamCircle( 200, 250, 30, 1);
            //Desenha o semi-circulo da trave B
            bresenhamCircle( 500, 250, 30, 2);
            bresenhamCircle( 500, 250, 30, 3);
            //desenha 1/4 do circulo no canto do campo
            bresenhamCircle( 100, 400, 20, 4);
            bresenhamCircle( 100, 100, 20, 1);
            bresenhamCircle( 600, 100, 20, 2);
            bresenhamCircle( 600, 400, 20, 3);
        }
        else {
            //Contorno
            lineEquation(100, 400, 100, 100);
            lineEquation(100, 100, 600, 100);
            lineEquation(600, 100, 600, 400);
            lineEquation(600, 400, 100, 400);
            //Linha do Meio
            lineEquation(350, 100, 350, 400);
            //Trave A
            lineEquation(100, 330, 200, 330);
            lineEquation(100, 170, 200, 170);
            lineEquation(200, 330, 200, 170);
            //Trave B
            lineEquation(500, 330, 600, 330);
            lineEquation(500, 170, 600, 170);
            lineEquation(500, 330, 500, 170);

            //desenha os pontos do penalti
            circleEquation( 150, 250, 2, 0);
            circleEquation( 550, 250, 2, 0);
            //Desenha o circulos do meio
            circleEquation( 350, 250, 5, 0);
            circleEquation( 350, 250, 50, 0);
            //Desenha o semi-circulo da trave A
            circleEquation( 200, 250, 30, 4);
            circleEquation( 200, 250, 30, 1);
            //Desenha o semi-circulo da trave B
            circleEquation( 500, 250, 30, 2);
            circleEquation( 500, 250, 30, 3);
            //desenha 1/4 do circulo no canto do campo
            circleEquation( 100, 400, 20, 4);
            circleEquation( 100, 100, 20, 1);
            circleEquation( 600, 100, 20, 2);
            circleEquation( 600, 400, 20, 3);
        }

    }

    void initializeGL() override {
        //Seta todos os pixels como preto
        glClearColor (0.0, 0.0, 0.0, 0.0);
        //Defini até onde se pode desenhar, os parametros sao os max e min de cada coordenada.
        glOrtho (0.0, width(), 0.0, height(), -1.0, 1.0);
        //Faz a projeção de acordo com as coordenadas do OpenGL (é diferente dos pixel da tela do PC)
        glMatrixMode (GL_PROJECTION);
    }

    /*
     * Função de renderização.
     */
    void paintGL() override {
        drawCamp();

        glColor3f(color[colorChosed][0], color[colorChosed][1], color[colorChosed][2]);
        for (const auto &r: rects) {
            bresenhamLine(r.left(), r.top(), r.right(), r.top() );
            bresenhamLine(r.right(), r.top(), r.right(), 500-r.bottom());
            bresenhamLine(r.left(), r.top(), r.left(), 500-r.bottom());
            bresenhamLine(r.left(), 500-r.bottom(), r.right(), 500-r.bottom());
        }

        if(MouseAux%2 != 0) {
            drawPoint(MousePoint.x(), MousePoint.y());
        }

        glFlush();
    }

    int colorChosed = 0;
    float lineSize = 3.0;
    bool algorithm = true;
};

#endif // OPENGLWIDGET_H
