/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_bonus.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbutter <mbutter@student.21-school.ru>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/13 14:49:38 by mbutter           #+#    #+#             */
/*   Updated: 2021/12/16 20:37:55 by mbutter          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex_bonus.h"

static void	child_one(char *argv, char **envp)
{
	int		fd[2];
	int		pid1;

	if (pipe(fd) == -1)
		return ;
	pid1 = fork();
	if (pid1 < 0)
		return ;
	if (pid1 == 0)
	{
		dup2(fd[1], STDOUT_FILENO);
		close(fd[0]);
		exec_proc(argv, envp);
	}
	dup2(fd[0], STDIN_FILENO);
	close(fd[1]);
	waitpid(pid1, NULL, 0);
}

static void	here_doc(char **argv, int *fd_io, int argc)
{
	int		fd[2];
	int		pid;
	char	*line;

	fd_io[1] = open(argv[argc - 1], O_CREAT | O_RDWR | O_APPEND, 0644);
	if (pipe(fd) == -1)
		return ;
	pid = fork();
	if (pid < 0)
		return ;
	if (pid == 0)
	{
		close(fd[0]);
		while (get_next_line(&line))
		{
			if (ft_strncmp(line, argv[2], ft_strlen(argv[2])) == 0)
				exit(EXIT_SUCCESS);
			write(fd[1], line, ft_strlen(line));
		}
	}
	close(fd[1]);
	dup2(fd[0], STDIN_FILENO);
	wait(NULL);
}

static void	pipex(int *fd_io, int argc, char **argv)
{
	fd_io[0] = open(argv[1], O_RDONLY);
	fd_io[1] = open(argv[argc - 1], O_CREAT | O_RDWR | O_TRUNC, 0644);
	dup2(fd_io[0], STDIN_FILENO);
}

int	main(int argc, char **argv, char **envp)
{
	int	fd_io[2];
	int	i;

	if (argc >= 5)
	{
		if (ft_strncmp(argv[1], "here_doc", 8) == 0)
		{
			here_doc(argv, fd_io, argc);
			i = 3;
		}
		else
		{
			pipex(fd_io, argc, argv);
			if (fd_io[0] < 0 || fd_io[1] < 0)
				return (-1);
			i = 2;
		}
		while (i < argc - 2)
			child_one(argv[i++], envp);
		dup2(fd_io[1], STDOUT_FILENO);
		exec_proc(argv[argc - 2], envp);
	}
	else
		ft_putstr_fd("Error with arguments\n", 2);
	return (0);
}
