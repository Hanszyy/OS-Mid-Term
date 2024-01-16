#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define MAX_CUSTOMERS 100

// Create customer in a format: customer# service_type
struct Customer {
    int number;
    char service_type;
};

int main() {
    struct Customer customers[MAX_CUSTOMERS];
    srand(time(NULL));

    // Generate the customers randomly
    for (int i = 0; i < MAX_CUSTOMERS; i++) {
        customers[i].number = i + 1;
        
        // Choose services type from W, D, I
        int rand_num = rand() % 3; 
        if (rand_num == 0) {
            customers[i].service_type = 'W';
        } else if (rand_num == 1) {
            customers[i].service_type = 'D';
        } else {
            customers[i].service_type = 'I';
        }
    }

    // Write the customers to the output c_file
    FILE* file = fopen("c_file.txt", "w");
    if (file == NULL) {
        printf("Error: file is not available!.\n");
        return 1;
    }

    for (int i = 0; i < MAX_CUSTOMERS; i++) {
        fprintf(file, "%d %c \n", customers[i].number, customers[i].service_type);
    }

    fclose(file);

    return 0;
}

/* How to compile:
    gcc -o create_cFile create_cFile.c
    ./create_cFile
*/