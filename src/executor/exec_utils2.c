/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_utils2.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tfiguero <tfiguero@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/20 20:15:50 by mlopez-i          #+#    #+#             */
/*   Updated: 2024/03/23 17:47:31 by tfiguero         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	ft_redir(t_pipe *p)
{
	if (p->in >= 0)
	{
		if (dup2(p->in, STDIN_FILENO) < 0)
			exit(1);
		close(p->in);
		p->in = -3;
	}
	if (p->out >= 0)
	{
		if (dup2(p->out, STDOUT_FILENO) < 0)
			exit(1);
		close(p->out);
		p->out = -3;
	}
}

int	ft_check_builtin(t_pipe *p)
{
	char	*str;
	int		ret;

	ret = 0;
	if (!p || !p->cmds || !p->cmds[0])
		return (0);
	str = ft_strdup(p->cmds[0]);
	if (ft_strcmp(ft_tolower(str), "echo") == 0)
		ret = 1;
	else if (ft_strcmp(ft_tolower(str), "cd") == 0)
		ret = 2;
	else if (ft_strcmp(ft_tolower(str), "pwd") == 0)
		ret = 3;
	else if (ft_strcmp(ft_tolower(str), "export") == 0)
		ret = 4;
	else if (ft_strcmp(ft_tolower(str), "unset") == 0)
		ret = 5;
	else if (ft_strcmp(ft_tolower(str), "env") == 0)
		ret = 6;
	else if (ft_strcmp(ft_tolower(str), "exit") == 0)
		ret = 7;
	free(str);
	return (ret);
}

int	ft_check_access_file(char *str, int mode)
{
	if (mode == 1)
	{
		if (access(str, F_OK) == -1)
			perror(str);
		if (access(str, R_OK) == -1)
			perror(str);
	}
	else
	{
		if (access(str, F_OK) == -1)
			perror(str);
		if (access(str, W_OK) == -1)
			perror(str);
	}
	return (1);
}

void	exit_status(t_data	*data, int j)
{
	while (++j <= data->pipes)
	{
		if (data->pid == wait(&data->status))
		{
			if (WIFEXITED(data->status))
				data->exit = WEXITSTATUS(data->status);
			else if (WIFSIGNALED(data->status))
			{
				if (WTERMSIG(data->status) == SIGINT)
				{
					printf("\n");
					data->exit = 130;
				}
				else if (WTERMSIG(data->status) == SIGQUIT)
				{
					data->exit = 131;
					printf("Quit: 3\n");
				}
			}
		}
	}
}
