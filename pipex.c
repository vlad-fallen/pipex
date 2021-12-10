/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbutter <mbutter@student.21-school.ru>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/04 15:34:53 by mbutter           #+#    #+#             */
/*   Updated: 2021/12/10 19:07:42 by mbutter          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

char *find_path(char *cmd, char **envp)
{
	char **path_envp;
	char *path;
	char *part_path;
	int i;

	i = 0;
	while (ft_strnstr(envp[i], "PATH", 4) == NULL)
		i++;
	path_envp = ft_split(envp[i] + 5, ':');
	i = 0;
	while (path_envp[i])
	{
		part_path = ft_strjoin(path_envp[i], "/");
		path = ft_strjoin(part_path, cmd);
		free(part_path);
		if (access(path, F_OK) == 0)
			return(path);
		free(path);
		i++;
	}
	return (NULL);
}

void child_one(int *fd, int fd1, char **arr1, char **envp)
{
	dup2(fd1, STDIN_FILENO);
	dup2(fd[1], STDOUT_FILENO);
	close(fd[0]);
	close(fd1);
	execve(find_path(arr1[0], envp), arr1, envp);
	ft_putstr_fd("zsh: command not found: ", 2);
	ft_putendl_fd(arr1[0], 2);
	exit(EXIT_FAILURE);
}

void child_two(int *fd, int fd2, char **arr2, char **envp)
{
	dup2(fd2, STDOUT_FILENO);
	dup2(fd[0], STDIN_FILENO);
	close(fd[1]);
	close(fd2);
	execve(find_path(arr2[0], envp), arr2, envp);
	ft_putstr_fd("zsh: command not found: ", 2);
	ft_putendl_fd(arr2[0], 2);
	exit(EXIT_FAILURE);
}

void pipex(int fd1, int fd2, char **arr1, char **arr2, char **envp)
{
	int fd[2];
	int pid1;
	int pid2;

	if (pipe(fd) == -1)
		return ;
	
	pid1 = fork();
	if (pid1 < 0)
		return ;
	if (pid1 == 0)
		child_one(fd, fd1, arr1, envp);
	pid2 = fork();
	if (pid2 < 0)
		return ;
	if (pid2 == 0)
		child_two(fd, fd2, arr2, envp);
	close(fd[0]);
	close(fd[1]);
	waitpid(pid1, NULL, 0);
	waitpid(pid2, NULL, 0);
}

int main(int argc, char **argv, char **envp)
{
	char **arr1 = ft_split(argv[2], ' ');
	char **arr2 = ft_split(argv[3], ' ');
	int fd_in;
	int fd_out;

	if (argc == 5)
	{
		fd_in = open(argv[1], O_RDONLY);
		fd_out = open(argv[4], O_CREAT | O_RDWR | O_TRUNC, 0644);
		if (fd_in < 0 || fd_out < 0)
        	return (-1);
		pipex(fd_in, fd_out, arr1, arr2, envp);
	}
	return (0);
}
