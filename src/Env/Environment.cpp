#include "Environment.hpp"
#include "LivingEntity.hpp"
#include <iostream>
#include <SFML/Graphics.hpp>
#include <algorithm>
#include <Random/Random.hpp>


using namespace std;

//======================== CONSTRUCTEURS/DESTRUCTEURS ================//

Environment::~Environment()
{
	reset();
}


Environment::Environment() : individu_traque(nullptr), nb_infectes(0)					//Initialisation de 3 troupeaux, et d'une map de troupeaux
{
	reset();
	Troupeau t1, t2, t3, t4, t5, t6;									// 3 troupeaux de moutons et 3 troupeaux de loups
	troupeaux = { { 0, t1 }, { 1, t2 }, { 2, t3 }, { 3, t4 }, { 4, t5 }, { 5, t6 } };
	
}


//========================= GETTEURS/SETTEURS ========================//

Animal* Environment::getChief(int id, Animal const* a) const {			//Retourne le chef d'un animal 'a' en allant chercher dans le troupeau correspondant à son identifiant actif
	return ((troupeaux.at(a->getidTroupeau())).chief);
}

LivingEntity* Environment::getIndivTraque() const {
	return individu_traque;
}

list <LivingEntity*> Environment::getEntitiesinSightforAnimal(Animal* animal_ptr) const {

	list<LivingEntity*> cibles;
	
	for(auto& entite: entite_orgas) {														
		if(entite != nullptr) {
			if(animal_ptr->isTargetInSight(entite->getPosition())) {	//Vérifie que l'entité sur laquelle on itère n'est pas morte(nullptr), et puis qu'elle est 
																		//Dans le champs de vision de l'animal 
				(cibles).push_back(entite);								//Si c'est le cas, on rajoute l'entité dans un tableau de cibles et on retourne à la fin toutes les cibles 
			
			}
			
			if(    (animal_ptr != entite)
			   and ((entite->getPosition() - animal_ptr->getPosition()).length() < getAppConfig().virus_infection_range)
			   and bernoulli(getAppConfig().virus_infection_probability)) {
																		//vérifie que l'animal ne se transmette pas le virus à lui-même & que l'entité soit dans le rayon de transmission
				animal_ptr->infection(entite);							//& teste la probabilité d'infection
																		//l'animal transmet l'infection à l'entité (sous les condition testées dans la fonction infection()
			}
		}
	}
	
	return cibles;
}

std::unordered_map<int, Troupeau> Environment::getMapTroupeaux() const {
	return troupeaux;
}

Animal* Environment::getPlusVieux(const Troupeau& t) const {						//Retourne dans un troupeau, l'animal le plus vieux
	
	Animal* animal_vieux;
	sf::Time max(sf::Time::Zero);
	
	for (auto entite : t.membresTroupeau) {
		
		if( max < entite->getLongevity()) {											//Itère sur tous les animaux du troupeau, va chercher leur âge avec le getteur et le compare au max
			max = entite->getLongevity();											
			animal_vieux = entite;
		}
	}
	
	return animal_vieux;															//Retourne l'animal de la liste avec l'âge le plus grand
}

void Environment::setNbInfectes(unsigned int nb) {
	nb_infectes = nb;
}

unsigned int Environment::getNbInfectes() const {
	return nb_infectes;
}

//========================== UPDATE ==================================//

void Environment::update(sf::Time dt) {
	
	
	
	setNbInfectes(0);
	
	for (auto& entite : entite_orgas) {									// Appelle la fonction update de Living Entity pour chaque entite de l'environnement
		entite -> update(dt);
		
		if(individu_traque != nullptr and individu_traque->morte_ou_non()) {
		stopTrackingAnyEntity();
		}
	
		if(entite->getImmuneSystem() != nullptr and (entite->getImmuneSystem())->isInfected())
			setNbInfectes(getNbInfectes() + 1);							//++ nb_infectes
		
		if(entite -> morte_ou_non()) {									// Si l'entité est morte, alors on la met à nullptr et on enlève l'adresse
			delete entite;
			entite = nullptr;
		}
			
	}	
	entite_orgas.erase(remove(entite_orgas.begin(), entite_orgas.end(), nullptr), entite_orgas.end());	//supprime les entités morte(nullptr) de la liste d'entité organique 
	
	

	for(auto& val: troupeaux) {											//Itère sur toutes les maps du troupeau 
		
		if(((val.second).chief) == nullptr) {							//Si le chef de la map est mort,
			
			if (not ((val.second).membresTroupeau).empty()) {			// et qu'il reste des animaux dans le troupeau, 
				(val.second).chief = getPlusVieux(val.second);			// le nouveau est chef est le plus vieux du troupeau
				
			}
					
		}

	}

}
 
//========================== DRAW ====================================//

void Environment::drawOn(sf::RenderTarget& targetWindow) const {

    for (auto const& entite: entite_orgas) {							//Itère sur toutes les entités organiques et les dessine avec la fonction draw de Living Entity
		entite->drawOn(targetWindow);
	}
	/*
	if(individu_traque != nullptr) {
		dessin_traque(targetWindow);
	}*/
}
/*
void Environment::dessin_traque(sf::RenderTarget& targetWindow) const {
	auto traqSprite = buildSprite(individu_traque->getPosition(), 20, getAppTexture(getAppConfig().entity_texture_tracked));
	targetWindow.draw(traqSprite);
}*/

//========================== FONCTIONS UTILITAIRES ===================//

