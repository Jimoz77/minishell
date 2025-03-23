/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_bonus.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lsadikaj <lsadikaj@student.42lausanne.ch>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/31 03:46:34 by anpayot           #+#    #+#             */
/*   Updated: 2025/02/12 10:28:59 by lsadikaj         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line_bonus.h"

static char	*get_line_from_buffer(char *buffer)
{
	char	*line;
	char	*ptr;
	char	*buf_ptr;
	size_t	len;

	if (!buffer || !*buffer)
		return (NULL);
	buf_ptr = buffer;
	while (*buf_ptr && *buf_ptr != '\n')
		buf_ptr++;
	len = buf_ptr - buffer;
	if (*buf_ptr == '\n')
		len++;
	line = malloc(len + 1);
	if (!line)
		return (NULL);
	ptr = line;
	while (buffer != buf_ptr)
		*ptr++ = *buffer++;
	if (*buffer == '\n')
		*ptr++ = '\n';
	*ptr = '\0';
	return (line);
}

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

char	*get_next_line(int fd)
{
	static char	*buffer[MAX_FILES];
	char		*line;

	if (fd < 0 || fd >= MAX_FILES || BUFFER_SIZE <= 0)
		return (NULL);
	buffer[fd] = read_buffer(fd, buffer[fd]);
	if (!buffer[fd])
		return (NULL);
	line = get_line_from_buffer(buffer[fd]);
	if (!line)
	{
		free_null(buffer[fd]);
		buffer[fd] = NULL;
		return (NULL);
	}
	buffer[fd] = update_buffer(buffer[fd]);
	return (line);
}
