#include "s21_viewer_3d.h"

/**
 * @brief Clear object structure
 *
 * @param obj Object
 */

void free_data(data_t *obj) {
  for (int i = 0; i < obj->facets; i++) {
    free(obj->polygons[i].vertex);
  }
  free(obj->object_3d.coordinates);
  free(obj->polygons);
  free(obj);
}

/**
 * @brief Start function
 *
 * @param filename path to filename
 * @param error Value to parse errors
 */

data_t *obj_point_output(char *filename, int *error) {
  *error = 0;
  data_t *obj = NULL;
  FILE *file = fopen(filename, "r");
  if (file != NULL) {
    obj = point_parsing(file, error);
    fclose(file);
  } else {
    *error = 2;
  }
  return obj;
}

/**
 * @brief Copy data object
 *
 * @param source Object
 * @param error Value to parse errors
 */

data_t *copyData(data_t *source, int *error) {
  *error = 0;
  data_t *obj = NULL;
  obj = (data_t *)malloc(sizeof(data_t));
  if (obj != NULL) {
    obj->object_3d.coordinates = NULL;
    obj->polygons = NULL;
    obj->facets = source->facets;
    obj->object_3d.coordinates =
        (point_t *)malloc(source->object_3d.amount_points * sizeof(point_t));
    if (obj->object_3d.coordinates != NULL) {
      memmove(obj->object_3d.coordinates, source->object_3d.coordinates,
              source->object_3d.amount_points * sizeof(point_t));
      memmove(obj->object_3d.minMax_x, source->object_3d.minMax_x,
              2 * sizeof(double));
      memmove(obj->object_3d.minMax_y, source->object_3d.minMax_y,
              2 * sizeof(double));
      memmove(obj->object_3d.minMax_z, source->object_3d.minMax_z,
              2 * sizeof(double));
      obj->object_3d.amount_points = source->object_3d.amount_points;
      obj->polygons = (polygon_t *)malloc(source->facets * sizeof(polygon_t));
      if (obj->polygons != NULL) {
        for (int i = 0; i < source->facets; i++) {
          obj->polygons[i].vertex = (int *)malloc(
              source->polygons[i].counter_of_vertexes * sizeof(int));
          if (obj->polygons[i].vertex != NULL) {
            memmove(obj->polygons[i].vertex, source->polygons[i].vertex,
                    source->polygons[i].counter_of_vertexes * sizeof(int));
            obj->polygons[i].counter_of_vertexes =
                source->polygons[i].counter_of_vertexes;
          } else {
            *error = 1;
          }
        }
      } else {
        *error = 1;
      }
    } else {
      *error = 1;
    }
  } else {
    *error = 1;
  }
  return obj;
}

/**
 * @brief Parser of points
 *
 * @param file File to read
 * @param error Value to parse errors
 */

