#include <stdio.h>

int main() {
	int n = 8;
	int m = 8;
	for(int i = 0; i < n; i++) {
	  for(int l = 0; l < i; l++) {
		printf(" ");
	  }
	  for(int j = 0; j < m - i; j++) {
		printf("*");
	  }
		printf("\n");
	}

	return 0;
}
