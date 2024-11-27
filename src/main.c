/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sinawara <sinawara@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/23 13:24:36 by sinawara          #+#    #+#             */
/*   Updated: 2024/11/27 11:11:38 by sinawara         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/pipex.h"

/* 
argv[0] = program name
argv[1] = infile
argv[2] = cmd1
argv[3] = cmd2
argv[4] = outfile
*/

int file_error(void)
{
	perror("Error opening the file ");
	exit (1);
}

// Frees any array that is passed as argument
void free_array(char **array)
{
	int i;
	
	i = 0;
	if(!array)
		return ;
	while(array[i])
	{
		free(array[i]);
		i++;
	}
	free(array);
}

// Function purely created to save lines
void *free_and_return(char **array, void *return_value)
{
    free_array(array);
    return (return_value);
}

int main(int argc, char **argv, char **env)
{
	int infile;
	int outfile;
	int pipe_fd[2];
	char *path_cmd1;
	char *path_cmd2;
	char **cmd1_args;
	char **cmd2_args;
	pid_t child1;
	pid_t child2;
	
	if (argc != 5)
	{
		ft_printf("Error -> usage : [ ./pipex file1 cmd1 cmd2 file2 ]\n");
		return (1);
	}
	else
	{
		if(open(argv[1], O_RDONLY) < 0)		// check if infile exists ✔
			file_error();
		if ((open(argv[4], O_WRONLY | O_TRUNC | O_CREAT, 0777 )) < 0) // create an outfile if it doesn't exist ✔
				file_error();
				
		cmd1_args = ft_split(argv[2], ' ');
		cmd2_args = ft_split(argv[3], ' ');
		
	

		path_cmd1 = build_path(cmd1_args[0], env);
		path_cmd2 = build_path(cmd2_args[0], env);
		
		if (!path_cmd1 || !path_cmd2 || !cmd1_args || !cmd2_args)
    	{
        	free(path_cmd1);
        	free(path_cmd2);
        	free_array(cmd1_args);
        	free_array(cmd2_args);
        	file_error();
    	}

		pipe(pipe_fd);
		child1 = fork();
		if (child1 == 0)
		{
			printf("Child1 has been created\n");
        	infile = open(argv[1], O_RDONLY);
        	dup2(infile, STDIN_FILENO); // Redirect stdin to infile
    		close(infile);
			dup2(pipe_fd[1], STDOUT_FILENO);
			close(pipe_fd[0]);

        	// Execute cmd1
        	execve(path_cmd1, cmd1_args, env);
        	perror("execve"); // If execve fails
			
        	exit(1);
		}
		
		child2 = fork();
    	if (child2 == 0)
    	{
			printf("Child2 has been created\n");
			outfile = open(argv[4], O_WRONLY | O_TRUNC | O_CREAT, 0777);
			dup2(pipe_fd[0], STDIN_FILENO);
			close(pipe_fd[1]);
        	dup2(outfile, STDOUT_FILENO); // Redirect stdout to outfile
        	close(outfile);
			
        	execve(path_cmd2, cmd2_args, env);
        	perror("execve"); // If execve fails
        	exit(1);
    	}
		
		printf("%s\n", cmd1_args[0]);
		printf("%s\n", cmd2_args[0]);
		if (ft_strncmp(cmd1_args[0], "sleep", 5) == 0 || ft_strncmp(cmd2_args[0], "sleep", 5) == 0)
		{
			printf("hello\n");
			waitpid(child1, NULL, 0);
			waitpid(child2, NULL, 0);
		}


		// Free allocated resources
		free(path_cmd1);
		free(path_cmd2);
		free_array(cmd1_args);
		free_array(cmd2_args);
		exit(1);
	}




	return (0);
}
