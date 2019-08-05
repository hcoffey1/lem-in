/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: smorty <smorty@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/07/21 17:58:11 by smorty            #+#    #+#             */
/*   Updated: 2019/08/05 23:53:40 by smorty           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lemin.h"

void		error(int error_type)
{
	if (error_type == ERR_ARGS)
		ft_putstr("ERROR: wrong arguments (use -h)\n");
	else if (error_type == ERR_LINE)
		ft_putstr("ERROR: invalid line\n");
	else if (error_type == ERR_ROOM)
		ft_putstr("ERROR: invalid room\n");
	else if (error_type == ERR_LINK)
		ft_putstr("ERROR: invalid links\n");
	else if (error_type == ERR_ANTS)
		ft_putstr("ERROR: invalid ants\n");
	else if (error_type == ERR_STEN)
		ft_putstr("ERROR: invalid ##start and/or ##end\n");
	else if (error_type == ERR_DUPE)
		ft_putstr("ERROR: duplicate room name and/or coordinates\n");
	else if (error_type == ERR_PATH)
		ft_putstr("ERROR: no paths found\n");
	else
		perror("ERROR");
	exit(error_type);
}

static void	usage(void)
{
	ft_putstr("usage: lem-in [-flags] < map_file\n");
	ft_putstr("\t(lem-in will wait for input from stdin, use '<' to redirect from file)\n");
	ft_putstr("flags:\n");
	ft_putstr("\t-h - show usage\n");
	ft_putstr("\t-v - visual mode\n");
	ft_putstr("\t-d - debug mode\n");
	ft_putstr("\t-s - slow mode (slightly better results with much more time)\n");
	ft_putstr("\t-n - no path nor map output, just the number of turns taken\n");
	exit(0);
}

static int	get_flags(char *arg)
{
	int flags;

	flags = 0;
	++arg;
	while (*arg)
	{
		if (*arg == 'h')
			usage();
		else if (*arg == 'v')
			flags |= 1;
		else if (*arg == 'd')
			flags |= 2;
		else if (*arg == 's')
			flags |= 4;
		else if (*arg == 'n')
			flags |= 8;
		else
			error(ERR_ARGS);
		++arg;
	}
	return (flags);
}

static void	print_file(t_mfile *map)
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

int			main(int argc, char **argv)
{
	t_lemin	*colony;
	t_mfile	*map;
	t_paths	*solution;
	int		fd;
	int		flags;

	if (argc == 2 && **(++argv) == '-')
		flags = get_flags(*argv);
	else if (argc != 1)
		error(ERR_ARGS);
	if (!(map = read_input(NULL)))
		error(ERR_LINE);
	colony = prepare_colony(map);
	colony->flags = flags;
	solution = find_paths(colony);
	print_file(map);
	open_the_gates(colony, solution, 1);
	return (0);
}
