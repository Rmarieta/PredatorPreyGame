#include <iostream>
#include <Application.hpp>
#include "Wolf.hpp"
#include <Genetics/Genome.hpp>

using namespace std;

unsigned int Wolf::nb_wolf(0);

//======================== CONSTRUCTEURS/DESTRUCTEURS ================//

Wolf::Wolf (const Vec2d& pos_ent, int unId,  Genome* genome_mere, Genome* genome_pere, bool sans_parents1)
	: Animal(pos_ent,getAppConfig().wolf_energy_initial, sf::seconds(getAppConfig().wolf_reproduction_gestation_time), genome_mere,genome_pere, sans_parents1, unId)
{
	++ nb_wolf;
}

Wolf::~Wolf() { -- nb_wolf; }

//========================= GETTEURS/SETTEURS ========================//

double Wolf::getViewRange() const {
	return getAppConfig().wolf_view_range;
}

double Wolf::getViewDistance() const {
	return getAppConfig().wolf_view_distance;
}

double Wolf::getStandardMaxSpeed() const {
	return getAppConfig().wolf_max_speed;
}
	
double Wolf::getMass() const {
	return getAppConfig().wolf_mass;
}
	
double Wolf::getRadius() const {
	return getAppConfig().wolf_size / 2.0;
}
	
double Wolf::getRandomWalkRadius() const {
	return getAppConfig().wolf_random_walk_radius;
}

double Wolf::getRandomWalkDistance() const {
	return getAppConfig().wolf_random_walk_distance;
}

double Wolf::getRandomWalkJitter() const {
	return getAppConfig().wolf_random_walk_jitter;
}

string Wolf::getTexture() const {
	return getAppConfig().wolf_texture;
}

double Wolf::getEnergieLossFactor() const  {
	return getAppConfig().wolf_energy_loss_factor;
}

sf::Time Wolf::getLongevity() const {
	return getAppConfig().wolf_longevity;
}

double Wolf::getBiteEnergie(LivingEntity* entite) const {
	return entite->getEnergie();
}

sf::Time Wolf::getGestationTime() const {
	return (sf::seconds(getAppConfig().wolf_reproduction_gestation_time));
}

double Wolf::getEnergieLossMatingMale() const {
	return getAppConfig().wolf_energy_loss_mating_male;
}


double Wolf::getEnergieLossPerChild() const {
	return getAppConfig().wolf_energy_loss_female_per_child;
}

int Wolf::getReproductionMinChildren() const {
	return getAppConfig().wolf_reproduction_min_children;
}

int Wolf::getReproductionMaxChildren() const {
	return getAppConfig().wolf_reproduction_max_children;
}

double Wolf::getCount() {
	return nb_wolf;
}

//========================== FONCTIONS UTILITAIRES ===================//


bool Wolf::eatable(LivingEntity const* other) const {
	return other->eatableBy(this);
}

bool Wolf::eatableBy(Wolf  const*) const {
	return false;
}

bool Wolf::eatableBy(Sheep const*) const {
	return false;
}

bool Wolf::eatableBy(Grass const*) const {
	return false;
}

bool Wolf::matable(LivingEntity const* other) const {
	return (other->canMate(this) and (gestation == NOTENCEINTE));
}

bool Wolf::canMate(Grass const*) const {
	return false;
}

bool Wolf::canMate(Sheep const*) const {
	return false;
}

bool Wolf::canMate(Wolf const* wolf) const {     												//Pareil que SHEEP
	
	if ((this->genome_animal.getSex() != wolf->genome_animal.getSex())) {
		if (isFemale())  {
			if (wolf->getEnergie() > getAppConfig().wolf_energy_min_mating_male and (this->getEnergie() > getAppConfig().sheep_energy_min_mating_male)) {
			return true;
			}
		}
		
		else {
			 if (wolf->getEnergie() > getAppConfig().wolf_energy_min_mating_female and (this->getEnergie() > getAppConfig().sheep_energy_min_mating_male))  {
			return true;
			}
		}
	}
	
	else {
		
		return false;
	}
}


bool Wolf::morte_ou_non() const {										// est mort si son age est supérieur à sa durée de vie max 
																		// ou si son énergie est inférieur au seuil d'énergie min pour vivre
																		// ou si son état de santé est inférieur à zéro
	return ((energie < getAppConfig().animal_min_energy) or (horloge > getAppConfig().wolf_longevity) or (getImmuneSystem()->getHealth() < 0.0));			
}

void Wolf::give_birth(Animal* animal) const {
	//le genome de la mère est de toute façon celui de l'instance courante dans laquelle give_birth est appelée
	getAppEnv().addEntity(new Wolf(animal->getPosition(), animal->getId(),new Genome(genome_mate),new Genome(genome_animal), false));
}

bool Wolf::infectableBy(LivingEntity const* other) const {
	return other->canInfect(this);
}

bool Wolf::canInfect(Wolf const* wolf) const {
	return getImmuneSystem()->isInfected();
}

bool Wolf::canInfect(Sheep const* sheep) const {
	return false;
}

bool Wolf::canInfect(Grass const* grass) const {
	return true;
}

