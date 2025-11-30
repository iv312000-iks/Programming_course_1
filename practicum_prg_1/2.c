#include <stdio.h>

int main() {
	float temp;
	char scale;
	printf("Enter the temp and scale: ");
	scanf("%f%c", &temp, &scale);
		if (scale == 'c' || scale == 'C') {
			printf("%.2f%c\n", (temp * 1.8) + 32, 'f');
		} else if (scale == 'f' || scale == 'F') {
 			printf("%.2f%c\n", (temp - 32) / 1.8, 'c');
		} else {
			printf("error\n");
		}
		return 0;
}
