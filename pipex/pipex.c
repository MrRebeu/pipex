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
// char *find_single_command_path(t_pipex *pipex, int cmd_index)
// {
//     int i = 0;
    
//     while (pipex->path_dir[i])
//     {
//         char *full_path = ft_strjoin(pipex->path_dir[i], "/");
//         if (!full_path)
//             print_error();
//         // Utiliser le premier argument (la commande) de cmd_args[cmd_index]
//         char *good_path = ft_strjoin(full_path, pipex->cmd_args[cmd_index][0]);
//         free(full_path);
//         if (!good_path)
//             print_error();
//         int fd = open(good_path, O_RDONLY);
//         if (fd != -1)
//         {
//             close(fd);
//             return (good_path);
//         }
//         free(good_path);
//         i++;
//     }
//     return (NULL);
// }
// void find_command_path(char **env, t_pipex *pipex)
// {
//     int i = 0;
    
//     // Trouver la variable PATH
//     while (env[i])
//     {
//         if (ft_strncmp(env[i], "PATH=", 5) == 0)
//         {
//             pipex->path_dir = ft_split(env[i] + 5, ':');
//             if (!pipex->path_dir)
//                 print_error();
//             break;
//         }
//         i++;
//     }
    
//     if (!env[i])
//         print_error(); // PATH non trouvé
    
//     // Allouer l'espace pour les chemins des commandes
//     pipex->cmd_paths = malloc(sizeof(char*) * pipex->cmd_count);
//     if (!pipex->cmd_paths)
//         print_error();
    
