/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_bonus.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sinawara <sinawara@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/23 13:24:36 by sinawara          #+#    #+#             */
/*   Updated: 2024/11/28 16:52:26 by sinawara         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/pipex.h"

/*
argv[0] = program name
argv[1] = infile
argv[2] = cmd1
argv[3] = cmd2
argv[4] = outfile
*/

void	handle_child1(t_args_bonus *args_bonus, char *in, char **env)
{
	int	infile;

	infile = open(in, O_RDONLY);
	dup2(infile, STDIN_FILENO);
	close(infile);
	dup2(args_bonus->pipe_fd[1], STDOUT_FILENO);
	close(args_bonus->pipe_fd[0]);
	execve(args_bonus->path_cmd1, args_bonus->cmd1_args, env);
	exit(1);
}

void	handle_child2(t_args_bonus *args_bonus, char *out, char **env)
{
	int	outfile;

	outfile = open(out, O_WRONLY | O_TRUNC | O_CREAT, 0777);
	dup2(args_bonus->pipe_fd[0], STDIN_FILENO);
	close(args_bonus->pipe_fd[1]);
	dup2(outfile, STDOUT_FILENO);
	close(outfile);
	execve(args_bonus->path_cmd2, args_bonus->cmd2_args, env);
	exit(1);
}

void	validate_inputs(int argc, char **argv)
{
	if (argc < 5)
	{
		ft_printf("Error, Use: ./pipex file1 cmd1 cmd2 cmd3 ... cmdn file2\n");
		exit(1);
	}
	if (open(argv[1], O_RDONLY) < 0)
		file_error();
}

t_args_bonus	*init_args_bonus(int argc, char **argv, char **env)
{
	t_args_bonus	*args_bonus;

	args_bonus = malloc(sizeof(t_args_bonus));
	if (!args_bonus)
		return (NULL);
	args_bonus->cmd1_args = ft_split(argv[2], ' ');
	args_bonus->cmd2_args = ft_split(argv[3], ' ');
	if (!args_bonus->cmd1_args[0] || !args_bonus->cmd2_args[0])
	{
		free_array(args_bonus->cmd1_args);
		free_array(args_bonus->cmd2_args);
		ft_printf("Error: Non-valid commands\n");
		exit(1);
	}
	args_bonus->path_cmd1 = build_path(args_bonus->cmd1_args[0], env);
	args_bonus->path_cmd2 = build_path(args_bonus->cmd2_args[0], env);
	if (!args_bonus->path_cmd1 || !args_bonus->path_cmd2)
	{
		free_three_bonus(args_bonus->path_cmd1, args_bonus->path_cmd2, args_bonus);
		ft_printf("Error: Non-valid commands\n");
		exit(1);
	}
	if (open(argv[argc - 1], O_WRONLY | O_TRUNC | O_CREAT, 0777) < 0)
		file_error();
	return (args_bonus);
}

int	main(int argc, char **argv, char **env)
{
	t_args_bonus	*args_bonus;
	pid_t	child1;
	pid_t	child2;

	validate_inputs(argc, argv);
	args_bonus = init_args_bonus(argc, argv, env);
	if (pipe(args_bonus->pipe_fd) < 0)
		return (perror("Pipe Err"), free_all(args_bonus->path_cmd1, args_bonus->path_cmd2,
				args_bonus->cmd1_args, args_bonus->cmd2_args), free(args_bonus), exit(1), 0);
	child1 = fork();
	if (child1 == 0)
		handle_child1(args_bonus, argv[1], env);
	child2 = fork();
	if (child2 == 0)
		handle_child2(args_bonus, argv[4], env);
	if (ft_strncmp(args_bonus->cmd1_args[0], "sleep", 5) == 0
		|| ft_strncmp(args_bonus->cmd2_args[0], "sleep", 5) == 0)
	{
		waitpid(child1, NULL, 0);
		waitpid(child2, NULL, 0);
	}
	free_all(args_bonus->path_cmd1, args_bonus->path_cmd2, args_bonus->cmd1_args,
		args_bonus->cmd2_args);
	exit(1);
}
