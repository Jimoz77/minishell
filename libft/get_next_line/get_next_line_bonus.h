/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_bonus.h                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lsadikaj <lsadikaj@student.42lausanne.ch>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/31 03:44:18 by anpayot           #+#    #+#             */
/*   Updated: 2025/02/12 10:30:38 by lsadikaj         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef GET_NEXT_LINE_BONUS_H
# define GET_NEXT_LINE_BONUS_H

# include <stdlib.h>
# include <unistd.h>
# include <limits.h>

# ifndef BUFFER_SIZE
#  define BUFFER_SIZE 32
# endif

# ifndef OPEN_MAX
#  define OPEN_MAX 1024
# endif
# ifndef MAX_FILES
#  define MAX_FILES OPEN_MAX
# endif

char	*get_next_line(int fd);
char	*gnl_strchr(const char *s, int c);
char	*gnl_strjoin(char *s1, const char *s2);
size_t	gnl_strlen(const char *s);
char	*free_null(void *ptr);

#endif