/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sinawara <sinawara@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/23 13:24:36 by sinawara          #+#    #+#             */
/*   Updated: 2024/11/26 19:04:14 by sinawara         ###   ########.fr       */
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
		printf("%s\n", path_cmd1);
		printf("%s\n", path_cmd2);
		execve(path_cmd1, *argv, *env);
		// We will have to fork twice, and assign child1 to execute cmd1, and child2 to execute cmd2.
		// The parent will waut at the end.

		// Before executing a command, you have to check if it exists before with the acess() function
	}
	return (0);
}
