/* sum.c (Roland Teodorowitsch; 16 set. 2021)
 * Compilation: gcc sum.c -o sum -fopenmp
 * Adapted from: http://lspd.mackenzie.br/marathon/06/sum.zip
 */

#include <stdio.h>
#include <string.h>
#include <omp.h>

#define START  1000000
#define STEP   1000000
#define END   10000000

#define DIGITS 1000

void sum(char* output, const long unsigned int d, const long unsigned int n) {
    long unsigned int processo, digit, i, remainder, div, mod;
    long unsigned int digits[d + 11];
    long unsigned int matriz[omp_get_max_threads()+1][d + 11];

    #pragma omp parallel for schedule(dynamic, 5)
    for (processo = 0; processo < omp_get_max_threads(); ++processo) {
      for (digit = 0; digit < d + 11; ++digit) {
          digits[digit] = 0;
          matriz[processo][digit] = 0;
      }
    }


    #pragma omp parallel
    {

      int thread_id = omp_get_thread_num();

      #pragma omp for
      for (int aux1 = 1; aux1 <= n; ++aux1) {
        long unsigned int primeiraDivisao, restante, restoDivisao;
        restante = 1;
          for (int aux2 = 0; (aux2 < d + 11 && restante); ++aux2) {
              primeiraDivisao = restante / aux1;
              restoDivisao = restante % aux1;
              matriz[thread_id][aux2] += primeiraDivisao;
              restante = restoDivisao * 10;
          }
      }

      #pragma omp for nowait
          for (int i = 0; i < (d + 11); ++i)
              for(int j = 0; j < omp_get_max_threads(); ++j)
                  digits[i] += matriz[j][i];
  }


    for (i = d + 11 - 1; i > 0; --i) {
        digits[i - 1] += digits[i] / 10;
        digits[i] %= 10;
    }
    if (digits[d + 1] >= 5) {
        ++digits[d];
    }
    for (i = d; i > 0; --i) {
        digits[i - 1] += digits[i] / 10;
        digits[i] %= 10;
    }
    sprintf(output,"%lu.",digits[0]);
    unsigned long int t = strlen(output);
    for (i = 1; i <= d; ++i)
        output[t++] = digits[i]+'0';
    output[t] = '\0';
}

int main() {
    long unsigned int n;
    double start, finish;
    char output[DIGITS + 10]; // extra chars to avoid error
    for (n=START; n<=END; n+=STEP) {
        start = omp_get_wtime();
        sum(output, DIGITS, n);
        finish = omp_get_wtime();
        fprintf(stdout,"%s\n",output);
        fprintf(stderr,"%lu %lf\n",n,finish-start);
    }
    return 0;
}
