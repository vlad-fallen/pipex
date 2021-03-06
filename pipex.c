/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbutter <mbutter@student.21-school.ru>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/04 15:34:53 by mbutter           #+#    #+#             */
/*   Updated: 2021/12/20 16:01:58 by mbutter          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

static void	proc_exec(char **cmd, char **envp)
{
	char	*path;

	if (!ft_strchr(cmd[0], '/'))
	{
		path = find_path(cmd[0], envp);
		execve(path, cmd, envp);
		ft_putstr_fd("pipex: command not found: ", 2);
		ft_putendl_fd(cmd[0], 2);
		exit(EXIT_FAILURE);
	}
	else
	{
		path = cmd[0];
		if (access(path, F_OK) == 0)
			execve(path, cmd, envp);
		else
		{
			ft_putstr_fd("pipex: command not found: ", 2);
			ft_putendl_fd(cmd[0], 2);
			exit(EXIT_FAILURE);
		}
	}
}

static void	child_one(int *fd, int fd_in, char **arr1, char **envp)
{
	int	pid1;

	pid1 = fork();
	if (pid1 < 0)
		err_arg(4);
	if (pid1 == 0)
	{
		if (dup2(fd_in, STDIN_FILENO) < 0)
			err_arg(5);
		if (dup2(fd[1], STDOUT_FILENO) < 0)
			err_arg(5);
		close(fd[0]);
		close(fd_in);
		proc_exec(arr1, envp);
	}
	close(fd[1]);
	waitpid(pid1, NULL, 0);
}

static void	child_two(int *fd, int fd_out, char **arr2, char **envp)
{
	int	pid2;

	pid2 = fork();
	if (pid2 < 0)
		err_arg(4);
	if (pid2 == 0)
	{
		if (dup2(fd_out, STDOUT_FILENO) < 0)
			err_arg(5);
		if (dup2(fd[0], STDIN_FILENO) < 0)
			err_arg(5);
		close(fd[1]);
		close(fd_out);
		proc_exec(arr2, envp);
	}
	close(fd[0]);
	waitpid(pid2, NULL, 0);
}

static void	pipex(int fd_in, int fd_out, char **argv, char **envp)
{
	int		fd[2];
	char	**arr1;
	char	**arr2;

	arr1 = ft_split(argv[2], ' ');
	arr2 = ft_split(argv[3], ' ');
	if (arr1[0] == NULL || arr2[0] == NULL)
		err_arg(2);
	if (pipe(fd) == -1)
		err_arg(3);
	child_one(fd, fd_in, arr1, envp);
	child_two(fd, fd_out, arr2, envp);
	close(fd[0]);
	close(fd[1]);
	arr_free(arr1);
	arr_free(arr2);
}

int	main(int argc, char **argv, char **envp)
{
	int		fd_in;
	int		fd_out;

	if (argc == 5)
	{
		fd_in = open(argv[1], O_RDONLY);
		fd_out = open(argv[4], O_CREAT | O_RDWR | O_TRUNC, 0644);
		if (fd_in < 0 || fd_out < 0)
			err_arg(1);
		pipex(fd_in, fd_out, argv, envp);
	}
	else
		err_arg(0);
	return (0);
}
