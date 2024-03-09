#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>

// Definición de la estructura para un nodo de la lista enlazada
struct Node {
    char *line;
    struct Node *next;
};

// Prototipos de funciones
void reversePrint(struct Node *head, FILE *outputFile);
void freeList(struct Node *head);

int main(int argc, char *argv[]) {
    FILE *inputFile = NULL;
    FILE *outputFile = NULL;
    struct Node *head = NULL;
    struct Node *current = NULL;
    char *line = NULL;
    size_t len = 0;
    ssize_t read;
    struct stat inputStat, outputStat;
    int err;

    // Verificación de argumentos de línea de comandos
    if (argc > 3) {
        fprintf(stderr, "usage: reverse <input> <output>\n");
        exit(1);
    }

    // Abrir el archivo de entrada
    if (argc > 1) {
        inputFile = fopen(argv[1], "r");
        if (inputFile == NULL) {
            fprintf(stderr, "reverse: cannot open file '%s'\n", argv[1]);
            exit(1);
        }

        err = stat(argv[1], &inputStat);
        if (err < 0) {
            perror("stat");
            exit(1);
        }
    } else {
        inputFile = stdin; // Utilizar la entrada estándar si no se especifica archivo de entrada
    }

    if (argc == 3) {
        // Abrir el archivo de salida si se especifica
        outputFile = fopen(argv[2], "w");
        if (outputFile == NULL) {
            fprintf(stderr, "error: cannot open file '%s'\n", argv[2]);
            // Liberar la memoria de la lista enlazada antes de salir
            exit(1);
        }

        err = stat(argv[2], &outputStat);
        if (err < 0) {
            perror("stat");
            exit(1);
        }

        if (strcmp(argv[1], argv[2]) == 0 || inputStat.st_ino == outputStat.st_ino) {
            fprintf(stderr, "reverse: input and output file must differ\n");
            exit(EXIT_FAILURE);
        }
    } else {
        outputFile = stdout; // Utilizar la salida estándar si no se especifica archivo de salida
    }

    // Leer el archivo línea por línea y almacenarlo en la lista enlazada
    while ((read = getline(&line, &len, inputFile)) != -1) {
        struct Node *newNode = (struct Node *) malloc(sizeof(struct Node));
        if (newNode == NULL) {
            fprintf(stderr, "malloc failed\n");
            exit(1);
        }
        newNode->line = strdup(line); // Almacenar la línea en el nodo
        newNode->next = NULL;

        if (head == NULL) {
            head = newNode;
            current = newNode;
        } else {
            current->next = newNode;
            current = newNode;
        }
    }

    // Cerrar el archivo de entrada si no es la entrada estándar
    if (inputFile != stdin) {
        fclose(inputFile);
    }

    // Imprimir la lista en orden inverso en el archivo de salida
    reversePrint(head, outputFile);

    // Cerrar el archivo de salida si no es la salida estándar
    if (outputFile != stdout) {
        fclose(outputFile);
    }

    // TODO: Liberar la memoria de la lista enlazada

    // Liberar memoria de la línea
    if (line) {
        free(line);
    }

    return 0;
}

// Función para imprimir la lista en orden inverso
void reversePrint(struct Node *head, FILE *outputFile) {
    if (head == NULL) {
        return;
    }
    reversePrint(head->next, outputFile);
    fprintf(outputFile, "%s", head->line);
}