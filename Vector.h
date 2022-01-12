#ifndef VECTOR_H_INCLUDED
#define VECTOR_H_INCLUDED

#define SCREEN_WIDTH 400
#define SCREEN_HEIGHT 400

#define WIDTH 500
#define HEIGHT 700

#define DELTA_WIDTH ((WIDTH) - (SCREEN_WIDTH))
#define DELTA_HEIGHT ((HEIGHT) - (SCREEN_HEIGHT))

typedef struct{

    double module;
    double angle;
    double x;
    double y;

}VECTOR;

#endif // VECTOR_H_INCLUDED
