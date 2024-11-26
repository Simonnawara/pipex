/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sinawara <sinawara@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/23 13:24:36 by sinawara          #+#    #+#             */
/*   Updated: 2024/11/26 13:20:51 by sinawara         ###   ########.fr       */
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

int main(int argc, char **argv, char **env)
{
	(void) env;
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
		// create an outfile if it doesn't exist

		// We will have to fork twice, and assign child1 to execute cmd1, and child2 to execute cmd2.
		// The parent will waut at the end.

		// Before executing a command, you have to check if it exists before with the acess() function
		
	}
	return (0);
}