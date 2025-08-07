#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define MAX_VALUE 10000

void allocate(int** matrix_ptr, int rows, int cols) {
    *matrix_ptr = (int*)malloc(rows * cols * sizeof(int));
    if (*matrix_ptr == NULL) {
        printf("Ошибка выделения памяти.\n");
        exit(EXIT_FAILURE);
    }
}

void deallocate(int* matrix) {
    free(matrix);
    matrix = NULL; // обнуляем указатель, чтобы избежать висячего указателя
}

void generate_random(int** matrix_ptr, int rows, int cols) {
    srand(time(NULL));
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            int rand_val = rand() % MAX_VALUE;
            (*matrix_ptr)[i * cols + j] = rand_val;
        }
    }
}

void print_matrix(int** matrix_ptr, int rows, int cols) {
    printf("\nЗначения в матрице:\n");
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            printf("%d ", (*matrix_ptr)[i * cols + j]);
        }
        printf("\n");
    }
    printf("\n");
}

int get_matrix_sum(int** matrix_ptr, int rows, int cols) {
    int sum = 0;
    int size = rows * cols;
    for (int i = 0; i < size; ++i) {
        sum += (*matrix_ptr)[i];
    }
    return sum;
}

int main() {
    int* matrix = NULL;
    int rows, cols = 0;
    printf("Введите кол-во рядов:\n");
    scanf("%d", &rows);
    if (rows < 0) {
        printf("Ошибка: введено отрицательное значение\n");
        exit(EXIT_FAILURE);
    }
    printf("Введите кол-во столбцов:\n");
    scanf("%d", &cols);
    if (cols < 0) {
        printf("Ошибка: введено отрицательное значение\n");
        exit(EXIT_FAILURE);
    }
    // allocate dynamic memory
    allocate(&matrix, rows, cols);

    // генерация случайных чисел от 0 до MAX_VALUE
    generate_random(&matrix, rows, cols);

    // вывод
    print_matrix(&matrix, rows, cols);

    // вывести сумму всех элементов в матрице
    printf("Cумма элементов матрицы: %d\n\n\n", get_matrix_sum(&matrix, rows, cols));

    deallocate(matrix);
    return EXIT_SUCCESS;
}