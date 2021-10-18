#include <math.h>
#include <pthread.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define IJK_TO_INDEX(n, i, j, k) (((k)*n + j) * n + i)

#define CALC(n, i, j, k, i_p, i_n, j_p, j_n, k_p, k_n)                       \
  args->next[IJK_TO_INDEX(n, i, j, k)] =                                           \
      (1.0 / 6.0) *                                                          \
      (args->curr[IJK_TO_INDEX(n, i_n, j, k)] + args->curr[IJK_TO_INDEX(n, i_p, j, k)] + \
       args->curr[IJK_TO_INDEX(n, i, j_n, k)] + args->curr[IJK_TO_INDEX(n, i, j_p, k)] + \
       args->curr[IJK_TO_INDEX(n, i, j, k_n)] + args->curr[IJK_TO_INDEX(n, i, j, k_p)] + \
       -args->delta * args->delta * args->source[IJK_TO_INDEX(n, i, j, k)]);

// Global flag
// set to true when operating in debug mode to enable verbose logging
static bool debug = false;

pthread_barrier_t barrier;

typedef struct {
  int thread_id;
  int start_plane;
  int end_plane;
  int n;
  int iters;
  double* restrict curr;
  double* restrict next;
  double* source;
  double delta;
} worker_args;

void* bottom_middle_planes(void* pargs) {
  worker_args* args = (worker_args*)pargs;
  int n = args->n;
  int n_2 = args->n - 2;
  int n_1 = args->n - 1;

  for (int iters = 0; iters < args->iters; iters++) {
    if (debug) {
      printf("Bottom middle planes %d to %d; thread: %d; iters: %d\n", args->start_plane, args->end_plane, args->thread_id, iters);
    }
    // Bottom plane top left corner
    CALC(n, 0, 0, 0, 1, 1, 1, 1, 1, 1);

    // Bottom plane top middle
    for (int i = 1; i < n_1; i++) {
      CALC(n, i, 0, 0, i - 1, i + 1, 1, 1, 1, 1);
    }

    // Bottom plane top right corner
    CALC(n, n_1, 0, 0, n_2, n_2, 1, 1, 1, 1);

    // Bottom plane middle + bottom plane left & right middle
    for (int j = 1; j < n_1; j++) {
      CALC(n, 0, j, 0, 1, 1, j - 1, j + 1, 1, 1);
      for (int i = 1; i < n_1; i++) {
        CALC(n, i, j, 0, i - 1, i + 1, j - 1, j + 1, 1, 1);
      }
      CALC(n, n_1, j, 0, n_2, n_2, j - 1, j + 1, 1, 1);
    }

    // Bottom plane bottom left corner
    CALC(n, 0, n_1, 0, 1, 1, n_2, n_2, 1, 1);

    // Bottom plane bottom middle
    for (int i = 1; i < n_1; i++) {
      CALC(n, i, n_1, 0, i - 1, i + 1, n_2, n_2, 1, 1);
    }

    // Bottom plane bottom right corner
    CALC(n, n_1, n_1, 0, n_2, n_2, n_2, n_2, 1, 1);

    if (debug) printf("Bottom plane computed: %d\n", 0);

    // Middle planes
    for (int k = 1; k < args->end_plane; k++) {
      // Middle plane top left corner
      CALC(n, 0, 0, k, 1, 1, 1, 1, k - 1, k + 1);

      // Middle plane top middle
      for (int i = 1; i < n_1; i++) {
        CALC(n, i, 0, k, i - 1, i + 1, 1, 1, k - 1, k + 1);
      }

      // Middle plane top right corner
      CALC(n, n_1, 0, k, n_2, n_2, 1, 1, k - 1, k + 1);

      // No boundary conditions -> core + middle plane left and right middle
      for (int j = 1; j < n_1; j++) {
        CALC(n, 0, j, k, 1, 1, j - 1, j + 1, k - 1, k + 1);
        for (int i = 1; i < n_1; i++) {
          CALC(n, i, j, k, i - 1, i + 1, j - 1, j + 1, k - 1, k + 1);
        }
        CALC(n, n_1, j, k, n_2, n_2, j - 1, j + 1, k - 1, k + 1);
      }

      // Middle plane bottom left corner
      CALC(n, 0, n_1, k, 1, 1, n_2, n_2, k - 1, k + 1);

      // Middle plane bottom middle
      for (int i = 1; i < n_1; i++) {
        CALC(n, i, n_1, k, i - 1, i + 1, n_2, n_2, k - 1, k + 1);
      }

      // Middle plane bottom right corner
      CALC(n, n_1, n_1, k, n_2, n_2, n_2, n_2, k - 1, k + 1);

      if (debug) printf("Bottom middle plane computed: %d\n", k);
    }

    pthread_barrier_wait(&barrier);

    double* p = args->curr;
    args->curr = args->next;
    args->next = p;
  }

  return NULL;
}

