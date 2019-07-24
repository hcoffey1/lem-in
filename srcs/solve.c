/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   solve.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: smorty <smorty@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/07/23 22:34:43 by smorty            #+#    #+#             */
/*   Updated: 2019/07/24 23:45:55 by smorty           ###   ########.fr       */
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

static void		relative_lens(t_path_list *path_list)
{
	int len;

	len = path_list->len;
	while (path_list)
	{
		path_list->len -= len;
		path_list = path_list->right;
	}
}

static void		distribute_paths(t_ants *ants, t_path_list *path_list, int ants_num) // smorty
{
	t_path_list *p;
	t_path_list *p0;

	relative_lens(path_list);
	(void)ants_num;
	p = path_list;
	while (p->right && p->len == path_list->len)
		p = p->right;
	p0 = path_list;
	while (ants)
	{
		while (ants && path_list != p)
		{
			ants->path = path_list->path;
			ants->room = ants->path->val;
			path_list = path_list->right;
			ants = ants->next;
		}
		if (ants && p && ants->num > p->len)
			p = p->right;
		path_list = p0;
	}
}
/*
static void		distribute_paths(t_ants *ants, t_path_list *path_list, int ants_num) // vrichese
{
	t_path_list	*p;
	int			min_len;
	int			distr;

	p = path_list->right;
	min_len = path_list->len;
	while (ants && p)
	{
		distr = (ants_num / p->len) * min_len;
		while (ants && distr--)
		{
			ants->path = p->path;
			ants->room = ants->path->val;
			ants = ants->next;
		}
		p = p->right;
	}
	while (ants)
	{
		ants->path = path_list->path;
		ants->room = ants->path->val;
		ants = ants->next;
	}
}*/

/*static void		distribute_paths(t_ants *ants, t_path_list *path_list, int ants_num) // another approach
{
	t_path_list	*longest;
	t_ants		*ants0;
	int			n;

	longest = path_list;
	ants0 = ants;
	while (ants0 && longest->right)
	{
		while (ants0 && ants0->num <= longest->len)
			ants0 = ants0->next;
		if (longest->right)
			longest = longest->right;
	}
	while (ants)
	{
		n = longest->len - path_list->len;
		if (!n)
			n = 1;
		while (n-- && ants)
		{
			ants->path = path_list->path;
			ants->room = ants->path->val;
			ants = ants->next;
		}
		if (!path_list->right)
			while (path_list->left)
				path_list = path_list->left;
		else
			path_list = path_list->right;
	}
}*/

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
		ft_printf("turn %d: ", ++num); // remove
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
	distribute_paths(ants_list, path_list, ants); // задаём каждому муравью путь
	ants_go(ants_list); // отправляем муравьёв
}
