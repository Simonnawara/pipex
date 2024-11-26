/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sinawara <sinawara@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/23 13:24:36 by sinawara          #+#    #+#             */
/*   Updated: 2024/11/26 20:10:38 by sinawara         ###   ########.fr       */
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
		if ((open(argv[4], O_WRONLY | O_TRUNC | O_CREAT | O_APPEND, 0777 )) < 0) // create an outfile if it doesn't exist ✔
			file_error();

		path_cmd1 = build_path(argv[2], env);
		path_cmd2 = build_path(argv[3], env);
		
		cmd1_args = ft_split(argv[2], ' ');
		cmd2_args = ft_split(argv[3], ' ');
		
		if (!path_cmd1 || !path_cmd2 || !cmd1_args || !cmd2_args)
    	{
        	free(path_cmd1);
        	free(path_cmd2);
        	free_array(cmd1_args);
        	free_array(cmd2_args);
        	file_error();
    	}

		child1 = fork();
		if (child1 == 0)
		{
			printf("Child1 has been created\n");
        	int infile = open(argv[1], O_RDONLY);
        	if (infile < 0)
			{
				printf("Error happening in Child 1\n");
            	file_error();
			}
        	dup2(infile, STDIN_FILENO); // Redirect stdin to infile
    		close(infile);

        	// Execute cmd1
        	execve(path_cmd1, cmd1_args, env);
        	perror("execve"); // If execve fails
        	exit(1);
		}
		
		child2 = fork();
    	if (child2 == 0)
    	{
			printf("Child2 has been created\n");
			int outfile = open(argv[4], O_WRONLY | O_TRUNC | O_CREAT | O_APPEND, 0777);
			if (outfile < 0)
			{
				printf("Error happening in Child 2\n");
            	file_error();
			}
        	dup2(outfile, STDOUT_FILENO); // Redirect stdout to outfile
        	close(outfile);
			
        	execve(path_cmd2, cmd2_args, env);
        	perror("execve"); // If execve fails
        	exit(1);
    	}
		
		// Parent waits for both children
    	waitpid(child1, NULL, 0);
    	waitpid(child2, NULL, 0);

		// Free allocated resources
		free(path_cmd1);
		free(path_cmd2);
		free_array(cmd1_args);
		free_array(cmd2_args);
	}




	return (0);
}