void* middle_planes(void* pargs) {
  worker_args* args = (worker_args*)pargs;

  int n = args->n;
  int n_2 = args->n - 2;
  int n_1 = args->n - 1;

  for (int iters = 0; iters < args->iters; iters++) {
    if (debug) {
      printf("Middle planes %d to %d; thread: %d; iters: %d\n", args->start_plane, args->end_plane, args->thread_id, iters);
    }
    // Middle planes
    for (int k = args->start_plane; k < args->end_plane; k++) {
      // Middle plane top left corner
      CALC(n, 0, 0, k, 1, 1, 1, 1, k - 1, k + 1);

      // Middle plane top middle
      for (int i = 1; i < n_1; i++) {
        CALC(n, i, 0, k, i - 1, i + 1, 1, 1, k - 1, k + 1);
      }

      // Middle plane top right corner
      CALC(n, n_1, 0, k, n_2, n_2, 1, 1, k - 1, k + 1);

      // No boundary conditions -> core + middle plane left and right middle
      for (int j = 1; j < n_1; j++) {
        CALC(n, 0, j, k, 1, 1, j - 1, j + 1, k - 1, k + 1);
        for (int i = 1; i < n_1; i++) {
          CALC(n, i, j, k, i - 1, i + 1, j - 1, j + 1, k - 1, k + 1);
        }
        CALC(n, n_1, j, k, n_2, n_2, j - 1, j + 1, k - 1, k + 1);
      }

      // Middle plane bottom left corner
      CALC(n, 0, n_1, k, 1, 1, n_2, n_2, k - 1, k + 1);

      // Middle plane bottom middle
      for (int i = 1; i < n_1; i++) {
        CALC(n, i, n_1, k, i - 1, i + 1, n_2, n_2, k - 1, k + 1);
      }

      // Middle plane bottom right corner
      CALC(n, n_1, n_1, k, n_2, n_2, n_2, n_2, k - 1, k + 1);

      if (debug) printf("Middle plane computed: %d\n", k);
    }

    pthread_barrier_wait(&barrier);

    double* p = args->curr;
    args->curr = args->next;
    args->next = p;
  }

  return NULL;
}

