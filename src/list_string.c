/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   list_string.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amouly <amouly@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/06 11:19:41 by amouly            #+#    #+#             */
/*   Updated: 2023/03/27 11:02:53 by llion            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"


int	lstadd_back_list_string(t_string **list, t_string *new)
{
	t_string      *temp;

    // HANDLE ERROR
	if (list == NULL && new == NULL)
		return (0);
	if ((*list) == NULL)
	{
		(*list) = new;
        new->previous = NULL;
		new->next = NULL;
	}
	else
	{
		temp = (*list);
		while (temp->next != NULL)
			temp = temp->next;
		temp->next = new;
        new->previous = temp;
	}
	return (1);
}



int fill_list_string(char *str, t_string **list)
{
    t_string *new;
    int i;

    i = 0;    
    new = malloc(sizeof(t_string));
    // HANDLE ERROR
    if (new == NULL)
        return (0);
    // cleaan la liste si il y a un souci
    new->string = malloc(sizeof(char) * (ft_strlen(str) + 1));
    // clean nul
    while(str[i])
    {    
        new->string[i] = str[i];
        i++;
    }
    new->string[i] = '\0';
    new->append_or_heredoc = 0;
    new->next = NULL;
    // HANDLE ERROR
    if (!lstadd_back_list_string(list, new ))
            return (0);
    // cleaan la liste si il y a un souci
    return (1);
}

int fill_list_string_append_or_heredoc(char *str, t_string **list)
{
    t_string *new;
    int i;

    i = 0;    
    new = malloc(sizeof(t_string));
    // HANDLE ERROR
    if (new == NULL)
        return (0);
    // cleaan la liste si il y a un souci
    new->string = malloc(sizeof(char) * (ft_strlen(str) + 1));
    // clean nul
    while(str[i])
    {    
        new->string[i] = str[i];
        i++;
    }
    new->string[i] = '\0';
    new->append_or_heredoc = 1;
    new->next = NULL;
    // HANDLE ERROR
    if (!lstadd_back_list_string(list, new ))
            return (0);
    // cleaan la liste si il y a un souci
    return (1);
}


void print_list_string_from_head(t_string *list)
{
    t_string    *head;
    
    head = list;
    // HANDLE ERROR
    if (head == NULL)
    {
        printf("NULL\n");
        return ;
    }
    while (head)
    {
        if (head->append_or_heredoc == 1)
            printf("append or Heredoc: %s\n", head->string);
        else 
            printf("%s\n", head->string);
        head = head->next;
    }
}

void print_list_string_from_head_command(t_string *list)
{
    t_string    *head;
    int         command;
    
    head = list;
    command = 0;
    // HANDLE ERROR
    if (head == NULL)
    {
        printf("NULL\n");
        return ;
    }
    while (head)
    {
        if (head->append_or_heredoc == 1)
            printf("mode append : %s\n", head->string);
        else
        {
            if (command == 0)
            {
                printf("la commande est : ");
                command = 1;
            }
            printf("%s\n", head->string);
        }
        head = head->next;
    }
}