//     // Trouver le chemin pour chaque commande
//     i = 0;
//     while (i < pipex->cmd_count)
//     {
//         pipex->cmd_paths[i] = find_single_command_path(pipex, i);
//         if (!pipex->cmd_paths[i])
//             print_error();
//         i++;
//     }
// }
char *find_single_command_path(t_pipex *pipex, int cmd_index)
{
    int i = 0;
    
    while (pipex->path_dir[i])
    {
        char *full_path = ft_strjoin(pipex->path_dir[i], "/");
        if (!full_path)
            print_error();
            
        char *good_path = ft_strjoin(full_path, pipex->cmd_args[cmd_index][0]);
        
        write(2, "Essai du chemin: ", 17);
        write(2, good_path, ft_strlen(good_path));
        write(2, "\n", 1);
        
        free(full_path);
        
        if (!good_path)
            print_error();
            
        int fd = open(good_path, O_RDONLY);
        if (fd != -1)
        {
            write(2, "Chemin valide trouvé!\n", 22);
            close(fd);
            return (good_path);
        }
        
        free(good_path);
        i++;
    }
    
    write(2, "Aucun chemin valide trouvé\n", 28);
    return (NULL);
}
void find_command_path(char **env, t_pipex *pipex)
{
    int i = 0;
    
    // Trouver la variable PATH
    while (env[i])
    {
        if (ft_strncmp(env[i], "PATH=", 5) == 0)
        {
            write(2, "PATH trouvé: ", 13);
            write(2, env[i] + 5, ft_strlen(env[i]) - 5);
            write(2, "\n", 1);
            
            pipex->path_dir = ft_split(env[i] + 5, ':');
            if (!pipex->path_dir)
                print_error();
                
            // Vérifier les chemins extraits
            int j = 0;
            while (pipex->path_dir[j])
            {
                write(2, "Chemin PATH: ", 13);
                write(2, pipex->path_dir[j], ft_strlen(pipex->path_dir[j]));
                write(2, "\n", 1);
                j++;
            }
            break;
        }
        i++;
    }
    
    if (!env[i])
    {
        write(2, "PATH non trouvé dans l'environnement\n", 37);
        print_error();
    }
    
    // Allouer l'espace pour les chemins des commandes
    pipex->cmd_paths = malloc(sizeof(char*) * pipex->cmd_count);
    if (!pipex->cmd_paths)
        print_error();
    
    // Trouver le chemin pour chaque commande
    i = 0;
    while (i < pipex->cmd_count)
    {
        write(2, "Recherche du chemin pour la commande: ", 38);
        write(2, pipex->cmd_args[i][0], ft_strlen(pipex->cmd_args[i][0]));
        write(2, "\n", 1);
        
        pipex->cmd_paths[i] = find_single_command_path(pipex, i);
        
        if (pipex->cmd_paths[i])
        {
            write(2, "Chemin trouvé: ", 15);
            write(2, pipex->cmd_paths[i], ft_strlen(pipex->cmd_paths[i]));
            write(2, "\n", 1);
        }
        else
        {
            write(2, "Commande non trouvée\n", 21);
            print_error();
        }
        i++;
    }
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

// int	setup_pipex_pipe(t_pipex	*pipex)
// {
// 	pipex->i = 0;
// 	pipex->pipe_count = pipex->cmd_count - 1;
// 	pipex->pipe = malloc(sizeof(int *) * (pipex->pipe_count));
// 	if (!pipex->pipe)
// 		return (1);
// 	while (pipex->i < pipex->pipe_count)
// 	{
// 		pipex->pipe[pipex->i] = malloc(sizeof(int) * 2);
// 		if (!pipex->pipe[pipex->i])
// 		{
// 			free_pipe(pipex);
// 			print_error();
// 		}
		
// 		if (pipe(pipex->pipe[pipex->i]) == -1)
// 		{
// 			free_pipe(pipex);
// 			print_error();
// 		}
// 		pipex->i++;
// 	}
// 	return (0);
// }
int setup_pipex_pipe(t_pipex *pipex)
{
    write(2, "Début setup_pipex_pipe\n", 23);
    pipex->i = 0;
    pipex->pipe_count = pipex->cmd_count - 1;
    
    write(2, "Allocation des pipes: ", 22);
    char msg[50];
    sprintf(msg, "%d pipes nécessaires\n", pipex->pipe_count);
    write(2, msg, strlen(msg));
    
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
        
        sprintf(msg, "Création du pipe %d\n", pipex->i);
        write(2, msg, strlen(msg));
        
        if (pipe(pipex->pipe[pipex->i]) == -1)
        {
            write(2, "Erreur création pipe\n", 21);
            free_pipe(pipex);
            print_error();
        }
        pipex->i++;
    }
    
    write(2, "Fin setup_pipex_pipe\n", 21);
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
	execve(pipex->cmd_paths[pipex->i], pipex->cmd_args[pipex->i], NULL);
	perror("execve failed");
	exit(1);
	}
