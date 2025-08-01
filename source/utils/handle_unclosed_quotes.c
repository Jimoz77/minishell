/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_unclosed_quotes.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jimpa <jimpa@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/21 12:32:19 by lsadikaj          #+#    #+#             */
/*   Updated: 2025/07/22 13:08:02 by jimpa            ###   ########.fr       */
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

// Gere la concatenation de l'entree supplementaire
static char	*concat_additional_input(char *input, char *additional_input)
{
	char	*temp;
	char	*result;

	temp = ft_strjoin(input, "\n");
	if (!temp)
	{
		free(additional_input);
		free(input);
		return (NULL);
	}
	result = ft_strjoin(temp, additional_input);
	free(temp);
	free(additional_input);
	if (!result)
	{
		free(input);
		return (NULL);
	}
	free(input);
	return (result);
}

// Lit l'entree supplementaire pour completer les quotes
static char	*read_additional_input(char *input)
{
	char	*additional_input;
	char	*new_input;

	while (has_unclosed_quotes(input))
	{
		additional_input = readline("> ");
		if (!additional_input)
		{
			free(input);
			return (NULL);
		}
		new_input = concat_additional_input(input, additional_input);
		if (!new_input)
		{
			free(input);
			return (NULL);
		}
		input = new_input;
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