void* top_middle_planes(void* pargs) {
  worker_args* args = (worker_args*)pargs;
  int n = args->n;
  int n_2 = args->n - 2;
  int n_1 = args->n - 1;

  for (int iters = 0; iters < args->iters; iters++) {
    if (debug) {
      printf("Top middle planes %d to %d; thread: %d; iters: %d\n", args->start_plane, args->end_plane, args->thread_id, iters);
    }
    // Middle planes
    for (int k = args->start_plane; k < args->end_plane - 1; k++) {
      // Middle plane top left corner
      CALC(n, 0, 0, k, 1, 1, 1, 1, k - 1, k + 1);

      // Middle plane top middle
      for (int i = 1; i < n_1; i++) {
        CALC(n, i, 0, k, i - 1, i + 1, 1, 1, k - 1, k + 1);
      }

      // Middle plane top right corner
      CALC(n, n_1, 0, k, n_2, n_2, 1, 1, k - 1, k + 1);

      // No boundary conditions -> core + middle plane left and right middle
      for (int j = 1; j < n_1; j++) {
        CALC(n, 0, j, k, 1, 1, j - 1, j + 1, k - 1, k + 1);
        for (int i = 1; i < n_1; i++) {
          CALC(n, i, j, k, i - 1, i + 1, j - 1, j + 1, k - 1, k + 1);
        }
        CALC(n, n_1, j, k, n_2, n_2, j - 1, j + 1, k - 1, k + 1);
      }

      // Middle plane bottom left corner
      CALC(n, 0, n_1, k, 1, 1, n_2, n_2, k - 1, k + 1);

      // Middle plane bottom middle
      for (int i = 1; i < n_1; i++) {
        CALC(n, i, n_1, k, i - 1, i + 1, n_2, n_2, k - 1, k + 1);
      }

      // Middle plane bottom right corner
      CALC(n, n_1, n_1, k, n_2, n_2, n_2, n_2, k - 1, k + 1);

      if (debug) printf("Top middle plane computed: %d\n", k);
    }

    // Top plane top left corner
    CALC(n, 0, 0, n_1, 1, 1, 1, 1, n_2, n_2);

    // Top plane top middle
    for (int i = 1; i < n_1; i++) {
      CALC(n, i, 0, n_1, i - 1, i + 1, 1, 1, n_2, n_2);
    }

    // Top plane top right corner
    CALC(n, n_1, 0, n_1, n_2, n_2, 1, 1, n_2, n_2);

    // Top plane middle + top plane left & right middle
    for (int j = 1; j < n_1; j++) {
      CALC(n, 0, j, n_1, 1, 1, j - 1, j + 1, n_2, n_2);
      for (int i = 1; i < n_1; i++) {
        CALC(n, i, j, n_1, i - 1, i + 1, j - 1, j + 1, n_2, n_2);
      }
      CALC(n, n_1, j, n_1, n_2, n_2, j - 1, j + 1, n_2, n_2);
    }

    // Top plane bottom left corner
    CALC(n, 0, n_1, n_1, 1, 1, n_2, n_2, n_2, n_2);

    // Top plane bottom middle
    for (int i = 1; i < n_1; i++) {
      CALC(n, i, n_1, n_1, i - 1, i + 1, n_2, n_2, n_2, n_2);
    }

    // Top plane bottom right corner
    CALC(n, n_1, n_1, n_1, n_2, n_2, n_2, n_2, n_2, n_2);

    if (debug) printf("Top plane computed: %d\n", n - 1);

    pthread_barrier_wait(&barrier);

    double* p = args->curr;
    args->curr = args->next;
    args->next = p;
  }

  return NULL;
}

/**
 * @brief Solve Poissons equation for a given cube with Neumann boundary
 * conditions on all sides.
 *
 * @param n             The edge length of the cube. n^3 number of elements.
 * @param source        Pointer to the source term cube, a.k.a. forcing
 * function.
 * @param iterations    Number of iterations to solve with.
 * @param threads       Number of threads to use for solving.
 * @param delta         Grid spacing.
 * @return double*      Solution to Poissons equation. Caller must free().
 */
