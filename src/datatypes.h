/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   datatypes.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: awoimbee <awoimbee@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/08/23 21:52:06 by arthur            #+#    #+#             */
/*   Updated: 2019/08/24 00:22:45 by awoimbee         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef DATATYPES_H
# define DATATYPES_H

# include "SDL2/SDL.h"

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
union u_object;
struct s_obj;

struct s_env;

/*
**	###############################
**	#           NOT TRASH         #
**	###############################
*/

typedef struct	s_id_dist
{
	int					id;
	float				dist;
}				t_id_dist;

/*
**	###################################
**	#        'BASIC' DATATYPES        #
**	###################################
*/

typedef struct	s_flt3
{
	float				x;
	float				y;
	float				z;
}				t_flt3;

typedef struct	s_flt2
{
	float				x;
	float				y;
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
	t_coords			org;
	t_vec3				dir;
}				t_ray;

/*
**	tfov contains tan(fov)
*/

typedef struct	s_disp
{
	uint32_t			w;
	uint32_t			h;
	float				aspect_ratio;
	float				tfov;
}				t_disp;

/*
**	#############################
**	#        MLX STRUCTS        #
**	#############################
*/

typedef struct	s_sdl
{
	unsigned int		*img;
	SDL_Renderer		*restrict renderer;
	SDL_Texture			*restrict texture;
	SDL_Window			*restrict win;
	SDL_Surface			*restrict surface;
}				t_sdl;

/*
**	########################
**	#        LIGHTS        #
**	########################
*/

typedef struct	s_light
{
	t_coords			pos;
	t_fcolor			intensity;
}				t_light;

/*
**	##########################################
**	#        OBJECTS STRUCTS / UNIONS        #
**	##########################################
*/

typedef struct	s_objgrp
{

}				t_objgrp;

typedef struct	s_model
{
	int					g_nb;
	t_objgrp			*groups;
	t_vec3				*v;		//vertices
	t_vec3				*vt;	//texture coordinates
}				t_model;

typedef struct	s_triangle
{
	uint32_t		v[3];
	uint32_t		vt[3];
}				t_triangle;


typedef float(*t_distfun)(const union u_object*, const t_ray*);
typedef t_vec3(*t_normfun)(const union u_object*, const t_vec3*);

union			u_object
{
	struct		s_sphere
	{
		t_vec3			orig;
		float			radius;
	}			sphere;
	struct		s_plane
	{
		t_vec3			orig;
		t_vec3			norm;
	}			plane;
	struct		s_disk
	{
		t_vec3			orig;
		t_vec3			norm;
		float			radius2;
	}			disk;
	struct		s_cylinder
	{
		t_vec3			org;
		t_vec3			end;
		float			radius;
	}			cylinder;
	struct		s_cone
	{
		t_vec3			org;
		t_vec3			dir;
		float			angle;
	}			cone;
	struct		s_tri
	{
		t_vec3			vert0;
		t_vec3			vert1;
		t_vec3			vert2;
	}			triangle;
	t_model		model;
};

typedef struct	s_obj
{
	t_distfun			distfun;
	t_normfun			normfun;
	float				diffuse;
	float				specular;
	t_fcolor			color;
	union u_object		this;
}				t_obj;

/*
**	############################
**	#        ENV STRUCT        #
**	############################
*/

typedef struct	s_thread
{
	SDL_Thread			*restrict ptr;
	struct s_env		*env;
	int					px_skip;
	uint32_t			line_start;
	uint32_t			line_end;
	uint32_t			*restrict px_start;
}				t_thread;

typedef struct	s_env
{
	struct s_sdl		sdl;
	struct s_disp		disp;
	struct s_ray		camera;
	int					objs_nb;
	struct s_obj		*restrict objs_arr;
	int					light_nb;
	struct s_light		*restrict light_arr;
	t_fcolor			bckgrnd_col;
	uint32_t			keys;
	SDL_GameController	*restrict controller;
	int					stop;
	int					supersampling_rate;
	SDL_Thread			*restrict rndr;
	uint32_t			threads_nb;
	t_thread			threads[];
}				t_env;

#endif
