/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pathfinding.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: smorty <smorty@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/07/29 21:17:41 by smorty            #+#    #+#             */
/*   Updated: 2019/08/06 23:51:27 by smorty           ###   ########.fr       */
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

static void		apply_paths(t_paths *path_list, int **edges, int verteces)
{
	static int	n = 5;
	t_queue		*path;
	int			i;

	i = 0;
	while (i < verteces)
		ft_bzero(edges[i++], sizeof(int) * verteces);
	i = ++n;
	while (path_list && --i)
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

static t_paths	*check_paths(t_lemin *colony, t_paths *path_list, int *turns)
{
	static int	max_paths_num = 0;
	t_paths		*set;
	t_queue		*path;
	int			len;
	int			curr_num;

	apply_paths(path_list, colony->edges, colony->verteces);
	set = NULL;
	curr_num = 0;
	while ((path = bfs(colony, &len)))
	{
		add_path(&set, path, len);
		++curr_num;
//		ft_printf("{green}%d {eoc}", len);
	}
	max_paths_num = curr_num;
	while (set->prev)
		set = set->prev;
	sort_paths(set);
//	if (curr_num <= max_paths_num && !(colony->flags & F_SLOW))
	*turns = open_the_gates(colony, set, 0);
//	ft_printf("{blue}%d {eoc}\n", *turns);
	return (set);
}

static void	restore_lens(t_paths *set)
{
	while (set)
	{
		set->len = set->len0;
		set = set->next;
	}
}

t_paths		*find_best_paths(t_lemin *colony, t_paths *path_list)
{
	t_paths	*p;
	t_paths	*best_set;
	t_paths	*curr_set;
	int		best_turns;
	int		curr_turns;

	best_set = check_paths(colony, path_list, &best_turns);
	p = path_list->next;
	while (p)
	{
		curr_set = check_paths(colony, path_list, &curr_turns);
		if (curr_turns < best_turns)
		{
			best_turns = curr_turns;
			clear_paths(best_set);
			best_set = curr_set;
		}
		else if (!(colony->flags & F_SLOW))
			break ;
		else
			clear_paths(curr_set);
		p = p->next;
	}
	clear_paths(path_list);
	restore_lens(best_set);
	return (best_set);
}
