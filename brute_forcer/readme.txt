Optimisation de la Correspondance de Cha�nes par Force Brute

Aper�u
Ce projet d�montre une solution de correspondance de cha�nes par force brute qui trouve efficacement une cha�ne cible
parmi un ensemble de cha�nes possibles. La solution comprend des approches s�quentielles et multithread pour optimiser
le temps d'ex�cution.

Technologies Utilis�es
Langage C: Pour l'impl�mentation des algorithmes de force brute.
API Windows: Pour cr�er et g�rer les threads dans l'approche multithread.
I/O de fichiers: Pour lire la cha�ne cible � partir d'un fichier (dummy.txt).

Structure du Projet
main.c: Le fichier principal pour ex�cuter la m�thode choisie (s�quentielle ou multithread).
brute_forcer.h: Fichier d'en-t�te contenant les prototypes de fonctions.
brute_forcer.c: Impl�mentation des m�thodes de force brute s�quentielle et multithread.
Fonctions

Ex�cution S�quentielle
Fonction: sequential_execution
Description:
G�n�re toutes les cha�nes possibles une par une jusqu'� trouver la cha�ne cible. Cette approche utilise un seul thread
et peut �tre lente pour de grandes cha�nes cibles.
Fonction Principale: generateString
Description: G�n�re r�cursivement toutes les cha�nes possibles d'une longueur sp�cifi�e et les compare � la cha�ne cible.

Ex�cution Multithread
Fonction: multithreaded_execution

Description:
Utilise plusieurs threads pour diviser la charge de travail, am�liorant ainsi consid�rablement le temps d'ex�cution.
Chaque thread est responsable de g�n�rer des cha�nes dans une plage de caract�res sp�cifique.

Fonctions Principales:

Fonction de Thread: threadFunction

Description: Fonction ex�cut�e par chaque thread, qui appelle generateStringMultithreaded.
G�n�ration de Cha�nes Multithread: generateStringMultithreaded

Description: Semblable � generateString mais inclut des param�tres de plage de caract�res pour le multithreading.

