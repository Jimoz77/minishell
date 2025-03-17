#include <stdio.h>
#include <stdlib.h>
#include <readline/readline.h>
#include <readline/history.h>

int main() {
    char *input;

    while (1) {
        // Afficher un prompt et lire l'entrée utilisateur
        input = readline("minishell> ");

        // Si l'utilisateur tape Ctrl+D (EOF), sortir de la boucle
        if (!input) {
            printf("\n");
            break;
        }

        // Ajouter la commande à l'historique
        if (*input) {
            add_history(input);
        }

        // Traiter la commande (ici, on l'affiche simplement)
        printf("Vous avez saisi : %s\n", input);

        // Libérer la mémoire allouée par readline
        free(input);
    }

    return 0;
}