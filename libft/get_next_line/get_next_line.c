/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lsadikaj <lsadikaj@student.42lausanne.ch>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/31 03:46:34 by anpayot           #+#    #+#             */
/*   Updated: 2025/02/12 10:29:55 by lsadikaj         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

/**
 * @brief Extracts the next line from the buffer up to newline or end
 * 
 * @param buffer The string buffer to extract the line from
 * @return char* Newly allocated string containing the extracted line, 
 *         or NULL if buffer is empty/allocation fails
 * @note The returned line includes the newline character if present
 */
static char	*get_line_from_buffer(char *buffer)
{
	char	*line;
	char	*ptr;
	char	*buffer_ptr;
	size_t	len;

	if (!buffer || !*buffer)
		return (NULL);
	buffer_ptr = buffer;
	while (*buffer_ptr && *buffer_ptr != '\n')
		buffer_ptr++;
	len = buffer_ptr - buffer;
	if (*buffer_ptr == '\n')
		len++;
	line = malloc(len + 1);
	if (!line)
		return (NULL);
	ptr = line;
	while (buffer != buffer_ptr)
		*ptr++ = *buffer++;
	if (*buffer == '\n')
		*ptr++ = '\n';
	*ptr = '\0';
	return (line);
}

/**
 * @brief Creates a new buffer starting after the first newline character
 * 
 * @param buffer Original buffer to be freed
 * @param newline_pos Pointer to newline character in original buffer
 * @return char* New allocated buffer containing remaining text,
 *         or NULL if allocation fails
 * @note Frees the original buffer before returning
 */
static char	*create_new_buffer(char *buffer, char *newline_pos)
{
	char	*new_buffer;
	char	*ptr;
	size_t	len;

	if (!newline_pos || !*(newline_pos + 1))
		return (free_null(buffer));
	len = gnl_strlen(newline_pos + 1);
	new_buffer = malloc(len + 1);
	if (!new_buffer)
		return (free_null(buffer));
	ptr = new_buffer;
	newline_pos++;
	while (*newline_pos)
		*ptr++ = *newline_pos++;
	*ptr = '\0';
	free(buffer);
	return (new_buffer);
}

/**
 * @brief Updates the static buffer by removing the extracted line
 * 
 * @param buffer Current buffer to update
 * @return char* New buffer with remaining content after newline,
 *         or NULL if no more content/error
 * @note Frees original buffer if no content remains
 */
static char	*update_buffer(char *buffer)
{
	char	*ptr;

	if (!buffer)
		return (NULL);
	ptr = buffer;
	while (*ptr && *ptr != '\n')
		ptr++;
	if (!*ptr)
		return (free_null(buffer));
	return (create_new_buffer(buffer, ptr));
}

/**
 * @brief Reads from file descriptor and appends to buffer until newline or EOF
 * 
 * @param fd File descriptor to read from
 * @param buffer Existing buffer to append to
 * @return char* Updated buffer with new content,
 *         or NULL if read error/allocation fail=s
 * @note Allocates BUFFER_SIZE + 1 bytes for temporary storage
 */
static char	*read_buffer(int fd, char *buffer)
{
	char	*temp;
	int		read_bytes;

	temp = malloc(BUFFER_SIZE + 1);
	if (!temp)
		return (free_null(buffer));
	read_bytes = 1;
	while (!gnl_strchr(buffer, '\n') && read_bytes > 0)
	{
		read_bytes = read(fd, temp, BUFFER_SIZE);
		if (read_bytes == -1)
		{
			free(temp);
			return (free_null(buffer));
		}
		temp[read_bytes] = '\0';
		buffer = gnl_strjoin(buffer, temp);
		if (!buffer)
		{
			free(temp);
			return (NULL);
		}
	}
	free(temp);
	return (buffer);
}

/**
 * @brief Gets the next line from a file descriptor
 * 
 * @param fd File descriptor to read from
 * @return char* Next line from file including newline if present,
 *         or NULL on EOF/error
 * @note Uses static buffer to maintain state between calls
 * @example
 * char *line;
 * while ((line = get_next_line(fd)) != NULL)
 * {
 *     printf("%s", line);
 *     free(line);
 * }
 */
char	*get_next_line(int fd)
{
	static char	*buffer;
	char		*line;

	if (fd < 0 || BUFFER_SIZE <= 0)
		return (NULL);
	buffer = read_buffer(fd, buffer);
	if (!buffer)
		return (NULL);
	line = get_line_from_buffer(buffer);
	if (!line)
	{
		free_null(buffer);
		buffer = NULL;
		return (NULL);
	}
	buffer = update_buffer(buffer);
	return (line);
}
