#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <gsl/gsl_chebyshev.h>
#include <gsl/gsl_fit.h>

#define EVAL_TO_FILE(file, expression) for(double x = -1; x <= 1; x+= 0.05) fprintf(file, "%g %g\n", x, expression)

double originalFunc(double x, void* params) {
    return pow(0.5, x*x + 2*x);
}

void saveOriginal() {
    FILE* file = fopen("original.txt", "w");
    EVAL_TO_FILE(file, originalFunc(x, NULL));
    fclose(file);
}

void approxSquares() {
    double xs[100], ys[100];
    for(int i = 0; i < 100; i++) {
        xs[i] = -1 + i * 2 / 100;
        ys[i] = originalFunc(xs[i], NULL);
    }
    double c0, c1;
    gsl_fit_linear(xs, 0, ys, 0, 100, &c0, &c1, null, null);
    FILE* approxFile = fopen("approx-squares.txt", "w");
    EVAL_TO_FILE(approxFile, c0 + c1 * x);
    fclose(approxFile);
}

void approximateChebyshev() {
    FILE *chebyshevFile = fopen("approx-chebyshev.txt", "w");
    gsl_cheb_series *series = gsl_cheb_alloc(5);
    gsl_function func;
    func.function = originalFunc;
    func.params = NULL;
    gsl_cheb_init(series, &func, -1, 1);

    EVAL_TO_FILE(chebyshevFile, gsl_cheb_eval(series, x));

    gsl_cheb_free(series);
    fclose(chebyshevFile);
}

int main() {
    saveOriginal();
    approxSquares();
    approximateChebyshev();

    system("gnuplot -e \"set terminal png size 1200,900; set output 'out.png'; "
           "plot 'original.txt' with lines title 'Oryginalny', 'approx-chebyshev.txt' with lines title 'Czebyszew', "
           "'approx-squares.txt' with lines title 'Sredniokwadratowa'\"");
}
