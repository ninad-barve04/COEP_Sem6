/**
 * 112003016_SelectionSort.c
 * Name : Ninad Barve
 * MIS  : 112003016
 * DIV  : 1
 * Batch: TY1-T1(WST1)
 * 
 * @brief DAA Tutorial 1 - Q2
 *        Implement any one algorithm from Q1 in both iterative and recursive form
 *        This is an implementation of Selection Sort
 */

#include <stdio.h>
#include <stdlib.h>

/**
 * @brief Iterative implementation of selection sort
 * 
 * @param array pointer to an integer array to be sorted
 * @param n length of the array
 */
void iterative_selection_sort(int **array, int n) {
    int min, temp;
    int *a = *array;
    for (int i = 0; i <= n; i++) {
        min = i;
        for (int j = i+1; j < n; j++) {
            if (a[j] < a[min]) {
                min = j;
            }
        }
        temp = a[min];
        a[min] = a[i];
        a[i] = temp;
    }
    return;
}

/**
 * @brief Get the index of the minimum element in array between the given limits
 * 
 * @param array array of integers
 * @param i range start value
 * @param j range end value
 * @return int minimum index
 */
int get_min(int *array, int i, int j) {
    int min;
    if (i == j) {
        return i;
    }
    min = get_min(array, i+1, j);

    if (array[i] < array[min]) {
        min = i;
    }

    return min;
}

/**
 * @brief Iterative implementation of selection sort
 * 
 * @param array pointer to an integer array to be sorted
 * @param len length of the array
 * @param pos index seperating the array. Start sorting with value 0
 */
void recursive_selection_sort(int **array, int len, int pos) {
    int min, temp;
    int *a = *array;
    if (len == pos) {
        return;
    }
    min = get_min(a, pos, len-1);

    if (min != pos) {
        temp = a[pos];
        a[pos] = a[min];
        a[min] = temp;
    }

    recursive_selection_sort(array, len, pos+1);
}


int main(int argc, char const *argv[])
{
    int *array = NULL;
    int len, val;

    /*Getting the values for sorting array iteratively*/
    printf("Iterative sorting:\n");
    printf("Enter length of array: ");
    scanf("%d", &len);
    array = (int *)malloc(len);
    printf("Enter digits in array seperated by space: ");
    for (int i = 0; i < len; i++) {
        scanf("%d", &val);
        array[i] = val;
    }

    for (int i = 0; i < len; i++) {
        printf("%d ", array[i]);
    }
    printf("\n");
    /*Iterative Selection sort function call*/
    iterative_selection_sort(&array, len);
    
    for (int i = 0; i < len; i++) {
        printf("%d ", array[i]);
    }
    free(array);
    array = NULL;
    printf("\n\n");
    
    /*Getting the values for sorting array recursively*/
    printf("Recursive sorting:\n");
    printf("Enter length of array: ");
    scanf("%d", &len);
    array = (int *)malloc(len);
    printf("Enter digits in array seperated by space: ");
    for (int i = 0; i < len; i++) {
        scanf("%d", &val);
        array[i] = val;
    }

    for (int i = 0; i < len; i++) {
        printf("%d ", array[i]);
    }
    printf("\n");
    /*Recursive Selection sort function call*/
    recursive_selection_sort(&array, len, 0);
    
    for (int i = 0; i < len; i++) {
        printf("%d ", array[i]);
    }
    printf("\n");

    return 0;
}

