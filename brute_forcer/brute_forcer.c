#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>
#include "brute_forcer.h"

#define TARGET_LENGTH 3
#define THREAD_COUNT 4

// Structure pour stocker les donn�es de chaque thread
typedef struct {
    char* str;               // La cha�ne en cours de g�n�ration
    int position;            // La position actuelle dans la cha�ne
    const char* targetString; // La cha�ne cible � trouver
    char startChar;          // Caract�re de d�part de la plage
    char endChar;            // Caract�re de fin de la plage
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

    // Allouer de la m�moire pour le contenu du fichier
    char* content = (char*)malloc(fileSize + 1);
    if (content == NULL) {
        perror("Impossible d'allouer de la m�moire");
        exit(EXIT_FAILURE);
    }

    // Lire le fichier dans le tampon
    fread(content, 1, fileSize, file);
    content[fileSize] = '\0'; // Ajouter le caract�re de fin de cha�ne

    fclose(file);
    return content;
}

// Fonction r�cursive pour g�n�rer des cha�nes et les comparer � la cha�ne cible
void generateString(char* str, int position, const char* targetString) {
    if (position == TARGET_LENGTH) {
        str[position] = '\0';
        printf("Essai: %s\n", str);
        if (strcmp(str, targetString) == 0) {
            printf("Correspondance trouv�e: %s\n", str);
            exit(0); // Quitter le programme si la correspondance est trouv�e
        }
        return;
    }

    for (char c = 32; c <= 126; c++) { // Caract�res imprimables ASCII
        str[position] = c;
        generateString(str, position + 1, targetString); // Appel r�cursif pour la prochaine position
    }
}

// Fonction r�cursive pour la g�n�ration multithread
void generateStringMultithreaded(char* str, int position, const char* targetString, char startChar, char endChar) {
    if (position == TARGET_LENGTH) {
        str[position] = '\0';
        printf("Essai: %s\n", str);
        if (strcmp(str, targetString) == 0) {
            printf("Correspondance trouv�e: %s\n", str);
            ExitProcess(0); // Quitter le processus entier si la correspondance est trouv�e
        }
        return;
    }

    for (char c = startChar; c <= endChar; c++) {
        str[position] = c;
        generateStringMultithreaded(str, position + 1, targetString, startChar, endChar); // Appel r�cursif pour la prochaine position
    }
}

// Fonction de thread pour g�n�rer des cha�nes dans une plage de caract�res
DWORD WINAPI threadFunction(LPVOID param) {
    ThreadData* data = (ThreadData*)param;
    char* str = data->str;
    int position = data->position;
    const char* targetString = data->targetString;
    char startChar = data->startChar;
    char endChar = data->endChar;

    generateStringMultithreaded(str, position, targetString, startChar, endChar); // Appel de la fonction de g�n�ration multithread

    return 0;
}

// Fonction pour l'ex�cution multithread
void multithreaded_execution() {
    const char* filename = "dummy.txt";
    char* fileContent = readFile(filename); // Lire le contenu du fichier

    // La cha�ne cible est suppos�e �tre les 60 premiers caract�res du fichier
    char targetString[TARGET_LENGTH + 1];
    strncpy(targetString, fileContent, TARGET_LENGTH); // Copier les premiers caract�res dans la cha�ne cible
    targetString[TARGET_LENGTH] = '\0';
    free(fileContent);

    HANDLE threads[THREAD_COUNT]; // Tableau pour les handles des threads
    ThreadData threadData[THREAD_COUNT]; // Tableau pour les donn�es des threads

    // D�finir les plages de caract�res pour chaque thread
    char rangeStart = 32; // D�but des caract�res imprimables ASCII
    char rangeEnd = 126; // Fin des caract�res imprimables ASCII
    int rangeSize = (rangeEnd - rangeStart + 1) / THREAD_COUNT; // Taille de la plage de caract�res pour chaque thread

    for (int i = 0; i < THREAD_COUNT; i++) {
        threadData[i].str = (char*)malloc(TARGET_LENGTH + 1); // Allouer de la m�moire pour la cha�ne
        memset(threadData[i].str, 0, TARGET_LENGTH + 1); // Initialiser la cha�ne avec des z�ros
        threadData[i].position = 0;
        threadData[i].targetString = targetString;
        threadData[i].startChar = rangeStart + i * rangeSize; // D�but de la plage de caract�res pour ce thread
        threadData[i].endChar = (i == THREAD_COUNT - 1) ? rangeEnd : rangeStart + (i + 1) * rangeSize - 1; // Fin de la plage de caract�res pour ce thread

        threads[i] = CreateThread(NULL, 0, threadFunction, &threadData[i], 0, NULL); // Cr�er le thread
        if (threads[i] == NULL) {
            perror("Impossible de cr�er le thread");
            exit(EXIT_FAILURE);
        }
    }

    WaitForMultipleObjects(THREAD_COUNT, threads, TRUE, INFINITE); // Attendre que tous les threads se terminent

    for (int i = 0; i < THREAD_COUNT; i++) {
        CloseHandle(threads[i]); // Fermer le handle du thread
        free(threadData[i].str); // Lib�rer la m�moire allou�e pour la cha�ne
    }
}

// Fonction pour l'ex�cution s�quentielle
void sequential_execution() {
    const char* filename = "dummy.txt";
    char* fileContent = readFile(filename); // Lire le contenu du fichier

    char targetString[TARGET_LENGTH + 1];
    strncpy(targetString, fileContent, TARGET_LENGTH); // Copier les premiers caract�res dans la cha�ne cible
    targetString[TARGET_LENGTH] = '\0';
    free(fileContent);

    char attempt[TARGET_LENGTH + 1] = {0}; // Initialiser la cha�ne de tentative avec des z�ros
    generateString(attempt, 0, targetString); // Appel de la fonction de g�n�ration s�quentielle
}
