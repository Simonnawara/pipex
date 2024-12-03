/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_and_error.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sinawara <sinawara@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/27 14:11:11 by sinawara          #+#    #+#             */
/*   Updated: 2024/12/03 12:55:59 by sinawara         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/pipex.h"

int	file_error(char *filename)
{
	write(2, filename, ft_strlen(filename));
	perror(" ");
	exit(EXIT_FAILURE);
}

// Frees any array that is passed as argument
void	free_array(char **array)
{
	int	i;

	i = 0;
	if (!array)
		return ;
	while (array[i])
	{
		free(array[i]);
		i++;
	}
	free(array);
}

// Function purely created to save lines
void	*free_and_return(char **array, void *return_value)
{
	free_array(array);
	return (return_value);
}

void	free_all(char *arg1, char *arg2, char **arr1, char **arr2)
{
	free(arg1);
	free(arg2);
	free_array(arr1);
	free_array(arr2);
}

void	free_three(char *arg1, char *arg2, t_args *arg3)
{
	free(arg1);
	free(arg2);
	free(arg3);
}
