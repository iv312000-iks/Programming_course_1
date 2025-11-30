#include <stdio.h>

int main() {
	int n, s;
	printf("Enter a number: ");
	scanf("%d", &n);
	for (int s = 1, i = 1; i <= n; i++) {
		 s = s * i;
		 printf("%d\n", s);
	}
	return 0;
}
