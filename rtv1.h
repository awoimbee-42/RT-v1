/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rtv1.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: awoimbee <awoimbee@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/01/08 10:37:06 by awoimbee          #+#    #+#             */
/*   Updated: 2019/01/20 23:49:40 by awoimbee         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef RTV1_H
# define RTV1_H

# include <fcntl.h>
# include <unistd.h>
# include <stdlib.h>
# include <stdio.h> //perror strerror
# include <math.h>
# include <limits.h>

# include "libft.h"
# include "mlx.h"

# define RES_H 1280
# define RES_V 720

struct s_vec3;
struct s_vec2;
struct s_int2;

union u_color;
struct s_ray;
struct s_disp;

struct s_img;
struct s_mlx;

enum e_material;
union u_object;
struct s_obj;

struct s_env;

/*	###############################
**	#        FUCKING TRASH        #
**	###############################
*/

typedef struct	s_id_dist
{
	int				id;
	float			dist;
}				t_id_dist;

/*
**	###################################
**	#        'BASIC' DATATYPES        #
**	###################################
*/

typedef struct	s_float3
{
	float			x;
	float			y;
	float			z;
}				t_flt3;

typedef struct	s_float2
{
	float			x;
	float			y;
}				t_flt2;

typedef struct	s_int2
{
	int				x;
	int				y;
}				t_int2;

typedef t_flt3
t_vec3;

typedef t_flt3
t_fcolor;

typedef t_flt3
t_coords;

/*
**	#####################################
**	#        RAY TRACING STRUCTS        #
**	#####################################
*/

typedef struct	s_ray
{
	t_coords		org;
	t_vec3			dir;
}				t_ray;

typedef struct	s_disp
{
	struct s_int2	res;
	float			aspect_ratio;
	float			fov;
}				t_disp;

/*
**	#############################
**	#        MLX STRUCTS        #
**	#############################
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
**	########################
**	#        LIGHTS        #
**	########################
*/

typedef struct	s_light
{
	t_vec3			pos;
	t_fcolor		intensity;
}				t_light;

/*
**	##########################################
**	#        OBJECTS STRUCTS / UNIONS        #
**	##########################################
*/

typedef enum	e_material
{
	MAT,
	GLOSS
}				t_material;

/*
**	Reading the function pointers is not problem as it is the common
**	 first member of all the structs.
**	Beware, the norminette doesn't like this.. thing!
*/

typedef float(*t_distfun)(const union u_object*, const t_ray);
typedef t_vec3(*t_normfun)(const union u_object*, const t_vec3);

union			u_object
{
	struct		s_sphere
	{
		t_vec3			orig;
		float			radius;
	}				sphere;
	struct		s_plane
	{
		t_vec3			orig;
		t_vec3			norm;
	}				plane;
	struct		s_disk
	{
		t_vec3			orig;
		t_vec3			norm;
		float			radius2;
	}				disk;
	struct		s_cone
	{
		t_vec3			orig;
		t_vec3			height; // it's a height vector, which goes from the origin (tip) to the base
		float			angle;
	}				cone;
};

typedef struct	s_obj
{
	t_distfun		distfun;
	t_normfun		normfun;
	// enum e_material	material;
	float			diffuse;
	float			specular;
	t_fcolor		color;
	union u_object	this;
}				t_obj;

/*
**	############################
**	#        ENV STRUCT        #
**	############################
*/

typedef struct	s_env
{
	struct s_mlx	*mlx;
	struct s_disp	disp;
	struct s_ray	camera;
	int				objs_nb;
	struct s_obj	*objs_arr;
	int				light_nb;
	struct s_light	*light_arr;
	t_fcolor		bckgrnd_col;
	long			keys_presed; // C'est pour utliser des operatioons binaires, sinon on peut utiliser un char[..]
}				t_env;

/*
**	###########################
**	#        FUNCTIONS        #
**	###########################
*/

void			render(const t_env *env);
void			error(int	err_nb);

/*
**	init_argv.c
*/
void			usage(void);
void			read_argv(t_env *env, char **argv, int argc);
void			init(t_env *env, t_mlx *mlx);

/*
**	flt3_op.c
*/
t_flt3			flt3_add(t_flt3 a, const t_flt3 b);
t_flt3			flt3_sub(t_flt3 a, const t_flt3 b);
t_flt3			flt3_mult(t_flt3 a, const t_flt3 b);
t_flt3			flt3_multf(t_flt3 a, const float b);
t_flt3			flt3_div(t_flt3 a, const t_flt3 b);
t_flt3			flt3_divf(t_flt3 a, const float b);
float			flt3_dot(const t_flt3 a, const t_flt3 b);
float			flt3_mod(const t_flt3 a);
t_flt3			flt3_normalize(t_flt3 a);


float			points_dist(const t_coords p1, const t_coords p2);

/*
**	dist_functions.c
*/
float			dist_sphere(const union u_object *obj, const t_ray ray);
float			dist_plane(const union u_object *obj, const t_ray ray);
float			dist_disk(const union u_object *obj, const t_ray ray);

/*
**	norm_functions.c
*/
t_vec3			norm_sphere(const union u_object *obj, const t_vec3 hit);
t_vec3			norm_plane(const union u_object *obj, const t_vec3 hit);

/*
**	color_op.c
*/
int				srgb(t_fcolor color);
t_fcolor		light_drop(const t_fcolor light, const float dist);
t_fcolor		tone_map(const t_fcolor px);

#endif
