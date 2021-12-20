/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbutter <mbutter@student.21-school.ru>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/13 16:56:32 by mbutter           #+#    #+#             */
/*   Updated: 2021/12/20 16:00:33 by mbutter          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

void	arr_free(char **arr)
{
	size_t	i;

	i = 0;
	while (arr[i] != NULL)
	{
		free(arr[i]);
		i++;
	}
	free(arr);
	arr = NULL;
}

char	*find_path(char *cmd, char **envp)
{
	char	**path_envp;
	char	*path;
	char	*part_path;
	int		i;

	i = 0;
	while (ft_strnstr(envp[i], "PATH", 4) == NULL)
		i++;
	path_envp = ft_split(envp[i] + 5, ':');
	i = 0;
	while (path_envp[i])
	{
		part_path = ft_strjoin(path_envp[i++], "/");
		path = ft_strjoin(part_path, cmd);
		free(part_path);
		if (access(path, F_OK) == 0)
		{
			arr_free(path_envp);
			return (path);
		}
		free(path);
	}
	arr_free(path_envp);
	return (NULL);
}

void	err_arg(int i)
{
	if (i == 0)
		ft_putstr_fd("Error with arguments\n", 2);
	else if (i == 1)
		ft_putstr_fd("Error with open files\n", 2);
	else if (i == 2)
		ft_putstr_fd("Error with command\n", 2);
	else if (i == 3)
		ft_putstr_fd("Error with pipe\n", 2);
	else if (i == 4)
		ft_putstr_fd("Error with command\n", 2);
	else if (i == 5)
		ft_putstr_fd("Error with duplicate\n", 2);
	exit(EXIT_FAILURE);
}
