/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sinawara <sinawara@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/23 13:25:52 by sinawara          #+#    #+#             */
/*   Updated: 2024/12/02 16:05:54 by sinawara         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PIPEX_H
# define PIPEX_H

# include "../libft/include/ft_printf.h"
# include "../libft/include/get_next_line.h"
# include "../libft/include/libft.h"
# include <fcntl.h>
# include <limits.h>
# include <stdio.h>
# include <stdlib.h>
# include <sys/types.h>
# include <unistd.h>
# include <string.h>

typedef struct s_args
{
	char	*path_cmd1;
	char	*path_cmd2;
	char	**cmd1_args;
	char	**cmd2_args;
	int		pipe_fd[2];
}			t_args;

typedef struct s_args_bonus
{
	int		pipe_fd[2];
	int		cmd_count;
	int		prev_pipe_fd;
	char	**cmd_path;
	char	***cmd_args;
}			t_args_bonus;

typedef struct s_path
{
	char	**paths;
	char	*path_var;
	char	*full_path;
	char	*temp;
}			t_path;

// path.c //
char		*get_path(char **env);
char		*build_path(char *cmd, char **env);

// free_and_error.c //
int			file_error(void);
void		free_array(char **array);
void		*free_and_return(char **array, void *return_value);
void		free_all(char *arg1, char *arg2, char **arr1, char **arr2);
void		free_three(char *arg1, char *arg2, t_args *arg3);
void	free_and_exit(char *arg1, char *arg2);

// main.c //
int			file_error(void);
void		free_array(char **array);
void		*free_and_return(char **array, void *return_value);
void		free_three_bonus(char *arg1, char *arg2, t_args_bonus *arg3);

// bonus.c //
void	validate_inputs(int argc, char **argv);
#endif