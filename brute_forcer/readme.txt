Optimisation de la Correspondance de Chaînes par Force Brute

Aperçu
Ce projet démontre une solution de correspondance de chaînes par force brute qui trouve efficacement une chaîne cible
parmi un ensemble de chaînes possibles. La solution comprend des approches séquentielles et multithread pour optimiser
le temps d'exécution.

Technologies Utilisées
Langage C: Pour l'implémentation des algorithmes de force brute.
API Windows: Pour créer et gérer les threads dans l'approche multithread.
I/O de fichiers: Pour lire la chaîne cible à partir d'un fichier (dummy.txt).

Structure du Projet
main.c: Le fichier principal pour exécuter la méthode choisie (séquentielle ou multithread).
brute_forcer.h: Fichier d'en-tête contenant les prototypes de fonctions.
brute_forcer.c: Implémentation des méthodes de force brute séquentielle et multithread.
Fonctions

Exécution Séquentielle
Fonction: sequential_execution
Description:
Génère toutes les chaînes possibles une par une jusqu'à trouver la chaîne cible. Cette approche utilise un seul thread
et peut être lente pour de grandes chaînes cibles.
Fonction Principale: generateString
Description: Génère récursivement toutes les chaînes possibles d'une longueur spécifiée et les compare à la chaîne cible.

Exécution Multithread
Fonction: multithreaded_execution

Description:
Utilise plusieurs threads pour diviser la charge de travail, améliorant ainsi considérablement le temps d'exécution.
Chaque thread est responsable de générer des chaînes dans une plage de caractères spécifique.

Fonctions Principales:

Fonction de Thread: threadFunction

Description: Fonction exécutée par chaque thread, qui appelle generateStringMultithreaded.
Génération de Chaînes Multithread: generateStringMultithreaded

Description: Semblable à generateString mais inclut des paramètres de plage de caractères pour le multithreading.

