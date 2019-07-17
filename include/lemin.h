/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lemin.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: smorty <smorty@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/07/17 23:36:34 by smorty            #+#    #+#             */
/*   Updated: 2019/07/18 00:12:55 by smorty           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LEMIN_H
# define LEMIN_H

typedef struct	s_room
{
	char			*name;
	int				x;
	int				y;
	int				status;
	struct s_lemin	**next;
}				t_room;

typedef struct	s_lemin
{
	int				ants;
	t_room			map;
}				t_lemin;

#endif
