#include "../include/brute_force.h"
#include "../include/matrix.h"
#include "../include/strassen.h"
#include <math.h>
#include <stddef.h>

Matrix combined_algorithm(Matrix a, Matrix b, void *crossover_point_ptr) {
  int crossover_point = *(int *)crossover_point_ptr;
  if (crossover_point == -1) {
    crossover_point = log2(a.rows) / 2;
  }

  if (a.rows <= crossover_point) {
    return brute_force_algorithm(a, b, NULL);
  } else {
    return strassen_algorithm(a, b, NULL);
  }
}
