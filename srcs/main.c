/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: smorty <smorty@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/07/21 17:58:11 by smorty            #+#    #+#             */
/*   Updated: 2019/07/27 18:47:23 by smorty           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lemin.h"

void		error(int error_type)
{
	(void)error_type;
	ft_printf("ERROR\n");
	exit(-1);
}

/*void	cleanup(t_vertex **list_nodes, t_path_list *path_list)
{
	t_queue			*path;
	void			*ptr;

	ptr = (t_vertex **)list_nodes;
	while (*list_nodes)
	{
		free((*list_nodes)->name);
		free((*list_nodes)->links);
		free(*list_nodes++);
	}
	free(ptr);
	while (path_list)
	{
		ptr = (t_path_list *)path_list;
		path = path_list->path;
		while (path->right)
		{
			path = path->right;
			free(path->left);
		}
		free(path);
		path_list = path_list->right;
		free(ptr);
	}
}*/

static void	print_file(int fd)
{
	char	*line;
	int		r;

	while ((r = get_next_line(fd, &line)))
	{
		if (r < 0)
			error(-1);
		ft_printf("%s\n", line);
		free(line);
	}
	ft_printf("\n");
	close(fd);
}

int			main(int argc, char **argv)
{
	t_lemin *colony;

	if (argc != 2)
		return (0);
	colony = create_ant_colony(*(argv + 1));
	int i = 0;
/*	int j;
	while (i < colony->verteces)
	{
		j = 0;
		while (j < colony->verteces)
		{
		if (!(colony->adjacency[i][j]))
			ft_printf("0 ");
		else
			ft_printf("1 ");
		++j;
		}
		ft_printf("\n");
		++i;
	}*/
	i = 0;
	while (i < colony->verteces)
	{
		ft_printf("%s %d\n", colony->rooms[i]->name, colony->rooms[i]->index);
		++i;
	}
//	print_file(open(*(argv + 1), O_RDONLY));
//	cleanup(list_nodes, path_list);
	return (0);
}
