/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   queue.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: smorty <smorty@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/07/23 17:06:53 by smorty            #+#    #+#             */
/*   Updated: 2019/07/23 19:30:58 by smorty           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lemin.h"

t_rooms_queue *new_queue(t_room *start)
{
	t_rooms_queue *new;

	new = (t_rooms_queue *)malloc(sizeof(t_rooms_queue));
	new->val = start;
	new->right = NULL;
	new->left = NULL;
	return (new);
}

void	push(t_rooms_queue **q, t_room *val)
{
	while ((*q)->right)
		*q = (*q)->right;
	(*q)->right = (t_rooms_queue *)malloc(sizeof(t_rooms_queue));
	(*q)->right->left = *q;
	(*q) = (*q)->right;
	(*q)->val = val;
	(*q)->right = NULL;
}

void	push_front(t_rooms_queue **q, t_room *val)
{
	while ((*q)->left)
		*q = (*q)->left;
	(*q)->left = (t_rooms_queue *)malloc(sizeof(t_rooms_queue));
	(*q)->left->right = *q;
	*q = (*q)->left;
	(*q)->val = val;
	(*q)->left = NULL;
}

void	pop(t_rooms_queue **q)
{
	while ((*q)->left)
		*q = (*q)->left;
	if ((*q)->right)
		*q = (*q)->right;
	if ((*q)->left)
	{
		free((*q)->left);
		(*q)->left = NULL;
	}
	else
	{
		free(*q);
		*q = NULL;
	}
}
