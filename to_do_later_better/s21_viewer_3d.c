#include "s21_viewer_3d.h"

/**
 * @brief Function to count number of vertexes and polygons for further memory allocation
 *
 * @param file File to read, .obj
 * @param str One string we read from file
 * @param vert Number of vertexes
 * @param face Number of facets
 */

int firstReaderCounter(FILE *file, char *str, int* vert, int* face) {
    int res = OK;
    int v_count = 0, f_count = 0;
    file = fopen("a.obj", "r");
    if (file != NULL) {
        while (!feof(file)) {
            str = calloc(100, sizeof(char));
            fgets(str, 100, file);
            if (str == NULL) {
                break;
            } else {
                if (strchr(str, 'v') != NULL) {
                    v_count++;
                } else if (strchr(str, 'f') != NULL) {
                    f_count++;
                }
            }
            free(str);
        }
        fclose(file);
    } else {
        res = ERROR;
    }
    *vert = v_count;
    *face = f_count;
    if (v_count == 0 && f_count == 0) {
        res = ERROR;
    }
    return res;
}



/**
 * @brief Function to count number of vertexes and polygons for further memory allocation
 *
 * @param result Allocated memory's matrix
 * @param rows Number of vertexes
 * @param columns Number of columns (always 3)
 */


int makeMatrix(int rows, int columns, vertex_t *result) {
  int res = ERROR;
  if (columns > 0 && rows > 0) {
    res = OK;
    result->rows = rows;
    result->columns = columns;
    result->coordinates = calloc(result->rows, sizeof(double *));
    if (result->coordinates == NULL) {
      res = ERROR;
    } else {
      int i = 0;
      while (i < result->rows) {
        result->coordinates[i] = calloc(result->columns, sizeof(double));
        if (result->coordinates[i] == NULL) {
          res = ERROR;
          free(result->coordinates[i]);
          break;
        }
        i++;
      }
    }
  }
  return res;
}



/**
 * @brief Function cleaning allocated memory
 *
 * @param pol Array of polygons
 * @param count Number of polygons
 */

void createVertexStructure(data_t *obj) {
    obj->polygons = calloc(obj->facets, sizeof(polygon_t));
    obj->polygons->vertex = calloc(obj->polygons->counter_of_vertexes - 1, sizeof(int));
}



/**
 * @brief Function cleaning allocated memory
 *
 * @param pol Array of polygons
 * @param count Number of polygons
 */

void cleanPolStruct(polygon_t *pol, int count) {
    int i = 0;
    while (i != count) {
        free(pol[i].vertex);
        i++;
    }
}



/**
 * @brief Function making structures
 *
 * @param obj Structure to fill
 * @param file File .obj
 */

int fillStructure(data_t *obj, FILE *file) {           // возвращает  j чтобы знать сколько чистить
    int i = 0, j = 0;       //!!! i
    double x = 0, y = 0, z = 0;     // координаты
    file = fopen("a.obj", "r");
    while(!feof(file)) {
        char *str = calloc(100, sizeof(char));
        fgets(str, 100, file);
        if (sscanf(str, "v %lf %lf %lf", &x, &y, &z) == 3) {
            obj->object_3d.coordinates[i][0] = x;       // i начинается с 1, память выделена на 1 больше чтобы счет начинался с 1
            obj->object_3d.coordinates[i][1] = y;
            obj->object_3d.coordinates[i][2] = z;
            i++;
        } else if (strchr(str, 'f') != NULL) {
            int p = parsePolygon(str, obj->polygons, 0, 0);        // считаем сколько вершин
            if (p < 3) {          // если вершин меньше 3, то стоп, по заданию от 3 до бесконечности
                free(str);
                break;
            } else {
                obj->polygons[j].counter_of_vertexes = p;   // сколько вершин соединяет полигон
                obj->polygons[j].vertex = calloc(obj->polygons->counter_of_vertexes, sizeof(int));
                parsePolygon(str, obj->polygons, 1, j);
                j++;
            }
        } else if (*str != '\n') {
            free(str);
            break;
        }
        free(str);
    }
    fclose(file);
    return j;
}



/**
 * @brief Two ways of using - to count polygons (0) or make strucure of polygon (1)
 *
 * @param buffer String of file we read
 * @param pol Array of polygons
 * @param count_or_make Flag-value, implemented by 0 or 1
 * @param index Current index of polygon
 */

int parsePolygon(char* buffer, polygon_t *pol, int count_or_make, int index) {     // 0) для подсчета, 1) для заполнения структуры полигонов
    int i = 0;
    char tmp[2] = "\0";
    int flag = 1;
    if (count_or_make == 0) {               // считаем полигоны
        while (*buffer != '\0') {
        if(isdigit(*buffer)) {
            flag = 0;
        } else if (*buffer == '/') {
            i++;
            while (*buffer != ' ') {
                if (*buffer == '\n' || *buffer == '\0') { break; }
                buffer++;
            }
        } else if (flag == 0) {
            i++;
        }
        buffer++;
    }
    } else if (count_or_make == 1) {        // задаем полигон
        char *number = calloc(10, sizeof(char));
        while (*buffer != '\0') {
            if(isdigit(*buffer)) {
                flag = 0;
                tmp[0] = *buffer;
                strncat(number, tmp, 1);
            } else if (*buffer == '/') {
                pol[index].vertex[i] = atol(number);
                i++;
                number[0] = '\0';
                while (*buffer != ' ') {
                    if (*buffer == '\n' || *buffer == '\0') { break; }
                    buffer++;
                }
            } else if (flag == 0) {
                pol[index].vertex[i] = atol(number);
                i++;
                number[0] = '\0';
            }
            buffer++;
        }
        free(number);
        tmp[0] = '\0';
    }
    return i;
}

