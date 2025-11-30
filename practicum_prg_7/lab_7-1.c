#include <stdio.h>
#include <string.h>
// нужно для isalpha
#include <ctype.h>

char reverse(char *str);

int main() {
    char text[] = """Lorem ipsum dolor sit amet, consectetur adipiscing elit,\
    sed do eiusmod tempor i5ncididunt ut labore et dolore magna aliqua. Ut enim \
    ad minim veniam, quis nostrud exercitation ullamco laboris nisi ut aliquip ex\
    ea commodo consequat. Duis aute irure dolor in reprehenderit in voluptate velit \
    esse cillum dolore eu fugiat nulla pariatur. Excepteur sint occaecat cupidatat non\
    proident, sunt in culpa qui officia deserunt mollit anim id est laborum.""";

    reverse(text);

    printf("%s\n", text);
    
    return 0;
}

            // указатель на строку для обработки
char reverse(char *str) {
    for(int i = 0; str[i]; i++) {
        // ф-ия isalpha ожидает аргумент типа int тк символ char может иметь знак
        // необходимо прописать unsigned char
        if (isalpha((unsigned char)str[i])) {
            int start = i;

            // конец слова
            while(str[i] && isalpha((unsigned char)str[i])) i++;
            int end = i - 1;
            
            // ревёрс первой и последней буквы
            if(end > start) {
                char temp = str[start];
                str[start] = str[end];
                str[end] = temp;
            }
            // нужно уменьшить счётчик тк после внутреннего цикла он стоит на первом
            // не буквенном месте, риск пропустить следующий буквенный символ
            i--;
        }
    }
}
