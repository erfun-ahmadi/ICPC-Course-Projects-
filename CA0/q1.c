#include <stdio.h>
int main() {
	int a, b, c, d, e, f, g, h;
	int x, y, z, t;
	scanf_s("%d %d %d %d %d %d %d %d", &a, &b, &c, &d, &e, &f, &g, &h);
	if (a > 0 && b > 0 && c > 0 && d > 0 && e > 0 && f > 0 && g > 0 && h > 0) {
		x = d * e * g;     //we suppose that x = 1. Then z = a/e, y = (a*f)/(d*e), t = (a*f*c)/(d*e*g). Then x = x*a*d*e*g, z = z*a*d*e*g, y= y*a*d*e*g, t = t*a*d*e*g to make them integer numbers
		y = a * f * g;
		z = a * d * g;
		t = a * f * c;
		if (x * b == t * h) {
			for (int i = 1; i <= x; i++)
				if (x % i == 0 && z % i == 0 && y % i == 0 && t % i == 0) {
					x = x / i;
					y = y / i;
					z = z / i;
					t = t / i;   //to calculate the least integer number
				}
			printf("\n%d \n%d \n%d \n%d", x, y, z, t);
		}
		else
			printf("-1");
	}
	return 0;
}