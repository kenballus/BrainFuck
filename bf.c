#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>

const int ARRAY_SIZE = 30000;
const int VISIBLE_RANGE = 10;

void interpret(char ifile[]) {
    int* array = (int*)malloc(sizeof(int) * ARRAY_SIZE); // The tape
    int index = 0; // Where we are on the tape

    int i;
    for (i = 0; i < ARRAY_SIZE; i++) {
        array[i] = 0;
    }

    int char_index; // Where we are in the input
    for (char_index = 0; ifile[char_index] != 0; char_index++) {
        char chr = ifile[char_index]; // The current input character

        switch (chr) {
        // Increment value
        case '+':
            array[index]++;
            break;

        // Decrement value
        case '-':
            array[index]--;
            break;

        // Increment pointer
        case '>':
            index++;
            if (index >= ARRAY_SIZE) {
                printf("ERROR: Indexed off the right end of the tape\n");
                return;
            }
            break;

        // Decrement pointer
        case '<':
            index--;
            if (index < 0) {
                printf("ERROR: Indexed off the left end of the tape\n");
                return;
            }
            break;

        // Output
        case '.':
            printf("%c", array[index]);
            break;

        // Input
        case ',':
            array[index] = getchar();
            break;

        // Begin loop
        case '[':
            if (array[index] == 0) { // Skip to the corresponding ']'
                int counter = 1;
                while (counter) {
                    char_index++;
                    if (ifile[char_index] == 0) {
                        printf("ERROR: \"[\" without corresponding \"]\"\n");
                        return;
                    }
                    if (ifile[char_index] == ']') {
                        counter--;
                    }
                    else if (ifile[char_index] == '[') {
                        counter++;
                    }
                }
            }
            break;

        // End loop
        case ']':
            if (array[index] != 0) { // Rewind to the corresponding '['
                int counter = 1;
                while (counter) {
                    char_index--;
                    if (char_index < 0) {
                        printf("ERROR: \"]\" without corresponding \"[\"\n");
                        return;
                    }
                    if (ifile[char_index] == ']') {
                        counter++;
                    }
                    else if (ifile[char_index] == '[') {
                        counter--;
                    }
                }
            }
            break;
        }
    }

    int start, end, mode;

    if (index <= VISIBLE_RANGE) {
        start = 0;
        end = index + VISIBLE_RANGE + 1;
        mode = 0; // beginning
    }
    else if (index < ARRAY_SIZE - VISIBLE_RANGE) {
        start = index - VISIBLE_RANGE;
        end = index + VISIBLE_RANGE + 1;
        mode = 1; // middle
    }
    else {
        start = index - VISIBLE_RANGE;
        end = ARRAY_SIZE;
        mode = 2; // end
    }

    printf("\n=> ");

    if (mode != 0) {
        printf("... ");
    }

    for (i = start; i < end; i++) {
        if (i == index) {
            printf("[%i] ", array[i]);
        }
        else {
            printf("%i ", array[i]);
        }
    }

    if (mode != 2) {
        printf("...");
    }

    printf("\n");
    free(array);
}

int main(const int argc, const char** argv) {
    if (argc != 2) {
        printf("USAGE: ./bf filename.bf\n");
        return 1;
    }

    FILE* fp = fopen(argv[1], "r");
    char ifile[100000]; // String containing the input

    if (fp == NULL) {
        printf("ERROR: Cannot open input file\n");
        return 1;         
    }

    // Read file until EOF (I know this is a terrible solution but the read kept stopping mid file)
    int k;
    for (k = 0; k < 100000; k++) {
        // if (ifile[k] == 0) break;
        fscanf(fp, "%c", ifile + k);
    }
    fclose(fp);

    interpret(ifile);

    return 0;
}