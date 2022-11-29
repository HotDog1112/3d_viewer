#include "3DViewer.h"

int main() {
    int error = 0;
    data_t *obj = obj_point_output("a.obj", &error);
    center(obj);
    if (error == 0) {
        for (int i = 0; i < obj->object_3d.amount_points; i++) {
            printf("%d x= %lf; y= %lf; z= %lf;\n", i+1, obj->object_3d.coordinates[i].x, obj->object_3d.coordinates[i].y, obj->object_3d.coordinates[i].z);
        }
        for (int i = 0; i < obj->facets; i++) {
            printf("%d - ", i+1);
            for (int j = 0; j < obj->polygons[i].counter_of_vertexes; j++) {
                printf("%d - ", obj->polygons[i].vertex[j]);
            }
            printf("%d\n",obj->polygons[i].counter_of_vertexes);
        }
        printf ("minX= %lf; maxX= %lf\n", obj->object_3d.minMax_x[0], obj->object_3d.minMax_x[1]);
        printf ("minY= %lf; maxY= %lf\n", obj->object_3d.minMax_y[0], obj->object_3d.minMax_y[1]);
        printf ("minZ= %lf; maxZ= %lf\n", obj->object_3d.minMax_z[0], obj->object_3d.minMax_z[1]);
        free_data(obj);
    } else if (error == 2) {
        printf("No such file!!!\n");
    } else {
        printf("ERROR!");
    }
}


void free_data(data_t *obj) {
    for (int i = 0; i < obj->facets; i++) {
        free(obj->polygons[i].vertex);
    }
    free(obj->object_3d.coordinates);
    free(obj->polygons);
    free(obj);
}

data_t* obj_point_output(char* filename, int* error) {
    *error = 0;
    data_t* obj = NULL;
    FILE* file = fopen(filename, "r");
    if (file != NULL) {
        obj = point_parsing(file, error);
        fclose(file);
    } else {
        *error = 2;
    }

    return obj;
}

data_t* point_parsing(FILE* file, int* error) {
    *error = 0;
    int MallocPoint = 0;
    int MallocPolygons = 0;
    int sizePoint = 0;
    int sizePolygons = 0;
    int flagMinMaxFirst = 1;
    data_t* obj = NULL;
    obj = (data_t *)malloc(sizeof(data_t));
    if (obj != NULL) {
        obj->object_3d.coordinates = NULL;
        obj->polygons = NULL;
        if (obj != NULL) {
            char c = getc(file);
            while (c != EOF) {
                if (sizePoint >= MallocPoint) {
                    if ((*error = reallocPoint(obj, &MallocPoint)) != 0) {
                        break;
                    }
                }
                if (sizePolygons >= MallocPolygons) {
                    if ((*error = reallocPolygons(obj, &MallocPolygons)) != 0) {
                        break;
                    }
                }
                pars_vertex_and_polygon_line(file, obj, c, &sizePoint, &sizePolygons, &flagMinMaxFirst);
                c = getc(file);
            }
            obj->object_3d.amount_points = sizePoint;
            obj->facets = sizePolygons;
        }
    } else {
        *error = 1;
    }
    return obj;
}


