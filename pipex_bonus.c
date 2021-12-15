/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_bonus.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbutter <mbutter@student.21-school.ru>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/13 14:49:38 by mbutter           #+#    #+#             */
/*   Updated: 2021/12/15 15:33:26 by mbutter          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex_bonus.h"

static void	child_one(char *argv, char **envp)
{
	int fd[2];
	int	pid1;
	char **arr1;

	arr1 = ft_split(argv, ' ');
	if (pipe(fd) == -1)
		return ;
	pid1 = fork();
	if (pid1 < 0)
		return ;
	if (pid1 == 0)
	{
		dup2(fd[1], STDOUT_FILENO);
		close(fd[0]);
		execve(find_path(arr1[0], envp), arr1, envp);
		ft_putstr_fd("zsh: command not found: ", 2);
		ft_putendl_fd(arr1[0], 2);
		exit(EXIT_FAILURE);
	}
	dup2(fd[0], STDIN_FILENO);
	close(fd[1]);
	waitpid(pid1, NULL, 0);
}

static void here_doc(int *fd_io, int argc, char **argv, char **envp)
{
	int fd[2];
	int pid;
	char *line;
	int i;
	char **arr;

	if (pipe(fd) == -1)
		return ;
	pid = fork();
	if (pid < 0)
		return ;
	if (pid == 0)
	{
		close(fd[0]);
		while(get_next_line(&line))
		{
			if(ft_strncmp(line, argv[2], ft_strlen(argv[2])) == 0)
				exit(EXIT_SUCCESS);
			write(fd[1], line, ft_strlen(line));
		}
	}
	else
	{
		close(fd[1]);
		dup2(fd[0], STDIN_FILENO);
		wait(NULL);
	}
	i = 3;
	while (i < argc - 2)
	{
		child_one(argv[i++], envp);
	}
	dup2(fd_io[1], STDOUT_FILENO);
	arr = ft_split(argv[argc - 2], ' ');
	execve(find_path(arr[0], envp), arr, envp);
	ft_putstr_fd("zsh: command not found: ", 2);
	ft_putendl_fd(arr[argc - 2], 2);
	exit(EXIT_FAILURE);
}

static void	pipex(int *fd_io, int argc, char **argv, char **envp)
{
	int i;
	char **arr;
	
	i = 2;
	dup2(fd_io[0], STDIN_FILENO);
	while (i < argc - 2)
	{
		child_one(argv[i++], envp);
	}
	dup2(fd_io[1], STDOUT_FILENO);
	arr = ft_split(argv[argc - 2], ' ');
	execve(find_path(arr[0], envp), arr, envp);
	ft_putstr_fd("zsh: command not found: ", 2);
	ft_putendl_fd(arr[argc - 2], 2);
	exit(EXIT_FAILURE);
}

int	main(int argc, char **argv, char **envp)
{
	int		fd_io[2];

	if (argc >= 5)
	{
		if (ft_strncmp(argv[1], "here_doc", 8) == 0)
		{
			fd_io[1] = open(argv[argc - 1], O_CREAT | O_RDWR | O_APPEND, 0644);
			here_doc(fd_io, argc, argv, envp);
		}
		else
		{
			fd_io[0] = open(argv[1], O_RDONLY);
			fd_io[1] = open(argv[argc - 1], O_CREAT | O_RDWR | O_TRUNC, 0644);
			if (fd_io[0] < 0 || fd_io[1] < 0)
				return (-1);
			pipex(fd_io, argc, argv, envp);
		}
	}
	return (0);
}