double* poisson_neumann(int n, double* source, int iterations, int threads_num,
                        float delta) {
  if (debug) {
    printf(
        "Starting solver with:\n"
        "n = %i\n"
        "iterations = %i\n"
        "threads = %i\n"
        "delta = %f\n",
        n, iterations, threads_num, delta);
  }

  // Allocate some buffers to calculate the solution in
  double* curr = (double*)calloc(n * n * n, sizeof(double));
  double* next = (double*)calloc(n * n * n, sizeof(double));

  // Ensure we haven't run out of memory
  if (curr == NULL || next == NULL) {
    fprintf(stderr,
            "Error: ran out of memory when trying to allocate %i sized cube\n",
            n);
    exit(EXIT_FAILURE);
  }

  if (threads_num > n) threads_num = n;

  pthread_barrier_init(&barrier, NULL, threads_num);

  pthread_t threads[threads_num];
  worker_args args[threads_num];

  // int plane_spacing = ((n - 1) + (threads_num - 1)) / threads_num;
  int plane_spacing = (n + 1) / threads_num;

  int start_plane = 0;
  int end_plane = 0;

  end_plane = start_plane + plane_spacing;

  // Fill in the arguments to the worker
  args[0].thread_id = 0;
  args[0].start_plane = start_plane;
  args[0].end_plane = end_plane;
  args[0].n = n;
  args[0].iters = iterations;
  args[0].curr = curr;
  args[0].next = next;
  args[0].source = source;
  args[0].delta = delta;

  // Create the worker thread
  if (pthread_create(&threads[0], NULL, &bottom_middle_planes, &args[0]) != 0) {
    fprintf(stderr, "Error creating worker thread!\n");
    exit(EXIT_FAILURE);
  }

  start_plane = end_plane;

  // Launch each of the new worker threads
  for (int t = 1; t < threads_num - 1; t++) {
    end_plane = start_plane + plane_spacing;

    // Fill in the arguments to the worker
    args[t].thread_id = t;
    args[t].start_plane = start_plane;
    args[t].end_plane = end_plane;
    args[t].n = n;
    args[t].iters = iterations;
    args[t].curr = curr;
    args[t].next = next;
    args[t].source = source;
    args[t].delta = delta;

    // Create the worker thread
    if (pthread_create(&threads[t], NULL, &middle_planes, &args[t]) != 0) {
      fprintf(stderr, "Error creating worker thread!\n");
      exit(EXIT_FAILURE);
    }

    start_plane = end_plane;
  }

  // Fill in the arguments to the worker
  args[threads_num - 1].thread_id = threads_num - 1;
  args[threads_num - 1].start_plane = start_plane;
  args[threads_num - 1].end_plane = n;
  args[threads_num - 1].n = n;
  args[threads_num - 1].iters = iterations;
  args[threads_num - 1].curr = curr;
  args[threads_num - 1].next = next;
  args[threads_num - 1].source = source;
  args[threads_num - 1].delta = delta;

  // Create the worker thread
  if (pthread_create(&threads[threads_num - 1], NULL, &top_middle_planes,
                     &args[threads_num - 1]) != 0) {
    fprintf(stderr, "Error creating worker thread!\n");
    exit(EXIT_FAILURE);
  }

  // Wait for all the threads to finish using join()
  for (int i = 0; i < threads_num; ++i) {
    pthread_join(threads[i], NULL);
    if (debug) printf("%d joined\n", i);
  }

  // Free one of the buffers and return the correct answer in the other.
  // The caller is now responsible for free'ing the returned pointer.
  free(next);

  if (debug) {
    printf("Finished solving.\n");
  }

  return curr;
}

int main(int argc, char** argv) {
  // default settings for solver
  int iterations = 10;
  int n = 7;
  int threads = 4;
  float delta = 1;

  // parse the command line arguments
  for (int i = 1; i < argc; ++i) {
    if (strcmp(argv[i], "-h") == 0 || strcmp(argv[i], "--help") == 0) {
      printf(
          "usage: poisson [-n size] [-i iterations] [-t threads] [--debug]\n");
      return EXIT_SUCCESS;
    }

    if (strcmp(argv[i], "-n") == 0) {
      if (i == argc - 1) {
        fprintf(stderr, "Error: expected size after -n!\n");
        return EXIT_FAILURE;
      }

      n = atoi(argv[++i]);
    }

    if (strcmp(argv[i], "-i") == 0) {
      if (i == argc - 1) {
        fprintf(stderr, "Error: expected iterations after -i!\n");
        return EXIT_FAILURE;
      }

      iterations = atoi(argv[++i]);
    }

    if (strcmp(argv[i], "-t") == 0) {
      if (i == argc - 1) {
        fprintf(stderr, "Error: expected threads after -t!\n");
        return EXIT_FAILURE;
      }

      threads = atoi(argv[++i]);
    }

    if (strcmp(argv[i], "--debug") == 0) {
      debug = true;
    }
  }

  // ensure we have an odd sized cube
  if (n % 2 == 0) {
    fprintf(stderr, "Error: n should be an odd number!\n");
    return EXIT_FAILURE;
  }

  // Create a source term with a single point in the centre
  double* source = (double*)calloc(n * n * n, sizeof(double));
  if (source == NULL) {
    fprintf(stderr, "Error: failed to allocated source term (n=%i)\n", n);
    return EXIT_FAILURE;
  }

  source[(n * n * n) / 2] = 1;

  // Calculate the resulting field with Neumann conditions
  double* result = poisson_neumann(n, source, iterations, threads, delta);

  // Print out the middle slice of the cube for validation
  for (int x = 0; x < n; ++x) {
    for (int y = 0; y < n; ++y) {
      printf("%0.5f ", result[((n / 2) * n + y) * n + x]);
    }
    printf("\n");
  }

  free(source);
  free(result);

  return EXIT_SUCCESS;
}
