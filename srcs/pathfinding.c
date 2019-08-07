/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pathfinding.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: smorty <smorty@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/07/29 21:17:41 by smorty            #+#    #+#             */
/*   Updated: 2019/08/07 23:27:54 by smorty           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lemin.h"

static void		remove_intersections(int **edges, int verteces)
{
	int i;
	int j;

	i = 0;
	while (i < verteces)
	{
		j = 0;
		while (j < verteces)
		{
			if (edges[i][j] && edges[j][i])
			{
				edges[i][j] = 0;
				edges[j][i] = 0;
			}
			++j;
		}
		++i;
	}
}

static void		routing(t_paths *path_list, int **edges, int verteces, int n)
{
	t_queue	*path;
	int		i;

	i = 0;
	while (i < verteces)
		ft_bzero(edges[i++], sizeof(int) * verteces);
	while (path_list && n--)
	{
		path = path_list->path;
		while (path->next)
		{
			++edges[path->top->index][path->next->top->index];
			path->top->splitted = 0;
			path = path->next;
		}
		path_list = path_list->next;
	}
	remove_intersections(edges, verteces);
}

static int		starting_point(t_paths *path_list, int flag_slow)
{
	int n;

	n = 0;
	while (path_list)
	{
		++n;
		path_list = path_list->next;
	}
	n = n / 3 - flag_slow;
	return (n < 4 ? 4 - flag_slow : n);
}

static t_paths	*check_paths(t_lemin *colony, t_paths *path_list, int *turns)
{
	static int	start = 0;
	t_paths		*set;
	t_queue		*path;
	int			len;

	if (!start)
		start = starting_point(path_list, colony->flags & F_SLOW);
	routing(path_list, colony->edges, colony->verteces, start++);
	set = NULL;
	while ((path = bfs(colony, &len)))
		add_path(&set, path, len);
	while (set->prev)
		set = set->prev;
	sort_paths(set);
	*turns = open_the_gates(colony, set, 0);
	return (set);
}

t_paths			*find_best_paths(t_lemin *colony, t_paths *path_list)
{
	t_paths	*p;
	t_paths	*best_set;
	t_paths	*curr_set;
	int		best_turns;
	int		curr_turns;

	best_set = check_paths(colony, (p = path_list), &best_turns);
	curr_set = NULL;
	while ((p = p->next))
	{
		curr_set = check_paths(colony, path_list, &curr_turns);
		if (curr_turns >= best_turns + (colony->flags & F_SLOW))
			break ;
		else if (curr_turns < best_turns)
		{
			best_turns = curr_turns;
			clear_paths(best_set);
			best_set = curr_set;
		}
		else
			clear_paths(curr_set);
	}
	clear_paths(curr_set);
	clear_paths(path_list);
	return (best_set);
}
