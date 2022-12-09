#include <SFML/Graphics.hpp>
#include "Virus.hpp"
#include <array>


#ifndef IMMUNESYSTEM_H
#define IMMUNESYSTEM_H


typedef std::array<double, 10> Profil;


//======================== PRE_DECLARATIONS ==========================//

class Animal;


class ImmuneSystem {

	public:

//======================== CONSTRUCTEURS/DESTRUCTEURS ================//
	
	ImmuneSystem(Animal* animal);
	~ImmuneSystem();
	
//========================= GETTEURS/SETTEURS ========================//
	
	double getHealth() const;
	double getActivationLevel() const;
	Animal* getHote() const;
	Virus* getVirus() const;
	void setVirus(Virus* v);
	Profil getAdaptative_profil() const;
	
//========================== UPDATE ==================================//
	
	void update(sf::Time dt);
	
//========================== FONCTIONS UTILITAIRES ===================//
	
	double calcul_score() const;
	double calcul_adaptatif() const;
	void adaptativeModif(sf::Time dt);
	void activationDecrease();
	void activationIncrease(sf::Time dt);
	void healthDecrease(sf::Time dt);
	void healthIncrease(sf::Time dt);
	bool isInfected() const;
	
//========================== ATTRIBUTS ===============================//

	private:
	
	double health;
	double activationLevel;
	Animal* hote;
	Virus* virus;	
	Profil adaptative_profil;
	
};



#endif
