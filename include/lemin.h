/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lemin.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: smorty <smorty@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/07/27 15:35:02 by smorty            #+#    #+#             */
/*   Updated: 2019/07/27 15:53:17 by smorty           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LEMIN_H
# define LEMIN_H

# define INF_WEIGHT	0xABCDEF

typedef struct	s_vertex
{
	char			*name;
	int				index;
	int				type;
	int				x;
	int				y;
	int				visited;
	struct s_vertex	*next;
	struct s_vertex	*prev;
}				t_vertex;

typedef struct	s_edge
{
	int				index;
	int				weight;
	int				direction;
	struct s_edge	*next;
	struct s_edge	*prev;
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
	t_vertex		*rooms;
	t_edge			**adjacency;
	t_vertex		*start;
	t_vertex		*end;
	int				ants_num;
	t_ants			*ants;
	t_paths			*path_list;
}				t_lemin;

#endif
