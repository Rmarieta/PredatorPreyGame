#include <iostream>
#include "LivingEntity.hpp"
#include "Application.hpp"


using namespace std;


//======================== CONSTRUCTEURS/DESTRUCTEURS ================//

LivingEntity::LivingEntity(const Vec2d& pos_ent, double energie_): 
		position_entite(pos_ent), energie(energie_), horloge(sf::Time::Zero) {}

//========================= GETTEURS/SETTEURS ========================//

double LivingEntity::getEnergie() const {
	return energie;
}

void LivingEntity::setPosition(Vec2d& new_pos) {
	verif_window(new_pos);
}

Vec2d LivingEntity::getPosition() const  {
	return position_entite;
}

double LivingEntity::getAge() const {
	return (horloge.asSeconds());
}

void LivingEntity::setEnergie(double e) {
	energie = e;
}


//========================== UPDATE ==================================//


void LivingEntity::update(sf::Time dt) {
	horloge += dt;
}


//======================== FONCTIONS UTILITAIRES =====================//


void LivingEntity::verif_window (Vec2d& new_pos) {

	while (new_pos.x < 0) {
		new_pos.x += getAppConfig().simulation_world_size;
	}
	
	while (new_pos.x > (getAppConfig().simulation_world_size)) {
		new_pos.x -= getAppConfig().simulation_world_size;
	}
	
	while (new_pos.y < 0) {
		new_pos.y += getAppConfig().simulation_world_size;
	}
	
	while (new_pos.y > (getAppConfig().simulation_world_size)) {
		new_pos.y -= getAppConfig().simulation_world_size;
	}
}

