#include "Shape_Program.h"

Shape_Program::Shape_Program(std::string file_name, Transform* transform) {
	srand(time(NULL));
	if (file_name.substr(file_name.find_last_of(".") + 1) == "txt")
	{
		std::ifstream txtfile(file_name);
		std::cout << file_name << std::endl;
		if (txtfile.is_open())
		{
			std::string line;
			unsigned int line_number = 0;
			bool constructing_string = FALSE;
			bool Saving_Geometry = FALSE;
			
			while (getline(txtfile, line))
			{
				std::stringstream ss;
				ss << line;
				std::string word;
				ss >> word;
				std::string content;
				if (constructing_string == FALSE && Saving_Geometry == FALSE) {
					if (word == "file")
					{
						std::string action;
						ss >> action;
						if (action == "start")
						{
							constructing_string = TRUE;
						}
					}
					else if (word == "rule")
					{
						std::string action;
						ss >> action;
						if (action == "start")
						{
							Saving_Geometry = TRUE;
						}
						else if (action == "end")
						{
							Saving_Geometry = FALSE;
						}
					}
				}
				else if (constructing_string == TRUE)
				{
					if (word == "file")
					{
						std::string action;
						ss >> action;
						if (action == "end")
						{
							constructing_string = FALSE;
						}
					}
					else {
						std::string obj_file_name;
						ss >> obj_file_name;
						this->index_OBJ.push_back(make_pair(word, obj_file_name));
					}
				}
				else if (Saving_Geometry == TRUE)
				{
					if (word == "rule")
					{
						std::string action;
						ss >> action;
						if (action == "end")
						{
							Saving_Geometry = FALSE;
						}
					}
					else {

						/*
							line starting with '/' means the head object and rule
							line starting with '-' means it is just a rule
						*/
						if (word == "/") {
							std::string Object_Index;
							ss >> Object_Index;
							auto it = std::find_if(index_OBJ.begin(), index_OBJ.end(), [Object_Index](const std::pair<std::string, std::string>& element) { return element.first == Object_Index; });
							std::string translate_pos;
							ss >> translate_pos;
							std::replace(translate_pos.begin(), translate_pos.end(), ',', ' ');
							Geometry* object = new Geometry(it->second, 1);
							starting_object = Object_Index;
							std::stringstream ss_t(translate_pos);
							//Defining the transform for the currnet object
							GLfloat x, y, z, size;
							ss_t >> x >> y >> z >> size;
							Transform* new_Transform = transform;
							new_Transform->update(glm::translate(glm::vec3(x, y, z)));
							object->scale(glm::vec3(size));
							new_Transform->addChild(object);
							starting_Geo = object;
							this->head_transform = new_Transform;
							vector<std::string> next_list;
							//Defining the next objects.
							while (ss >> content) {
								if (content == "->") {
									ss >> content;
									std::string next_transform;
									ss >> next_transform;
									std::string next_list_string;
									next_list_string.append(content);
									next_list_string.append(" ");
									next_list_string.append(next_transform);
									next_list.push_back(next_list_string);
								}
							}
							allTransform.push_back(make_tuple(new_Transform, Object_Index, next_list));
						}
						else if (word == "-")
						{
							std::string Object_Index;
							ss >> Object_Index;
							auto it = std::find_if(index_OBJ.begin(), index_OBJ.end(), [Object_Index](const std::pair<std::string, std::string>& element) { return element.first == Object_Index; });
							std::string translate_pos;
							ss >> translate_pos;
							std::replace(translate_pos.begin(), translate_pos.end(), ',', ' ');
							//Defining the transform for the currnet object
							Geometry* object = new Geometry(it->second, 1);
							GLfloat x, y, z, size;
							std::stringstream ss_float(translate_pos);
							ss_float >> x >> y >> z >> size;
							Transform* new_Transform = new Transform(glm::translate(glm::vec3(x, y, z)));
							object->scale(glm::vec3(size));
							new_Transform->addChild(object);
							vector<std::string> next_list;
							//Defining the next objects.
							while (ss >> content) {
								if (content == "->") {
									ss >> content;
									std::string next_transform;
									ss >> next_transform;
									std::string next_list_string;
									next_list_string.append(content);
									next_list_string.append(" ");
									next_list_string.append(next_transform);
									next_list.push_back(next_list_string);
								}
							}
							allTransform.push_back(make_tuple(new_Transform, Object_Index, next_list));
						}
					}
				}
					//Format
					/*
						constructuing the string file.
						start
						A "string of the obj file"
						B "string of the obj file
						.
						.
						.
						end
						. . .
						rule
						start
						/A x y z size -> B x y z size -> D x y z size
						-B x y z size -> C x y z size
						end
					*/
			}
			connect_Obj(starting_object, this->head_transform);

	}
	else
	{
		std::cerr << "Can't open the file " << file_name << std::endl;
	}
		txtfile.close();
	}
}

