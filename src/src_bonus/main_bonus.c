/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_bonus.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sinawara <sinawara@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/23 13:24:36 by sinawara          #+#    #+#             */
/*   Updated: 2024/12/02 13:04:55 by sinawara         ###   ########.fr       */
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

// Validates the number of arguments and the input file
void validate_inputs(int argc, char **argv)
{
    if (argc < 5)
    {
        ft_printf("Error: Use ./pipex file1 cmd1 cmd2 cmd3 ... cmdn file2\n");
        exit(EXIT_FAILURE);
    }
    if (open(argv[1], O_RDONLY) < 0)
        file_error();
}

// Initialize t_args_bonus structure and allocate memory
t_args_bonus *init_args_bonus(int argc, char **argv, char **env)
{
    t_args_bonus *args_bonus;
    int i;

    args_bonus = malloc(sizeof(t_args_bonus));
    if (!args_bonus)
        return (NULL);

    args_bonus->cmd_count = argc - 3;
    args_bonus->cmd_args = malloc(sizeof(char **) * args_bonus->cmd_count);
    args_bonus->cmd_path = malloc(sizeof(char *) * args_bonus->cmd_count);
    if (!args_bonus->cmd_args || !args_bonus->cmd_path)
    {
        free(args_bonus->cmd_args);
        free(args_bonus->cmd_path);
        free(args_bonus);
        return (NULL);
    }

    // Process and validate each command
    i = 0;
    while (i < args_bonus->cmd_count)
    {
        args_bonus->cmd_args[i] = ft_split(argv[i + 2], ' ');
        if (!args_bonus->cmd_args[i] || !args_bonus->cmd_args[i][0])
        {
            // free_array(args_bonus->cmd_args);
            free(args_bonus->cmd_path);
            free(args_bonus);
            ft_printf("Error: Invalid command\n");
            exit(EXIT_FAILURE);
        }
        args_bonus->cmd_path[i] = build_path(args_bonus->cmd_args[i][0], env);
        if (!args_bonus->cmd_path[i])
        {
            // free_array(args_bonus->cmd_args, args_bonus->cmd_count);
            free(args_bonus->cmd_path);
            free(args_bonus);
            ft_printf("Error: Command not found: %s\n", args_bonus->cmd_args[i][0]);
            exit(EXIT_FAILURE);
        }
        i++;
    }

    // Validate output file access
    if (open(argv[argc - 1], O_WRONLY | O_TRUNC | O_CREAT, 0644) < 0)
    {
        file_error();
        // free_array(args_bonus->cmd_args, args_bonus->cmd_count);
        free(args_bonus->cmd_path);
        free(args_bonus);
        exit(EXIT_FAILURE);
    }

    return (args_bonus);
}

// Executes a single command based on its index
static void execute_command(t_args_bonus *args_bonus, int cmd_idx, 
                            char **env, char **argv, int prev_pipe_fd)
{
    int infile;
    int outfile;

    if (cmd_idx == 0) // First command
    {
        infile = open(argv[1], O_RDONLY);
        dup2(infile, STDIN_FILENO);
        close(infile);
        dup2(args_bonus->pipe_fd[1], STDOUT_FILENO);
    }
    else if (cmd_idx == args_bonus->cmd_count - 1) // Last command
    {
        outfile = open(argv[args_bonus->cmd_count + 2], O_WRONLY | O_CREAT | O_TRUNC, 0644);
        dup2(prev_pipe_fd, STDIN_FILENO);
        close(prev_pipe_fd);
        dup2(outfile, STDOUT_FILENO);
        close(outfile);
    }
    else // Intermediate commands
    {
        dup2(prev_pipe_fd, STDIN_FILENO);
        dup2(args_bonus->pipe_fd[1], STDOUT_FILENO);
    }

    close(args_bonus->pipe_fd[0]);
    close(args_bonus->pipe_fd[1]);
    if (prev_pipe_fd >= 0)
        close(prev_pipe_fd);

    execve(args_bonus->cmd_path[cmd_idx], args_bonus->cmd_args[cmd_idx], env);
    perror("execve failed");
    exit(EXIT_FAILURE);
}

// Creates pipes and forks processes for all commands
static void create_pipes_and_forks(t_args_bonus *args_bonus, char **argv, char **env)
{
    int i;
    int prev_pipe_fd;
    pid_t pid;
    int status;

    prev_pipe_fd = -1;
    i = 0;
    while (i < args_bonus->cmd_count)
    {
        if (i < args_bonus->cmd_count - 1 && pipe(args_bonus->pipe_fd) < 0)
            exit(EXIT_FAILURE);
        pid = fork();
        if (pid < 0)
            exit(EXIT_FAILURE);
        if (pid == 0) // Child process
            execute_command(args_bonus, i, env, argv, prev_pipe_fd);
        if (prev_pipe_fd >= 0) // Parent closes previous pipe
            close(prev_pipe_fd);
        if (i < args_bonus->cmd_count - 1)
            prev_pipe_fd = args_bonus->pipe_fd[0];
        close(args_bonus->pipe_fd[1]); // Parent doesn't need write end
        i++;
    }

    // Wait for all child processes
    i = 0;
    while (i++ < args_bonus->cmd_count)
        wait(&status);
}

// Frees memory used by t_args_bonus
static void free_args_bonus(t_args_bonus *args_bonus)
{
    int i;

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

// Main function
int main(int argc, char **argv, char **env)
{
    t_args_bonus *args_bonus;

    validate_inputs(argc, argv);
    args_bonus = init_args_bonus(argc, argv, env);
    if (!args_bonus)
        return (EXIT_FAILURE);

    create_pipes_and_forks(args_bonus, argv, env);
    free_args_bonus(args_bonus);

    return (0);
}
