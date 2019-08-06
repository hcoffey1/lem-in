/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: smorty <smorty@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/07/21 17:58:11 by smorty            #+#    #+#             */
/*   Updated: 2019/08/07 00:04:18 by smorty           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lemin.h"

void		error(int error_type)
{
	if (error_type == ERR_ARGS)
		ft_putstr("ERROR: wrong arguments (use -h)\n");
	else if (error_type == ERR_ROOM)
		ft_putstr("ERROR: invalid room\n");
	else if (error_type == ERR_ANTS)
		ft_putstr("ERROR: invalid ants\n");
	else if (error_type == ERR_ENDS)
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
	ft_putstr("\t(lem-in will wait for input from stdin, ");
	ft_putstr("use '<' to redirect from file)\n");
	ft_putstr("flags:\n");
	ft_putstr("\t-h - usage\n");
	ft_putstr("\t-v - visual mode\n");
	ft_putstr("\t-d - debug mode (show paths found)\n");
	ft_putstr("\t-p - precise mode (a bit better results but slower)\n");
	ft_putstr("\t-n - output only the number of turns taken\n");
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
		else if (*arg == 'v')
			flags |= F_VISUAL;
		else if (*arg == 'd')
			flags |= F_DEBUG + F_TURNS;
		else if (*arg == 'p')
			flags |= F_SLOW;
		else if (*arg == 'n')
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

	flags = F_FULL;
	if (argc == 2 && **(++argv) == '-')
		flags = get_flags(*argv);
	else if (argc != 1)
		error(ERR_ARGS);
	if (!(map = read_input(NULL)))
		error(ERR_ANTS);
	colony = prepare_colony(map);
	colony->flags = flags;
	solution = explore_anthill(colony);
	if (flags & F_FULL)
		print_file(map);
	if (flags & F_DEBUG)
		print_paths(solution);
	open_the_gates(colony, solution, flags);
	return (0);
}
