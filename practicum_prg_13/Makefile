.PHONY: all clean run

# Собираем 10.exe
all: 10.exe

# Линковка с готовой библиотекой
10.exe: 10.o libMY_LIB.a
	gcc 10.o -L. -lMY_LIB -o 10.exe

# Компиляция
10.o: src/10.c
	gcc -c src/10.c -o 10.o -I ./include

# Очистка
clean:
	del *.o *.exe

# Запуск
run: 10.exe
	10.exe

