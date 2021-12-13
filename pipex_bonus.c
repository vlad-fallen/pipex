/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_bonus.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbutter <mbutter@student.21-school.ru>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/13 14:49:38 by mbutter           #+#    #+#             */
/*   Updated: 2021/12/13 17:01:04 by mbutter          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex_bonus.h"

static void	pipex(int fd_in, int fd_out, char **argv, char **envp)
{
	int fd[2];

	if (pipe(fd) == -1)
		return ;
}

int	main(int argc, char **argv, char **envp)
{
	int		fd_in;
	int		fd_out;

	if (argc >= 5)
	{
		fd_in = open(argv[1], O_RDONLY);
		fd_out = open(argv[4], O_CREAT | O_RDWR | O_TRUNC, 0644);
		if (fd_in < 0 || fd_out < 0)
			return (-1);
		pipex(fd_in, fd_out, argv, envp);
	}
	return (0);
}