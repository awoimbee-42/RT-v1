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

void	load_obj(const t_flt3 *pos)
{
	int			fd;
	int			file_o_id;
	int			o_nb;
	char		*line;
	t_objgrp	modl;

	if (!(fd = open("model.obj", O_RDONLY)))
		msg_exit("EIOOOOPWHGBVVCJDS:LFHFUPREGOWEF(could NOT OpEn FiLeE)", NULL);
	file_o_id = 1;
	o_nb = 0;
	while (get_next_line(fd, &line))
	{
		if (line[0] == 'o' && line[1] == ' ' && o_nb < file_o_id)
			o_nb += 1;

		else if (line[0] == 'v' && line[1] == ' ' && (o_nb == file_o_id || o_nb == 0))
		{
			t_flt3	vert;
			while (('a' <= *line && *line <= 'z') || *line == ' ')
				++line;
			vert = parse_f3(line, 42424242, -1e36);
			//ADD VERT TO VERTICES
		}
		// else if (line[0] == 'v' && line[1] == 'n' && (o_nb == file_o_id || o_nb == 0)) { //FACE NORMAL< NOT USED FOR RT
		// 			std::vector<float> normal(3);
		// 			char trash[2];
		// 			iss >> trash >> normal[0] >> normal[1] >> normal[2];
		// 			this->normals.push_back(normal);
		// 		}
		else if (line[0] == 'f' && line[1] == ' ' && (o_nb == file_o_id || o_nb == 0))
		{
			//
			t_triangle t;

			line += 2;
			int i = -1;
			while (++i < 3)
			{
				line = skip_whitespaces(line);
				t.v[i] = ft_atoi_mv(&line);
				if (*line == '/')
				{
					++line;
					t.vt[i] = -1; //??
					if (*line != '/')
						t.vt[i] = ft_atoi_mv(&line);
					if (*line == '/' && ++line)
						(void)ft_atoi_mv(&line);
				}
			}
			// do something with the triangle t
		}
		else if (line[0] == 'g' && line[1] == ' ')
		{
			//change color or something
		}
		else if (line[0] == 'o' && line[1] == ' ')
		{	// /!\ multiple objects in the same file
			ft_printf("{red}O{ylw}UG{CYN}A BO{blu}U<bold>G{GRN}A{eof}\n");
			break;
		}
	}
	close(fd);
}

// Object(std::vector<int> position = { 0, 0, 0 }, std::vector<int> color = { 255,255,255 }) {
// 		std::ifstream modelFile;
// 		modelFile.open("Alucy.obj");
// 		std::string line;
// 		int file_o_id = 1;
// 		int o_nb = 0;
// 		if (modelFile.is_open()) {
// 			std::cout << "got openned" << std::endl;
// 			while (!modelFile.eof()) {
// 				std::getline(modelFile, line);
// 				std::istringstream iss(line);

// 				if (line[0] == 'o' && line[1] == ' ' && o_nb < file_o_id) {
// 					o_nb += 1;
// 				}
// 				else if (line[0] == 'v' && line[1] == ' ' && (o_nb == file_o_id || o_nb == 0)) {
// 					std::vector<float> vertex(3);
// 					char trash;
// 					iss >> trash >> vertex[0] >> vertex[1] >> vertex[2];
// 					this->vertices.push_back(vertex);
// 				}
// 				else if (line[0] == 'v' && line[1] == 'n' && (o_nb == file_o_id || o_nb == 0)) {
// 					std::vector<float> normal(3);
// 					char trash[2];
// 					iss >> trash >> normal[0] >> normal[1] >> normal[2];
// 					this->normals.push_back(normal);
// 				}
// 				else if (line[0] == 'f' && line[1] == ' ' && (o_nb == file_o_id || o_nb == 0)) {
// 					std::vector<int> faceVerts;
// 					std::vector<int> faceNorms;
// 					std::vector<std::string> faceFull;
// 					faceFull.reserve(4);
// 					std::string temp;
// 					std::getline(iss, temp, ' ');
// 					while (std::getline(iss, temp, ' ')){
// 						faceFull.push_back(temp);
// 					}

// 					for (std::string& face : faceFull) { // v/vt/vn
// 						/*
// 						std::vector<int> vertexFull;
// 						strtk::parse(face, "/", vertexFull);
// 						faceVerts.push_back(vertexFull[0]);
// 						I ONLY CARE ABOUT THE FIRST INFO FOR NOW
// 						*/
// 						std::istringstream issFace(face);
// 						std::getline(issFace, temp, '/');
// 						faceVerts.push_back(std::stoi(temp)-1);
// 						std::getline(issFace, temp, '/'); // it's texture = trash for now
// 						std::getline(issFace, temp, '/');
// 						faceNorms.push_back(std::stoi(temp) - 1);
// 					}

// 					this->faces.push_back({ faceVerts, faceNorms });
// 				}
// 				else if (line[0] == 'g' && line[1] == ' ') {
// 				//change color or something
// 				}
// 				else if (line[0] == 'o' && line[1] == ' ') {
// 					std::cout << "FUUUUUUUCK" << std::endl;
// 					break;
// 				}
// 			}
// 			modelFile.close();
// 		}


// 		this->color = color;
// 		for (int axis = 0; axis < 3; ++axis) {
// 			for (int i = 0; i < this->vertices.size(); ++i) {
// 				this->vertices[i][axis] += this->vertices[i][axis] + position[axis];
// 			}
// 		}
// 		std::cout << "All done !" << std::endl;

// 	}
