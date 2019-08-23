/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rtv1.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: awoimbee <awoimbee@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/01/08 10:37:06 by awoimbee          #+#    #+#             */
/*   Updated: 2019/08/24 00:22:51 by awoimbee         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef RTV1_H
# define RTV1_H

# include <fcntl.h>
# include <unistd.h>
# include <stdlib.h>
# include <stdio.h>
# include <math.h>
# include <limits.h>

# include "libft.h"
# define SDL_MAIN_HANDLED
# include "SDL2/SDL.h"
# include "SDL2/SDL_thread.h"
# include "SDL2/SDL_gamecontroller.h"

# include <datatypes.h>
# include <light_op.h>
# include <flt3_op.h>

# define PX_RENDERED	0x1000000U

/*
**	NB_PX_SKIP: the size of the `big pixels` when whe render
**		MUST NOT BE AN EVEN NUMBER
*/

# define NB_PX_SKIP		9
# define PX_SKIP_STEP	8
# if NB_PX_SKIP % PX_SKIP_STEP != 1 && PX_SKIP_STEP != 1
#  error "NB_PX_SKIP % PX_SKIP_STEP shall be equal to 1"
# endif

# define SDL_ERR	1
# define MALLOC_ERR	2

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
# define BT_Q		(1 << 8)
# define BT_E		(1 << 9)

/*
**	###########################
**	#        FUNCTIONS        #
**	###########################
*/

int				render(t_env *env);
void			supersample(t_env *env);
uint32_t		launch_ray(const int x, const int y, const t_env *env);
uint32_t		launch_ray_supersample(const int x, const int y, const t_env *env, int sampling);

/*
**	main.c
*/
void			exit_cleanly(t_env *env);
void			error(int	err_nb);

/*
**	./parser/scene.c
*/
void			parse_scene(t_env *env, const char *filename);

/*
**	init_argv.c
*/
void			usage(void);
// void			read_argv(t_env *env, char **argv, int argc);
void			init(t_env *env);
void			resize(t_env *env);

/*
**	operators/flt3_opX.c
*/

/*
**	t_obj/dist.c
*/
float			dist_sphere(const union u_object *obj, const t_ray *ray);
float			dist_plane(const union u_object *obj, const t_ray *ray);
float			dist_disk(const union u_object *obj, const t_ray *ray);
float			dist_cylinder(const union u_object *obj, const t_ray *ray);
float			dist_cone(const union u_object *obj, const t_ray *ray);
float			dist_triangle(const union u_object *obj, const t_ray *ray);

/*
**	t_obj/norm.c
*/
t_vec3			norm_sphere(const union u_object *obj, const t_vec3 *hit);
t_vec3			norm_plane(const union u_object *obj, const t_vec3 *hit);
t_vec3			norm_cylinder(const union u_object *obj, const t_vec3 *hit);
t_vec3			norm_cone(const union u_object *obj, const t_vec3 *hit);
t_vec3			norm_triangle(const union u_object *obj, const t_vec3 *hit);

/*
**	operators/special_op.c
*/

/*
**	operators/light.c
*/
t_id_dist		nearest_obj(const t_env *env, const t_ray *ray);
float			get_specular(const t_vec3 *dir, const t_vec3 *light_dir,
	const float specular, const float k);
t_fcolor		fast_diffuse(const t_env *env, t_ray *hit, t_obj *obj,
	t_vec3 *norm);

/*
**	operators/camera.c
*/
void			apply_camera_rot(const t_env *env, t_vec3 *s);
void			move_camera(t_env *env, int dir);

/*
**	keys_handlers.c
*/
void			loop(t_env *env, SDL_GameController *controller);
void			key_pressed(SDL_Keycode key, t_env *env);
void			key_released(SDL_Keycode key, t_env *env);

#endif
