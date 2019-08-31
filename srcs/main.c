/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: smorty <smorty@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/07/21 17:58:11 by smorty            #+#    #+#             */
/*   Updated: 2019/08/31 14:15:22 by smorty           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lemin.h"

static void	usage(void)
{
	ft_putstr("usage: lem-in [-flags] < map_file\n");
	ft_putstr("\t(lem-in will wait for input from stdin, ");
	ft_putstr("use '<' to redirect from file)\n");
	ft_putstr("flags:\n");
	ft_putstr("\t-h - usage\n");
	ft_putstr("\t-v - visual mode\n");
	ft_putstr("\t-d - debug mode (show paths found)\n");
	ft_putstr("\t-p - precise mode (a bit better results but slower)\n");
	ft_putstr("\t-n - don't show map and ants movements\n");
	exit(0);
}

static int	get_flags(char *arg)
{
	int flags;

	flags = F_FULL;
	++arg;
	while (*arg)
	{
		if (*arg == 'h')
			usage();
		else if (*arg == 'v' && !(flags & F_VISUAL))
			flags |= F_VISUAL;
		else if (*arg == 'd' && !(flags & F_DEBUG))
			flags |= F_DEBUG + F_TURNS;
		else if (*arg == 'p' && !(flags & F_SLOW))
			flags |= F_SLOW;
		else if (*arg == 'n' && (flags & F_FULL))
		{
			flags ^= F_FULL;
			flags |= F_TURNS;
		}
		else
			error(ERR_ARGS);
		++arg;
	}
	return (flags);
}

static void	cleanup(t_lemin *colony, t_paths *solution)
{
	void	*clean;
	int		i;

	clear_paths(solution);
	while (colony->ants)
	{
		clean = colony->ants;
		colony->ants = colony->ants->next;
		free(clean);
	}
	i = colony->verteces;
	while (i--)
	{
		free(colony->rooms[i]->name);
		free(colony->rooms[i]);
		free(colony->edges[i]);
	}
	free(colony->rooms);
	free(colony->edges);
	free(colony);
}

static void	print_file(t_input *map)
{
	while (map->next)
	{
		ft_putstr(map->line);
		ft_putchar('\n');
		free(map->line);
		map = map->next;
		free(map->prev);
	}
	ft_putstr(map->line);
	ft_putstr("\n\n");
	free(map->line);
	free(map);
}

static int	**save_original_matrix(int **matrix, int verteces)
{
	int **copy;
	int	i;
	int	j;

	if (!(copy = (int **)malloc(sizeof(int *) * verteces)))
		error(errno);
	i = verteces;
	while (i--)
	{
		if (!(copy[i] = (int *)malloc(sizeof(int) * verteces)))
			error(errno);
		j = verteces;
		while (j--)
			copy[i][j] = matrix[i][j];
	}
	return (copy);
}

static void	restore_original_connections(t_lemin *colony, int **matrix)
{
	int i;

	i = colony->verteces;
	while (i--)
	{
		free(colony->edges[i]);
		colony->edges[i] = matrix[i];
	}
	free(matrix);
}

int			main(int argc, char **argv)
{
	t_lemin	*colony;
	t_input	*map;
	t_paths	*solution;
	int		**edges_copy;
	int		flags;

	flags = F_FULL;
	if (argc == 2 && **(++argv) == '-')
		flags = get_flags(*argv);
	else if (argc != 1)
		error(ERR_ARGS);
	if (!(map = store_file(NULL)))
		error(ERR_ANTS);
	colony = prepare_colony(map);
	colony->flags = flags;
	edges_copy = save_original_matrix(colony->edges, colony->verteces);
	solution = explore_anthill(colony);
	restore_original_connections(colony, edges_copy);
	if (flags & F_FULL)
		print_file(map);
	if (flags & F_DEBUG)
		print_paths(solution);
	open_the_gates(colony, solution, flags);
	if (flags & F_VISUAL)
		visualizer(colony, map);
	cleanup(colony, solution);
	exit(0);
}
