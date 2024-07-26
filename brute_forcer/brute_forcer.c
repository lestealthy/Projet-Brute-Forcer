#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>
#include "brute_forcer.h"

#define TARGET_LENGTH 3
#define THREAD_COUNT 4

// Structure pour stocker les données de chaque thread
typedef struct {
    char* str;               // La chaîne en cours de génération
    int position;            // La position actuelle dans la chaîne
    const char* targetString; // La chaîne cible à trouver
    char startChar;          // Caractère de départ de la plage
    char endChar;            // Caractère de fin de la plage
} ThreadData;

// Fonction pour lire le contenu du fichier
char* readFile(const char* filename) {
    FILE* file = fopen(filename, "r"); // Ouvrir le fichier en mode lecture
    if (file == NULL) {
        perror("Impossible d'ouvrir le fichier");
        exit(EXIT_FAILURE); // Quitter le programme en cas d'erreur
    }

    // Obtenir la taille du fichier
    fseek(file, 0, SEEK_END);
    long fileSize = ftell(file);
    rewind(file);

    // Allouer de la mémoire pour le contenu du fichier
    char* content = (char*)malloc(fileSize + 1);
    if (content == NULL) {
        perror("Impossible d'allouer de la mémoire");
        exit(EXIT_FAILURE);
    }

    // Lire le fichier dans le tampon
    fread(content, 1, fileSize, file);
    content[fileSize] = '\0'; // Ajouter le caractère de fin de chaîne

    fclose(file);
    return content;
}

// Fonction récursive pour générer des chaînes et les comparer à la chaîne cible
void generateString(char* str, int position, const char* targetString) {
    if (position == TARGET_LENGTH) {
        str[position] = '\0';
        printf("Essai: %s\n", str);
        if (strcmp(str, targetString) == 0) {
            printf("Correspondance trouvée: %s\n", str);
            exit(0); // Quitter le programme si la correspondance est trouvée
        }
        return;
    }

    for (char c = 32; c <= 126; c++) { // Caractères imprimables ASCII
        str[position] = c;
        generateString(str, position + 1, targetString); // Appel récursif pour la prochaine position
    }
}

// Fonction récursive pour la génération multithread
void generateStringMultithreaded(char* str, int position, const char* targetString, char startChar, char endChar) {
    if (position == TARGET_LENGTH) {
        str[position] = '\0';
        printf("Essai: %s\n", str);
        if (strcmp(str, targetString) == 0) {
            printf("Correspondance trouvée: %s\n", str);
            ExitProcess(0); // Quitter le processus entier si la correspondance est trouvée
        }
        return;
    }

    for (char c = startChar; c <= endChar; c++) {
        str[position] = c;
        generateStringMultithreaded(str, position + 1, targetString, startChar, endChar); // Appel récursif pour la prochaine position
    }
}

// Fonction de thread pour générer des chaînes dans une plage de caractères
DWORD WINAPI threadFunction(LPVOID param) {
    ThreadData* data = (ThreadData*)param;
    char* str = data->str;
    int position = data->position;
    const char* targetString = data->targetString;
    char startChar = data->startChar;
    char endChar = data->endChar;

    generateStringMultithreaded(str, position, targetString, startChar, endChar); // Appel de la fonction de génération multithread

    return 0;
}

// Fonction pour l'exécution multithread
void multithreaded_execution() {
    const char* filename = "dummy.txt";
    char* fileContent = readFile(filename); // Lire le contenu du fichier

    // La chaîne cible est supposée être les 60 premiers caractères du fichier
    char targetString[TARGET_LENGTH + 1];
    strncpy(targetString, fileContent, TARGET_LENGTH); // Copier les premiers caractères dans la chaîne cible
    targetString[TARGET_LENGTH] = '\0';
    free(fileContent);

    HANDLE threads[THREAD_COUNT]; // Tableau pour les handles des threads
    ThreadData threadData[THREAD_COUNT]; // Tableau pour les données des threads

    // Définir les plages de caractères pour chaque thread
    char rangeStart = 32; // Début des caractères imprimables ASCII
    char rangeEnd = 126; // Fin des caractères imprimables ASCII
    int rangeSize = (rangeEnd - rangeStart + 1) / THREAD_COUNT; // Taille de la plage de caractères pour chaque thread

    for (int i = 0; i < THREAD_COUNT; i++) {
        threadData[i].str = (char*)malloc(TARGET_LENGTH + 1); // Allouer de la mémoire pour la chaîne
        memset(threadData[i].str, 0, TARGET_LENGTH + 1); // Initialiser la chaîne avec des zéros
        threadData[i].position = 0;
        threadData[i].targetString = targetString;
        threadData[i].startChar = rangeStart + i * rangeSize; // Début de la plage de caractères pour ce thread
        threadData[i].endChar = (i == THREAD_COUNT - 1) ? rangeEnd : rangeStart + (i + 1) * rangeSize - 1; // Fin de la plage de caractères pour ce thread

        threads[i] = CreateThread(NULL, 0, threadFunction, &threadData[i], 0, NULL); // Créer le thread
        if (threads[i] == NULL) {
            perror("Impossible de créer le thread");
            exit(EXIT_FAILURE);
        }
    }

    WaitForMultipleObjects(THREAD_COUNT, threads, TRUE, INFINITE); // Attendre que tous les threads se terminent

    for (int i = 0; i < THREAD_COUNT; i++) {
        CloseHandle(threads[i]); // Fermer le handle du thread
        free(threadData[i].str); // Libérer la mémoire allouée pour la chaîne
    }
}

// Fonction pour l'exécution séquentielle
void sequential_execution() {
    const char* filename = "dummy.txt";
    char* fileContent = readFile(filename); // Lire le contenu du fichier

    char targetString[TARGET_LENGTH + 1];
    strncpy(targetString, fileContent, TARGET_LENGTH); // Copier les premiers caractères dans la chaîne cible
    targetString[TARGET_LENGTH] = '\0';
    free(fileContent);

    char attempt[TARGET_LENGTH + 1] = {0}; // Initialiser la chaîne de tentative avec des zéros
    generateString(attempt, 0, targetString); // Appel de la fonction de génération séquentielle
}
