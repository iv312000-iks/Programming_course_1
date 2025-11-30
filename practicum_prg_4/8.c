#include <stdio.h>

int main() {
	int a;
	printf("Enter a number: ");
	scanf("%d", &a);

	int pos = -1;
	for(int i = 0; i < 32; i++) {
		if(!(a & 1)) {
		   pos = i;
		   break;
		}
		a = a >> 1;
	}
	printf("%d\n", pos);
	return 0;
}
