#include "Utility/Vec2d.hpp"
#include <SFML/Graphics.hpp>
#include <list>
#include "Animal.hpp"
#include "LivingEntity.hpp"
#include <unordered_map>
#include <Utility/Constants.hpp>
#include "Grass.hpp"
#include "Wolf.hpp"
#include "Sheep.hpp"
#include <string>
#include <Application.hpp>
#include <Genetics/Virus.hpp>


#ifndef ENVIRONMENT_H
#define ENVIRONMENT_H



//======================== PRE-DELCARATIONS ==========================//

class Animal;

//======================== CLASSE TROUPEAU ===========================//
																		// Création d'une classe Troupeau qui prend en attribut un chef(initialisé à nullptr) et des membres du troupeau
class Troupeau {
	public:
	Troupeau() : chief(nullptr) {}
	
	Animal* chief;
	std::list<Animal*> membresTroupeau;

};

class Environment {

	public:

//======================== CONSTRUCTEURS/DESTRUCTEURS ================//
	
	Environment(Environment const& envi) = delete;						//Supprime la copie d'un environnement 
	Environment& operator=(Environment other) = delete;
	Environment();
	virtual ~Environment();
	
//========================= GETTEURS/SETTEURS ========================//
	
	Animal* getChief(int , Animal const*) const;
	
//========================== UPDATE ==================================//
	
	void update(sf::Time dt);
	
//========================== DRAW ====================================//

	void drawOn(sf::RenderTarget& targetWindow) const;
	//void dessin_traque(sf::RenderTarget& targetWindow) const;
	
//========================== FONCTIONS UTILITAIRES ===================//

	void addEntity(LivingEntity*);
	void reset();
	std::list<LivingEntity*> getEntitiesinSightforAnimal(Animal* animal_ptr) const;
	std::unordered_map<std::string, double> fetchData(std::string title) const;
	std::unordered_map<int, Troupeau> getMapTroupeaux() const;
	void ajouterAnimalTroupeau(Animal*);
	void ajouterChefTroupeau(Animal*);
	void killEntity(Animal const* a);
	Animal* getPlusVieux(const Troupeau&) const;
	void supprimeListe(Animal* ptr);
	Vec2d force_evitement(Animal* animal) const;
	void trackEntity(const Vec2d& posCurseur);
	void stopTrackingAnyEntity();
	LivingEntity* getIndivTraque() const;
	void infectEntity(const Vec2d& posCurseur, Virus* v);
	void infectEntity(const Vec2d& posCurseur);
	void setNbInfectes(unsigned int nb);
	unsigned int getNbInfectes() const;
	
	private:
	
	std::list<LivingEntity*> entite_orgas;
	std::unordered_map<int, Troupeau> troupeaux;
	LivingEntity* individu_traque;
	unsigned int nb_infectes;
};


#endif
