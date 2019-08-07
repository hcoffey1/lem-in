/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lemin.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: smorty <smorty@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/07/27 15:35:02 by smorty            #+#    #+#             */
/*   Updated: 2019/08/07 20:08:09 by smorty           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LEMIN_H
# define LEMIN_H

# include <stdlib.h>
# include <stdio.h>
# include <errno.h>
# include "libft.h"

# define BUFF_SIZE		12

# define ERR_ARGS -1
# define ERR_ANTS -2
# define ERR_ROOM -3
# define ERR_ENDS -4
# define ERR_DUPE -5
# define ERR_PATH -6

# define F_FULL 1
# define F_SLOW 2
# define F_DEBUG 4
# define F_VISUAL 8
# define F_TURNS 16

# define OTHER 0
# define COMMENT 1
# define ROOM 2
# define LINK 3
# define START 4
# define END 5

typedef struct	s_mfile
{
	char			*line;
	int				type;
	struct s_mfile	*next;
	struct s_mfile	*prev;
}				t_mfile;

typedef struct	s_vertex
{
	char			*name;
	int				index;
	int				x;
	int				y;
	int				splitted;
	int				closed;
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
	int				len0;
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
	int				flags;
	int				**edges;
	t_vertex		**rooms;
	t_vertex		*start;
	t_vertex		*end;
	t_ants			*ants;
}				t_lemin;

t_lemin			*prepare_colony(t_mfile *map);
t_mfile			*read_input(t_mfile *prev);
void			validate(t_mfile *map, int *ants, int *verteces);
t_vertex		**build_anthill(t_mfile *map, int **edges, int verteces);
void			error(int error_type);
int				ft_printf(const char *s, ...);
t_paths			*explore_anthill(t_lemin *colony);
t_queue			*bfs(t_lemin *colony, int *len);
t_paths			*find_best_paths(t_lemin *colony, t_paths *path_list);
int				open_the_gates(t_lemin *colony, t_paths *path_list, int flags);
void			print_file(t_mfile *map);

t_queue			*new_queue(t_vertex *start);
void			push(t_queue **q, t_vertex *val);
void			push_front(t_queue **q, t_vertex *val);
void			pop(t_queue **q);

void			add_path(t_paths **path_list, t_queue *path, int len);
void			print_paths(t_paths *path_list);
void			sort_paths(t_paths *list);
void			clear_paths(t_paths *path_list);

#endif
