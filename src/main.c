/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sinawara <sinawara@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/23 13:24:36 by sinawara          #+#    #+#             */
/*   Updated: 2024/12/03 13:06:57 by sinawara         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/pipex.h"

// Function that checks if all inputs are valid before proceeding
void	validate_inputs(int argc, char **argv)
{
	if (argc != 5)
	{
		ft_putendl_fd("Error, Use: './pipex file1 cmd1 cmd2 file2'", 2);
		exit(EXIT_FAILURE);
	}
	if (open(argv[1], O_RDONLY) < 0)
	{
		open(argv[argc - 1], O_WRONLY | O_TRUNC | O_CREAT, 0644);
		file_error(argv[1]);
	}
	if (open(argv[4], O_WRONLY | O_TRUNC | O_CREAT, 0777) < 0)
		file_error(argv[4]);
}

// Handles the first child created by the fork process
// The first child corresponds to the first command given in the arguments
void	handle_child1(t_args *args, char *in, char **env)
{
	int	infile;

	infile = open(in, O_RDONLY);
	dup2(infile, STDIN_FILENO);
	close(infile);
	dup2(args->pipe_fd[1], STDOUT_FILENO);
	close(args->pipe_fd[0]);
	execve(args->path_cmd1, args->cmd1_args, env);
	exit(EXIT_SUCCESS);
}

// Same process as child1
void	handle_child2(t_args *args, char *out, char **env)
{
	int	outfile;

	outfile = open(out, O_WRONLY | O_TRUNC | O_CREAT, 0777);
	dup2(args->pipe_fd[0], STDIN_FILENO);
	close(args->pipe_fd[1]);
	dup2(outfile, STDOUT_FILENO);
	close(outfile);
	execve(args->path_cmd2, args->cmd2_args, env);
	exit(EXIT_SUCCESS);
}

// Initializes the arguments structure
t_args	*init_args(char **argv, char **env)
{
	t_args	*args;

	args = malloc(sizeof(t_args));
	if (!args)
		return (NULL);
	args->cmd1_args = ft_split(argv[2], ' ');
	args->cmd2_args = ft_split(argv[3], ' ');
	if (!args->cmd1_args[0] || !args->cmd2_args[0])
	{
		free_array(args->cmd1_args);
		free_array(args->cmd2_args);
		ft_putendl_fd("Error: command not found", 2);
		exit(EXIT_FAILURE);
	}
	args->path_cmd1 = build_path(args->cmd1_args[0], env);
	args->path_cmd2 = build_path(args->cmd2_args[0], env);
	if (!args->path_cmd1 || !args->path_cmd2)
	{
		free_three(args->path_cmd1, args->path_cmd2, args);
		ft_putendl_fd("Error: command not found", 2);
		exit(EXIT_FAILURE);
	}
	return (args);
}

int	main(int argc, char **argv, char **env)
{
	t_args	*args;
	pid_t	child1;
	pid_t	child2;

	validate_inputs(argc, argv);
	args = init_args(argv, env);
	if (pipe(args->pipe_fd) < 0)
		return (perror("Pipe Err"), free_all(args->path_cmd1, args->path_cmd2,
				args->cmd1_args, args->cmd2_args), free(args), exit(1), 0);
	child1 = fork();
	if (child1 == 0)
		handle_child1(args, argv[1], env);
	child2 = fork();
	if (child2 == 0)
		handle_child2(args, argv[4], env);
	close(args->pipe_fd[0]);
	close(args->pipe_fd[1]);
	waitpid(child1, NULL, 0);
	waitpid(child2, NULL, 0);
	free_all(args->path_cmd1, args->path_cmd2, args->cmd1_args,
		args->cmd2_args);
	free(args);
	exit(0);
}
