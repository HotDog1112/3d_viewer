#ifndef S21_VIEWER_3D_H
#define S21_VIEWER_3D_H

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define OK 0
#define ERROR 1

typedef struct Matrix {
    double** coordinates;
    int rows;  // индексы вершин, номер строки
    int columns;
} vertex_t;

typedef struct facets {
    int *vertex;    // номера вершин, которые надо соединить
    int counter_of_vertexes;
} polygon_t;

typedef struct obj_data {
    int vertexes;   // количество вершин
    int facets;     // количество полигонов
    vertex_t object_3d;     // матрица для вершин x y z (v)
    polygon_t *polygons;    // полигоны (f)
} data_t;


//__________create_object____________//

int parsePolygon(char* buffer, polygon_t *pol, int count_or_make, int index);
int firstReaderCounter(FILE *file, char *str, int* vert, int* face);
int makeMatrix(int rows, int columns, vertex_t *result);
void createVertexStructure(data_t *obj);
int fillStructure(data_t *obj, FILE *file);
void cleanVertexMatrix(vertex_t *A);
void cleanPolStruct(polygon_t *pol, int count);

//__________affinity_transformation____________//

void moveObj_x(data_t* obj, double num);
void moveObj_y(data_t* obj, double num);
void moveObj_z(data_t* obj, double num);
void scaleObj(data_t* obj, double num);

//__________extra_functions____________//

void printer(data_t* obj);

#endif // S21_VIEWER_3D_H
