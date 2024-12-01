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
	int	i;
	t_args_bonus	*args_bonus;

	args_bonus = malloc(sizeof(t_args_bonus));
	if (!args_bonus)
		return (NULL);
	args_bonus->cmd_count = argc - 3;
	args_bonus->cmd_args = malloc(sizeof(char **) * args_bonus->cmd_count);
	if (!args_bonus->cmd_args)
		return (NULL);
	args_bonus->cmd_path = malloc(sizeof(char *) * args_bonus->cmd_count);
	if (!args_bonus->cmd_path)
		return (NULL);
    args_bonus->pipe_fd = malloc(sizeof(int *) * (args_bonus->cmd_count - 1));
	if (!args_bonus->pipe_fd)
		return (NULL);
	i = 0;
	while(i < args_bonus->cmd_count - 1)
	{
		args_bonus->pipe_fd[i] = malloc(sizeof(int) * 2);
		i++;
	}
	i = 0;
	while(i < args_bonus->cmd_count)
	{
		args_bonus->cmd_args[i] = ft_split(argv[i + 2], ' ');
		if (!args_bonus->cmd_args[i][0])
		{
			free_array(args_bonus->cmd_args);
			ft_printf("Error: Non-valid commands\n");
			exit(1);
		}
        args_bonus->cmd_path[i] = build_path(args_bonus->cmd_args[i][0], env);
        if (!args_bonus->cmd_path[i])
		{
			free(args_bonus->cmd_path);
			free(args_bonus);
			ft_printf("Error: Non-valid commands\n");
			exit(1);
		}
	}
	if (open(argv[argc - 1], O_WRONLY | O_TRUNC | O_CREAT, 0777) < 0)
		file_error();
	return (args_bonus);
}

void execute_command(t_args_bonus *args_bonus, int cmd_idx, char **env, char *infile, char *outfile)
{
    if (cmd_idx == 0) // First command
    {
        int infile;

		infile = open(infile, O_RDONLY);
        dup2(infile, STDIN_FILENO);
		close(infile);
        dup2(args_bonus->pipe_fd[0][1], STDOUT_FILENO);
		//close(args_bonus->pipe_fd[0][1]);
    }
    else if (cmd_idx == args_bonus->cmd_count - 1) // Last command
    {
        int outfile;

		outfile = open(outfile, O_WRONLY | O_TRUNC | O_CREAT, 0777);
        dup2(args_bonus->pipe_fd[cmd_idx - 1][0], STDIN_FILENO);
		//close(args_bonus->pipe_fd[cmd_idx - 1][0]);
        dup2(outfile, STDOUT_FILENO);
        close(outfile);
    }
    else // Middle commands
    {
        dup2(args_bonus->pipe_fd[cmd_idx - 1][0], STDIN_FILENO);
		close(args_bonus->pipe_fd[cmd_idx - 1][0]);
        dup2(args_bonus->pipe_fd[cmd_idx][1], STDOUT_FILENO);
		//close(args_bonus->pipe_fd[cmd_idx][1]);
    }

    for (int i = 0; i < args_bonus->cmd_count - 1; i++) // Close all pipes
    {
        close(args_bonus->pipe_fd[i][0]);
        close(args_bonus->pipe_fd[i][1]);
    }
    execve(args_bonus->cmd_path[cmd_idx], args_bonus->cmd_args[cmd_idx], env);
    perror("Execution error");
    exit(1);
}

void create_pipes_and_forks(t_args_bonus *args_bonus, char **argv, int argc, char **env)
{
    for (int i = 0; i < args_bonus->cmd_count - 1; i++)
        pipe(args_bonus->pipe_fd[i]);

    for (int i = 0; i < args_bonus->cmd_count; i++)
    {
        pid_t pid = fork();
        if (pid == 0)
            execute_command(args_bonus, i, env, argv[1], argv[argc - 1]);
    }

    for (int i = 0; i < args_bonus->cmd_count - 1; i++) // Parent closes all pipes
    {
        close(args_bonus->pipe_fd[i][0]);
        close(args_bonus->pipe_fd[i][1]);
    }

    for (int i = 0; i < args_bonus->cmd_count; i++) // Wait for all children
        wait(NULL);
}

void free_args_bonus(t_args_bonus *args_bonus)
{
    for (int i = 0; i < args_bonus->cmd_count; i++)
    {
        free_array(args_bonus->cmd_args[i]);
        free(args_bonus->cmd_path[i]);
    }
    for (int i = 0; i < args_bonus->cmd_count - 1; i++)
        free(args_bonus->pipe_fd[i]);
    free(args_bonus->pipe_fd);
    free(args_bonus->cmd_args);
    free(args_bonus->cmd_path);
    free(args_bonus);
}

int main(int argc, char **argv, char **env)
{
    validate_inputs(argc, argv);
    t_args_bonus *args_bonus = init_args_bonus(argc, argv, env);
    create_pipes_and_forks(args_bonus, argv, argc ,env);
    free_args_bonus(args_bonus);
    return 0;
}
