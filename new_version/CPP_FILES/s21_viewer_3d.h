#ifndef S21_VIEWER_3D_H
#define S21_VIEWER_3D_H

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define EPS 1E-6

typedef struct point {
    double x;
    double y;
    double z;
} point_t;

typedef struct vertex_list {
    point_t *coordinates;
    int amount_points;
    double minMax_x[2];
    double minMax_y[2];
    double minMax_z[2];
} vertex_t;

typedef struct polygon {
    int *vertex;    // номера вершин, которые надо соединить
    int counter_of_vertexes;
} polygon_t;

typedef struct obj_data {
    int facets;     // количество полигонов
    vertex_t object_3d;     // для вершин x y z (v)
    polygon_t *polygons;    // полигоны (f)
} data_t;

data_t* obj_point_output(char* filename, int* error);
data_t* point_parsing(FILE* file, int* error);
void pars_vertex_and_polygon_line(FILE* file, data_t *obj, char c, int *sizePoint, int *sizePolygons, int *flagMinMaxFirst);
int reallocPolygons(data_t *obj, int* MallocPolygons);
int reallocPoint(data_t *obj, int* MallocPoint);
int reallocBuf(char **buf, int* MallocBuf);
void pars_xyz(data_t *obj, int xyz, int *sizePoint, char *buf);
int pars_vertex(data_t *obj, char *buf, int *MallocVertex, int *sizeVertex, int *sizePolygons);
void pars_minMax_points(data_t *obj, int *sizePoint, int xyz, int *flagMinMaxFirst);
void free_data(data_t *obj);
void center(data_t *obj);
double max3(double n1, double n2, double n3);
void adjust(data_t *obj, double value);
void scale(data_t *obj, double scale);

void rotateX(data_t* obj, double num);
void rotateY(data_t* obj, double num);
void rotateZ(data_t* obj, double num);

void moveObj_x(data_t* obj, double num);
void moveObj_y(data_t* obj, double num);
void moveObj_z(data_t* obj, double num);

//void moveObj_x(data_t* obj, double num);
//void moveObj_y(data_t* obj, double num);
//void moveObj_z(data_t* obj, double num);



#endif // S21_VIEWER_3D_H
