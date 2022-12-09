#include "Grass.hpp"
#include <Config.hpp>
#include <Application.hpp>
#include "Utility/Utility.hpp"


using namespace std;

unsigned int Grass::nb_grass(0);


//======================== CONSTRUCTEURS/DESTRUCTEURS ================//

/* On initialise le immunesystem à nullptr, car une grass n'en a pas, et ça nous permettra
 * dans Animal, par ex., de différencier une entité infectable ou non en testant la condition
 * getImmuneSystem() == nullptr, et si oui c'est une grass
*/

Grass::Grass(Vec2d pos_ent) : LivingEntity(pos_ent,getAppConfig().grass_initial_energy), immuneSystem(nullptr)
{
	verif_window(position_entite);
	
	++ nb_grass;
}

Grass::~Grass() { -- nb_grass; }

//========================= GETTEURS/SETTEURS ========================//

sf::Time Grass::getLongevity() const {
	return sf::seconds(1E9);
}

double Grass::getRadius() const {
	return energie / 2.0;
}

string Grass::getTexture() const {
	return getAppConfig().grass_texture;
}

ImmuneSystem* Grass::getImmuneSystem() const {
	return immuneSystem;
}

double Grass::getCount() {
	return nb_grass;
}

string Grass::getStatLabel() const {
	return s::GRASS_INDIVIDUAL;
}


//========================== DRAW ====================================//	

void Grass::drawOn(sf::RenderTarget& targetWindow) const {

    auto planteSprite = buildSprite(position_entite,(getRadius()*2), getAppTexture(getTexture()));
    targetWindow.draw(planteSprite);
    
    if (this == getAppEnv().getIndivTraque()) {
		auto planteSprite = buildSprite(position_entite,(getRadius()*2), getAppTexture(getAppConfig().entity_texture_tracked));
		targetWindow.draw(planteSprite);
   }
}



//========================== UPDATE ==================================//

void Grass::update(sf::Time dt) {
	
	LivingEntity::update(dt);
	if((energie + getAppConfig().grass_growth_factor * dt.asSeconds()) <= getAppConfig().grass_max_energy)
		energie += getAppConfig().grass_growth_factor * dt.asSeconds();
}



//======================== FONCTIONS UTILITAIRES =====================//


bool Grass::eatable(LivingEntity const* other) const {
	return other->eatableBy(this);
}

bool Grass::eatableBy(Wolf  const*) const {
	return false;
}

bool Grass::eatableBy(Sheep const*) const {
	return true;
}

bool Grass::eatableBy(Grass const*) const {
	return false;
}

bool Grass::morte_ou_non() const {
	return ((energie < getAppConfig().animal_min_energy) or (horloge > sf::seconds(1E9))); //ils parlent de par défault ça veut dire qqchose en particulier ?
}


bool Grass::matable(LivingEntity const* other) const {
	return other->canMate(this);
}

bool Grass::canMate(Grass const*) const {
	return true;
}

bool Grass::canMate(Sheep const*) const {
	return false;
}

bool Grass::canMate(Wolf const*) const {
	return false;
}


bool Grass::isEnceinte() const {
	return false;
}


void Grass::infect(Virus* v) {											//une grass n'est pas atteinte par une infection
	return ;
}

bool Grass::infectableBy(LivingEntity const* other) const {
	return other->canInfect(this);
}

bool Grass::canInfect(Wolf const* wolf) const {
	return false;
}

bool Grass::canInfect(Sheep const* sheep) const {
	return false;
}

bool Grass::canInfect(Grass const* grass) const {
	return false;
}

