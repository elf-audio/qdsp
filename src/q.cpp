#include <iostream>
#include "qfloat.h"
#include <stdio.h>

void run(float v) {
	qfloat q = QFLOAT(v);
	float a = Q_FLOAT(q);
	int n = qfloat_to_int16(q);
	int m = qfloat_to_int16_slow_bad(q);

	printf("%.4f          Q: 0x%x         n: %d          m: %d\n", v, q, n, m);
}
int main() {
	std::cout << "Hello World!\n";
	printf("qfloat_1 = 0x%x   %.2f  %.2f\n", qfloat_1, Q_FLOAT(qfloat_1), Q_FLOAT(QFLOAT(1)));
	printf("%f\n", Q_FLOAT(qmul(QFLOAT(0.5), QFLOAT(0.1))));

	run(0.5);
	run(0.999);
	run(-0.999);
	run(1.0);
	run(-1);
	run(0);
}