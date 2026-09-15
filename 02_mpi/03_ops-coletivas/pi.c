#include <stdio.h>

double f(double x) {
    return 4.0 / (1.0 + x * x);
}

int main() {
    long n = 9000000000; // quanto maior, mais preciso
    double step = 1.0 / n;
    double sum = 0.0;

    for (long i = 0; i < n; i++) {
        double x = (i + 0.5) * step; // ponto médio
        sum += f(x);
    }

    double pi = step * sum;
    printf("Aproximação de pi: %.30f\n", pi);

    return 0;
}