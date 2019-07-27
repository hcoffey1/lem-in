/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   search.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: smorty <smorty@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/07/21 16:37:02 by smorty            #+#    #+#             */
/*   Updated: 2019/07/26 22:31:42 by smorty           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lemin.h"

static void				set_direction(t_link *p, t_vertex *end, int dir)
{
	while (p)
	{
		if (p->link == end->path)
			p->dir = dir;
		p = p->next;
	}
}

static t_vertexs_queue	*get_path(t_vertex *end)
{
	t_vertexs_queue	*path;

	path = new_queue(end);
	while (end->type != 3)
	{
		set_direction(end->links, end->path, 1);
		set_direction(end->path->links, end, -1);
		if (end->type == 2)
			end->minlen = ~end->minlen + 1;
		end = end->path;
//		end->closed = 1;
		push_front(&path, end);
	}
//	end->closed = 0;
	return (path);
}

t_vertexs_queue			*bfs(t_vertex *start)
{
	t_vertexs_queue	*q;
	t_link			*p;

	q = new_queue(start);
	start->visited = 1;
	while (start->type != 4) // пока не нашлась комната с типом end
	{
		p = start->links;
		while (start->links && p)
		{
			if (!p->link->visited && !p->link->closed) // добавляем все связанные комнаты в очередь, кроме уже просмотренных из других комнат (visited),
			{															//либо закрытых другими найденными путями (closed)
				push(&q, p->link);
				p->link->path = start; // отмечаем из какой комнаты пришли, чтобы потом составить маршрут
				p->link->visited = 1;
			}
			p = p->next;
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

void					dijkstra(t_vertex *start)
{
	t_vertexs_queue	*q;
	t_link			*p;

	q = new_queue(start);
	start->minlen = 0;
	start->visited = 1;
	while (q)
	{
		p = start->links;
		while (p)
		{
			if (!p->link->visited && !p->link->closed && p->dir >= 0)
			{
				p->link->visited = 1;
				push(&q, p->link);
				if (p->link->minlen == __INT_MAX__ / 2 || p->link->minlen > start->minlen + 1)
				{
					p->link->path = start;
					p->link->minlen = start->minlen + 1;
				}
			}
			p = p->next;
		}
		pop(&q);
		if (q)
			start = q->val;
	}
}

int						is_measured(t_vertex **list_vertexs)
{
	while (*list_vertexs)
	{
		if ((*list_vertexs)->minlen == __INT_MAX__ / 2)
			return (0);
		++list_vertexs;
	}
	return (1);
}

void					bellman_ford(t_vertex **list_vertexs)
{
	t_vertex	**list0;
	t_link	*p;
	int		weight = 1;
	int n = 0;

	list0 = list_vertexs;
	while (!is_measured(list0) )
	{
		p = (*list_vertexs)->links;
		while (p)
		{
			if ((*list_vertexs)->minlen + weight < p->link->minlen)
			{
				p->link->minlen = (*list_vertexs)->minlen + weight;
				p->link->path = *list_vertexs;
			}
			p = p->next;
		}
		++list_vertexs;
		if (!*list_vertexs)
		{
			list_vertexs = list0;
		}
		ft_printf("%d ", ++n);
	}
	ft_printf("\n");
	while (*list0)
	{
		ft_printf("%d ", (*list0)->minlen);
		++list0;
	}
}
