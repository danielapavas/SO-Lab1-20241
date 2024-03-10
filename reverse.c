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
    // Declaración de variables para manejo de archivos
    FILE *inputFile = NULL;
    FILE *outputFile = NULL;

    // Declaración de estructuras y variables para generación de lista ligada
    struct Node *head = NULL;
    struct Node *current = NULL;
    char *line = NULL; 

    // Declaración de variables para lectura de archivos
    size_t len = 0;
    ssize_t read;

    //Declaración de variables para información de comando stat
    struct stat inputStat, outputStat;
    int err;

    // Verificación de cantidad de argumentos en la línea de comandos
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
        
        // Llamado a función stat para obtener el inode del archivo de entrada
        err = stat(argv[1], &inputStat);
        if (err < 0) {
            fprintf(stderr, "error: executing stat function for input file");
            exit(1);
        }
    } else {
        inputFile = stdin; // Utilizar la entrada estándar si no se especifica archivo de entrada
    }

    // Validaciones en caso de ejecución con archivos de entrada y salida
    if (argc == 3 ) {
       
        // Abrir el archivo de salida
        outputFile = fopen(argv[2], "w");
        if (outputFile == NULL) {
            fprintf(stderr, "error: cannot open file '%s'\n", argv[2]);
            exit(1);
        }

        // Llamado a función stat para obtener el inode del archivo de salida
        err = stat(argv[2], &outputStat);
        if (err < 0) {
            fprintf(stderr,"error: executing stat function for output file");
            exit(1);
        }

        if (strcmp(argv[1], argv[2]) == 0 || inputStat.st_ino == outputStat.st_ino) {
            fprintf(stderr, "reverse: input and output file must differ\n");
            exit(1);
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

        // Inicializa atributos de estructura del nodo creado
        newNode->line = strdup(line); // Almacenar la línea en el nodo
        newNode->next = NULL;

        // Generando la lista ligada
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

    // Liberar la memoria de la lista enlazada
    freeList(head);

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

// Función para liberar la memoria de la lista enlazada
void freeList(struct Node *head) {
    struct Node *temp;
    while (head != NULL) {
        temp = head;
        head = head->next;
        free(temp->line); // Liberar la memoria de la línea
        free(temp); // Liberar la memoria del nodo
    }
}