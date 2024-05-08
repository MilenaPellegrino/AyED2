#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#define MAX_SIZE 1000

static void dump(char a[], unsigned int length) {
    printf("\"");
    for (unsigned int j=0u; j < length; j++) {
        printf("%c", a[j]);
    }
    printf("\"");
    printf("\n\n");
}
unsigned int data_from_file(const char *path, unsigned int indexes[], char letters[], unsigned int max_size){
    FILE *file = NULL;
    file = fopen(path, "r");
    if (file == NULL) {
        fprintf(stderr, "File does not exist.\n");
    }
    unsigned int i = 0;
    while(!feof(file) && i<max_size){
        int res = fscanf(file, "%u -> *%c* ", &indexes[i], &letters[i]);
        if (res != 2){
            fprintf(stderr, "Invalid file\n");
            exit(EXIT_FAILURE);
        }
        i++;
    }
    fclose(file);
    return i;
}

char *parse_filepath(int argc, char *argv[]) {
    char *result = NULL;
    bool valid_args_count = (argc == 2);

    if (!valid_args_count) {
        exit(EXIT_FAILURE);
    }

    result = argv[1];

    return result;
}
void asorted(unsigned int indexes[], char letters[], unsigned int length, char sorted[]){
    unsigned int i=0;
    while(i<length && i<MAX_SIZE){
        sorted[indexes[i]] = letters[i]; 
        i++; 
    }
}
int main(int argc, char *argv[]) {
    char *filepath = NULL;
    filepath = parse_filepath(argc, argv);
    unsigned int indexes[MAX_SIZE];
    char letters[MAX_SIZE];
    char sorted[MAX_SIZE];
    unsigned int length=0; 
    length = data_from_file(filepath, indexes, letters, MAX_SIZE);
    asorted(indexes, letters, length, sorted);
    dump(sorted, length);

    return EXIT_SUCCESS;
}

