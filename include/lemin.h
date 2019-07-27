/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lemin.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: smorty <smorty@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/07/27 15:35:02 by smorty            #+#    #+#             */
/*   Updated: 2019/07/27 18:35:54 by smorty           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LEMIN_H
# define LEMIN_H

# include <stdlib.h>
# include "libft.h"
# include "get_next_line.h"

# define INF_WEIGHT	0xABCDEF

typedef struct	s_vertex
{
	char			*name;
	int				index;
	int				type;
	int				weight;
	int				x;
	int				y;
	int				visited;
}				t_vertex;

typedef struct	s_edge
{
	int				index;
	int				weight;
	int				direction;
	t_vertex		*left;
	t_vertex		*right;
}				t_edge;

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
	t_vertex		**rooms;
	t_edge			***adjacency;
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

#endif
