#ifndef PIPEX_H
# define PIPEX_H

# include <unistd.h>     /* Pour write, read, pipe, dup2, execve, etc. */
# include <stdlib.h>     /* Pour malloc, free, exit */
# include <fcntl.h>      /* Pour open, O_RDONLY, etc. */
# include <sys/wait.h>   /* Pour wait, waitpid */
# include <sys/types.h>  /* Pour pid_t */
# include <stdio.h>      /* Pour perror */
# include <errno.h>      /* Pour errno, ECHILD */
# include <string.h>     /* Pour strerror */
# include "libft.h"

typedef struct s_pipex
{
	char	**av_copy;
	int	file_in;         // Descripteur du fichier d'entrée
	int	file_out;        // Descripteur du fichier de sortie
	int	cmd_count;       // Nombre de commandes
	int	pipe_count;      // Nombre de pipes
	int	**pipe;
	pid_t *pids;
	char 	**path;
	char    **cmd;           // Commandes originales
	char    **cmd_paths;     // Chemins complets des commandes
	char    ***cmd_args;	
	char 	**path_dir;
	int	i;
	int	j;
	char *pipex.delimiter;
} t_pipex;

void free_pipe(t_pipex *pipex);
void print_error();
char *find_single_command_path(t_pipex *pipex, int cmd_index);
void find_command_path(char **env, t_pipex *pipex);
int setup_pipex_cmd(t_pipex *pipex, char **av);
int setup_pipex_pipe(t_pipex *pipex);
void close_pipes(t_pipex *pipex);
int setup_pipex_pids2(t_pipex *pipex, char **av);
int setup_pipex_pids(t_pipex *pipex, char **av);
void setup_pipex(t_pipex *pipex, char **av);

#endif
