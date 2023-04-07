#include "../include/brute_force.h"
#include "../include/matrix.h"
#include "../include/strassen.h"

Matrix combined_multiply(Matrix a, Matrix b, int crossover_point) {
  if (a.rows <= crossover_point || a.cols <= crossover_point ||
      b.cols <= crossover_point) {
    return brute_force_algorithm(a, b);
  } else {
    return strassen_algorithm(a, b);
  }
}
