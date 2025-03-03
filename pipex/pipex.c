#include "pipex.h"

void	free_pipe(t_pipex *pipex)
{
	pipex->i = 0;
	if (pipex->pipe)
	{
		while (pipex->i < pipex->cmd_count - 1)
		{
			free(pipex->pipe[pipex->i]);
			pipex->i++;
		}
		free(pipex->pipe);
	}
}

void	print_error()
{
	write(2, "Error\n", 6);
	exit(1);
}
char *find_one_command_path(t_pipex pipex, char *env)
{
	int i = 0;
	while (pipex->paths[i])
	{
		pipex->paths[i] = ft_strjoin(pipex->paths[i], "/");
		if (!pipex->paths[i])
			print_error();
		int fd = open(pipex->paths, O_RDONLY);
		if (fd != -1)
		{
			close(fd);
			return (pipex->paths[i]);
		}
		i++;
	}
}
char *find_command_path(char *env, char *path)
{
	int	i;

	i = 0;
	while (env[i])
	{
		if (ft_strncmp(env[i] ,"PATH=", 5) == 0)
		{
			pipex->paths = ft_split(pipex->paths, ':');
			if (!pipex->paths)
				print_error();
		}
	}
	i = 0;
	while (pipex->paths[i])
	{
		pipex->paths[i] = ft_strjoin(pipex->paths[i], "/");
		if (!pipex->paths[i])
			print_error();
		int fd = open(pipex->paths, O_RDONLY);
		if (fd != -1)
		{
			close(fd);
			return (piepex->paths[i]);
		}
		i++;
	}
	return (NULL);
}

int	setup_pipex_cmd(t_pipex *pipex, char **av)
{
	pipex->j = 0;
	pipex->i = 0;
	pipex->cmd = malloc(sizeof(char*) * pipex->cmd_count);
	if (!pipex->cmd)
		return (1);
	while (pipex->i < pipex->cmd_count)
	{
		pipex->cmd[pipex->i] = malloc(sizeof(char) * (ft_strlen(av[pipex->i + 2])+ 1));
		if (!pipex->cmd[pipex->i])
		{
			while (pipex->j < pipex->i)
			{
				free(pipex->cmd[pipex->j]);
				pipex->j++;
			}
			free(pipex->cmd);
			print_error();
			pipex->i++;
		}
	}
	pipex->i = 0;
	while (pipex->i < pipex->cmd_count)
	{
		ft_strcpy(pipex->cmd[pipex->i],av[pipex->i + 2]);
		pipex->i++;
	}
	return (0);
}

int	setup_pipex_pipe(t_pipex	*pipex)
{
	pipex->i = 0;
	pipex->pipe_count = pipex->cmd_count - 1;
	pipex->pipe = malloc(sizeof(int *) * (pipex->pipe_count));
	if (!pipex->pipe)
		return (1);
	while (pipex->i < pipex->pipe_count)
	{
		pipex->pipe[pipex->i] = malloc(sizeof(int) * 2);
		if (!pipex->pipe[pipex->i])
		{
			free_pipe(pipex);
			print_error();
		}
		
		if (pipe(pipex->pipe[pipex->i]) == -1)
		{
			free_pipe(pipex);
			print_error();
		}
		pipex->i++;
	}
	return (0);
}
void close_pipes(t_pipex *pipex)
{
    pipex->j = 0;

    while (pipex->j < pipex->cmd_count - 1)
    {
        close(pipex->pipe[pipex->j][0]);
        close(pipex->pipe[pipex->j][1]);
        pipex->j++;
    }
    close(pipex->file_in);
    close(pipex->file_out);
}
int setup_pipex_pids2(t_pipex *pipex, char **av)
{
	pipex->j = 0;

	if (pipex->i == 0)
	{
		dup2(pipex->file_in, 0);
		dup2(pipex->pipe[pipex->i][1], 1);
	}
	else if (pipex->i == pipex->cmd_count - 1)
	{
		dup2(pipex->pipe[pipex->i - 1][0], 0);
		dup2(pipex->file_out, 1);
	}
	else
	{
		dup2(pipex->pipe[pipex->i - 1][0], 0);
		dup2(pipex->pipe[pipex->i][1], 1);
	}
	pipex->j = 0;
	while (pipex->j < pipex->cmd_count - 1)
	{
		close(pipex->pipe[pipex->j][0]);
		close(pipex->pipe[pipex->j][1]);
		pipex->j++;
	}
	close(pipex->file_in);
	close(pipex->file_out);
	char *args[2];
	args[0] = av[pipex->i + 2];
	args[1] = NULL;
	execve(av[pipex->i + 2], args, NULL);
	perror("execve failed");
	exit(1);
	}
int	setup_pipex_pids(t_pipex *pipex, char **av)
{
	pipex->i = 0;
	pipex->pids = malloc(sizeof(pid_t) * pipex->cmd_count);
	if (!pipex->pids)
	{
		free_pipe(pipex);
		print_error();
	}
	pipex->i = 0;
	while (pipex->i < pipex->cmd_count)
	{
		pipex->pids[pipex->i] = fork();
		if (pipex->pids[pipex->i] == -1)
		{
			free_pipe(pipex);
			print_error();
		}
		else if (pipex->pids[pipex->i] == 0)
			setup_pipex_pids2(pipex, av);
		pipex->i++;
	}
	close_pipes(pipex);
	pipex->i = 0;
	while (pipex->i < pipex->cmd_count)
	{
		waitpid(pipex->pids[pipex->i], NULL, 0);
		pipex->i++;
	}
	return (0);
}
void setup_pipex(t_pipex *pipex, char **av)
{
	setup_pipex_pipe(pipex);   // Créer les pipes d'abord
	setup_pipex_cmd(pipex, av); // Préparer les commandes ensuite
	setup_pipex_pids(pipex, av); // Créer les processus en dernier
}
int main(int ac, char **av, char **env)
{
	t_pipex	pipex;

	if (ac >= 5)
	{
		pipex->path = find_command_path(env, "PATH");
		if (!pipex->path)
			print_error();
		pipex.cmd_count = (ac - 3);
		pipex.file_in = open(av[1], O_RDONLY);
		if (pipex.file_in == -1)
			print_error();
		pipex.file_out = open(av[ac - 1], O_WRONLY | O_CREAT | O_TRUNC, 0644);
		if (pipex.file_out == -1)
			print_error();
		setup_pipex(&pipex, av);
	}
}
