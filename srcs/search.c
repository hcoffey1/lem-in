/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   search.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: smorty <smorty@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/07/21 16:37:02 by smorty            #+#    #+#             */
/*   Updated: 2019/07/22 19:59:15 by smorty           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lemin.h"

int		depth_first_search(t_room *start)
{
	t_room	*next;
	int		i;

	if (start->type == 4)
	{
		printf("%s ", start->name);
		return (1);
	}
	i = 0;
	start->visited = 1;
	while (i < start->edges)
	{
		next = start->links[i++];
		if (!next->visited)
			if (depth_first_search(next))
				printf("%s ", start->name);
	}
	start->visited = 0;
	return (0);
}

t_rooms_queue		*get_path(t_room *start)
{
	t_rooms_queue *path;
	t_rooms_queue *p;

	path = (t_rooms_queue *)malloc(sizeof(t_rooms_queue));
	p = path;
	path->val = start;
	start->closed = 1;
	start = start->path;
	while (start)
	{
		path->next = (t_rooms_queue *)malloc(sizeof(t_rooms_queue));
		path = path->next;
		path->val = start;
		start->closed = 1;
		start = start->path;
		path->next = NULL;
	}
	return (p);
}

t_rooms_queue	*breadth_first_search(t_room *start, t_rooms_queue *q)
{
	t_rooms_queue *qptr;
	int		i;

	i = 0;
	if (start->type == 3) // конечная нода
		return (get_path(start));
	start->visited = 1;
	qptr = q;
	while (qptr->next)
		qptr = qptr->next;
	while (start->links[i]) // добавляем дочерние непосещённые и незакрытые ноды в очередь
	{
		if (!start->links[i]->visited && !start->links[i]->closed)
		{
			qptr->next = (t_rooms_queue *)malloc(sizeof(t_rooms_queue));
			qptr = qptr->next;
			qptr->val = start->links[i];
			qptr->val->path = start; //запоминаем маршрут
			qptr->next = NULL;
		}
		++i;
	}
	while (q && q->val->visited)
	{
		qptr = q;
		q = q->next;
		free(qptr);
	}
	if (!q)
		return (NULL);
	return (breadth_first_search(q->val, q)); // переходим к следующему элементу очереди
}
