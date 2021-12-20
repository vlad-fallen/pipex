/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_bonus.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbutter <mbutter@student.21-school.ru>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/16 19:02:41 by mbutter           #+#    #+#             */
/*   Updated: 2021/12/18 14:03:09 by mbutter          ###   ########.fr       */
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
	int		rd;
	int		i;
	char	*buf;

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

void	exec_proc(char *argv, char **envp)
{
	char	**cmd;
	char	*path;

	cmd = ft_split(argv, ' ');
	if (!ft_strchr(cmd[0], '/'))
	{
		path = find_path(cmd[0], envp);
		execve(path, cmd, envp);
		ft_putstr_fd("zsh: command not found: ", 2);
		ft_putendl_fd(cmd[0], 2);
		exit(EXIT_FAILURE);
	}
	else
	{
		path = cmd[0];
		if (access(path, F_OK) == 0)
		{
			execve(path, cmd, envp);
		}
		else
		{
			ft_putstr_fd("zsh: command not found: ", 2);
			ft_putendl_fd(cmd[0], 2);
			exit(EXIT_FAILURE);
		}
	}
}

void	open_fd(char **argv, int *fd_io, int argc, int i)
{
	if (i == 0)
	{
		fd_io[0] = open(argv[1], O_RDONLY);
		fd_io[1] = open(argv[argc - 1], O_CREAT | O_RDWR | O_TRUNC, 0644);
		dup2(fd_io[0], STDIN_FILENO);
		if (fd_io[0] < 0 || fd_io[1] < 0)
		{
			ft_putstr_fd("Error with open files\n", 2);
			exit(EXIT_FAILURE);
		}
	}
	else if (i == 1)
	{
		fd_io[1] = open(argv[argc - 1], O_CREAT | O_RDWR | O_APPEND, 0644);
		if (fd_io[1] < 0)
		{
			ft_putstr_fd("Error with open files\n", 2);
			exit(EXIT_FAILURE);
		}
	}
}
