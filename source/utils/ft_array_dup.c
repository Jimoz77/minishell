#include "../include/minishell.h"

char **ft_array_dup(char **array)
{
    int count = 0;
    char **new;

    while (array && array[count])
        count++;
    
    new = malloc((count + 1) * sizeof(char *));
    if (!new)
        return NULL;

    for (int i = 0; i < count; i++)
        new[i] = ft_strdup(array[i]);
    
    new[count] = NULL;
    return new;
}