void pars_vertex_and_polygon_line(FILE* file, data_t *obj, char c, int *sizePoint, int *sizePolygons, int *flagMinMaxFirst) {
    char* buf = NULL;
    int MallocBuf = 0;
    int sizeBuf = 0;
    int flagBuf = 0;
    int flagChar = 0;
    if (c == 'v') {
        int xyz = 0;
        c = getc(file);
        while (c != '\r' && c != '\n' && c != EOF) {
            if (flagChar == 0) {
                if (sizeBuf >= MallocBuf) {
                    if (reallocBuf(&buf, &MallocBuf) != 0) {
                        flagChar = 1;
                    }
                }
                if (((c >= '0' && c <= '9') || c == '.' || c == '-') && flagChar == 0) {
                    buf[sizeBuf] = c;
                    sizeBuf++;
                    flagBuf = 1;
                } else if (c == ' ' && flagBuf == 1 && flagChar == 0) {
                    buf[sizeBuf] = '\0';
                    pars_xyz(obj, xyz, sizePoint, buf);
                    pars_minMax_points(obj, sizePoint, xyz, flagMinMaxFirst);
                    xyz++;
                    sizeBuf = 0;
                    flagBuf = 0;
                } else if (c != ' '){
                    flagChar = 1;
                }
            }
            c = getc(file);
        }
        if (xyz == 2 && flagBuf != 0 && flagChar == 0) {
            buf[sizeBuf] = '\0';
            pars_xyz(obj, xyz, sizePoint, buf);
            pars_minMax_points(obj, sizePoint, xyz, flagMinMaxFirst);
            xyz++;
            (*sizePoint)++;
        }
    } else if (c == 'f') {
        obj->polygons[*sizePolygons].vertex = NULL;
        int sizeVertex = 0; // минимум 3
        int flagSlash = 0;
        int MallocVertex = 0;
        c = getc(file);
        while (c != '\r' && c != '\n' && c != EOF) {
            if (flagChar == 0) {
                if (sizeBuf >= MallocBuf) {
                    if (reallocBuf(&buf, &MallocBuf) != 0) {
                        flagChar = 1;
                    }
                }
                if (c >= '0' && c <= '9' && flagSlash == 0 && flagChar == 0) {
                    buf[sizeBuf] = c;
                    sizeBuf++;
                    flagBuf = 1;
                } else if (c == ' ' && flagBuf == 1 && flagChar == 0) {
                    buf[sizeBuf] = '\0';
                    if (pars_vertex(obj, buf, &MallocVertex, &sizeVertex, sizePolygons) == 0) {
                        sizeBuf = 0;
                        flagBuf = 0;
                        flagSlash = 0;
                    } else {
                        flagChar = 1;
                    }
                } else if (c == '/') {
                    flagSlash = 1;
                } else if (c != ' ' && flagSlash != 1){
                    flagChar = 1;
                }
            }
            c = getc(file);
        }
        if (flagBuf != 0 && flagChar == 0) {
            buf[sizeBuf] = '\0';
            if (pars_vertex(obj, buf, &MallocVertex, &sizeVertex, sizePolygons) == 0) {
                if (sizeVertex >= 3) {
                    obj->polygons[*sizePolygons].counter_of_vertexes = sizeVertex;
                    (*sizePolygons)++;
                }
            }
        } else if (flagChar == 0) {
            if (sizeVertex >= 3) {
                obj->polygons[*sizePolygons].counter_of_vertexes = sizeVertex;
                (*sizePolygons)++;
            }
        } else if (flagChar == 1) {
            if (obj->polygons[*sizePolygons].vertex != NULL) {
                free(obj->polygons[*sizePolygons].vertex);
            }
        }
    }
    if (buf != NULL) {
        free(buf);
    }
}

int pars_vertex(data_t *obj, char *buf, int *MallocVertex, int *sizeVertex, int *sizePolygons) {
    int error = 0;
    if (*sizeVertex >= *MallocVertex) {
        (*MallocVertex) += 5;
        int* tmp = (int*)realloc(obj->polygons[*sizePolygons].vertex, *MallocVertex * sizeof(int));
        if (tmp != NULL) {
            obj->polygons[*sizePolygons].vertex = tmp;
        } else {
            error = 1;
        }
    }
    obj->polygons[*sizePolygons].vertex[*sizeVertex] = (atoi(buf) - 1);
    (*sizeVertex)++;
    return error;
}

void pars_xyz(data_t *obj, int xyz, int *sizePoint, char *buf) {
    if (xyz == 0) {
        obj->object_3d.coordinates[*sizePoint].x = atof(buf);
    } else if (xyz == 1) {
        obj->object_3d.coordinates[*sizePoint].y = atof(buf);
    } else if (xyz == 2) {
        obj->object_3d.coordinates[*sizePoint].z = atof(buf);
    }
}

