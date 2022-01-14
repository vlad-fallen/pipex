/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_bonus.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbutter <mbutter@student.21-school.ru>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/13 14:49:38 by mbutter           #+#    #+#             */
/*   Updated: 2021/12/20 17:36:06 by mbutter          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex_bonus.h"

static void	child_one(char *argv, char **envp)
{
	int		fd[2];
	int		pid;
	char	**cmd;

	cmd = ft_split(argv, ' ');
	if (cmd[0] == NULL)
		err_arg(2);
	if (pipe(fd) == -1)
		err_arg(3);
	pid = fork();
	if (pid < 0)
		err_arg(4);
	if (pid == 0)
	{
		if (dup2(fd[1], STDOUT_FILENO) < 0)
			err_arg(5);
		close(fd[0]);
		exec_proc(cmd, envp);
	}
	if (dup2(fd[0], STDIN_FILENO) < 0)
		err_arg(5);
	close(fd[1]);
	waitpid(pid, NULL, 0);
}

static void	here_doc(char **argv)
{
	int		fd[2];
	int		pid;
	char	*line;

	if (pipe(fd) == -1)
		err_arg(3);
	pid = fork();
	if (pid < 0)
		err_arg(4);
	if (pid == 0)
	{
		close(fd[0]);
		while (1)
		{
			write(1, "heredoc> ", 9);
			get_next_line(&line);
			if (ft_strncmp(line, argv[2], ft_strlen(argv[2])) == 0)
				exit(EXIT_FAILURE);
			write(fd[1], line, ft_strlen(line));
		}
	}
	if (dup2(fd[0], STDIN_FILENO) < 0)
		err_arg(5);
	close(fd[1]);
	waitpid(pid, NULL, 0);
}

static void	open_fd(char **argv, int *fd_io, int argc, int i)
{
	if (i == 0)
	{
		fd_io[0] = open(argv[1], O_RDONLY);
		fd_io[1] = open(argv[argc - 1], O_CREAT | O_RDWR | O_TRUNC, 0644);
		if (dup2(fd_io[0], STDIN_FILENO) < 0)
			err_arg(5);
		if (fd_io[0] < 0 || fd_io[1] < 0)
			err_arg(1);
	}
	else if (i == 1)
	{
		fd_io[1] = open(argv[argc - 1], O_CREAT | O_RDWR | O_APPEND, 0644);
		if (fd_io[1] < 0)
			err_arg(1);
	}
}

static void	parrent_proc(int fd, int argc, char **argv, char **envp)
{
	char	**cmd;

	cmd = ft_split(argv[argc - 2], ' ');
	if (cmd[0] == NULL)
		err_arg(2);
	if (dup2(fd, STDOUT_FILENO) < 0)
		err_arg(5);
	exec_proc(cmd, envp);
}

int	main(int argc, char **argv, char **envp)
{
	int	fd_io[2];
	int	i;

	if (argc >= 5)
	{
		if (ft_strncmp(argv[1], "here_doc", 8) == 0)
		{
			if (argc < 6)
				err_arg(0);
			here_doc(argv);
			open_fd(argv, fd_io, argc, 1);
			i = 3;
		}
		else
		{
			open_fd(argv, fd_io, argc, 0);
			i = 2;
		}
		while (i < argc - 2)
			child_one(argv[i++], envp);
		parrent_proc(fd_io[1], argc, argv, envp);
	}
	else
		err_arg(0);
	return (0);
}
