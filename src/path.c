/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   path.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sinawara <sinawara@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/26 17:55:58 by sinawara          #+#    #+#             */
/*   Updated: 2024/11/27 16:46:45 by sinawara         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/pipex.h"

// Finds the PATH variable in the environment
char	*get_path(char **env)
{
	int	i;

	i = 0;
	while (env[i])
	{
		if (ft_strncmp(env[i], "PATH", 4) == 0)
			return (env[i] + 5);
		i++;
	}
	return (NULL);
}

char	*build_path(char *cmd, char **env)
{
	int		i;
	char	**paths;
	char	*path_var;
	char	*full_path;
	char	*temp;

	path_var = get_path(env);
	if (!path_var)
		return (NULL);
	paths = ft_split(path_var, ':');
	if (!paths)
		return (NULL);
	i = -1;
	while (paths[++i])
	{
		temp = ft_strjoin(paths[i], "/");
		if (!temp)
			return ((char *)free_and_return(paths, NULL));
		full_path = ft_strjoin(temp, cmd);
		free(temp);
		if (!full_path)
			return ((char *)free_and_return(paths, NULL));
		if (access(full_path, F_OK | X_OK) == 0)
			return ((char *)free_and_return(paths, full_path));
		free(full_path);
	}
	return (free_and_return(paths, NULL));
}