void pars_minMax_points(data_t *obj, int *sizePoint, int xyz, int *flagMinMaxFirst) {
    if (*flagMinMaxFirst == 1) {
        if (xyz == 0) {
            obj->object_3d.minMax_x[0] = obj->object_3d.coordinates[*sizePoint].x;
            obj->object_3d.minMax_x[1] = obj->object_3d.coordinates[*sizePoint].x;
        } else if (xyz == 1) {
            obj->object_3d.minMax_y[0] = obj->object_3d.coordinates[*sizePoint].y;
            obj->object_3d.minMax_y[1] = obj->object_3d.coordinates[*sizePoint].y;
        } else if (xyz == 2) {
            obj->object_3d.minMax_z[0] = obj->object_3d.coordinates[*sizePoint].z;
            obj->object_3d.minMax_z[1] = obj->object_3d.coordinates[*sizePoint].z;
            *flagMinMaxFirst = 0;
        }
    } else {
        if (xyz == 0) {
            if ((obj->object_3d.minMax_x[0] - obj->object_3d.coordinates[*sizePoint].x) > EPS) {
                obj->object_3d.minMax_x[0] = obj->object_3d.coordinates[*sizePoint].x;
            }
            if ((obj->object_3d.minMax_x[1] - obj->object_3d.coordinates[*sizePoint].x) < EPS) {
                obj->object_3d.minMax_x[1] = obj->object_3d.coordinates[*sizePoint].x;
            }
        } else if (xyz == 1) {
            if ((obj->object_3d.minMax_y[0] - obj->object_3d.coordinates[*sizePoint].y) > EPS) {
                obj->object_3d.minMax_y[0] = obj->object_3d.coordinates[*sizePoint].y;
            }
            if ((obj->object_3d.minMax_y[1] - obj->object_3d.coordinates[*sizePoint].y) < EPS) {
                obj->object_3d.minMax_y[1] = obj->object_3d.coordinates[*sizePoint].y;
            }
        } else if (xyz == 2) {
            if ((obj->object_3d.minMax_z[0] - obj->object_3d.coordinates[*sizePoint].z) > EPS) {
                obj->object_3d.minMax_z[0] = obj->object_3d.coordinates[*sizePoint].z;
            }
            if ((obj->object_3d.minMax_z[1] - obj->object_3d.coordinates[*sizePoint].z) < EPS) {
                obj->object_3d.minMax_z[1] = obj->object_3d.coordinates[*sizePoint].z;
            }
        }
    }
}

void center(data_t *obj) {
    double centerX = obj->object_3d.minMax_x[0] + (obj->object_3d.minMax_x[1] - obj->object_3d.minMax_x[0]) / 2;
    double centerY = obj->object_3d.minMax_y[0] + (obj->object_3d.minMax_y[1] - obj->object_3d.minMax_y[0]) / 2;
    double centerZ = obj->object_3d.minMax_z[0] + (obj->object_3d.minMax_z[1] - obj->object_3d.minMax_z[0]) / 2;
    for (int i = 0; i < obj->object_3d.amount_points; i++) {
        obj->object_3d.coordinates[i].x -= centerX;
        obj->object_3d.coordinates[i].y -= centerY;
        obj->object_3d.coordinates[i].z -= centerZ;
    }
}

void scaling(data_t *obj, double value) {
    double X = obj->object_3d.minMax_x[1] - obj->object_3d.minMax_x[0];
    double Y = obj->object_3d.minMax_y[1] - obj->object_3d.minMax_y[0];
    double Z = obj->object_3d.minMax_z[1] - obj->object_3d.minMax_z[0];
    double dmax = max3(X, Y, Z);

    double scale = (value - (value * -1)) / dmax;
    for (int i = 0; i < obj->object_3d.amount_points; i++) {
        obj->object_3d.coordinates[i].x *= scale;
        obj->object_3d.coordinates[i].y *= scale;
        obj->object_3d.coordinates[i].z *= scale;
    }
}

double max3(double n1, double n2, double n3) {
     return (n1 >= n2 && n1 >= n3) ? n1 : (n2 >= n3 ? n2 : n3);
}

int reallocBuf(char **buf, int* MallocBuf) {
    int error = 0;
    *MallocBuf += 15;
    char* tmp = (char*)realloc(*buf, *MallocBuf * sizeof(char));
    if (tmp != NULL) {
        *buf = tmp;
    } else {
        error = 1;
        if (*buf != NULL) {
            free(*buf);
        }
    }
    return error;
}

int reallocPolygons(data_t *obj, int* MallocPolygons) {
    int error = 0;
    *MallocPolygons = *MallocPolygons + 25000;
    polygon_t *tmp = (polygon_t*)realloc(obj->polygons, *MallocPolygons * sizeof(polygon_t));
    if (tmp != NULL) {
        obj->polygons = tmp;
    } else {
        error = 1;
        if (obj->polygons != NULL) {
            free(obj->polygons);
        }
    }

    return error;
}

int reallocPoint(data_t *obj, int* MallocPoint) {
    int error = 0;
    *MallocPoint = *MallocPoint + 25000;
    point_t *tmp = (point_t*)realloc(obj->object_3d.coordinates, *MallocPoint * sizeof(point_t));
    if (tmp != NULL) {
        obj->object_3d.coordinates = tmp;
    } else {
        error = 1;
        if (obj->object_3d.coordinates != NULL) {
            free(obj->object_3d.coordinates);
        }
    }

    return error;
}