void Environment::reset() {												//Supprime toutes les entités de l'environnement
	
	if(individu_traque != nullptr) {
		stopTrackingAnyEntity();
	}
	
	for (auto& entite : entite_orgas) {
		delete entite;
		entite = nullptr;
	}

	entite_orgas.clear();											//Nettoie le tableau d'entités organiques
	setNbInfectes(0);
}

void Environment::addEntity(LivingEntity* entite) {  
	entite_orgas.push_back(entite);
}

unordered_map<string, double> Environment::fetchData(string title) const {
	
	unordered_map<string, double> new_data = {};
	
	if(title == s::GENERAL)														//Si le titre de la map est GENERAL, on retourne une map qui donne le nombre de loups, de moutons et de grass
		new_data = { { s::WOLVES, Wolf::getCount() }, { s::SHEEP, Sheep::getCount() }, { s::GRASS, Grass::getCount() }, { s::INFECTED, getNbInfectes() } };
			
	if(title == s::GRASS_INDIVIDUAL)
		new_data = { { s::ENERGY, individu_traque->getEnergie() } };

	if(individu_traque != nullptr) {

		if(title == s::ANIMAL_INDIVIDUAL and (individu_traque->getImmuneSystem()) != nullptr) {
																			//le nullptr vérifie que c'est bien un animal et qu'il a un sys. immun.
			if((individu_traque->getImmuneSystem())->getVirus() != nullptr) {
				new_data = { { s::ENERGY, individu_traque->getEnergie() },
							 { s::HEALTH, (individu_traque->getImmuneSystem())->getHealth() },
							 { s::VIRUS, ((individu_traque->getImmuneSystem())->getVirus())->getQte() },
							 { s::ADASCORE, (individu_traque->getImmuneSystem())->calcul_adaptatif() },
							 { s::IMUNAC, (individu_traque->getImmuneSystem())->getActivationLevel() },
							 { s::SCORE, (individu_traque->getImmuneSystem())->calcul_score() } };
			} else {
				new_data = { { s::ENERGY, individu_traque->getEnergie() },
							 { s::HEALTH, (individu_traque->getImmuneSystem())->getHealth() },
							 { s::VIRUS, 0.0 },
							 { s::ADASCORE, (individu_traque->getImmuneSystem())->calcul_adaptatif() },
							 { s::IMUNAC, (individu_traque->getImmuneSystem())->getActivationLevel() },
							 { s::SCORE, (individu_traque->getImmuneSystem())->calcul_score() } };
			}
		}
	} 
	
	return new_data;
	
}

void Environment::ajouterAnimalTroupeau(Animal* a) {
		
	(troupeaux.at(a->getidTroupeau()).membresTroupeau).push_back(a);	//Ajoute l'animal dont son identifiant actif correspond à celui d'un troupeau
	
}

void Environment::ajouterChefTroupeau(Animal* a) {

	if(((troupeaux.at(a->getidTroupeau())).membresTroupeau).empty()) {					//Regarde si le tableau est vide 
		ajouterAnimalTroupeau(a);														// Dans ce cas, ajoute l'animal 'a' au troupeau et celui ci devient chef 
		troupeaux.at(a->getidTroupeau()).chief = a;
	}	
	
	ajouterAnimalTroupeau(a);											//Si le tableau n'est pas vide, alors on ne fait que ajouter un animal sans le chef
}
	
void Environment::killEntity(Animal const* a) {							//Tue le chef d'un animal 'a' (le met à nullptr)
	troupeaux.at(a->getidTroupeau()).chief = nullptr;
}
	
Vec2d Environment::force_evitement(Animal* animal) const {				//Calcul la force qui permet aux entités de pas s'agglutiner entre eux
	
	int coeff(3000);
	int seuil(100);
	Vec2d force_evitement(0.0,0.0);
	
	for(auto const& entite : entite_orgas) {
		
		if ((entite != nullptr) and (animal != nullptr)) {
		
			if ((entite-> getPosition() != animal->getPosition())) {	//Vérifie que l'animal qu'on passe en argument n'est pas l'instance courante
				
				if ((entite->getPosition() - animal->getPosition()).length() < seuil) {		//Si la distance entre deux animaux est inférieur à un seuil, on calcul la force d'évitement
					force_evitement += ((coeff * (animal->getPosition() - entite->getPosition())) / ((animal->getPosition() - entite->getPosition()).lengthSquared()));
				}
			}
	}
}
	return force_evitement;
	
}

void Environment::supprimeListe(Animal* ptr) {

	((troupeaux.at(ptr->getidTroupeau())).membresTroupeau).remove(ptr);		//Supprime un animal de son troupeau quand il est mort avec la fonction remove

}

void Environment::trackEntity(const Vec2d& posCurseur) {
	
	for(auto const& entite : entite_orgas) {
		
		if(entite->isPointInside(posCurseur)) {
			individu_traque = entite;
		}
	}
	
	if(individu_traque != nullptr)
		getApp().focusOnStat(individu_traque->getStatLabel());
		
}

void Environment::stopTrackingAnyEntity() {
	individu_traque = nullptr;
	getApp().focusOnStat(s::GENERAL);
}

void Environment::infectEntity(const Vec2d& posCurseur, Virus* v) {
	
	for(auto const& entite : entite_orgas) {
		
		if(entite->isPointInside(posCurseur)) {
			
			entite->infect(v);
		}
	}
}

void Environment::infectEntity(const Vec2d& posCurseur) {
	
	Virus* v(new Virus());

	infectEntity(posCurseur,v);
}


