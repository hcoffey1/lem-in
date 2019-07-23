/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pathfinding.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: smorty <smorty@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/07/23 18:32:02 by smorty            #+#    #+#             */
/*   Updated: 2019/07/23 22:29:00 by smorty           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lemin.h"

static void			clear_path(t_room **list_nodes, int flag)
{
	while (*list_nodes)
	{
		(*list_nodes)->visited = 0;
		(*list_nodes)->path = NULL;
		if (flag)
			(*list_nodes)->closed = 0;
		++list_nodes;
	}
}

static t_path_list	*add_path(t_path_list *path_list, t_rooms_queue *path, int len)
{
	t_path_list *new;

	new = (t_path_list *)malloc(sizeof(t_path_list));
	new->path = path;
	new->len = len;
	new->left = path_list;
	new->right = NULL;
	if (new->left)
		new->left->right = new;
	return (new);
}

static int			path_len(t_rooms_queue *path)
{
	int len;

	len = 0;
	while (path)
	{
		++len;
		path = path->right;
	}
	return (len);
}

static void			print_paths(t_path_list *path_list)
{
	t_rooms_queue *path;
	int n;

	n = 0;
	while (path_list)
	{
		printf("Path %d, len %d:\n", ++n, path_list->len);
		path = path_list->path;
		while (path)
		{
			printf("%s", path->val->name);
			path = path->right;
			printf(path ? "-" : "\n");
		}
		path_list = path_list->right;
	}
}

t_path_list			*pathfinding(t_room **list_nodes)
{
	t_path_list		*path_list;
	t_rooms_queue	*path;
	t_room			*start;
	int				i;

	i = 0;
	while (list_nodes[i]->type != 3) // поиск стартовой ноды
		++i;
	start = list_nodes[i];
	path_list = NULL;
	while ((path = bfs(start))) // пока находятся новые пути, доабавляем их в список путей
	{
		path_list = add_path(path_list, path, path_len(path));
		clear_path(list_nodes, 0); //удаляем отметки о просмотренных комнатах
	}
	clear_path(list_nodes, 1); //удаляем отметки о закрытых комнатах
	while (path_list->left)
		path_list = path_list->left;
	print_paths(path_list);
	return (path_list);
}
