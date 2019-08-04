/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   queue.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: smorty <smorty@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/07/23 17:06:53 by smorty            #+#    #+#             */
/*   Updated: 2019/08/04 18:28:30 by smorty           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lemin.h"

t_queue *new_queue(t_vertex *start)
{
	t_queue *new;

	if (!(new = (t_queue *)malloc(sizeof(t_queue))))
		error(errno);
	new->top = start;
	new->next = NULL;
	new->prev = NULL;
	return (new);
}

void	push(t_queue **q, t_vertex *val)
{
	while ((*q)->next)
		*q = (*q)->next;
	if (!((*q)->next = (t_queue *)malloc(sizeof(t_queue))))
		error(errno);
	(*q)->next->prev = *q;
	(*q) = (*q)->next;
	(*q)->top = val;
	(*q)->next = NULL;
}

void	push_front(t_queue **q, t_vertex *val)
{
	while ((*q)->prev)
		*q = (*q)->prev;
	if (!((*q)->prev = (t_queue *)malloc(sizeof(t_queue))))
		error(errno);
	(*q)->prev->next = *q;
	*q = (*q)->prev;
	(*q)->top = val;
	(*q)->prev = NULL;
}

void	pop(t_queue **q)
{
	while ((*q)->prev)
		*q = (*q)->prev;
	if ((*q)->next)
		*q = (*q)->next;
	if ((*q)->prev)
	{
		free((*q)->prev);
		(*q)->prev = NULL;
	}
	else
	{
		free(*q);
		*q = NULL;
	}
}
