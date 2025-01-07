#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

typedef struct
{
    int *array;
    int size;
    int start;
    int end;
}ThreadArgs;

int *array1;
int *array2;
int *origin_array;
int *result_array;

void *quick_sort(void *arguments);
void *merge_arrays(void *arguments);

int main()
{
    int size;
    printf("Enter the number of all elements: \n");
    scanf("%d", &size);

    array1 = (int *)malloc(size / 2 * sizeof(int));
    array2 = (int *)malloc((size - size / 2) * sizeof(int));
    origin_array = (int *)malloc(size * sizeof(int));
    result_array = (int *)malloc(size * sizeof(int));

    printf("Enter the elements: \n");
    for(int i = 0; i < size; i ++ )
        scanf("%d", &origin_array[i]);
    for(int i = 0; i < size / 2; i ++ )
        array1[i] = origin_array[i];
    for(int i = 0; i < size - size / 2; i ++ )
        array2[i] = origin_array[i + size / 2];

    ThreadArgs args1 = {array1, size / 2, 0, size / 2 - 1};
    ThreadArgs args2 = {array2, size - size / 2, 0, size - size / 2 - 1};
    ThreadArgs merge_args = {result_array, size, 0, size - 1};

    pthread_t sorting_thread1, sorting_thread2, merging_thread;

    pthread_create(&sorting_thread1, NULL, quick_sort, (void *)&args1);
    pthread_create(&sorting_thread2, NULL, quick_sort, (void *)&args2);
    pthread_join(sorting_thread1, NULL);
    pthread_join(sorting_thread2, NULL);

    printf("The sorted result of thread 1:\n");
    for(int i = 0; i < args1.size; i ++ )
        printf("%d ", args1.array[i]);
    printf("\n");
    printf("The sorted result of thread 2:\n");
    for(int i = 0; i < args2.size; i ++ )
        printf("%d ", args2.array[i]);
    printf("\n");

    pthread_create(&merging_thread, NULL, merge_arrays, (void *)&merge_args);
    pthread_join(merging_thread, NULL);

    printf("The sorted result of merged thread:\n");
    for(int i = 0; i < size; i ++ )
        printf("%d ", result_array[i]);
    printf("\n");

    return 0;
}


void *quick_sort(void *arguments)
{
    ThreadArgs *args = (ThreadArgs *)arguments;
    int *array = args->array;
    int start = args->start;
    int end = args->end;

    if (start < end)
    {
        int pivot = array[end];
        int i = start - 1;

        for (int j = start; j <= end - 1; j++)
        {
            if (array[j] < pivot)
            {
                i++;
                int temp = array[i];
                array[i] = array[j];
                array[j] = temp;
            }
        }

        int temp = array[i + 1];
        array[i + 1] = array[end];
        array[end] = temp;

        ThreadArgs args_left = {array, 0, start, i};
        ThreadArgs args_right = {array, 0, i + 2, end};

        quick_sort(&args_left);
        quick_sort(&args_right);
    }

    return NULL;
}



void *merge_arrays(void *arguments) {
    ThreadArgs *args = (ThreadArgs *)arguments;
    int *result_array = args -> array;
    int size = args -> size;

    int index1 = 0;
    int index2 = 0;
    int index_merged = 0;

    while (index1 < size / 2 && index2 < (size - size / 2)) {
        if (array1[index1] < array2[index2]) {
            result_array[index_merged ++ ] = array1[index1 ++ ];
        } else {
            result_array[index_merged ++ ] = array2[index2 ++ ];
        }
    }

    while (index1 < size / 2) {
        result_array[index_merged ++ ] = array1[index1 ++ ];
    }

    while (index2 < (size - size / 2)) {
        result_array[index_merged ++ ] = array2[index2 ++ ];
    }

    return NULL;
}