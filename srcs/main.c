/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: smorty <smorty@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/07/21 17:58:11 by smorty            #+#    #+#             */
/*   Updated: 2019/07/23 22:49:30 by smorty           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lemin.h"
#include "get_next_line.h"

void	cleanup(t_room **list_nodes, t_path_list *path_list)
{
	t_rooms_queue	*path;
	void			*ptr;

	ptr = (t_room **)list_nodes;
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

int main(int argc, char **argv)
{
	t_room		**list_nodes;
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
	path_list = pathfinding(list_nodes);
	printf("-----------------\n");
	open_the_gates(path_list, ants);
	cleanup(list_nodes, path_list);
	return (0);
}