/**
 * @brief Cleaning memory we allocated fo matrix structure of coordinates
 *
 * @param A Matrix includes coordinates
 */

void cleanVertexMatrix(vertex_t *A) {
  if (A != NULL) {
    for (int i = 0; i < A->rows; i++) {
      free(A->coordinates[i]);
    }
    free(A->coordinates);
  }
}



/**
 * @brief Function shows structure, extra
 *
 * @param obj The structure including coordinates
 */

void printer(data_t* obj) {
    int i = 0, j = 0;
    printf("vertexes: %d facets: %d\n", obj->vertexes, obj->facets);
    while (i < obj->vertexes) {
        printf("%d) x: %lf   y: %lf   z: %lf)\n", i + 1, obj->object_3d.coordinates[i][0], obj->object_3d.coordinates[i][1], obj->object_3d.coordinates[i][2]);
        i++;
    }
    while (j < obj->facets) {
        int k = 0;
        while (k < obj->polygons[j].counter_of_vertexes) {
            printf("%ld ", obj->polygons[j].vertex[k]);
            k++;
        }
        printf("\n");
        j++;
    }
}



/**
 * @brief Function, which changes x-coordinate of vertex
 *
 * @param obj The structure including coordinates
 * @param num Adds value to existing vertex coordinate
 */

void moveObj_x(data_t* obj, double num) {
    for (int i = 0; i < obj->object_3d.rows; i++) {
        obj->object_3d.coordinates[i][0] += num;
    }
}



/**
 * @brief Function, which changes y-coordinate of vertex
 *
 * @param obj The structure including coordinates
 * @param num Adds value to existing vertex coordinate
 */

void moveObj_y(data_t* obj, double num) {
    for (int i = 0; i < obj->object_3d.rows; i++) {
        obj->object_3d.coordinates[i][1] += num;
    }
}



/**
 * @brief Function, which changes x-coordinate of vertex
 *
 * @param obj The structure including coordinates
 * @param num Adds value to existing vertex coordinate
 */

void moveObj_z(data_t* obj, double num) {
    for (int i = 0; i < obj->object_3d.rows; i++) {
        obj->object_3d.coordinates[i][2] += num;
    }
}



/**
 * @brief Function to scale object, changes vertex coordinates multiplying them by a coefficient
 *
 * @param obj The structure including coordinates
 * @param num Coefficient (can be negative)
 */

void scaleObj(data_t* obj, double num) {
    if (num > 0) {
        for (int i = 0; i < obj->vertexes; i++) {
            for (int j = 0; j <= 2; j++) {
                obj->object_3d.coordinates[i][j] *= num;
            }
        }
    }
}

/**
 * @brief Function to rotate object about an axis x
 *
 * @param obj The structure including coordinates
 * @param num In this case the x coordinate
 */

void rotateX(data_t* obj, double num) {
    for (int i = 0; i < obj->vertexes; i++) {
        double tmp_y = obj->object_3d.coordinates[i][1];
        double tmp_z = obj->object_3d.coordinates[i][2];
        obj->object_3d.coordinates[i][1] = cos(num) * tmp_y - sin(num) * tmp_z;
        obj->object_3d.coordinates[i][2] = sin(num) * tmp_y + cos(num) * tmp_z;
    }
}



/**
 * @brief Function to rotate object about an axis y
 *
 * @param obj The structure including coordinates
 * @param num In this case the y coordinate
 */

void rotateY(data_t* obj, double num) {
    for (int i = 0; i < obj->vertexes; i++) {
        double tmp_x = obj->object_3d.coordinates[i][0];
        double tmp_z = obj->object_3d.coordinates[i][2];
        obj->object_3d.coordinates[i][0] = sin(num) * tmp_z + cos(num) * tmp_x;
        obj->object_3d.coordinates[i][2] = cos(num) * tmp_z - sin(num) * tmp_x;
    }
}



/**
 * @brief Function to rotate object about an axis z
 *
 * @param obj The structure including coordinates
 * @param num In this case the z coordinate
 */

void rotateZ(data_t* obj, double num) {
    for (int i = 0; i < obj->vertexes; i++) {
        double tmp_x = obj->object_3d.coordinates[i][0];
        double tmp_y = obj->object_3d.coordinates[i][1];
        obj->object_3d.coordinates[i][0] = cos(num) * tmp_x + sin(num) * tmp_y;
        obj->object_3d.coordinates[i][1] = cos(num) * tmp_y - sin(num) * tmp_x;
    }
}

int main() {
    int error_parse = OK, vertex = 0, face = 0;
    FILE *f = NULL;
    char *s = "";
    data_t *obj = malloc(sizeof(data_t));
    error_parse = firstReaderCounter(f, s, &vertex, &face);
    if (error_parse == OK && vertex != 0) {
        obj->vertexes = vertex;
        obj->facets = face;
        makeMatrix(vertex + 1, 3, &obj->object_3d);
        createVertexStructure(obj);
        int cleaner = fillStructure(obj, f);
        printer(obj);
        moveObj_x(obj, 0);
        moveObj_y(obj, 0);
        moveObj_z(obj, 0);
        scaleObj(obj, 2);
        cleanVertexMatrix(&obj->object_3d);
        cleanPolStruct(obj->polygons, cleaner);
        free(obj->polygons);
        free(obj);
    } else {
        free(obj);
    }
    return 0;
}

// one memory leak instead
// checking of file should be better, no certainty that it will be no error or incorrect count of v/f
// https://grafika.me/node/82 - about rotates
