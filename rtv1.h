/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rtv1.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: awoimbee <awoimbee@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/01/08 10:37:06 by awoimbee          #+#    #+#             */
/*   Updated: 2019/01/09 15:56:29 by awoimbee         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef RTV1_H
# define RTV1_H

# include <fcntl.h>
# include <unistd.h>
# include <stdlib.h>
# include <stdio.h> //perror strerror
# include <math.h>

# include "libft.h"
# include "mlx.h"

# define RES_H 1280
# define RES_V 720
# define NEAR_PLANE 0.0001
# define FAR_PLANE 500.

struct s_env;

union u_color;
struct s_ray;
struct s_disp;

struct s_img;
struct s_mlx;

struct s_vec3;
struct s_vec2;
struct s_int2;

/*
**	'BASIC' DATATYPES
*/

typedef struct	s_vec3
{
	float			x;
	float			y;
	float			z;
}				t_vec3;

typedef struct	s_vec2
{
	float			x;
	float			y;
}				t_vec2;

typedef struct	s_int2
{
	int				x;
	int				y;
}				t_int2;

/*
**	RAY TRACING STRUCTS
*/

typedef union	u_color
{
	struct
	{
		char	r;
		char	g;
		char	b;
		char	a;
	}				charc;
	unsigned int	intc;
}				t_color;

typedef struct	s_ray
{
	struct s_vec3	org;
	struct s_vec3	dir;
}				t_ray;

typedef struct	s_disp
{
	struct s_int2	res;
	float			aspect_ratio;
	struct s_vec2	fov;
}				t_disp;

/*
**	MLX STRUCTS
*/

typedef struct	s_img
{
	void			*ptr;
	int				*data;
	int				line_s;
	int				bpp;
	int				endian;
}				t_img;

typedef struct	s_mlx
{
	void			*ptr;
	void			*win;
	struct s_img	img;
}				t_mlx;

/*
**	OBJECTS
**	Reading the function pointer is not problem as it is the common first member
**	of all the structs, it's part of the C standard :)
**	DON'T TOUCH u_object TOO MUCH, NORMINETTE FREAKS OUT EASILY
*/

typedef enum	e_material
{
	MAT,
	GLOSS
}				t_material;

typedef float(*t_intsect)(const union u_object*, t_ray);

union			u_object
{
	struct		s_sphere
	{
		t_intsect		intersect;
		t_vec3			orig;
		float			radius;
	}				sphere;
	struct		s_plane
	{
		t_intsect		intersect;
		t_vec3			orig;
		t_vec3			norm;
	}				plane;
};

typedef struct	s_obj
{
	enum e_material	material;
	union u_color	color;
	union u_object	this;
}				t_obj;

/*
**	ENV
*/

typedef struct	s_env
{
	struct s_mlx	*mlx;
	struct s_disp	disp;
	struct s_ray	camera;
	int				objs_nb;
	struct s_obj	*objs_arr;
	long			keys_presed; // C'est pour utliser des operatioons binaires, sinon on peut utiliser un char[..]
}				t_env;

union u_color	render(t_env *env);
void			error(int	err_nb);

/*
**	init_argv.c
*/
void			usage(void);
void			read_argv(t_env *env, char **argv, int argc);
void			init(t_env *env, t_mlx *mlx);

/*
**	vec3_op.c
*/
t_vec3			*vec3_add(t_vec3 *restrict a, const t_vec3 *restrict b);
t_vec3			*vec3_sub(t_vec3 *restrict a, const t_vec3 *restrict b);
t_vec3			*vec3_multv(t_vec3 *restrict a, const t_vec3 *restrict b);
t_vec3			*vec3_multf(t_vec3 *restrict a, const float b);
t_vec3			*vec3_div(t_vec3 *restrict a, const t_vec3 *restrict b);
float			vec3_dot(const t_vec3 *restrict a, const t_vec3 *restrict b);
float			vec3_mod(const t_vec3 *restrict a);
t_vec3			*vec3_normalize(t_vec3 *restrict a);

/*
**	intersection.c
*/
float			hit_distance_sphere(const union u_object *obj, t_ray ray);

#endif
