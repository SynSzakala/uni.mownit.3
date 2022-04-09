#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <gsl/gsl_chebyshev.h>

double exp_func(double x, void *params) {
    return exp(x * x);
}

double abs_func(double x, void *params) {
    return fabs(x); // abs for floating type
}

double sign_func(double x, void *params) {
    if (x > 0)return 1;
    if (x == 0) return 0;
    else return -1;
}

void approximate(int order, double (*function)(double x, void *params), FILE *approx) {
    gsl_cheb_series *series = gsl_cheb_alloc(order);
    gsl_function func;
    func.function = function;
    func.params = NULL;
    gsl_cheb_init(series, &func, -1, 1);

    for (double x = -1; x <= 1; x += 0.05) {
        double value = gsl_cheb_eval(series, x);
        fprintf(approx, "%g %g\n", x, value);
    }

    gsl_cheb_free(series);
}

FILE *makeFile(char *prefix, char *suffix, char *output) {
    sprintf(output, "%s-%s", prefix, suffix);
    return fopen(output, "w");
}

void approxAndSave(char *filePrefix, double (*function)(double x, void *params)) {
    char originalFileName[100], approx3FileName[100], approx6FileName[100], approx10FileName[100];
    FILE *originalFile = makeFile(filePrefix, "original.txt", originalFileName);
    FILE *approx3File = makeFile(filePrefix, "approx3.txt", approx3FileName);
    FILE *approx6File = makeFile(filePrefix, "approx6.txt", approx6FileName);
    FILE *approx10File = makeFile(filePrefix, "approx10.txt", approx10FileName);

    for (double x = -1; x <= 1; x += 0.05) {
        double value = function(x, NULL);
        fprintf(originalFile, "%g %g\n", x, value);
    }

    approximate(3, function, approx3File);
    approximate(6, function, approx6File);
    approximate(10, function, approx10File);

    fclose(originalFile);
    fclose(approx3File);
    fclose(approx6File);
    fclose(approx10File);

    char gnuplot_command[1000];
    sprintf(
            gnuplot_command,
            "gnuplot -e \"set terminal png size 1200,900; set output '%s.png'; "
            "plot '%s' with lines title 'Oryginalny', '%s' with lines title 'Stopien = 3', "
            "'%s' with lines title 'Stopien = 6', '%s' with lines title 'Stopien = 10'\"",
            filePrefix,
            originalFileName,
            approx3FileName,
            approx6FileName,
            approx10FileName
    );

    system(gnuplot_command);
}

int main() {
    approxAndSave("exp", exp_func);
    approxAndSave("abs", abs_func);
    approxAndSave("sign", sign_func);
}
