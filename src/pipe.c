/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe_simple_copy.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amouly <amouly@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/17 10:22:46 by amouly            #+#    #+#             */
/*   Updated: 2023/03/20 17:34:35 by amouly           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int **create_pipes(int nb_of_pipes, int **fd)
{
    int i;
    
    i = 0;
    while (i < nb_of_pipes)
    {
        fd[i] = malloc(sizeof(int) * 2);
        if (pipe(fd[i]) < 0)
            return NULL;
        i++;
        //error + handle les pipes crees
    }
    return (fd);
}
void close_fd_everyhing(int **fd, int nbr_of_pipes)
{
    int i;

    i = 0;
    while (i < nbr_of_pipes)
    {
        close (fd[i][0]);
        close (fd[i][1]);
        i++;
    }
}
void close_fd_everyhing_but(int **fd, int nbr_of_pipes, int a, int b)
{
    int i;

    i = 0;
    while (i < nbr_of_pipes)
    {
        if (fd[a][b] != fd [i][0])
            close (fd[i][0]);
        if (fd[a][b] != fd [i][1])
            close (fd[i][1]);
        i++;
    }
}
void close_fd_everyhing_but_two(int **fd, int nbr_of_pipes, int read, int write)
{
    int i;

    i = 0;
    while (i < nbr_of_pipes)
    {
        if (fd[read][0] != fd [i][0])
            close (fd[i][0]);
        if (fd[write][1] != fd [i][1])
            close (fd[i][1]);
        i++;
    }
}




void wait_all_pid(int *pid, int nbr_of_command)
{
    int i;

    i = 0;
    while(i < nbr_of_command)
    {
        waitpid(pid[i], NULL, 0);
        i++;
    }
}

int child_process(t_pipe *pipe_info, int **fd)
{
        if (pipe_info->i == 0)
        {
            close_fd_everyhing_but(fd,pipe_info->nbr_of_pipes,pipe_info->i,1);
            dup2(fd[pipe_info->i][1], STDOUT_FILENO);
            exec_command(pipe_info->cmd, pipe_info->tab_arg, pipe_info->envp);
            close(fd[pipe_info->i][1]);
            exit (0);
        }
        else if (pipe_info->i == pipe_info->nbr_of_commands - 1 )
        {
            close_fd_everyhing_but(fd,pipe_info->nbr_of_pipes,pipe_info->i-1,0);
            dup2(fd[pipe_info->i -1][0], STDIN_FILENO);
            exec_command(pipe_info->cmd, pipe_info->tab_arg, pipe_info->envp);
            close(fd[pipe_info->i - 1][0]);
            exit (0);
        }
        else
        {
            close_fd_everyhing_but_two(fd,pipe_info->nbr_of_pipes,pipe_info->i-1,pipe_info->i);
            dup2(fd[pipe_info->i -1][0], STDIN_FILENO);
            dup2(fd[pipe_info->i][1], STDOUT_FILENO);
            exec_command(pipe_info->cmd, pipe_info->tab_arg, pipe_info->envp);
            close(fd[pipe_info->i][1]);
            close(fd[pipe_info->i - 1][0]);
            exit (0);
        }
    return (0);
}


void    init_fd_test(t_pipe *pipe_info, t_command *list)
{
    (void) list;
    
    pipe_info->fd_input = STDIN_FILENO ;
    pipe_info->fd_output = STDOUT_FILENO;
}

int managing_fork(int **fd, int nb_of_pipes, t_command *list, char **envp, int nbr_of_commands )
{
    t_pipe pipe_info;
    t_command *temp = list;

    pipe_info.nbr_of_pipes = nb_of_pipes;
    pipe_info.nbr_of_commands = nbr_of_commands;
    pipe_info.envp = envp;    
    pipe_info.i = 0;
    int pid[pipe_info.nbr_of_commands];
    init_fd_test(&pipe_info, list);
    while (pipe_info.i <= nb_of_pipes)
    {
        pipe_info.cmd = copy_string(temp->command->string, envp);
        pipe_info.tab_arg = list_to_tab(temp->command, envp);
        pid[pipe_info.i] = fork();
        if (pid[pipe_info.i] < 0)
            printf("error\n");
        else if(pid[pipe_info.i] == 0)
            child_process(&pipe_info, fd);
        if (pipe_info.i < nb_of_pipes)
            temp = temp->next; 
        pipe_info.i++;  
    }   
    close_fd_everyhing(fd,nb_of_pipes);
    wait_all_pid(pid,nbr_of_commands);
    return (0);
}



int managing_pipe(t_command *list , char **envp)
{
    int nb_of_command;
    int nb_of_pipes;
    int i;

    i = 0;
    nb_of_pipes = 0;
    nb_of_command = length_list_command(list, &nb_of_pipes);
    if (nb_of_pipes > 0)
    {
        int **fd;
        fd = malloc(sizeof (int *) * nb_of_pipes);
        fd = create_pipes(nb_of_pipes, fd);
        managing_fork(fd, nb_of_pipes, list, envp, nb_of_command);
    }
    else 
        exec_command(copy_string(list->command->string, envp), list_to_tab(list->command, envp), envp);
    return (0);
}