#ifndef CHURCH_H
#define CHURCH_H

#include <stdint.h>
#include <stdio.h>
#include "esp_heap_caps.h"

// Pointer to hold the large array in PSRAM
__attribute__((aligned(16))) int16_t *example_element = NULL;

// Function to allocate and initialize the array in PSRAM
static void allocate_example_element() {
    // Define the size of your array (e.g., 224*224*3 for your image)
    size_t array_size = 224 * 224 * 3;

    // Allocate memory in PSRAM
    example_element = (int16_t *) heap_caps_malloc(array_size * sizeof(int16_t), MALLOC_CAP_SPIRAM);
    if (example_element == NULL) {
        // Handle memory allocation failure
        printf("Failed to allocate memory in PSRAM\n");
        return;
    }

    // Open the file containing the data
    FILE *file = fopen("/spiffs/please_work.txt", "r");
    if (file == NULL) {
        printf("Failed to open data file\n");
        return;
    }

    // Read data from the file
    size_t i = 0;
    while (fscanf(file, "%hd,", &example_element[i]) == 1 && i < array_size) {
        i++;
    }

    if (i != array_size) {
        printf("Error: expected %zu elements, but only read %zu\n", array_size, i);
    }

    fclose(file);
}

#endif // CHURCH_H
