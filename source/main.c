#include "../include/minishell.h"

int	main(int argc, char **argv, char **envp)
{
	(void)argc;
	(void)argv;
	ft_read_line(envp);
	// Charger l'historique depuis un fichier au démarrage (si existe)

	return (0);
}
