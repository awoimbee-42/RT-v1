/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   intrisics.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: awoimbee <awoimbee@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/29 23:27:57 by awoimbee          #+#    #+#             */
/*   Updated: 2019/08/24 00:22:59 by awoimbee         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef INTRISICS_H
# define INTRISICS_H
# include <inttypes.h>
# include <string.h>
# include <x86intrin.h>

# ifdef __AVX__
#  define LFT_AVX 1
# else
#  define LFT_AVX 0
# endif
# ifdef __SSE2__
#  define LFT_SSE2 1
# else
#  define LFT_SSE2 0
# endif

# define GNL_BUFF_SIZE 32768
# define GNL_FLUSH (char**)INTPTR_MAX

typedef uint32_t	t_queued;
typedef void		t_listed;
typedef int			t_vected;

typedef int			t_map_key;
typedef long double	t_map_data;

typedef struct	s_list
{
	t_listed		*content;
	size_t			content_size;
	struct s_list	*next;
}				t_list;

typedef struct	s_queue
{
	int				start;
	int				end;
	int				size;
	t_queued		*arr;
}				t_queue;

typedef struct	s_vector
{
	t_vected		*arr;
	size_t			len;
	size_t			mem;
}				t_vector;

typedef struct	s_garbage
{
	void			**pointers;
	size_t			arr_len;
	size_t			mem_len;
}				t_garbage;

enum			e_rbtn_color
{
	BLACK = 0,
	RED
};

typedef struct	s_rbtmap
{
	t_map_key			key;
	t_map_data			*dat;
	enum e_rbtn_color	col;
	struct s_rbtmap		*lft;
	struct s_rbtmap		*rgt;
	struct s_rbtmap		*up;
}				t_rbtmap;

#endif
