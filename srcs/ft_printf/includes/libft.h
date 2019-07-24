/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   libft.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: smorty <smorty@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/03 20:54:28 by smorty            #+#    #+#             */
/*   Updated: 2019/06/05 19:54:03 by smorty           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LIBFT_H
# define LIBFT_H

# include <string.h>
# include <stdlib.h>
# include <unistd.h>

int				ft_atoi(char const *s);
void			ft_bzero(void *s, size_t n);
void			*ft_memset(void *s, int c, size_t n);
size_t			ft_strlen(char const *s);
int				ft_strnequ(char const *s1, char const *s2, size_t n);

#endif
