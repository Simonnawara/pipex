/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sinawara <sinawara@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/23 13:24:36 by sinawara          #+#    #+#             */
/*   Updated: 2024/11/25 18:12:29 by sinawara         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/pipex.h"

int main(void)
{
	pid_t pid;
	pid_t pid2;

	pid = fork();
	if (pid == 0)
	{
		pid2 = fork();
		printf("pid : %d\n", pid2);
	}
	else if (pid > 0)
		printf("pid > 0\n");
	return (0);
}