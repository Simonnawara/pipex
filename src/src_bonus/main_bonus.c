/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_bonus.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sinawara <sinawara@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/23 13:24:36 by sinawara          #+#    #+#             */
/*   Updated: 2024/12/03 13:07:15 by sinawara         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/pipex.h"

// Function that checks if all inputs are valid before proceeding
void	validate_inputs_bonus(int argc, char **argv)
{
	if (argc < 5)
	{
		ft_putendl_fd
		("Error: Use ./pipex file1 cmd1 cmd2 cmd3 ... cmdn file2", 2);
		exit(EXIT_FAILURE);
	}
	if (open(argv[1], O_RDONLY) < 0)
	{
		open(argv[argc - 1], O_WRONLY | O_TRUNC | O_CREAT, 0644);
		file_error_bonus(argv[1]);
	}
	if (open(argv[argc - 1], O_WRONLY | O_TRUNC | O_CREAT, 0644) < 0)
		file_error_bonus(argv[argc - 1]);
}

// Initialize t_args_bonus structure and allocate memory
t_args_bonus	*init_args_bonus(int argc, char **argv, char **env)
{
	t_args_bonus	*args_bonus;
	int				i;

	args_bonus = malloc(sizeof(t_args_bonus));
	if (!args_bonus)
		return (NULL);
	args_bonus->cmd_count = argc - 3;
	args_bonus->cmd_args = malloc(sizeof(char **) * args_bonus->cmd_count);
	args_bonus->cmd_path = malloc(sizeof(char *) * args_bonus->cmd_count);
	if (!args_bonus->cmd_args || !args_bonus->cmd_path)
	{
		free(args_bonus);
		return (NULL);
	}
	i = -1;
	while (++i < args_bonus->cmd_count)
	{
		args_bonus->cmd_args[i] = ft_split(argv[i + 2], ' ');
		if (!args_bonus->cmd_args[i] || !args_bonus->cmd_args[i][0])
			free_and_exit(args_bonus->cmd_path, args_bonus);
		args_bonus->cmd_path[i] = build_path(args_bonus->cmd_args[i][0], env);
		if (!args_bonus->cmd_path[i])
			free_and_exit(args_bonus->cmd_path, args_bonus);
	}
	return (args_bonus);
}

// Function created to save lines in the execute_command function
void	redirect_input(t_args_bonus *args_bonus, char **argv)
{
	int	infile;

	infile = open(argv[1], O_RDONLY);
	dup2(infile, STDIN_FILENO);
	close(infile);
	dup2(args_bonus->pipe_fd[1], STDOUT_FILENO);
}

// Function that executes the command given as arguments
// based on their position (first, middle, last).
void	execute_command(t_args_bonus *args_bonus, int cmd_idx, char **env,
		char **argv)
{
	int	outfile;

	if (cmd_idx == 0)
		redirect_input(args_bonus, argv);
	else if (cmd_idx == args_bonus->cmd_count - 1)
	{
		outfile = open(argv[args_bonus->cmd_count + 2],
				O_WRONLY | O_CREAT | O_TRUNC, 0777);
		dup2(args_bonus->prev_pipe_fd, STDIN_FILENO);
		close(args_bonus->prev_pipe_fd);
		dup2(outfile, STDOUT_FILENO);
		close(outfile);
	}
	else
	{
		dup2(args_bonus->prev_pipe_fd, STDIN_FILENO);
		dup2(args_bonus->pipe_fd[1], STDOUT_FILENO);
	}
	close(args_bonus->pipe_fd[0]);
	close(args_bonus->pipe_fd[1]);
	if (args_bonus->prev_pipe_fd >= 0)
		close(args_bonus->prev_pipe_fd);
	execve(args_bonus->cmd_path[cmd_idx], args_bonus->cmd_args[cmd_idx], env);
	perror("execve failed");
	exit(EXIT_FAILURE);
}

int	main(int argc, char **argv, char **env)
{
	t_args_bonus	*args_bonus;

	validate_inputs_bonus(argc, argv);
	args_bonus = init_args_bonus(argc, argv, env);
	if (!args_bonus)
		return (EXIT_FAILURE);
	create_pipes_and_forks(args_bonus, argv, env);
	free_args_bonus(args_bonus);
	return (0);
}
