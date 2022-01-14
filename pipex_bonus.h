/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_bonus.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbutter <mbutter@student.21-school.ru>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/06 15:25:02 by mbutter           #+#    #+#             */
/*   Updated: 2021/12/20 17:31:30 by mbutter          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PIPEX_BONUS_H
# define PIPEX_BONUS_H

# include <unistd.h>
# include <stdlib.h>
# include <fcntl.h>
# include "./libft/libft.h"
# include <errno.h>

char	*find_path(char *cmd, char **envp);
void	arr_free(char **arr);
int		get_next_line(char **line);
void	exec_proc(char **argv, char **envp);
void	err_arg(int i);

#endif