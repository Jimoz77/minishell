/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor_cmd_utils3.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lsadikaj <lsadikaj@student.42lausanne.ch>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/10 17:17:11 by lsadikaj          #+#    #+#             */
/*   Updated: 2025/06/27 18:20:23 by lsadikaj         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

static t_token	*find_node_tokens(t_node *node, t_shell *shell)
{
	t_token	*current;
	
	if (!node->cmd || !node->cmd[0] || !shell->tokens)
		return (NULL);
		
	current = shell->tokens;
	while (current)
	{
		if (current->type == TOKEN_WORD && current->value && 
			ft_strcmp(current->value, node->cmd[0]) == 0)
			return (current);
		current = current->next;
	}
	return (NULL);
}

static int	process_cmd_expansion(t_token *temp_tokens, t_shell *shell)
{
	t_token	*current;
	char	*unquoted_value;

	current = temp_tokens;
	while (current)
	{
		// Expansion des variables (sauf dans les single quotes)
		if (!current->parts || (current->parts
				&& current->parts->type != QUOTE_SINGLE))
		{
			shell->tokens = temp_tokens;
			scan_envar_execution_phase(shell, current);
		}
		
		// Suppression des quotes après expansion
		if (current->parts)
		{
			unquoted_value = build_unquoted_value(current->parts);
			if (unquoted_value)
			{
				free(current->value);
				current->value = unquoted_value;
			}
		}
		current = current->next;
	}
	return (1);
}

static int	expand_cmd_with_original_tokens(t_node *node, t_shell *shell)
{
	t_token	*cmd_tokens;
	t_token	*current;
	char	*unquoted_value;
	t_token	*original_tokens;

	cmd_tokens = find_node_tokens(node, shell);
	if (!cmd_tokens)
		return (-1);
	
	// Sauvegarder les tokens originaux
	original_tokens = shell->tokens;
	
	// Expansion des variables sur le token trouvé et suivants
	current = cmd_tokens;
	while (current && current->type == TOKEN_WORD)
	{
		if (!current->parts || current->parts->type != QUOTE_SINGLE)
			scan_envar_execution_phase(shell, current);
		current = current->next;
	}
	
	// Suppression des quotes
	current = cmd_tokens;
	while (current && current->type == TOKEN_WORD)
	{
		if (current->parts)
		{
			unquoted_value = build_unquoted_value(current->parts);
			if (unquoted_value)
			{
				free(current->value);
				current->value = unquoted_value;
			}
		}
		current = current->next;
	}
	
	// Restaurer
	shell->tokens = original_tokens;
	update_cmd_from_tokens(node->cmd, cmd_tokens);
	return (0);
}

static int	handle_no_tokens_case(t_node *node, char ***envp, t_shell *shell)
{
	if (ft_is_builtin(node->cmd, envp))
		return (exec_builtin_with_redirections(node, envp));
	else
		return (exec_cmd_with_redirections(node, *envp, shell));
}

int	exec_external(char **cmd, char **envp)
{
	char	*path;
	int		special;

	if (!cmd || !cmd[0])
		return (1);
	special = handle_special_commands(cmd);
	if (special)
		return (special);
	path = handle_direct_path(cmd[0]);
	if (!path)
		path = ft_path_finder(cmd[0], &envp);
	if (!path)
	{
		ft_putstr_fd("minishell: ", 2);
		ft_putstr_fd(cmd[0], 2);
		ft_putendl_fd(": command not found", 2);
		return (127);
	}
	execve(path, cmd, envp);
	perror("minishell: execve");
	free(path);
	return (126);
}

int	exec_builtin_with_redirections(t_node *node, char ***envp)
{
	t_redirect	red;
	int			result;

	if (!node->redirections)
		return (execute_builtin(node->cmd, envp));
	init_redirect(&red);
	if (!apply_node_redirections(node, &red))
	{
		close_redirect_fds(&red);
		restore_std_fds(&red);
		return (1);
	}
	result = execute_builtin(node->cmd, envp);
	close_redirect_fds(&red);
	restore_std_fds(&red);
	return (result);
}

int	process_cmd_tokens(t_node *node, char ***envp, t_shell *shell)
{
	t_token		*original_tokens;
	t_token		*temp_tokens;
	int			result;

	// D'abord, essayer avec les tokens originaux (pour préserver les quotes)
	if (expand_cmd_with_original_tokens(node, shell) == 0)
	{
		// Si ça a marché, exécuter avec la commande mise à jour
		if (ft_is_builtin(node->cmd, envp))
			return (exec_builtin_with_redirections(node, envp));
		else
			return (exec_cmd_with_redirections(node, *envp, shell));
	}
	
	// Si on n'a pas trouvé les tokens originaux, utiliser l'ancienne méthode
	// (création de nouveaux tokens - utile pour les cas comme l$VAR)
	original_tokens = shell->tokens;
	temp_tokens = create_tokens_from_cmd(node->cmd, shell);
	if (!temp_tokens)
	{
		shell->tokens = original_tokens;
		return (handle_no_tokens_case(node, envp, shell));
	}
	
	// Appliquer l'expansion sur les tokens temporaires
	process_cmd_expansion(temp_tokens, shell);
	shell->tokens = original_tokens;
	
	// Mettre à jour la commande avec les nouveaux tokens
	update_cmd_from_tokens(node->cmd, temp_tokens);
	
	// Exécuter la commande
	if (ft_is_builtin(node->cmd, envp))
		result = exec_builtin_with_redirections(node, envp);
	else
		result = exec_cmd_with_redirections(node, *envp, shell);
		
	if (temp_tokens)
		free_tokens(temp_tokens);
	return (result);
}