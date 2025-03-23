/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.h                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lsadikaj <lsadikaj@student.42lausanne.ch>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/31 03:44:18 by anpayot           #+#    #+#             */
/*   Updated: 2025/02/12 10:30:09 by lsadikaj         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef GET_NEXT_LINE_H
# define GET_NEXT_LINE_H

# include <stdlib.h>
# include <unistd.h>

# ifndef BUFFER_SIZE
#  define BUFFER_SIZE 32
# endif

char	*get_next_line(int fd);
char	*gnl_strchr(const char *s, int c);
char	*gnl_strjoin(char *s1, const char *s2);
size_t	gnl_strlen(const char *s);
char	*free_null(void *ptr);

#endif