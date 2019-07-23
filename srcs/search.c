/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   search.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: smorty <smorty@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/07/21 16:37:02 by smorty            #+#    #+#             */
/*   Updated: 2019/07/23 22:34:30 by smorty           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lemin.h"

static t_rooms_queue	*get_path(t_room *end)
{
	t_rooms_queue	*path;

	path = new_queue(end);
	while (end->type != 3)
	{
		end = end->path;
		end->closed = 1;
		push_front(&path, end);
	}
	end->closed = 0;
	return (path);
}

t_rooms_queue			*bfs(t_room *start)
{
	t_rooms_queue	*q;
	int				i;

	q = new_queue(start);
	start->visited = 1;
	while (start->type != 4) // пока не нашлась комната с типом end
	{
		i = 0;
		while (start->links[i])
		{
			if (!start->links[i]->visited && !start->links[i]->closed) // добавляем все связанные комнаты в очередь, кроме уже просмотренных из других комнат (visited),
			{															//либо закрытых другими найденными путями (closed)
				push(&q, start->links[i]);
				start->links[i]->path = start; // отмечаем из какой комнаты пришли, чтобы потом составить маршрут
				start->links[i]->visited = 1;
			}
			++i;
		}
		pop(&q); // удаляем первый элемент из очереди (текущую комнату)
		if (!q)	// если комнат больше не осталось, значит, все уже проверены, и пути нет
			return (NULL);
		start = q->val;
	}
	while (q)
		pop(&q);
	return (get_path(start)); // составляем маршрут в виде очереди комнат
}
