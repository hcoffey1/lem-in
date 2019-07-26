/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lemin.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: smorty <smorty@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/07/17 23:36:34 by smorty            #+#    #+#             */
/*   Updated: 2019/07/26 22:46:27 by smorty           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LEMIN_H
# define LEMIN_H

#include <stdlib.h>
#include <unistd.h>
#include <mlx.h>
#include <math.h>

#define X 0
#define Y 1
#define LEMIN_VIS_WIDTH 1920
#define LEMIN_VIS_HEIGHT 1200

typedef struct	s_lemin
{
	void			*mlx_p;
	void			*win_p;
    void			*img_p;
	char			*map;
	int				bpp;
	int				size_line;
	int				endian;
	int				width;
	int				height;
}				t_lemin;

typedef struct	s_edge
{
	int				dir;
	int				weight;
	struct s_edge	*next;
	struct s_vertex *right;
	struct s_vertex	*left;
}				t_edge;

typedef struct	s_vertex
{
	char			*name;
	int				x;
	int				y;
	int				type;
	int				visited;
	struct s_links	*edges;
}				t_vertex;

typedef struct	s_links
{
	t_edge			*edge;
	struct s_links	*next;
}				t_links;

typedef struct	s_queue
{
	t_vertex		*val;
	struct s_queue	*right;
	struct squeue	*left;
}				t_queue;

typedef struct	s_path_list
{
	t_queue				*path;
	int					len;
	struct s_path_list	*right;
	struct s_path_list	*left;
}				t_path_list;

typedef struct	s_ants
{
	int					num;
	t_vertex			*room;
	t_queue				*path;
	struct s_ants		*next;
}				t_ants;

void			validate(int fd, int *ants, int *verteces, int *connects);
t_vertex		**read_file(int fd, int verteces, int connects);
t_queue			*bfs(t_vertex *start);
t_queue			*new_queue(t_vertex *start);
void			push(t_queue **q, t_vertex *val);
void			push_front(t_queue **q, t_vertex *val);
void			pop(t_queue **q);
t_path_list		*pathfinding(t_vertex **list_nodes);
void			open_the_gates(t_path_list *path_list, int ants);
int				ft_printf(const char *s, ...);
void			error(int error_type);
void			ft_bzero(void *s, size_t n);
void			visualize_graph(t_vertex **list_vertexs);
void			dijkstra(t_vertex *start);
void			bellman_ford(t_vertex **list_vertexs);

#endif
