/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lemin_visualizer.h                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: smorty <smorty@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/09/03 17:49:39 by smorty            #+#    #+#             */
/*   Updated: 2019/09/03 23:09:12 by smorty           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LEMIN_VISUALIZER_H
# define LEMIN_VISUALIZER_H

# include <math.h>
# include "SDL.h"
# include "SDL_ttf.h"
# include "lemin.h"

# define LEMIN_SCREEN_WIDTH 1920
# define LEMIN_SCREEN_HEIGHT 1200
# define X_OFFSET 50
# define Y_OFFSET 100

typedef struct	s_assets
{
	TTF_Font		*font;
	SDL_Texture		*node;
	SDL_Texture		*start;
	SDL_Texture		*end;
	SDL_Texture		*closed;
	SDL_Texture		*ant;
}				t_assets;

typedef struct  s_vis_status
{
	int				speed;
	int				paths_shown;
	int				pause;
	int				ended;
	int				quit;
}               t_vis_status;

typedef struct	s_lemin_vis
{
	SDL_Renderer	*renderer;
	t_assets		*assets;
	const t_lemin	*colony;
    t_vis_status	*status;
	float			x_scale;
	float			y_scale;
	float			object_scale;
	int				node_size;
	int				moves;
}				t_lemin_vis;

typedef struct	s_ants_vis
{
	SDL_Rect			*ant;
	float				curr_x;
	float				curr_y;
	float				delta_x;
	float				delta_y;
	int					x_dest;
	int					y_dest;
	struct s_ants_vis	*next;
	struct s_ants_vis	*prev;
}				t_ants_vis;

t_lemin_vis		*init_visualizer(SDL_Window **window, const t_lemin *colony);
void			visualization_loop(t_lemin_vis *game, t_paths *solution);
void			visualize(t_lemin_vis *game, t_ants_vis **ants_list);
void			render_edges(t_lemin_vis *game);
void			render_nodes(t_lemin_vis *game);
void			render_ants(t_lemin_vis *game, t_ants_vis **ants_list);
void			render_numbers(t_lemin_vis *game);
SDL_Texture		*get_string_texture(t_lemin_vis *game, const char *text);
void			error_vis(const char *error_msg);

#endif