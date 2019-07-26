/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: smorty <smorty@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/07/21 17:58:11 by smorty            #+#    #+#             */
/*   Updated: 2019/07/26 22:46:03 by smorty           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lemin.h"
#include "get_next_line.h"

void	error(int error_type)
{
	(void)error_type;
	ft_printf("ERROR\n");
	exit(-1);
}

void	cleanup(t_vertex **list_nodes, t_path_list *path_list)
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
}

void	print_file(int fd)
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
}

int main(int argc, char **argv)
{
	t_vertex		**list_nodes;
	t_path_list *path_list;
	int		fd;
	int		ants;
	int		verteces;
	int		connects;

	if (argc != 2)
		return (0);
	fd = open(*(argv + 1), O_RDONLY);
	validate(fd, &ants, &verteces, &connects);
	close(fd);
	fd = open(*(argv + 1), O_RDONLY);
	list_nodes = read_file(fd, verteces, connects);
	close(fd);
	fd = open(*(argv + 1), O_RDONLY);
	print_file(fd);
	close(fd);
//	visualize_graph(list_nodes);
	path_list = pathfinding(list_nodes);
//	open_the_gates(path_list, ants);
//	cleanup(list_nodes, path_list);
	return (0);
}
