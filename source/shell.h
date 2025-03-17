#ifndef SHELL_H
#define SHELL_H

#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<readline/readline.h>
#include<readline/history.h>

char	**ft_split(char const *s, char c);
char	*ft_strjoin(const char *s1, const char *s2);


#endif