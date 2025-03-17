#include "shell.h"

//? COMENTAIRE EN COULEUR AVEC BETTER COMMENTS \\
//?--------------------------------------------\\

            //! EN ROUGE = A FAIRE \\
            //* EN VERT = INFO \\

//?---------------------------------------------\\



char *cell_read_line(char **env_path) //! env_path n'est pas correctement initialisé
{
    char cwd[BUFSIZ]; //* current working directory
    char *buf;
    int i = 0;
    pid_t pid;
    int status; //* utilisé dans waitpid

    getcwd(cwd, sizeof(cwd)); //* donne le path actuel a cwd
    printf("[~%s]\n", cwd);


    char **result; //* stock la suite de commande ex: "ls -a"

    while(1) //!gerer plus tard condition de sortie
    {
        buf = readline("minishell> "); //* affiche un prompt et lis l'entrée utilisateur
        if(!buf) //! gerer les cas ou : ctrl + d
        {
            printf("\n");
            break;
        }
        if(*buf)
        {
            add_history(buf); //* ajoute la commande entrée dans l'historique
        }
        
        
        result = ft_split(buf,' '); //* sépare les argument de la commande

        pid = fork(); //! je suis pas encore sûr
        if(pid == 0)
        {
            char *dir = ft_strjoin("/bin/", result[0]); //! A CHANGER PAR ALGO QUI TEST TOUT LES PATHS //   pour l instant fonctionne uniquement avec les fonction du path: "/bin/"
            execve(dir , result ,env_path); //! coder fonction pour extraire toutes les env_var dans la variable env_path (nom de var pas claire a changer)
            perror("execve");
            exit(1);
        }
        else if(pid > 0)
        {
            waitpid(pid, &status, 0); //! pas sûr
        }
        else
            perror("fork");


        /*while(result[i]) pour visualiser les args
        {
            printf("%s\n", result[i]);
            i++;
        } */
        free(buf);
    }
return (0);
}



int main (int ac, char **av)
{
    char *path;
    char **all_paths;
    path = getenv("PATH"); //* extrait tout les path de la env_var séparé par des ":"
    char *r1;

    all_paths = ft_split(path,':'); //* sépare tout les paths

    cell_read_line(all_paths);

}

