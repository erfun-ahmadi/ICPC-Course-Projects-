#include <stdio.h>
int main() {
	int n, i, d;
	scanf_s("%d", &n);
	if (n > 0 && n <= 1000)
	{
		if (n > 2) {
			i = 2;
			while (i < n) {
				d = n % i;
				if (d == 0) {
					printf("NO"); //it's not prime
					break;
				}
				i++;		
			}
			if (d != 0)
				printf("YES"); //it's prime
		}	
		else if (n == 2)
			printf("YES");
		else
			printf("NO");
	} 
	return 0;
}