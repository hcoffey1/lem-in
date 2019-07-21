/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: smorty <smorty@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/07/21 17:58:11 by smorty            #+#    #+#             */
/*   Updated: 2019/07/21 22:12:05 by smorty           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lemin.h"
#include "get_next_line.h"

void	clear_list(t_room **list_nodes)
{
	while (*list_nodes)
	{
		(*list_nodes)->visited = 0;
		(*list_nodes)->path = NULL;
		if ((*list_nodes)->type > 2)
			(*list_nodes)->closed = 0;
		++list_nodes;
	}
}

void	clear_list_2(t_room **list_nodes)
{
	while (*list_nodes)
	{
		(*list_nodes)->visited = 0;
		(*list_nodes)->path = NULL;
		(*list_nodes)->closed = 0;
		++list_nodes;
	}
}

int main(int argc, char **argv)
{
	t_room **list_nodes;
	t_room *start;
	int		fd;
	int		ants;
	int		verteces;
	int		connects;
	int		i = 0;

	if (argc != 2)
		return (0);
	fd = open(*(argv + 1), O_RDONLY);
	validate(fd, &ants, &verteces, &connects);
	close(fd);
	fd = open(*(argv + 1), O_RDONLY);
	list_nodes = read_file(fd, verteces, connects);
	close(fd);
	start = *list_nodes;
	while (list_nodes[i]->type != 3)
		++i;
	start = list_nodes[i];
/*	int i = 0;
	while (start[i])
	{
		int j = 0;
		printf("%s (%d) ", start[i]->name, start[i]->type);
		while (j < start[i]->edges)
			printf("%s ", start[i]->links[j++]->name);
		printf("\n");
		++i;
	}*/
	while (ants)
	{
		t_rooms_queue *q;
		q = (t_rooms_queue *)malloc(sizeof(t_rooms_queue));
		q->val = start;
		q->next = NULL;
		while (ants && breadth_first_search(start, q))
		{
			clear_list(list_nodes);
			q = (t_rooms_queue *)malloc(sizeof(t_rooms_queue));
			q->val = start;
			q->next = NULL;
			--ants;
		}
		clear_list_2(list_nodes);
	}
//	depth_first_search(start);
	// printf("\nlen %d\n", breadth_first_search(start, q));
	// q = (t_rooms_queue *)malloc(sizeof(t_rooms_queue));
	// q->val = start;
	// q->next = NULL;
	// printf("\nlen %d\n", breadth_first_search(start, q));
}