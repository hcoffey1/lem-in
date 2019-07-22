/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: smorty <smorty@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/07/21 17:58:11 by smorty            #+#    #+#             */
/*   Updated: 2019/07/22 22:29:00 by smorty           ###   ########.fr       */
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

int		find_max_len(t_path_list *path_list)
{
	int len;

	len = 0;
	while (path_list)
	{
		if (len < path_list->len)
			len = path_list->len;
		path_list = path_list->right;
	}
	return (len);
}

void	dupe_path(t_path_list *path_list)
{
	t_path_list *new;
	int			lensum;
	int			maxlen;

	maxlen = find_max_len(path_list);
	while (path_list->right)
	{
		lensum = path_list->len * 2;
		while (lensum <= maxlen && path_list->len != maxlen)
		{
			lensum += path_list->len;
			new = (t_path_list *)malloc(sizeof(t_path_list));
			new->path = path_list->path;
			new->len = path_list->len;
			new->right = path_list->right;
			new->left = path_list;
			path_list->right->left = new;
			path_list->right = new;
			path_list = path_list->right;
		}
//		while (path_list->len == path_list->left->len)
			path_list = path_list->right;
	}
}

int		end(t_path_list *path_list)
{
	while (path_list)
	{
		if (path_list->path)
			return (0);
		path_list = path_list->right;
	}
	return (1);
}

void	go(t_path_list *path_list)
{
	t_path_list *p;
	int n = 0;

	while (!end(path_list))
	{
		printf("--step %d\n", ++n);
		p = path_list;
		while (p)
		{
			if (p->path && p->path->next && !p->path->next->val->closed)
			{
				printf("%s-", p->path->val->name);
				p->path->val->closed = 0;
				p->path = p->path->next;
				printf("%s ", p->path->val->name);
				if (p->path && p->path->val->type == 2)
					p->path->val->closed = 1;
				else
					p->path = p->path->next;
			}
			p = p->right;
		}
		printf("\n");
	}
}

void	path_graph(t_path_list *path_list)
{
	t_rooms_queue *path;
	t_path_list *p;
	int n = 0;

	p = path_list;
	while (p)
	{
		++n;
		p = p->right;
	}
	free(path_list->path->val->links);
	path_list->path->val->links = (t_room **)malloc(sizeof(t_room *) * n);
	path_list->path->val->links[--n] = NULL;
	p = path_list;
	while (p)
	{
		if (p->path && p->path->next && p->path->next->val)
			path_list->path->val->links[--n] = p->path->next->val;
		p = p->right;
	}
	while (path_list)
	{
		path = path_list->path;
		while (path)
		{
			if (path->val->type == 2)
			{
				free(path->val->links);
				path->val->links = (t_room **)malloc(sizeof(t_room *) * 2);
				path->val->links[0] = path->next->val;
				path->val->links[1] = NULL;
			}
			path = path->next;
		}
		path_list = path_list->right;
	}
}

int path_len(t_room *start)
{
	int len;

	len = 0;
	while (start->type != 4)
	{
		start = start->links[0];
		++len;
	}
	return (len);
}

void	print_queue(t_rooms_queue *q)
{
	while (q)
	{
		printf("%s-", q->val->name);
		q = q->next;
		printf("%s ", q->val->name);
		q = q->next;
	}
}

int	add_to_queue(t_rooms_queue *q, t_room *elem)
{
	int qsize;

	qsize = 1;
	if (q->val)
	{
		while (q->next)
		{
			q = q->next;
			++qsize;
		}
		q->next = (t_rooms_queue *)malloc(sizeof(t_rooms_queue));
		q = q->next;
	}
	q->val = elem;
	q->next = NULL;
	return (qsize);
}

void	solve(int ants, t_room *start)
{
	int i;
	int qsize;
	t_rooms_queue *q;
	t_rooms_queue *head;

	q = (t_rooms_queue *)malloc(sizeof(t_rooms_queue));
	q->val = NULL;
	q->next = NULL;
	head = q;
	while (ants)
	{
		i = 0;
		while (start->links[i])
		{
			add_to_queue(q, start);
			qsize = add_to_queue(q, start->links[i]);
			--ants;
			if (!ants)
				break ;
			++i;
		}
		q = head;
		while (--qsize)
		{
			if (q->val->type != 4)
			{
				add_to_queue(q, q->val);
				add_to_queue(q, q->val->links[0]);
			}
			q = q->next;
		}
	}
	print_queue(head);
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
	while (list_nodes[i]->type != 4) // ищем стартовую ноду
		++i;
	start = list_nodes[i];
	i = 0;
	t_rooms_queue	*path;
	t_path_list		*path_list;
	t_rooms_queue *q;
	int len = 0;
	
	q = (t_rooms_queue *)malloc(sizeof(t_rooms_queue));
	q->val = start;
	q->next = NULL;
	path_list = (t_path_list *)malloc(sizeof(t_path_list));
	path_list->right = NULL;
	path_list->left = NULL;
	while ((path_list->path = breadth_first_search(start, q)))
	{
		path_list->right = (t_path_list *)malloc(sizeof(t_path_list));
		path_list->right->left = path_list;
		path_list = path_list->right;
		path_list->right = NULL;
		clear_list(list_nodes);
		q = (t_rooms_queue *)malloc(sizeof(t_rooms_queue));
		q->val = start;
		q->next = NULL;
	}
	while (path_list->left)
	{
		len = 0;
		path_list = path_list->left;
		path = path_list->path;
		while (path)
		{
			printf("%s-", path->val->name);
			path = path->next;
			++len;
		}
		path_list->len = len;
		printf("\nlen %d\n", path_list->len);
	}
	dupe_path(path_list);
	clear_list_2(list_nodes);
	path_graph(path_list);
/*	while (*list_nodes)
	{
		printf("%s ", (*list_nodes)->name);
		t_room **tmp = (*list_nodes)->links;
		while (*tmp)
			printf("%s ", (*tmp++)->name);
		printf("\n");
		++list_nodes;
	}*/
	while (path_list)
	{
		path = path_list->path;
		while (path)
		{
			printf(" %s ", path->val->name);
			i = 0;
			while (path->val->links[i])
				printf("<%s>", path->val->links[i++]->name);
			path = path->next;
		}
		printf("\nlen %d\n", path_list->len);
		path_list = path_list->right;
	}
	i = 0;
	while (list_nodes[i]->type != 3) // ищем стартовую ноду
		++i;
	start = list_nodes[i];
//	printf("%s ", start->name);
	solve(ants, start);
//	go(path_list);
/*	int n = 0;
	int len = 0;
	while (ants)
	{
		t_rooms_queue *q;
		q = (t_rooms_queue *)malloc(sizeof(t_rooms_queue));
		q->val = start;
		q->next = NULL;
		printf("step %d\n", ++n);
		while (ants && (path = breadth_first_search(start, q))) // ищем непересекающиеся пути, пока не закончатся
		{
			len = 0;
			while (path)
			{
				printf("%s-", path->val->name);
				path = path->next;
				++len;
			}
			printf("\nlen %d\n", len);
			clear_list(list_nodes);
			q = (t_rooms_queue *)malloc(sizeof(t_rooms_queue));
			q->val = start;
			q->next = NULL;
			--ants;
		}
		clear_list_2(list_nodes); //очищаем все отметки о закрытых путях, следующий набор муравьёв пойдёт по старым путям
	}*/
	return (0);
}
