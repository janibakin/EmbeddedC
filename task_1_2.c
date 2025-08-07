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
*/

#include <stdio.h>

int main() {
    int sum = 0;

    for (int i = 1; i <= 50; ++i)
        sum += i * i;

    printf("Sum of squares: %d\n", sum);

    return 0;
}