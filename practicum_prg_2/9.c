#include <stdio.h>

int main() {
	int k, a, s;
	printf("Enter a number: ");
	scanf("%d", &a);
	for (int n = a; n != 0; n = n / 10) {
		k = n % 10;
		if (k > s)
		 s = k;
	}
	printf("%d\n", s);
	return 0;
}
