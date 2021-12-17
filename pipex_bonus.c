/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_bonus.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbutter <mbutter@student.21-school.ru>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/13 14:49:38 by mbutter           #+#    #+#             */
/*   Updated: 2021/12/17 18:45:32 by mbutter          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex_bonus.h"

static void	child_one(char *argv, char **envp)
{
	int		fd[2];
	int		pid;

	if (pipe(fd) == -1)
		return ;
	pid = fork();
	if (pid < 0)
		return ;
	if (pid == 0)
	{
		dup2(fd[1], STDOUT_FILENO);
		close(fd[0]);
		exec_proc(argv, envp);
	}
	dup2(fd[0], STDIN_FILENO);
	close(fd[1]);
	waitpid(pid, NULL, 0);
}

static void	here_doc(char **argv)
{
	int		fd[2];
	int		pid;
	char	*line;

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
	dup2(fd[0], STDIN_FILENO);
	close(fd[1]);
	waitpid(pid, NULL, 0);
}

int	main(int argc, char **argv, char **envp)
{
	int	fd_io[2];
	int	i;

	if (argc >= 5)
	{
		if (ft_strncmp(argv[1], "here_doc", 8) == 0)
		{
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
		dup2(fd_io[1], STDOUT_FILENO);
		exec_proc(argv[argc - 2], envp);
	}
	else
		ft_putstr_fd("Error with arguments\n", 2);
	return (0);
}
