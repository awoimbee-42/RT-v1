/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   load_obj.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: awoimbee <awoimbee@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/02 03:53:13 by awoimbee          #+#    #+#             */
/*   Updated: 2019/03/02 03:53:15 by awoimbee         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

Object(std::vector<int> position = { 0, 0, 0 }, std::vector<int> color = { 255,255,255 }) {
		std::ifstream modelFile;
		modelFile.open("Alucy.obj");
		std::string readLine;
		int fileOindex = 1;
		int oNumber = 0;
		if (modelFile.is_open()) {
			std::cout << "got openned" << std::endl;
			while (!modelFile.eof()) {
				std::getline(modelFile, readLine);
				std::istringstream iss(readLine);

				if (readLine[0] == 'o' && readLine[1] == ' ' && oNumber < fileOindex) {
					oNumber += 1;
				}
				else if (readLine[0] == 'v' && readLine[1] == ' ' && (oNumber == fileOindex || oNumber == 0)) {
					std::vector<float> vertex(3);
					char trash;
					iss >> trash >> vertex[0] >> vertex[1] >> vertex[2];
					this->vertices.push_back(vertex);
				}
				else if (readLine[0] == 'v' && readLine[1] == 'n' && (oNumber == fileOindex || oNumber == 0)) {
					std::vector<float> normal(3);
					char trash[2];
					iss >> trash >> normal[0] >> normal[1] >> normal[2];
					this->normals.push_back(normal);
				}
				else if (readLine[0] == 'f' && readLine[1] == ' ' && (oNumber == fileOindex || oNumber == 0)) {
					std::vector<int> faceVerts;
					std::vector<int> faceNorms;
					std::vector<std::string> faceFull;
					faceFull.reserve(4);
					std::string temp;
					std::getline(iss, temp, ' ');
					while (std::getline(iss, temp, ' ')){
						faceFull.push_back(temp);
					}

					for (std::string& face : faceFull) { // v/vt/vn
						/*
						std::vector<int> vertexFull;
						strtk::parse(face, "/", vertexFull);
						faceVerts.push_back(vertexFull[0]);
						I ONLY CARE ABOUT THE FIRST INFO FOR NOW
						*/
						std::istringstream issFace(face);
						std::getline(issFace, temp, '/');
						faceVerts.push_back(std::stoi(temp)-1);
						std::getline(issFace, temp, '/'); // it's texture = trash for now
						std::getline(issFace, temp, '/');
						faceNorms.push_back(std::stoi(temp) - 1);
					}

					this->faces.push_back({ faceVerts, faceNorms });
				}
				else if (readLine[0] == 'g' && readLine[1] == ' ') {
				//change color or something
				}
				else if (readLine[0] == 'o' && readLine[1] == ' ') {
					std::cout << "FUUUUUUUCK" << std::endl;
					break;
				}
			}
			modelFile.close();
		}


		this->color = color;
		for (int axis = 0; axis < 3; ++axis) {
			for (int i = 0; i < this->vertices.size(); ++i) {
				this->vertices[i][axis] += this->vertices[i][axis] + position[axis];
			}
		}
		std::cout << "All done !" << std::endl;

	}