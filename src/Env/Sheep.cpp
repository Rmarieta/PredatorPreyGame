#include <Application.hpp>
#include "Sheep.hpp"
#include "LivingEntity.hpp"
#include <Genetics/Genome.hpp>

using namespace std;

unsigned int Sheep::nb_sheep(0);

//======================== CONSTRUCTEURS/DESTRUCTEURS ================//

Sheep::Sheep(const Vec2d& pos_ent, int unId, Genome* genome_mere, Genome* genome_pere, bool sans_parents1)			
	:	Animal(pos_ent,getAppConfig().sheep_energy_initial, sf::seconds(getAppConfig().sheep_reproduction_gestation_time), genome_mere,genome_pere, sans_parents1, unId)
{
	++ nb_sheep;																				// A chaque construction d'un mouton, on incrémente un au compteur de mouton pour le graphe
}

Sheep::~Sheep() { -- nb_sheep; }										// A chaque destruction d'un mouton, on enlève 1 au compteur de mouton 

//========================= GETTEURS/SETTEURS ========================//

double Sheep::getViewRange() const {															//METHODES POLYMORPHIQUES
	return getAppConfig().sheep_view_range;
}

double Sheep::getViewDistance() const {
	return getAppConfig().sheep_view_distance;
}

double Sheep::getStandardMaxSpeed() const {
	return getAppConfig().sheep_max_speed;
}

double Sheep::getMass() const {
	return getAppConfig().sheep_mass ;
}

double Sheep::getRadius() const {
	return getAppConfig().sheep_size / 2.0;
}

double Sheep::getRandomWalkRadius() const {
	return getAppConfig().sheep_random_walk_radius;
}

double Sheep::getRandomWalkDistance() const {
	return getAppConfig().sheep_random_walk_distance;
}

double Sheep::getRandomWalkJitter() const {
	return getAppConfig().sheep_random_walk_jitter;
}

string Sheep::getTexture() const {
	
	if (genome_animal.getColorPhenotype() == BLACK) 
		return getAppConfig().sheep_texture_black;
	else 
		return getAppConfig().sheep_texture_white;
}

sf::Time Sheep::getLongevity() const {
	return getAppConfig().sheep_longevity;
}

sf::Time Sheep::getGestationTime() const {
	return (sf::seconds(getAppConfig().sheep_reproduction_gestation_time));
}

double Sheep::getEnergieLossPerChild() const {
	return getAppConfig().sheep_energy_loss_female_per_child;
}

double Sheep::getEnergieLossMatingMale() const {
	return getAppConfig().sheep_energy_loss_mating_male;
}

int Sheep::getReproductionMinChildren() const {
	return getAppConfig().sheep_reproduction_min_children;
}

int Sheep::getReproductionMaxChildren() const {
	return getAppConfig().sheep_reproduction_max_children;
}

double Sheep::getEnergieLossFactor() const {
	return getAppConfig().sheep_energy_loss_factor;
}

double Sheep::getBiteEnergie(LivingEntity* entite) const {
	return getAppConfig().sheep_energy_bite;
}

double Sheep::getCount() {
	return nb_sheep;
}

//========================== FONCTIONS UTILITAIRES ===================//

bool Sheep::eatable(LivingEntity const* other) const {					//Retourne true si other est mangeable par l'entité courante
	return other->eatableBy(this);
}

bool Sheep::eatableBy(Wolf  const*) const {								//Le mouton peut être mangé par le loup
	return true;
}

bool Sheep::eatableBy(Sheep const*) const {
	return false;
}

bool Sheep::eatableBy(Grass const*) const {
	return false;
}

bool Sheep::morte_ou_non() const {																				// est mort si son age est supérieur à sa durée de vie max 
																												// ou si son énergie est inférieur au seuil d'énergie min pour vivre
																												// ou si son état de santé est inférieur à zéro
	return ((energie < getAppConfig().animal_min_energy) or (horloge > getAppConfig().sheep_longevity) or (getImmuneSystem()->getHealth() < 0.0));
}

void Sheep::perte_energy(sf::Time dt) {																			// l'instance courante perd de l'énergie à chaque action
	
	double perte_energie;
	perte_energie = getAppConfig().animal_base_energy_consumption + vitesse * getAppConfig().sheep_energy_loss_factor * dt.asSeconds();
	energie -= perte_energie;		
	
}

bool Sheep::matable(LivingEntity const* other) const {
	return (other->canMate(this));
}

bool Sheep::canMate(Grass const*) const {
	return false;
}

bool Sheep::canMate(Wolf const*) const {
	return false;
}

bool Sheep::canMate(Sheep const* sheep) const {     											//Permet de savoir si l'instance courante peut se reproduire avec un mouton donné
	
	if ((this->genome_animal.getSex() != sheep->genome_animal.getSex())) {						//Vérifie que les 2 entités sont de sexes opposés
		if (isFemale())  {																		//Vérifie que l'énergie nécessaire pour se reproduire est assez grande
			if (sheep->getEnergie() > getAppConfig().sheep_energy_min_mating_male and (this->getEnergie() > getAppConfig().sheep_energy_min_mating_male)) {
			return true;
			}
		}
		
		else {
			 if ((sheep->getEnergie() > getAppConfig().sheep_energy_min_mating_female) and (this->getEnergie() > getAppConfig().sheep_energy_min_mating_male)) {
			return true;
			}
		}
	}
	
	else {
		return false;
	}
}


void Sheep::give_birth(Animal* animal) const {
																															//le genome de la mère est de toute façon celui de l'instance courante dans laquelle give_birth est appelée																																	//le génome de l'enfant est celui de la mère 
	getAppEnv().addEntity(new Sheep(animal->getPosition(), animal->getId(), new Genome(genome_mate),new Genome(genome_animal), false));
	
}

bool Sheep::infectableBy(LivingEntity const* other) const {
	return other->canInfect(this);
}

bool Sheep::canInfect(Wolf const* wolf) const {
	return false;
}

bool Sheep::canInfect(Sheep const* sheep) const {
	return getImmuneSystem()->isInfected();
}

bool Sheep::canInfect(Grass const* grass) const {
	return false;
}

	


