#include <stdio.h>

int main() {
	int n = 2, flag = 0;
	int a;
	printf("Enter a number: ");
	scanf("%d", &a);
	while (a % n != 0) {
		n++;
		if (n == a) {
			flag = 1;
			break;
		}
	}
	printf("%d\n", flag);
	return 0;

}


