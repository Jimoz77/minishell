#include "shell.h"

size_t	ft_strlen(const char *str)
{
	int	i;

	i = 0;
	while (str[i])
		i++;
	return (i);
}

char	*ft_strjoin(const char *s1, const char *s2)
{
	char	*point;
	int		i;
	int		j;
	int		totalen;

	j = 0;
	i = 0;
	totalen = ft_strlen(s1) + ft_strlen(s2);
	point = malloc(sizeof(char) * totalen + 1);
	if (!point)
		return (0);
	while (s1[i] != '\0')
	{
		point[i] = s1[i];
		i++;
	}
	while (s2[j] != '\0')
	{
		point[i] = s2[j];
		i++;
		j++;
	}
	point[i] = '\0';
	return (point);
}