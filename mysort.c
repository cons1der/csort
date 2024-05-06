#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct {
    int *array;
    int start;
    int end;
} SortSegment;

void merge(int *arr, int l, int m, int r) {
    int i, j, k;
    int n1 = m - l + 1;
    int n2 = r - m;
    int *L = malloc(n1 * sizeof(int));
    int *R = malloc(n2 * sizeof(int));
    for (i = 0; i < n1; i++) L[i] = arr[l + i];
    for (j = 0; j < n2; j++) R[j] = arr[m + 1 + j];
    i = 0; j = 0; k = l;
    while (i < n1 && j < n2) arr[k++] = (L[i] <= R[j]) ? L[i++] : R[j++];
    while (i < n1) arr[k++] = L[i++];
    while (j < n2) arr[k++] = R[j++];
    free(L);
    free(R);
}

void *thread_sort(void *arg) {
    SortSegment *seg = (SortSegment *)arg;
    int mid = seg->start + (seg->end - seg->start) / 2;
    if (seg->start < seg->end) {
        thread_sort((void *)&(SortSegment){seg->array, seg->start, mid});
        thread_sort((void *)&(SortSegment){seg->array, mid + 1, seg->end});
        merge(seg->array, seg->start, mid, seg->end);
    }
    return NULL;
}

void sort_data(int *array, int array_size, int num_threads) {
    pthread_t threads[num_threads];
    SortSegment segments[num_threads];
    int segment_size = array_size / num_threads;
    for (int i = 0; i < num_threads; i++) {
        segments[i] = (SortSegment){array, i * segment_size, (i + 1) * segment_size - 1};
        if (i == num_threads - 1) segments[i].end = array_size - 1;
        pthread_create(&threads[i], NULL, thread_sort, &segments[i]);
    }
    for (int i = 0; i < num_threads; i++) pthread_join(threads[i], NULL);
    for (int size = segment_size; size < array_size; size *= 2) {
        for (int left_start = 0; left_start < array_size - size; left_start += 2 * size) {
            int mid = left_start + size - 1;
            int right_end = left_start + 2 * size - 1;
            if (right_end >= array_size) right_end = array_size - 1;
            merge(array, left_start, mid, right_end);
        }
    }
}

void sort_from_file(const char *input_filename, const char *output_filename, int num_threads) {
    FILE *input_file = fopen(input_filename, "r");
    if (!input_file) {
        perror("Failed to open input file");
        return;
    }
    int *array = malloc(1000000 * sizeof(int));
    int count = 0;
    while (fscanf(input_file, "%d", &array[count]) == 1 && count < 999999) {
        count++;
    }
    fclose(input_file);

    sort_data(array, count, num_threads);

    FILE *output_file = fopen(output_filename, "w");
    if (!output_file) {
        perror("Failed to open output file");
        return;
    }
    for (int i = 0; i < count; i++) {
        fprintf(output_file, "%d\n", array[i]);
    }
    fclose(output_file);
    free(array);
}

int main(int argc, char *argv[]) {
    if (argc != 4) {
        fprintf(stderr, "Usage: %s <input file> <output file> <num threads>\n", argv[0]);
        return 1;
    }
    const char *input_filename = argv[1];
    const char *output_filename = argv[2];
    int num_threads = atoi(argv[3]);

    sort_from_file(input_filename, output_filename, num_threads);

    return 0;
}

