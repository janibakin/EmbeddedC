#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <time.h>

#define MAX_VALUE 10000

void allocate(int** matrix_ptr, int rows, int cols) {
    *matrix_ptr = calloc(rows * cols, sizeof(int)); // вместо malloc используем calloc для инициализации нулями
    if (*matrix_ptr == NULL) {
        fprintf(stderr, "Ошибка выделения памяти.\n");
        exit(EXIT_FAILURE);
    }
}

void deallocate(int** matrix) {
    free(*matrix);
    *matrix = NULL; // обнуляем указатель, чтобы избежать висячего указателя
}

void generate_random(int* matrix, int rows, int cols) {
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            int rand_val = rand() % MAX_VALUE;
            matrix[i * cols + j] = rand_val;
        }
    }
}

void print_matrix(int* matrix, int rows, int cols) {
    printf("\nЗначения в матрице:\n");
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            printf("%d ", matrix[i * cols + j]);
        }
        printf("\n");
    }
    printf("\n");
}

int64_t get_matrix_sum(int* matrix, int rows, int cols) {
    int64_t sum = 0;
    size_t size = (size_t)rows * cols;
    // Проверка на переполнение при умножении
    if (size > SIZE_MAX / sizeof(int)) {
        fprintf(stderr, "Ошибка: переполнение при вычислении размера матрицы.\n");
        exit(EXIT_FAILURE);
    }
    for (size_t i = 0; i < size; ++i) {
        sum += matrix[i];
    }
    return sum;
}

int main() {
    int* matrix = NULL;
    int rows, cols = 0;
    printf("Введите кол-во рядов:\n");
    if (scanf("%d", &rows) != 1 || rows <= 0) {
        fprintf(stderr, "Неверный ввод.\n");
        return EXIT_FAILURE;
    }
    printf("Введите кол-во столбцов:\n");
    if (scanf("%d", &cols) != 1 || cols <= 0) {
        fprintf(stderr, "Неверный ввод.\n");
        return EXIT_FAILURE;
    }
    // выделить динамическую память
    allocate(&matrix, rows, cols);

    // вывод до генерации случайных чисел
    print_matrix(matrix, rows, cols);

    // генерация случайных чисел от 0 до MAX_VALUE
    srand(time(NULL));
    generate_random(matrix, rows, cols);

    // вывод после генерации случайных чисел
    print_matrix(matrix, rows, cols);

    // вывести сумму всех элементов в матрице
    printf("Cумма элементов матрицы: %ld\n\n\n", get_matrix_sum(matrix, rows, cols));

    // освободить динамическую память
    deallocate(&matrix);
    return EXIT_SUCCESS;
}