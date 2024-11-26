/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sinawara <sinawara@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/23 13:24:36 by sinawara          #+#    #+#             */
/*   Updated: 2024/11/26 16:09:09 by sinawara         ###   ########.fr       */
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

char **split_path(char *path)
{
	return (ft_split(path, ':'));
}

int main(int argc, char **argv, char **env)
{
	int i = 0;
	while (env[i])
	{
		printf("%s\n", env[i]);
		i++;
	}
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

		// We will have to fork twice, and assign child1 to execute cmd1, and child2 to execute cmd2.
		// The parent will waut at the end.

		// Before executing a command, you have to check if it exists before with the acess() function
		
	}
	return (0);
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
		full_path = ft_strjoin(paths, "/");
		if (!full_path)
        {
            free_array(paths);
            return (NULL);
        }
		full_path = ft_strjoin(full_path, cmd);
		if (!full_path)
        {
            free_array(paths); // Free all allocated paths
            return (NULL);
        }
		if (access(full_path, F_OK | X_OK) == 0);
			return (full_path);
		free(full_path);
		i++;
	}
	return (NULL);
}
