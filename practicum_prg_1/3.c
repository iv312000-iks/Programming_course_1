#include <stdio.h>

int main() {
	double x, y;
	printf("Enter the coordinate x: ");
	scanf("%lf", &x);
	printf("Enter the coordinate y: ");
	scanf("%lf", &y);
	if ((-2.0 < x && x <= 1.0) && (-2.0 < y && y <= 0)) {
		printf("yes\n");
	} else if ((-2.0 < x && x <= -1.0) && (0.0 <= y && y <= 1.0)) {
		printf("yes\n");
	} else if ((-1.0 <= x && x < 0.0) && (y <= -x)) {
		printf("yes\n");
	} else {
		printf("no\n");
	}
	return 0;
}
