/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lemin.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: smorty <smorty@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/07/17 23:36:34 by smorty            #+#    #+#             */
/*   Updated: 2019/07/22 15:10:14 by smorty           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LEMIN_H
# define LEMIN_H

#include <stdio.h> //remove
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

typedef struct	s_lemin
{
	int				ants;
	t_room			map;
}				t_lemin;


typedef struct	s_rooms_queue
{
	t_room			*val;
	struct s_rooms_queue	*next;
}				t_rooms_queue;

typedef struct	s_path_list
{
	t_rooms_queue		*path;
	int					len;
	struct s_path_list	*right;
	struct s_path_list	*left;
}				t_path_list;

int				depth_first_search(t_room *start);
t_rooms_queue	*breadth_first_search(t_room *start, t_rooms_queue *q);
void			validate(int fd, int *ants, int *verteces, int *connects);
t_room			**read_file(int fd, int verteces, int connects);

#endif
