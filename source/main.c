#include "minishell.h"

int	main(int argc, char **argv, char **envp)
{
	char	*input;

	(void)argc;
	(void)argv;

	// Charger l'historique depuis un fichier au démarrage (si existe)
	read_history(".minishell_history");

	while (1)
	{
		input = readline("minishell> ");
		if (!input)
		{
			printf("\n");
			break;
		}
		if (*input)
			add_history(input);
		ft_printf("Vous avez saisi : %s\n", input);
		free(input);
	}

	int i = 0;
	while (envp[i])
		printf("%s", envp[i]);

	// Sauvegarder l'historique à la fin de la session
	write_history(".minishell_history");

	return (0);
}
