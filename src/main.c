/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sinawara <sinawara@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/23 13:24:36 by sinawara          #+#    #+#             */
/*   Updated: 2024/11/25 12:00:02 by sinawara         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/pipex.h"

int main(int argc, char **argv, char **env)
{

	/*
	int i = 0;
	while (env[i])
	{
		printf("%s\n", env[i]);
		i++;
	}
	*/

	int fd = open("./t.txt", O_WRONLY | O_APPEND);
	dup2(fd, 2);
	perror("I will be printed in the file t.txt\n");
	return (0);
}