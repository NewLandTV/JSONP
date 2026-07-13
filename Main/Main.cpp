#include <iostream>
#include "Json.h"

int main()
{
	char json[] = "{ \"name\": \"JkhTV\", \"age\": 13, \"man\": true, \"weight\": 55.2 }";
	char* next = nullptr;

	auto data = JSON::ParseObject(json, next);

	std::cout << data->find("name")->first << " : " << data->find("name")->second->Repr() << std::endl;
	std::cout << data->find("age")->first << " : " << data->find("age")->second->Repr() << std::endl;
	std::cout << data->find("man")->first << " : " << data->find("man")->second->Repr() << std::endl;
	std::cout << data->find("weight")->first << " : " << data->find("weight")->second->Repr() << std::endl;

	delete data;

	return 0;
}