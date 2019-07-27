/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   queue.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: smorty <smorty@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/07/23 17:06:53 by smorty            #+#    #+#             */
/*   Updated: 2019/07/26 22:31:42 by smorty           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lemin.h"

t_vertexs_queue *new_queue(t_vertex *start)
{
	t_vertexs_queue *new;

	new = (t_vertexs_queue *)malloc(sizeof(t_vertexs_queue));
	new->val = start;
	new->right = NULL;
	new->left = NULL;
	return (new);
}

void	push(t_vertexs_queue **q, t_vertex *val)
{
	while ((*q)->right)
		*q = (*q)->right;
	(*q)->right = (t_vertexs_queue *)malloc(sizeof(t_vertexs_queue));
	(*q)->right->left = *q;
	(*q) = (*q)->right;
	(*q)->val = val;
	(*q)->right = NULL;
}

void	push_front(t_vertexs_queue **q, t_vertex *val)
{
	while ((*q)->left)
		*q = (*q)->left;
	(*q)->left = (t_vertexs_queue *)malloc(sizeof(t_vertexs_queue));
	(*q)->left->right = *q;
	*q = (*q)->left;
	(*q)->val = val;
	(*q)->left = NULL;
}

void	pop(t_vertexs_queue **q)
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
