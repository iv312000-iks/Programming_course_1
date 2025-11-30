#include <stdio.h>

int main() {
	int a, s, k;
	printf("Enter a number: ");
	scanf("%d", &a);
	for (int n = a; n != 0; n = n / 10) {
		k = n % 10;
		s = s * 10 + k;
	}
	printf("%d\n", s);
	return 0;
}
