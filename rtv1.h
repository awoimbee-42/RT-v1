/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rtv1.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: awoimbee <awoimbee@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/01/08 10:37:06 by awoimbee          #+#    #+#             */
/*   Updated: 2019/01/24 17:49:49 by cpoirier         ###   ########.fr       */
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
# include "SDL2/SDL.h"

# define WIN_W 1280
# define WIN_H 720

# define SDL_ERR	1
# define MALLOC_ERR	2
# define RES_ERR	3

/*
**	Define bitwise inputs
*/

# define BT_W		(1 << 0)
# define BT_A		(1 << 1)
# define BT_S		(1 << 2)
# define BT_D		(1 << 3)
# define BT_UP		(1 << 4)
# define BT_DOWN	(1 << 5)
# define BT_LEFT	(1 << 6)
# define BT_RIGHT	(1 << 7)

/*
**	All of the structures & such, in order
*/

struct s_id_dist;

struct s_float3;
struct s_float2;
struct s_int2;

struct s_ray;
struct s_disp;

struct s_img;
struct s_mlx;

struct s_light;
enum e_material;
union u_object;
struct s_obj;

struct s_env;

/*
**	###############################
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

typedef t_flt3	t_vec3;

typedef t_flt3	t_fcolor;

typedef t_flt3	t_coords;

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

/*
**	tfov contains tan(fov)
*/

typedef struct	s_disp
{
	struct s_int2	res;
	float			aspect_ratio;
	float			tfov;
}				t_disp;

/*
**	#############################
**	#        MLX STRUCTS        #
**	#############################
*/

typedef struct	s_sdl
{
	unsigned int	*img;
	SDL_Renderer	*renderer;
	SDL_Texture		*texture;
	SDL_Window		*win;
    SDL_Surface		*surface;
}				t_sdl;

/*
**	########################
**	#        LIGHTS        #
**	########################
*/

typedef struct	s_light
{
	t_coords		pos;
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
	struct		s_cylinder
	{
		t_vec3			org;
		t_vec3			end;
		float			radius;
	}					cylinder;
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
	struct s_sdl	*sdl;
	struct s_disp	disp;
	struct s_ray	camera;
	int				objs_nb;
	struct s_obj	*objs_arr;
	int				light_nb;
	struct s_light	*light_arr;
	t_fcolor		bckgrnd_col;
	long			keys_pressed;
}				t_env;

/*
**	###########################
**	#        FUNCTIONS        #
**	###########################
*/

void			render(const t_env *env);

/*
**	main.c
*/
void			exit_cleanly(t_env *env);
void			error(int	err_nb);

/*
**	init_argv.c
*/
void			usage(void);
void			read_argv(t_env *env, char **argv, int argc);
void			init(t_env *env, t_sdl *sdl);

/*
**	operators/flt3_opX.c
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
t_flt3			flt3_cross(t_flt3 a, const t_flt3 b);

/*
**	t_obj/dist.c
*/
float			dist_sphere(const union u_object *obj, const t_ray ray);
float			dist_plane(const union u_object *obj, const t_ray ray);
float			dist_disk(const union u_object *obj, const t_ray ray);
float			dist_cylinder(const union u_object *obj, const t_ray ray);

/*
**	t_obj/norm.c
*/
t_vec3			norm_sphere(const union u_object *obj, const t_vec3 hit);
t_vec3			norm_plane(const union u_object *obj, const t_vec3 hit);
t_vec3			norm_cylinder(const union u_object *obj, const t_vec3 hit);

/*
**	operators/special_op.c
*/
unsigned int	srgb(t_fcolor color);
t_fcolor		light_drop(const t_fcolor light, const float dist);
t_fcolor		tone_map(const t_fcolor px);
float			points_dist(const t_coords p1, const t_coords p2);

/*
**	camera_ope.c
*/
void			apply_camera_rot(const t_env *v, t_vec3 *s);
void			move_camera(t_env *env, int dir);

/*
**	keys_handlers.c
*/
void			loop(t_env *env);
void			key_pressed(SDL_Keycode key, t_env *env);
void			key_released(SDL_Keycode key, t_env *env);

#endif