Shape_Program::~Shape_Program()
{
}

void Shape_Program::connect_Obj(std::string starting_obj, Transform * head_transform)
{
	auto it = std::find_if(allTransform.begin(), allTransform.end(), [starting_obj](const std::tuple<Transform*, std::string, vector<std::string>>& element) { return get<1>(element) == starting_obj; });
	std::vector<std::string> next_str_list = get<2>(*it);
	if (next_str_list.empty() == FALSE) {
		for (unsigned int i = 0; i < next_str_list.size(); i++)
		{
			std::stringstream ss(next_str_list[i]);
			std::string Object_Index;
			//Can modify Object_Index to read | and use rand() to generate different object.
			std::string Transforms;
			ss >> Object_Index >> Transforms;
			std::replace(Object_Index.begin(), Object_Index.end(), '|', ' ');
			std::stringstream or_detect(Object_Index);
			unsigned int j = 1;
			std::string index_name;
			std::vector<std::string> SomeString;
			while (or_detect >> index_name)
			{

				SomeString.push_back(index_name);
			}
			if (SomeString.size() >= 2)
			{
				unsigned int k;
				k = rand() % (SomeString.size());
				Object_Index = SomeString[k];
				std::cout << Object_Index << std::endl;

			}
			auto new_obj_index = std::find_if(allTransform.begin(), allTransform.end(), [Object_Index](const std::tuple<Transform*, std::string, vector<std::string>>& element) { return get<1>(element) == Object_Index; });
			std::replace(Transforms.begin(), Transforms.end(), ',', ' ');
			std::stringstream ss_t(Transforms);
			//Defining the transform for the currnet object
			GLfloat x, y, z, size;
			ss_t >> x >> y >> z >> size;
			Transform* new_Transform = new Transform(glm::translate(glm::vec3(x, y, z)));
			auto Geometry_index = std::find_if(index_OBJ.begin(), index_OBJ.end(), [Object_Index](const std::pair<std::string, std::string>& element) { return element.first == Object_Index; });
			Geometry* new_object = new Geometry(Geometry_index->second, 1);
			new_object->scale(glm::vec3(size));
			new_Transform->addChild(new_object);
			head_transform->addChild(new_Transform);
			try {
				connect_Obj(Object_Index, new_Transform);
			}
			catch (int e)
			{
				std::cout << "An error occured. You did not correctly define " << Object_Index << " in txt file " << std::endl;
			}

		}
	}
}

void Shape_Program::draw(glm::mat4 matrix, GLuint program)
{
	head_transform->draw(glm::mat4(1), program);
}

void Shape_Program::updateHeadTransform(Transform* transform)
{
	//head_transform->update(glm::inverse(head_transform->getTransform()));
	std::string object_index = starting_object;
	head_transform = transform;
	head_transform->addChild(starting_Geo);
	connect_Obj(starting_object, this->head_transform);
}