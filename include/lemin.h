/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lemin.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: smorty <smorty@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/07/17 23:36:34 by smorty            #+#    #+#             */
/*   Updated: 2019/07/24 21:23:58 by smorty           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LEMIN_H
# define LEMIN_H

#include <stdlib.h>
#include <unistd.h>

typedef struct	s_room
{
	char			*name;
	int				x;
	int				y;
	int				type;
	int				edges;
	int				visited;
	int				closed;
	struct	s_room	*path;
	struct	s_room	**links;
}				t_room;

typedef struct	s_rooms_queue
{
	t_room					*val;
	struct s_rooms_queue	*right;
	struct s_rooms_queue	*left;
}				t_rooms_queue;

typedef struct	s_path_list
{
	t_rooms_queue		*path;
	int					len;
	struct s_path_list	*right;
	struct s_path_list	*left;
}				t_path_list;

typedef struct	s_ants
{
	int					num;
	t_room				*room;
	t_rooms_queue		*path;
	struct s_ants		*next;
}				t_ants;


void			validate(int fd, int *ants, int *verteces, int *connects);
t_room			**read_file(int fd, int verteces, int connects);
t_rooms_queue	*bfs(t_room *start);
t_rooms_queue	*new_queue(t_room *start);
void			push(t_rooms_queue **q, t_room *val);
void			push_front(t_rooms_queue **q, t_room *val);
void			pop(t_rooms_queue **q);
t_path_list		*pathfinding(t_room **list_nodes);
void			open_the_gates(t_path_list *path_list, int ants);
int				ft_printf(const char *s, ...);
void			error(int error_type);

#endif
