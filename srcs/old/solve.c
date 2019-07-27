/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   solve.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: smorty <smorty@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/07/23 22:34:43 by smorty            #+#    #+#             */
/*   Updated: 2019/07/26 22:31:42 by smorty           ###   ########.fr       */
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

static t_vertexs_queue	*shortest_path(t_path_list *path_list)
{
	t_path_list	*path;
	int			len;

	path = path_list;
	while (path->right)
		path = path->right;
	len = path->len;
	while (path_list)
	{
		if (path_list->len < len)
		{
			len = path_list->len;
			path = path_list;
		}
		path_list = path_list->right;
	}
	if (path->len)
		++path->len;
	return (path->path);
}

static void		distribute_paths(t_ants *ants, t_path_list *path_list)
{
	t_path_list *p;
	t_path_list *p0;

	while (ants)
	{
		ants->path = shortest_path(path_list);
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
	int num = 0;

	ants0 = ants;
	while (!all_gone(ants0)) // пока все не находятся в последней комнате
	{
		ft_printf("turn %3d: ", ++num); // remove
		ants = ants0;
		while (ants) // для каждого муравья
		{
			if (ants->room->type != 4 && !ants->path->right->val->closed) // если текущая комната муравья не последняя и следующая не занята
			{
				ft_printf("L%d-", ants->num);
				ants->room->closed = 0; // открываем текующую комнату
				ants->path = ants->path->right; // переходим к следующей
				ants->room = ants->path->val;
				if (ants->room->type != 4) // и закрываем её
					ants->room->closed = 1;
				ft_printf("%s ", ants->room->name);
			}
			ants = ants->next;
		}
		ft_printf("\n");
	}
}

void			open_the_gates(t_path_list *path_list, int ants)
{
	t_ants *ants_list;

	ants_list = prepare_ants(ants); // собираем муравьёв в список
	distribute_paths(ants_list, path_list); // задаём каждому муравью путь
	ants_go(ants_list); // отправляем муравьёв
}
