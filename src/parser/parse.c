/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: awoimbee <awoimbee@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/01/22 12:38:31 by awoimbee          #+#    #+#             */
/*   Updated: 2019/01/22 13:07:00 by awoimbee         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rtv1.h"
#include "parser.h"

void	placeholder_fill_objs(t_env *env)
{
	env->objs_arr = malloc(sizeof(t_obj) * 4);

	env->objs_arr[0].distfun = &dist_sphere;
	env->objs_arr[0].normfun = &norm_sphere;
	env->objs_arr[0].this.sphere.orig = (t_vec3){0, 0, 5.};
	env->objs_arr[0].this.sphere.radius = 1;
	env->objs_arr[0].color = (t_fcolor){1, 0, 1};

	env->objs_arr[1].distfun = &dist_sphere;
	env->objs_arr[1].normfun = &norm_sphere;
	env->objs_arr[1].this.sphere.orig = (t_vec3){0.8, 2.1, 7.};
	env->objs_arr[1].this.sphere.radius = 2.;
	env->objs_arr[1].color = (t_fcolor){0.2, 0.4, 0.8};

	env->objs_arr[2].distfun = &dist_disk;
	env->objs_arr[2].normfun = &norm_plane;
	env->objs_arr[2].this.disk.orig = (t_vec3){0.8, 2.1, 8.};
	env->objs_arr[2].this.disk.norm = (t_vec3){0, 0, 1};
	env->objs_arr[2].this.disk.radius2 = 16.;
	env->objs_arr[2].color = (t_fcolor){0.8, 0.8, 0.8};

	env->objs_arr[3].distfun = &dist_plane;
	env->objs_arr[3].normfun = &norm_plane;
	env->objs_arr[3].this.plane.orig = (t_vec3){0, -2, 9.};
	env->objs_arr[3].this.plane.norm = (t_vec3){0, 1, 0};
	env->objs_arr[3].color = (t_fcolor){0.5, 0, 0};

	env->objs_nb = 4;
	//####################
	env->light_arr = malloc(sizeof(t_light) * 2);

	env->light_arr[0].pos = (t_vec3){0, -1.2, 5};
	env->light_arr[0].intensity = (t_fcolor){5, 5, 5};

	env->light_arr[1].pos = (t_vec3){0, 100, 0};
	env->light_arr[1].intensity = (t_fcolor){10000, 10000, 10000};

	env->light_nb = 1;
	//######################
}

env->disp = (t_disp)
	{
		.res = (t_int2){WIN_W, WIN_H},
		.aspect_ratio = (float)WIN_W / WIN_H,
		.tfov = tan(75. / 2. * M_PI / 180.)
	};
	env->camera = (t_ray)
	{
		(t_vec3){1., 1., 0.},
		(t_vec3){-0.6, 0., 1.}
	};
	env->bckgrnd_col = (t_fcolor){0.05, 0.05, 0.05};


/*
**	file format:
**		env
**			.background_light = {.., .., ..}
**
**		display
**			.width = 1000
**			.height = 500
**
**		camera
**			.pos = {0., 0., 0.}
**			.dir = {1., 2., 0.}
**			.fov = 75
**
**		objs[X] :
**		{
**			Sphere
**				.origin = {0., 0., 0.}
**				.radius = 5.
**				.color = {0.2, 0.4, 0.8}
**			Plane
**				...
**		}
**		lights[x] :
**		{
**		...
**		}
*/



void		parse_scene(t_env *env)
{
	int		fd;
	char	*line;
	int		done;

	done = 0;
	if ((fd = open("scene.rt", O_RDONLY)) == -1)
		error(FILE_ERR);
	while (get_next_line(fd, &line) > 0)
	{
		if (!ft_strcmp(line, "env") && (done |= ENV))
			parse_env(env);
		else if (!ft_strcmp(line, "display") && (done |= DISP))
			pase_disp(env);
		else if (!ft_strcmp(line, "camera") && (done |= CAM))
			pase_camera(env);
		else if (!ft_strncmp(line, "objs", 4) && (done |= OBJS))
			pase_objects(env, line);
		else if (!ft_strncmp(line, "lights", 6) && (done |= LIGHTS))
			pase_objects(env, line);
	}

}