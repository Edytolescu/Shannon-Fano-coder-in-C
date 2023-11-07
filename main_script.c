#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_CODE_LENGTH 20
#define MAX_SYMBOLS 26

typedef struct {
    int data;
    char code[MAX_CODE_LENGTH];
} CodeTableEntry;

int g_level = 0;

void shannon(int start, int end, int arr[], char code[][MAX_CODE_LENGTH], int level)
{
    int i = start;
    int j = end;
    int isum = arr[i], jsum = arr[j];
    
    if (level > g_level)
        g_level = level;
    
    while (i < (j - 1))
    {
        while (isum > jsum && i < (j - 1))
        {
            j--;
            jsum += arr[j];
        }
        while (isum < jsum && i < (j - 1))
        {
            i++;
            isum += arr[i];
        }
    }
    
    if (i == start)
        code[start][level] = '0';
    else if ((i - start) >= 1)
    {
        int k;
        for (k = start; k <= i; k++)
            code[k][level] = '0';
        shannon(start, i, arr, code, level + 1);
    }
    
    if (j == end)
        code[end][level] = '1';
    else if ((end - j) >= 1)
    {
        int k;
        for (k = start; k <= end; ++k)
            code[k][level] = '1';
        shannon(j, end, arr, code, level + 1);
    }
}

void encode_data(const char* output_file) {
    int arr[MAX_SYMBOLS];
    int freq[MAX_SYMBOLS]; 
    int n, i, j;
    printf("Enter the number of symbols: ");
    scanf("%d", &n);
    for (i = 0; i < n; ++i) {
        printf("Enter symbol: ");
        scanf("%d", &arr[i]);
        freq[arr[i]]++; 
    }
    char code[MAX_SYMBOLS][MAX_CODE_LENGTH];
    for (i = 0; i < n; i++) {
        arr[i] = freq[arr[i]];
    }
    shannon(0, n - 1, arr, code, 0);
    FILE* file = fopen(output_file, "w");
    if (file == NULL) {
        printf("Failed to open the file.\n");
        return;
    }
    fprintf(file, "%d\n", n);
    for (i = 0; i < n; i++) {
        fprintf(file, "%d ", arr[i]);
        for (j = 0; j < n; j++) {
            if (code[i][j] != 'X')
                fprintf(file, "%c", code[i][j]);
        }
        fprintf(file, "\n");
    }
    fclose(file);
    printf("Data encoded and saved to '%s'.\n", output_file);
}

void decode_data(const char* input_file) {
    FILE* file = fopen(input_file, "r");
    if (file == NULL) {
        printf("Failed to open the file.\n");
        return;
    }
    int n, i, j;
    fscanf(file, "%d", &n);
    int arr[20];
    char code[20][MAX_CODE_LENGTH];
    for (i = 0; i < n; i++) {
        fscanf(file, "%d", &arr[i]);
        fscanf(file, "%s", code[i]);
    }
    fclose(file);
    printf("\nDecoded data:\n");
    printf("DATA   CODE\n");
    for (i = 0; i < n; i++) {
        printf("%4d  : %s\n", arr[i], code[i]);
    }
}

int main() {
    const char* encoded_file = "encoded_data.txt";
    encode_data(encoded_file);
    decode_data(encoded_file);
    
    return 0;
}