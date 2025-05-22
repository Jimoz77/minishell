/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_unclosed_quotes.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lsadikaj <lsadikaj@student.42lausanne.ch>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/21 12:32:19 by lsadikaj          #+#    #+#             */
/*   Updated: 2025/05/21 14:11:26 by lsadikaj         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

// Verifie si l'entree contient des quotes non fermees
static int	has_unclosed_quotes(const char *input)
{
	int	in_single_quote;
	int	in_double_quote;
	int	i;

	in_single_quote = 0;
	in_double_quote = 0;
	i = 0;
	while (input[i])
	{
		if (input[i] == '\'' && !in_double_quote)
			in_single_quote = !in_single_quote;
		else if (input[i] == '"' && !in_single_quote)
			in_double_quote = !in_double_quote;
		i++;
	}
	return (in_single_quote || in_double_quote);
}

// Libere la memoire en cas d'erreur
static char	*cleanup_and_return_null(char *input, char *additional_input)
{
	if (input)
		free(input);
	if (additional_input)
		free(additional_input);
	return (NULL);
}

// Gere la concatenation de l'entree supplementaire
static char	*concat_additional_input(char *input, char *additional_input)
{
	char	*temp;

	temp = ft_strjoin(input, "\n");
	free(input);
	if (!temp)
		return (cleanup_and_return_null(NULL, additional_input));
	input = ft_strjoin(temp, additional_input);
	free(temp);
	free(additional_input);
	if (!input)
		return (NULL);
	return (input);
}

// Lit l'entree supplementaire pour completer les quotes
static char	*read_additional_input(char *input)
{
	char	*additional_input;

	while (has_unclosed_quotes(input))
	{
		additional_input = readline("> ");
		if (!additional_input)
			return (cleanup_and_return_null(input, NULL));
		input = concat_additional_input(input, additional_input);
		if (!input)
			return (NULL);
	}
	return (input);
}

// Fonction principale pour gerer les quotes non fermees
char	*handle_unclosed_quotes(char *input)
{
	if (!input)
		return (NULL);
	if (!has_unclosed_quotes(input))
		return (input);
	return (read_additional_input(input));
}
