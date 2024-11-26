/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sinawara <sinawara@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/23 13:24:36 by sinawara          #+#    #+#             */
/*   Updated: 2024/11/26 17:51:00 by sinawara         ###   ########.fr       */
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

// Finds the PATH variable in the environment
char *get_path(char **env)
{
	int i;

	i = 0;
	while(env[i])
	{
		if(ft_strncmp(env[i], "PATH", 4) == 0)
			return (env[i] + 5);
		i++;
	}
	return (NULL);
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

void *free_and_return(char **array, void *return_value)
{
    free_array(array); // Free the array
    return (return_value); // Return the specified value
}


char *build_path(char *cmd, char **env)
{
	int	i;
	char **paths;
	char *path_var;
	char *full_path;

	path_var = get_path(env);

	if (!path_var)
		return (NULL);

	paths = ft_split(path_var, ':');
	if (!paths)
		return (NULL);

	i = 0;
	while(paths[i])
	{
		full_path = ft_strjoin(paths[i], "/");
		if (!full_path)
			return ((char *)free_and_return(paths, NULL));
		full_path = ft_strjoin(full_path, cmd);
		if (!full_path)
      		return ((char *)free_and_return(paths, NULL));
		if (access(full_path, F_OK | X_OK) == 0)
	        return ((char *)free_and_return(paths, full_path));
		free(full_path);
		i++;
	}
	return (free_and_return(paths, NULL));
}


int main(int argc, char **argv, char **env)
{
	if (argc != 5)
	{
		ft_printf("Error -> usage : [ ./pipex file1 cmd1 cmd2 file2 ]\n");
		return (1);
	}
	else
	{
		if(open(argv[1], O_RDONLY) < 0)
			file_error();

		if ((open(argv[4], O_WRONLY | O_TRUNC | O_CREAT | O_APPEND, 0777 )) < 0)
			file_error();
		// check if infile exists ✔
		// create an outfile if it doesn't exist ✔

		char *test_path = build_path(argv[2], env);
		printf("%s\n", test_path);
		// We will have to fork twice, and assign child1 to execute cmd1, and child2 to execute cmd2.
		// The parent will waut at the end.

		// Before executing a command, you have to check if it exists before with the acess() function
		
	}
	return (0);
}
