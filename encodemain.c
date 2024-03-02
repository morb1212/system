#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "compress.h"

void writeToFile(const char* fileName, const char* content) {
    FILE* fp = fopen(fileName, "w");
    if (fp == NULL) {
        printf("Error opening file for writing.\n");
        return;
    }
    fprintf(fp, "%s", content);
    fclose(fp);
}

int main() {
    char choice[10];
    char file[100];

    printf("Enter choice: ");
    scanf("%s", choice);
    printf("Enter file name: ");
    scanf("%s", file);

    char* type = strrchr(file, '.');
    if (type != NULL) {
        type++; 
        FILE* fp = fopen(file, "r");
        if (fp == NULL) {
            printf("Error opening file.\n");
            return 1;
        }

        // Read the content of the file
        char content[1000];
        size_t num_read = fread(content, sizeof(char), sizeof(content), fp);
        fclose(fp);
        content[num_read] = '\0'; // Null-terminate the string

        char result[1000] = {0}; // Initialize to zeros

        if (strcmp(choice, "-c") == 0 && strcmp(type, "txt") == 0) {
            // Compression operation
            int num = atoi(content);
            int numdig = numDig(num);
            int even=numdig;
            if (content[0] == '0') {
                strcat(result, "1111");
                even++;
            }
            
            for (int i = numdig - 1; i >= 0; i--) {
                char* now = code_NumToBin(getDigit(num, i));
                strcat(result, now);
            }

            // If numdig is odd, add "0000" to the end
            if (even % 2 != 0) {
                strcat(result, "0000");
            }

            // Write the encoded result to a new file with proper extension
            char outputFileName[100];
            strncpy(outputFileName, file, strlen(file) - strlen(type)); // Remove extension
            outputFileName[strlen(file) - strlen(type)] = '\0'; // Null-terminate
            strcat(outputFileName, "bin");
            writeToFile(outputFileName, result);
        } else if (strcmp(choice, "-d") == 0 && strcmp(type, "bin") == 0) {
            // Decompression operation
            int numdig = strlen(content) / 4;
            int dig = strlen(content);
            for (int i = numdig-1; i >=0 ; i--) { // Change condition to < instead of <=
                char* now = code_BinToNum(&content[i*4]);  // Pass a pointer to the substring
                strcat(result, now);
                memmove(content, content + 4, dig - 4 + 1);
                dig -= 4;
            }

            // Write the decoded result to a new file with proper extension
            char outputFileName[100];
            strncpy(outputFileName, file, strlen(file) - strlen(type)); // Remove extension
            outputFileName[strlen(file) - strlen(type)] = '\0'; // Null-terminate
            strcat(outputFileName, "txt");
            writeToFile(outputFileName, result);
        } else {
            printf("Invalid choice or file type.\n");
            return 1;
        }
    }

    return 0;
}

