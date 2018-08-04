#include<simplecpp>

namespace simplecpp{


COLR::COLR(){init();}

void COLR::init(){
	clr.insert(std::make_pair("white", glm::vec3(255,255,255)));	
	clr.insert(std::make_pair("red", glm::vec3(255,0,0)));
	clr.insert(std::make_pair("green", glm::vec3(0,255,0)));
	clr.insert(std::make_pair("blue", glm::vec3(0,0,255)));
	clr.insert(std::make_pair("black", glm::vec3(0,0,0)));
	clr.insert(std::make_pair("yellow", glm::vec3(255,255,0)));
	clr.insert(std::make_pair("gray", glm::vec3(128,128,128)));
	clr.insert(std::make_pair("orange", glm::vec3(255,69,0)));	
}

glm::vec3 COLR::getColor(std::string c){
	it = clr.find(c);
	if(it != clr.end()){
		return it->second;
	}
	else return glm::vec3(1,0,0);
}

}
