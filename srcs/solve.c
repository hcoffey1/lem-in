/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   solve.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: smorty <smorty@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/07/23 22:34:43 by smorty            #+#    #+#             */
/*   Updated: 2019/07/23 23:10:50 by smorty           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lemin.h"

static t_ants	*prepare_ants(int num)
{
	t_ants		*ants;
	static int	n = 0;

	if (!num)
		return (NULL);
	ants = (t_ants *)malloc(sizeof(t_ants));
	ants->num = ++n;
	ants->path = NULL;
	ants->room = NULL;
	ants->next = prepare_ants(--num);
	return (ants);
}

static void		distribute_paths(t_ants *ants, t_path_list *path_list)
{
	while (ants)
	{
		if (ants->num > path_list->len && path_list->right) // пока просто если номер муравья больше длины маршрута
			path_list = path_list->right; 					// переходим к следующему маршруту
		ants->path = path_list->path;
		ants->room = ants->path->val;
		ants = ants->next;
	}
}

static int		all_gone(t_ants *ants)
{
	while (ants)
	{
		if (ants->room->type != 4)
			return (0);
		ants = ants->next;
	}
	return (1);
}

static void		ants_go(t_ants *ants)
{
	t_ants *ants0;

	ants0 = ants;
	while (!all_gone(ants0)) // пока все не находятся в последней комнате
	{
		ants = ants0;
		while (ants) // для каждого муравья
		{
			if (ants->room->type != 4 && !ants->path->right->val->closed) // если текущая комната муравья не последняя и следующая не занята
			{
				printf("L%d %s-", ants->num, ants->room->name);
				ants->room->closed = 0; // открываем текующую комнату
				ants->path = ants->path->right; // переходим к следующей
				ants->room = ants->path->val;
				if (ants->room->type != 4) // и закрываем её
					ants->room->closed = 1;
				printf("%s | ", ants->room->name);
			}
			ants = ants->next;
		}
		printf("\n");
	}
}

void			open_the_gates(t_path_list *path_list, int ants)
{
	t_ants *ants_list;

	ants_list = prepare_ants(ants); // собираем муравьёв в список
	distribute_paths(ants_list, path_list); // задаём каждому муравью путь
	ants_go(ants_list); // отправляем муравьёв
}
