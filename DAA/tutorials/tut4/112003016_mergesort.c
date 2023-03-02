#include <stdio.h>
#include <stdlib.h>

void merge(int arr[], int left[], int leftSize, int right[], int rightSize) {
    int i = 0, j = 0, k = 0;

    while (i < leftSize && j < rightSize) {
        if (left[i] <= right[j]) {
            arr[k++] = left[i++];
        } else {
            arr[k++] = right[j++];
        }
    }

    while (i < leftSize) {
        arr[k++] = left[i++];
    }

    while (j < rightSize) {
        arr[k++] = right[j++];
    }
}

void mergesort(int arr[], int arrSize) {
    if (arrSize < 2) {
        return;
    }

    int mid = arrSize / 2;

    int *left = (int*)malloc(mid * sizeof(int));
    int *right = (int*)malloc((arrSize - mid) * sizeof(int));

    for (int i = 0; i < mid; i++) {
        left[i] = arr[i];
    }

    for (int i = mid; i < arrSize; i++) {
        right[i - mid] = arr[i];
    }

    mergesort(left, mid);
    mergesort(right, arrSize - mid);
    merge(arr, left, mid, right, arrSize - mid);

    free(left);
    free(right);
}

int main() {
    int arr[] = {5, 1, 6, 2, 3, 4};
    int arrSize = sizeof(arr) / sizeof(arr[0]);

    printf("Unsorted array: ");
    for (int i = 0; i < arrSize; i++) {
        printf("%d ", arr[i]);
    }

    mergesort(arr, arrSize);

    printf("\nSorted array: ");
    for (int i = 0; i < arrSize; i++) {
        printf("%d ", arr[i]);
    }
    printf("\n");

    return 0;
}
