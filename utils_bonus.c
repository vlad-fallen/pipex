/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_bonus.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbutter <mbutter@student.21-school.ru>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/16 19:02:41 by mbutter           #+#    #+#             */
/*   Updated: 2021/12/16 19:13:30 by mbutter          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex_bonus.h"

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

int	get_next_line(char **line)
{
	int rd;
	int i;
	char *buf;

	i = 0;
	rd = 0;
	buf = (char *)malloc(10000);
	rd = read(0, &buf[i], 1);
	while (rd && buf[i] != '\n')
	{
		i++;
		rd = read(0, &buf[i], 1);
	}
	buf[++i] = '\0';
	*line = buf;
	free(buf);
	return (rd);
}

void exec_proc(int *fd_io, int argc, char **argv, char **envp)
{
	char **arr;

	dup2(fd_io[1], STDOUT_FILENO);
	arr = ft_split(argv[argc - 2], ' ');
	execve(find_path(arr[0], envp), arr, envp);
	ft_putstr_fd("zsh: command not found: ", 2);
	ft_putendl_fd(arr[argc - 2], 2);
	exit(EXIT_FAILURE);
}