data_t *point_parsing(FILE *file, int *error) {
  *error = 0;
  int MallocPoint = 0;
  int MallocPolygons = 0;
  int sizePoint = 0;
  int sizePolygons = 0;
  int flagMinMaxFirst = 1;
  data_t *obj = NULL;
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
        pars_vertex_and_polygon_line(file, obj, c, &sizePoint, &sizePolygons,
                                     &flagMinMaxFirst);
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

/**
 * @brief Parser of vertexes and facets
 *
 * @param file File to read
 * @param obj Structure to fill
 * @param sizePoint array to parse size of v
 * @param sizePolygons array to parse size of f
 * @param flagMinMaxFirst Parser array of min and max coordinates
 */

void pars_vertex_and_polygon_line(FILE *file, data_t *obj, char c,
                                  int *sizePoint, int *sizePolygons,
                                  int *flagMinMaxFirst) {
  char *buf = NULL;
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
        } else if (c != ' ') {
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
    int sizeVertex = 0;
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
          if (pars_vertex(obj, buf, &MallocVertex, &sizeVertex, sizePolygons) ==
              0) {
            sizeBuf = 0;
            flagBuf = 0;
            flagSlash = 0;
          } else {
            flagChar = 1;
          }
        } else if (c == '/') {
          flagSlash = 1;
        } else if (c != ' ' && flagSlash != 1) {
          flagChar = 1;
        }
      }
      c = getc(file);
    }
    if (flagBuf != 0 && flagChar == 0) {
      buf[sizeBuf] = '\0';
      if (pars_vertex(obj, buf, &MallocVertex, &sizeVertex, sizePolygons) ==
          0) {
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

/**
 * @brief Parser of vertexes
 *
 * @param buf string form number
 * @param obj Structure to fill
 * @param sizePoint array to parse size of v
 * @param sizePolygons array to parse size of f
 * @param MallocVertex size we have to malloc
 */

int pars_vertex(data_t *obj, char *buf, int *MallocVertex, int *sizeVertex,
                int *sizePolygons) {
  int error = 0;
  if (*sizeVertex >= *MallocVertex) {
    (*MallocVertex) += 5;
    int *tmp = (int *)realloc(obj->polygons[*sizePolygons].vertex,
                              *MallocVertex * sizeof(int));
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

/**
 * @brief Fill coordinate x,y,z
 *
 * @param buf string form number
 * @param obj Structure of object
 * @param sizePoint array to parse size of v
 * @param xyz type of coordinate (x/y/z)
 */

void pars_xyz(data_t *obj, int xyz, int *sizePoint, char *buf) {
  if (xyz == 0) {
    obj->object_3d.coordinates[*sizePoint].x = atof(buf);
  } else if (xyz == 1) {
    obj->object_3d.coordinates[*sizePoint].y = atof(buf);
  } else if (xyz == 2) {
    obj->object_3d.coordinates[*sizePoint].z = atof(buf);
  }
}

/**
 * @brief Parse minimal and maximal coordinate of object
 *
 * @param flagMinMaxFirst Parse min and max coordinate
 * @param obj Structure of object
 * @param sizePoint array to parse size of v
 * @param xyz type of coordinate (x/y/z)
 */

void pars_minMax_points(data_t *obj, int *sizePoint, int xyz,
                        int *flagMinMaxFirst) {
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
      if ((obj->object_3d.minMax_x[0] -
           obj->object_3d.coordinates[*sizePoint].x) > EPS) {
        obj->object_3d.minMax_x[0] = obj->object_3d.coordinates[*sizePoint].x;
      }
      if ((obj->object_3d.minMax_x[1] -
           obj->object_3d.coordinates[*sizePoint].x) < EPS) {
        obj->object_3d.minMax_x[1] = obj->object_3d.coordinates[*sizePoint].x;
      }
    } else if (xyz == 1) {
      if ((obj->object_3d.minMax_y[0] -
           obj->object_3d.coordinates[*sizePoint].y) > EPS) {
        obj->object_3d.minMax_y[0] = obj->object_3d.coordinates[*sizePoint].y;
      }
      if ((obj->object_3d.minMax_y[1] -
           obj->object_3d.coordinates[*sizePoint].y) < EPS) {
        obj->object_3d.minMax_y[1] = obj->object_3d.coordinates[*sizePoint].y;
      }
    } else if (xyz == 2) {
      if ((obj->object_3d.minMax_z[0] -
           obj->object_3d.coordinates[*sizePoint].z) > EPS) {
        obj->object_3d.minMax_z[0] = obj->object_3d.coordinates[*sizePoint].z;
      }
      if ((obj->object_3d.minMax_z[1] -
           obj->object_3d.coordinates[*sizePoint].z) < EPS) {
        obj->object_3d.minMax_z[1] = obj->object_3d.coordinates[*sizePoint].z;
      }
    }
  }
}

/**
 * @brief Bring figure to the center of axis
 *
 * @param obj Structure of object
 */

void center(data_t *obj) {
  double centerX =
      obj->object_3d.minMax_x[0] +
      (obj->object_3d.minMax_x[1] - obj->object_3d.minMax_x[0]) / 2;
  double centerY =
      obj->object_3d.minMax_y[0] +
      (obj->object_3d.minMax_y[1] - obj->object_3d.minMax_y[0]) / 2;
  double centerZ =
      obj->object_3d.minMax_z[0] +
      (obj->object_3d.minMax_z[1] - obj->object_3d.minMax_z[0]) / 2;
  for (int i = 0; i < obj->object_3d.amount_points; i++) {
    obj->object_3d.coordinates[i].x -= centerX;
    obj->object_3d.coordinates[i].y -= centerY;
    obj->object_3d.coordinates[i].z -= centerZ;
  }
}

/**
 * @brief Set figure location and size
 * @param obj Structure of object
 * @param value factor
 */

void adjust(data_t *obj, double value) {
  double X = obj->object_3d.minMax_x[1] - obj->object_3d.minMax_x[0];
  double Y = obj->object_3d.minMax_y[1] - obj->object_3d.minMax_y[0];
  double Z = obj->object_3d.minMax_z[1] - obj->object_3d.minMax_z[0];
  double dmax = max3(X, Y, Z);

  double scal = (value - (value * (-1))) / dmax;
  scale(obj, scal);
}

/**
 * @brief Scale object
 *
 * @param obj Structure of object
 * @param scale coefficient to scale
 */

void scale(data_t *obj, double scale) {
  for (int i = 0; i < obj->object_3d.amount_points; i++) {
    obj->object_3d.coordinates[i].x *= scale;
    obj->object_3d.coordinates[i].y *= scale;
    obj->object_3d.coordinates[i].z *= scale;
  }
}

/**
 * @brief Choose max coordinate
 *
 * @param n1 X value
 * @param n2 Y value
 * @param n3 Z value
 */

double max3(double n1, double n2, double n3) {
  return (n1 >= n2 && n1 >= n3) ? n1 : (n2 >= n3 ? n2 : n3);
}

/**
 * @brief change memory size of buffer
 *
 * @param buf string
 * @param MallocBuf add memory value
 */

int reallocBuf(char **buf, int *MallocBuf) {
  int error = 0;
  *MallocBuf += 15;
  char *tmp = (char *)realloc(*buf, *MallocBuf * sizeof(char));
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

/**
 * @brief change memory size to polygons
 *
 * @param obj object structure
 * @param MallocPolygons add memory value
 */

int reallocPolygons(data_t *obj, int *MallocPolygons) {
  int error = 0;
  *MallocPolygons = *MallocPolygons + 25000;
  polygon_t *tmp =
      (polygon_t *)realloc(obj->polygons, *MallocPolygons * sizeof(polygon_t));
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

/**
 * @brief change memory size to vertexes
 *
 * @param obj object structure
 * @param MallocPoint add memory value
 */

int reallocPoint(data_t *obj, int *MallocPoint) {
  int error = 0;
  *MallocPoint = *MallocPoint + 25000;
  point_t *tmp = (point_t *)realloc(obj->object_3d.coordinates,
                                    *MallocPoint * sizeof(point_t));
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

/**
 * @brief Rotate around an axis X
 *
 * @param obj object structure
 * @param num factor value
 */

void rotateX(data_t *obj, double num) {
  for (int i = 0; i < obj->object_3d.amount_points; i++) {
    double tmp_y = obj->object_3d.coordinates[i].y;
    double tmp_z = obj->object_3d.coordinates[i].z;
    obj->object_3d.coordinates[i].y = cos(num) * tmp_y - sin(num) * tmp_z;
    obj->object_3d.coordinates[i].z = sin(num) * tmp_y + cos(num) * tmp_z;
  }
}

/**
 * @brief Rotate around an axis Y
 *
 * @param obj object structure
 * @param num factor value
 */

void rotateY(data_t *obj, double num) {
  for (int i = 0; i < obj->object_3d.amount_points; i++) {
    double tmp_x = obj->object_3d.coordinates[i].x;
    double tmp_z = obj->object_3d.coordinates[i].z;
    obj->object_3d.coordinates[i].x = sin(num) * tmp_z + cos(num) * tmp_x;
    obj->object_3d.coordinates[i].z = cos(num) * tmp_z - sin(num) * tmp_x;
  }
}

/**
 * @brief Rotate around an axis Z
 *
 * @param obj object structure
 * @param num factor value
 */

void rotateZ(data_t *obj, double num) {
  for (int i = 0; i < obj->object_3d.amount_points; i++) {
    double tmp_x = obj->object_3d.coordinates[i].x;
    double tmp_y = obj->object_3d.coordinates[i].y;
    obj->object_3d.coordinates[i].x = cos(num) * tmp_x + sin(num) * tmp_y;
    obj->object_3d.coordinates[i].y = cos(num) * tmp_y - sin(num) * tmp_x;
  }
}

/**
 * @brief Move Axis X
 *
 * @param obj object structure
 * @param num factor value
 */

void moveObj_x(data_t *obj, double num) {
  for (int i = 0; i < obj->object_3d.amount_points; i++) {
    obj->object_3d.coordinates[i].x += num;
  }
}

/**
 * @brief Move Axis Y
 *
 * @param obj object structure
 * @param num factor value
 */

void moveObj_y(data_t *obj, double num) {
  for (int i = 0; i < obj->object_3d.amount_points; i++) {
    obj->object_3d.coordinates[i].y += num;
  }
}

/**
 * @brief Move Axis Z
 *
 * @param obj object structure
 * @param num factor value
 */

void moveObj_z(data_t *obj, double num) {
  for (int i = 0; i < obj->object_3d.amount_points; i++) {
    obj->object_3d.coordinates[i].z += num;
  }
}
