/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lemin.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: smorty <smorty@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/07/27 15:35:02 by smorty            #+#    #+#             */
/*   Updated: 2019/07/30 23:25:17 by smorty           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LEMIN_H
# define LEMIN_H

# include <stdlib.h>
# include <math.h>
# include "libft.h"
# include "get_next_line.h"

# define INF_PATH		0xABCDEF
# define ZERO_WEIGHT	0xBBCCDD
# define ABS(x) (x > 0 ? x : -x)

typedef struct	s_vertex
{
	char			*name;
	int				index;
	int				type;
	int				minpath;
	int				x;
	int				y;
	int				visited;
	int				splitted;
	struct s_vertex	*path;
}				t_vertex;

typedef struct	s_queue
{
	t_vertex		*top;
	struct s_queue	*next;
	struct s_queue	*prev;
}				t_queue;

typedef struct	s_paths
{
	t_queue			*path;
	int				len;
	struct s_paths	*next;
	struct s_paths	*prev;
}				t_paths;

typedef struct	s_ants
{
	int				num;
	t_vertex		*room;
	t_queue			*path;
	struct s_ants	*next;
}				t_ants;

typedef struct	s_lemin
{
	int				ants_num;
	int				verteces;
	int				**edges;
	t_vertex		**rooms;
	t_vertex		*start;
	t_vertex		*end;
	t_ants			*ants;
	t_paths			*path_list;
}				t_lemin;

t_lemin			*create_ant_colony(char *filename);
t_vertex		**process_file(t_lemin *colony, int fd);
int				check_line(char *line);
void			error(int error_type);
int				ft_printf(const char *s, ...);

t_queue			*new_queue(t_vertex *start);
void			push(t_queue **q, t_vertex *val);
void			push_front(t_queue **q, t_vertex *val);
void			pop(t_queue **q);


void			clear_path(t_vertex **rooms, int verteces);
t_paths			*find_paths(t_lemin *colony);
t_queue			*dijkstra(t_lemin *colony);
t_queue			*bellman_ford_algorithm(t_lemin *ant_farm);
t_queue			*get_path(t_lemin *colony);
void			open_the_gates(t_ants *ants, t_paths *path_list);

#endif
