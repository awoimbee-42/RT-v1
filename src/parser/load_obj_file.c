/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   load_obj.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: awoimbee <awoimbee@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/02 03:53:13 by awoimbee          #+#    #+#             */
/*   Updated: 2019/03/07 01:25:53 by awoimbee         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rtv1.h"
#include "parser.h"


static void	count_obj_elements(int fd, int *v_count, int *f_count)
{
	char	*line;
	char	*ptr;
	int		vert_count;

	*v_count = 0;
	*f_count = 0;
	while (get_next_line(fd, &line) > 0)
	{
		if (line[0] == 'v' && line[1] == ' ')
			(*v_count)++;
		else if (line[0] == 'f' && line[1] == ' ')
		{
			vert_count = 0;
			ptr = line + 2;
			while (*ptr)
			{
				if (*ptr != ' ' && *ptr != '\t' && (ptr == line + 2 || *(ptr-1) == ' ' || *(ptr-1) == '\t'))
					vert_count++;
				ptr++;
			}
			*f_count += (vert_count >= 3) ? (vert_count - 2) : 0;
		}
		ft_memdel((void**)&line);
	}
	lseek(fd, 0, SEEK_SET);
}

/*
**	parse_vertex: Parse a vertex line "v x y z"
*/

static t_flt3	parse_vertex(char *line)
{
	t_flt3	vert;

	line += 2;
	line = skip_whitespaces(line);
	vert.x = ft_atof_mv(&line);
	line = skip_whitespaces(line);
	vert.y = ft_atof_mv(&line);
	line = skip_whitespaces(line);
	vert.z = ft_atof_mv(&line);
	return (vert);
}

/*
**	parse_face: Parse a face line "f v1/vt1/vn1 v2/vt2/vn2 v3/vt3/vn3 [v4/vt4/vn4]"
**	OBJ indices start at 1, so we subtract 1
**	We only care about vertex indices (v), ignore texture and normal indices
**	Supports triangles (3 vertices) and quads (4 vertices)
**	Returns the number of vertices parsed (3 or 4)
*/

static int	parse_face_indices(char *line, int *indices, int v_count, int max_vertices)
{
	int		i;
	long    val;
	int     parsed;

	line += 2;
	i = 0;
	while (i < max_vertices && *line)
	{
		line = skip_whitespaces(line);
		if (*line == '\0')
			break;
		parsed = 0;
		val = 0;
		if (*line == '-')
		{
			parsed = 1;
			line++;
			while (*line >= '0' && *line <= '9')
			{
				val = val * 10 + (*line - '0');
				parsed = 2;
				line++;
			}
			if (parsed >= 2)
				val = (long)v_count - (long)val;
			else
				val = -1;
		}
		else
		{
			while (*line >= '0' && *line <= '9')
			{
				val = val * 10 + (*line - '0');
				parsed = 1;
				line++;
			}
		}
		while (*line && *line != ' ' && *line != '\t')
			line++;
		if (val > 0)
			indices[i] = (int)(val - 1);
		else if (val == 0)
			indices[i] = -1;
		else
			indices[i] = (int)val;
		i++;
	}
	return (i);
}

/*
**	load_vertices: Load all vertices from the OBJ file
*/

static void	load_vertices(int fd, t_flt3 *vertices, int v_count)
{
	char	*line;
	int		v_idx;

	v_idx = 0;
	while (get_next_line(fd, &line) > 0 && v_idx < v_count)
	{
		if (line[0] == 'v' && line[1] == ' ')
		{
			vertices[v_idx] = parse_vertex(line);
			v_idx++;
		}
		ft_memdel((void**)&line);
	}
	ft_memdel((void**)&line);
	lseek(fd, 0, SEEK_SET);
}

/*
**	create_triangle_obj: Create a triangle object from vertices
*/

static t_obj	create_triangle_obj(t_flt3 *vertices, int *indices,
									t_flt3 pos, float scale, t_obj *props, int v_count)
{
	t_obj	tri;

	if (indices[0] < 0 || indices[0] >= v_count ||
		indices[1] < 0 || indices[1] >= v_count ||
		indices[2] < 0 || indices[2] >= v_count)
	{
		ft_msg_exit("Invalid face index", NULL);
	}
	tri.distfun = &dist_triangle;
	tri.normfun = &norm_triangle;
	tri.diffuse = props->diffuse;
	tri.specular = props->specular;
	tri.color = props->color;
	tri.this.triangle.vert0.x = vertices[indices[0]].x * scale + pos.x;
	tri.this.triangle.vert0.y = vertices[indices[0]].y * scale + pos.y;
	tri.this.triangle.vert0.z = vertices[indices[0]].z * scale + pos.z;
	tri.this.triangle.vert1.x = vertices[indices[1]].x * scale + pos.x;
	tri.this.triangle.vert1.y = vertices[indices[1]].y * scale + pos.y;
	tri.this.triangle.vert1.z = vertices[indices[1]].z * scale + pos.z;
	tri.this.triangle.vert2.x = vertices[indices[2]].x * scale + pos.x;
	tri.this.triangle.vert2.y = vertices[indices[2]].y * scale + pos.y;
	tri.this.triangle.vert2.z = vertices[indices[2]].z * scale + pos.z;
	return tri;
}

/*
**	load_faces_into_array: Load faces directly into the provided array
*/

