/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   visualization.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: smorty <smorty@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/07/26 13:50:49 by smorty            #+#    #+#             */
/*   Updated: 2019/07/26 22:31:42 by smorty           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lemin.h"

static void	add_pixel(t_lemin *v, int x, int y)
{
	int pixel;

	if (x > 0 && x < v->width && y > 0 && y < v->height)
	{
		pixel = (x * v->bpp / 8) + (y * v->size_line);
		v->map[pixel] = (char)128;
		v->map[++pixel] = (char)128;
		v->map[++pixel] = (char)128;
		v->map[++pixel] = 0;
	}
}

static void	draw_line(t_lemin *v, int dots[4])
{
	int delta[2];
	int sign[2];
	int error[2];

	delta[X] = abs(dots[X + 2] - dots[X]);
	delta[Y] = abs(dots[Y + 2] - dots[Y]);
	sign[X] = dots[X] < dots[X + 2] ? 1 : -1;
	sign[Y] = dots[Y] < dots[Y + 2] ? 1 : -1;
	error[0] = delta[X] - delta[Y];
	while (dots[X] != dots[X + 2] || dots[Y] != dots[Y + 2])
	{
		add_pixel(v, dots[X], dots[Y]);
		error[1] = error[0] * 2;
		if (error[1] > -delta[Y])
		{
			error[0] -= delta[Y];
			dots[X] += sign[X];
		}
		if (error[1] < delta[X])
		{
			error[0] += delta[X];
			dots[Y] += sign[Y];
		}
	}
	add_pixel(v, dots[X + 2], dots[Y + 2]);
}

void		connect_dots(t_lemin *v, t_vertex **list_vertexs)
{
	int dots[4];
	int scale = 10;
	t_link	*p;

	while (*list_vertexs)
	{
		p = (*list_vertexs)->links;
		while (p)
		{
			dots[X] = (int)round((*list_vertexs)->x) * scale + v->width / 2;
			dots[Y] = (int)round((*list_vertexs)->y) * scale + v->height / 2;
			dots[X + 2] = (int)round(p->link->x) * scale + v->width / 2;
			dots[Y + 2] = (int)round(p->link->y) * scale + v->height / 2;
			draw_line(v, dots);
			p = p->next;
		}
		++list_vertexs;
	}
}

void        visualize_graph(t_vertex **list_vertexs)
{
    t_lemin *v;

	if (!(v = (t_lemin *)malloc(sizeof(t_lemin))))
		exit(-1);
    v->width = LEMIN_VIS_WIDTH;
	v->height = LEMIN_VIS_HEIGHT;
	if (!(v->mlx_p = mlx_init()))
		exit(-1);
	v->win_p = mlx_new_window(v->mlx_p, v->width, v->height, "Ant colony");
	v->img_p = mlx_new_image(v->mlx_p, v->width, v->height);
	if (!v->win_p || !v->img_p)
		exit(-1);
	if (!(v->map = mlx_get_data_addr(v->img_p, &v->bpp, &v->size_line, &v->endian)))
		exit(-1);
	ft_bzero(v->map, v->width * v->height * 4);
	connect_dots(v, list_vertexs);
	mlx_put_image_to_window(v->mlx_p, v->win_p, v->img_p, 0, 0);
//	mlx_hook(v->win_p, 2, 0, (int (*)())key_press, m);
	mlx_loop(v->mlx_p);
}