// int	setup_pipex_pids(t_pipex *pipex, char **av)
// {
// 	pipex->i = 0;
// 	pipex->pids = malloc(sizeof(pid_t) * pipex->cmd_count);
// 	if (!pipex->pids)
// 	{
// 		free_pipe(pipex);
// 		print_error();
// 	}
// 	pipex->i = 0;
// 	while (pipex->i < pipex->cmd_count)
// 	{
// 		pipex->pids[pipex->i] = fork();
// 		if (pipex->pids[pipex->i] == -1)
// 		{
// 			free_pipe(pipex);
// 			print_error();
// 		}
// 		else if (pipex->pids[pipex->i] == 0)
// 			setup_pipex_pids2(pipex, av);
// 		pipex->i++;
// 	}
// 	close_pipes(pipex);
// 	pipex->i = 0;
// 	while (pipex->i < pipex->cmd_count)
// 	{
// 		waitpid(pipex->pids[pipex->i], NULL, 0);
// 		pipex->i++;
// 	}
// 	return (0);
// }
int setup_pipex_pids(t_pipex *pipex, char **av)
{
    write(2, "Début setup_pipex_pids\n", 23);
    
    pipex->i = 0;
    char msg[100];
    
    sprintf(msg, "Allocation pour %d processus\n", pipex->cmd_count);
    write(2, msg, strlen(msg));
    
    pipex->pids = malloc(sizeof(pid_t) * pipex->cmd_count);
    if (!pipex->pids)
    {
        free_pipe(pipex);
        print_error();
    }
    
    pipex->i = 0;
    while (pipex->i < pipex->cmd_count)
    {
        sprintf(msg, "Création processus %d\n", pipex->i);
        write(2, msg, strlen(msg));
        
        pipex->pids[pipex->i] = fork();
        
        if (pipex->pids[pipex->i] == -1)
        {
            write(2, "Erreur fork\n", 13);
            free_pipe(pipex);
            print_error();
        }
        else if (pipex->pids[pipex->i] == 0)
        {
            // C'est le processus enfant
            write(2, "Dans l'enfant, avant setup_pipex_pids2\n", 39);
            setup_pipex_pids2(pipex, av);
            // Cette ligne ne devrait jamais être atteinte car setup_pipex_pids2 se termine par execve ou exit
            write(2, "Erreur: retour de setup_pipex_pids2\n", 36);
            exit(1);
        }
        else
        {
            // C'est le processus parent
            sprintf(msg, "Processus parent: enfant %d créé avec PID %d\n", pipex->i, pipex->pids[pipex->i]);
            write(2, msg, strlen(msg));
        }
        
        pipex->i++;
    }
    
    write(2, "Fermeture des pipes dans le parent\n", 35);
    close_pipes(pipex);
    
    write(2, "Attente des processus enfants\n", 30);
    pipex->i = 0;
    while (pipex->i < pipex->cmd_count)
    {
        sprintf(msg, "Attente du processus %d (PID %d)\n", pipex->i, pipex->pids[pipex->i]);
        write(2, msg, strlen(msg));
        
        int status;
        waitpid(pipex->pids[pipex->i], &status, 0);
        
        if (WIFEXITED(status))
        {
            sprintf(msg, "Processus %d terminé avec code %d\n", pipex->i, WEXITSTATUS(status));
            write(2, msg, strlen(msg));
        }
        else if (WIFSIGNALED(status))
        {
            sprintf(msg, "Processus %d terminé par signal %d\n", pipex->i, WTERMSIG(status));
            write(2, msg, strlen(msg));
        }
        
        pipex->i++;
    }
    
    write(2, "Fin setup_pipex_pids\n", 21);
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
    t_pipex pipex;
    
    if (ac >= 5)
    {
        // Initialiser à zéro pour éviter les comportements indéfinis
        memset(&pipex, 0, sizeof(t_pipex));
        
        // Définir cmd_count en premier!
        pipex.cmd_count = (ac - 3);
        
        // Allouer cmd_args avant find_command_path
        pipex.cmd_args = malloc(sizeof(char**) * pipex.cmd_count);
        if (!pipex.cmd_args)
            print_error();
            
        // Initialiser cmd_args
        int i;
        for (i = 0; i < pipex.cmd_count; i++)
        {
            pipex.cmd_args[i] = ft_split(av[i + 2], ' ');
            if (!pipex.cmd_args[i])
                print_error();
        }
        
        // Chercher les chemins
        find_command_path(env, &pipex);
        
        // Ouvrir les fichiers
        pipex.file_in = open(av[1], O_RDONLY);
        if (pipex.file_in == -1)
            print_error();
            
        pipex.file_out = open(av[ac - 1], O_WRONLY | O_CREAT | O_TRUNC, 0644);
        if (pipex.file_out == -1)
            print_error();
        
        // IMPORTANT: Simplifier setup_pipex pour n'appeler que ces deux fonctions
        write(2, "Appel de setup_pipex_pipe\n", 26);
        setup_pipex_pipe(&pipex);
        
        write(2, "Appel de setup_pipex_pids\n", 26);
        setup_pipex_pids(&pipex, av);
    }
    return 0;
}
