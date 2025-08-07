/*
Задача 2: Отладка и оптимизация
Используя следующий фрагмент кода C, выявите и исправьте любые ошибки, если они
есть. Кроме того, если возможно, предложите варианты оптимизации.
 #include <stdio.h>

int main() {
    int i, sum = 0;

    for (i = 1; i <= 50; i++)
        sum = i * i;

    printf("Sum of squares: %d\n", sum);

    return 0;
}

Ошибка: "sum +=" нужно было вместо "sum ="
Оптимизация: использовать математическую формулу для суммы квадратов
*/

#include <stdio.h>

int main() {
    const int n = 100;
    long long sum = (long long)n * (n + 1) * (2 * n + 1) / 6;
    printf("Sum of squares: %lld\n", sum);
    return 0;
}