static void	load_faces_into_array(int fd, t_flt3 *vertices, int v_count,
									t_obj *obj_arr, int max_count,
									t_flt3 pos, float scale, t_obj *props)
{
	char	*line;
	int		indices[64];
	int		tri_indices[3];
	int		tri_idx;
	int		vert_count;

	tri_idx = 0;
	while (get_next_line(fd, &line) > 0)
	{
		if (line[0] == 'f' && line[1] == ' ')
		{
			vert_count = parse_face_indices(line, indices, v_count, 64);
			if (vert_count == 3)
			{
				// Triangle: add as-is
				if (tri_idx < max_count &&
					indices[0] >= 0 && indices[1] >= 0 && indices[2] >= 0 &&
					indices[0] < v_count && indices[1] < v_count && indices[2] < v_count)
				{
					tri_indices[0] = indices[0];
					tri_indices[1] = indices[1];
					tri_indices[2] = indices[2];
					obj_arr[tri_idx++] = create_triangle_obj(vertices, tri_indices,
						pos, scale, props, v_count);
				}
			}
			else if (vert_count >= 4)
			{
				for (int i = 2; i < vert_count; ++i)
				{
					if (tri_idx >= max_count)
					{
						break;
					}
					if (indices[0] >= 0 && indices[i-1] >= 0 && indices[i] >= 0 &&
						indices[0] < v_count && indices[i-1] < v_count && indices[i] < v_count)
					{
						tri_indices[0] = indices[0];
						tri_indices[1] = indices[i-1];
						tri_indices[2] = indices[i];
						obj_arr[tri_idx++] = create_triangle_obj(vertices, tri_indices,
							pos, scale, props, v_count);
					}
				}
			}
		}
		ft_memdel((void**)&line);
	}
	ft_memdel((void**)&line);
}

/*
**	load_obj_into_array: Load triangles from OBJ file into provided array
*/

void			load_obj_into_array(const char *filename, t_obj *obj_arr,
								int expected_count, const t_flt3 *pos,
								float scale, t_obj *props)
{
	int		fd;
	int		v_count;
	int		f_count;
	t_flt3	*vertices;
	t_flt3	position;
	t_obj	default_props;

	printf("Loading OBJ file: %s\n", filename);
	if ((fd = open(filename, O_RDONLY)) < 0)
		ft_msg_exit("Could not open OBJ file: %s\n", filename);
	count_obj_elements(fd, &v_count, &f_count);
	if (v_count == 0 || f_count == 0)
	{
		close(fd);
		ft_msg_exit("Empty or invalid OBJ file: %s\n", filename);
	}
	if (f_count != expected_count)
		ft_msg_exit("OBJ file has %d faces, which is not the configured number\n", &f_count);
	if (!(vertices = malloc(sizeof(t_flt3) * v_count)))
	{
		close(fd);
		error(MALLOC_ERR);
	}
	position = pos ? *pos : (t_flt3){0.0f, 0.0f, 0.0f};
	if (!props)
	{
		default_props.color = (t_fcolor){200.0f, 200.0f, 200.0f};
		default_props.diffuse = 0.8f;
		default_props.specular = 0.2f;
		props = &default_props;
	}
	load_vertices(fd, vertices, v_count);
	load_faces_into_array(fd, vertices, v_count, obj_arr, expected_count, position, scale, props);
	free(vertices);
	close(fd);
}

static char	*parse_string(char *str, unsigned int line_nb)
{
	char	*result;
	char	*start;
	int		len;

	str = skip_whitespaces(str);
	if (*str != '=')
		ft_msg_exit("'=' sign not found line %d!\n", &line_nb);
	str = skip_whitespaces(str + 1);
	start = str;
	len = 0;
	while (*str && *str != ';' && *str != ' ' && *str != '\t')
	{
		len++;
		str++;
	}
	if (!(result = malloc(len + 1)))
		error(MALLOC_ERR);
	ft_strncpy(result, start, len);
	result[len] = '\0';
	return (result);
}


void	parse_obj_file(int fd, unsigned int *line_nb, t_obj *obj_arr,
						int expected_count)
{
	char	*line;
	int		done;
	t_obj	obj_props;
	char	*filename;
	t_flt3	position;
	float	scale;

	done = 0;
	filename = NULL;
	position = (t_flt3){0.0f, 0.0f, 0.0f};
	scale = 1.0f;
	obj_props.color = (t_fcolor){200.0f, 200.0f, 200.0f};
	obj_props.diffuse = 0.8f;
	obj_props.specular = 0.2f;
	while (get_next_line(fd, &line) > 0 && ++*line_nb)
	{
		if (!ft_strncmp(line, "\t\t.file", 7) && (done |= 0xF000))
			filename = parse_string(line + 7, *line_nb);
		else if (!ft_strncmp(line, "\t\t.position", 11) && (done |= 0xF0000))
			position = parse_f3(line + 11, *line_nb, -1e36f);
		else if (!ft_strncmp(line, "\t\t.scale", 8) && (done |= 0xF00000))
			scale = parse_f(line + 8, *line_nb, 0.001f);
		else if (!tobj_parse(&obj_props, line, &done, *line_nb) && !is_comment(line))
			break ;
		if (ft_strstr(line, ";") && (done |= 0xF000000))
			break ;
		ft_memdel((void**)&line);
	}
	ft_memdel((void**)&line);
	if ((done & 0xF000) == 0)
		ft_msg_exit("OBJ: .file property is required, line %d\n", line_nb);
	load_obj_into_array(filename, obj_arr, expected_count, &position, scale, &obj_props);
	free(filename);
}
