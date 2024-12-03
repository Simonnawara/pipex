/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bonus_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sinawara <sinawara@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/27 14:11:11 by sinawara          #+#    #+#             */
/*   Updated: 2024/12/03 10:11:14 by sinawara         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/pipex.h"

void	create_pipes_and_forks(t_args_bonus *args_bonus, char **argv,
		char **env)
{
	int		i;
	pid_t	pid;
	int		status;

	args_bonus->prev_pipe_fd = -1;
	i = -1;
	while (++i < args_bonus->cmd_count)
	{
		if (i < args_bonus->cmd_count - 1 && pipe(args_bonus->pipe_fd) < 0)
			exit(EXIT_FAILURE);
		pid = fork();
		if (pid < 0)
			exit(EXIT_FAILURE);
		if (pid == 0)
			execute_command(args_bonus, i, env, argv);
		if (args_bonus->prev_pipe_fd >= 0)
			close(args_bonus->prev_pipe_fd);
		if (i < args_bonus->cmd_count - 1)
			args_bonus->prev_pipe_fd = args_bonus->pipe_fd[0];
		close(args_bonus->pipe_fd[1]);
	}
	i = 0;
	while (i++ < args_bonus->cmd_count)
		wait(&status);
}

// Frees memory used by t_args_bonus
void	free_args_bonus(t_args_bonus *args_bonus)
{
	int	i;

	i = 0;
	while (i < args_bonus->cmd_count)
	{
		free_array(args_bonus->cmd_args[i]);
		free(args_bonus->cmd_path[i]);
		i++;
	}
	free(args_bonus->cmd_args);
	free(args_bonus->cmd_path);
	free(args_bonus);
}

void	free_and_exit(void *arg1, void *arg2)
{
	free(arg1);
	free(arg2);
	ft_printf("Error: Invalid command\n");
	exit(EXIT_FAILURE);
}
