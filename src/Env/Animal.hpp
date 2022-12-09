#include <SFML/Graphics.hpp>
#include "Utility/Vec2d.hpp"
#include "Utility/Constants.hpp"
#include <Genetics/Genome.hpp>
#include "Obstacle.hpp"
#include "LivingEntity.hpp"
#include <list>
#include <Genetics/ImmuneSystem.hpp>
#include <Genetics/Virus.hpp>
#include <array>


#ifndef ANIMAL_H
#define ANIMAL_H

typedef std::array<double, 10> Profil;

//======================== ENUMS =====================================//

enum Deceleration {
	STRONG=3,
	VERY_STRONG=30,
	MEDIUM=2,
	WEAK=1,
};


enum Enceinte {
	
	ENCEINTE,
	NOTENCEINTE,
};

//======================== CLASSE ANIMAL =============================//

class Animal: public LivingEntity {
	
//======================== CONSTRUCTEURS/DESTRUCTEURS ================//
	public:

	Animal(const Vec2d& pos_ent, double energie_, sf::Time unTemps, Genome* genome_mere=nullptr, Genome* genome_pere=nullptr, bool sans_parents1=true, int unId = -1);
	virtual ~Animal();
	
	
//========================= GETTEURS/SETTEURS ========================//

virtual double getStandardMaxSpeed() const=0;
virtual double getMass() const=0;
virtual double getViewRange() const=0;
virtual double getViewDistance() const=0;
virtual double getRandomWalkRadius() const=0;
virtual double getRandomWalkDistance() const=0;
virtual double getRandomWalkJitter() const=0;
virtual double getEnergieLossFactor() const=0;
virtual double getBiteEnergie(LivingEntity*) const = 0;
virtual double getEnergieLossPerChild() const =0;
virtual double getEnergieLossMatingMale() const=0;
virtual int getReproductionMinChildren() const=0;
virtual int getReproductionMaxChildren() const=0;
virtual sf::Time getGestationTime() const=0;

	Vec2d getSpeedVector() const;	
	void setTargetPosition(const Vec2d&);
	double getRotation() const;
	void setRotation(double angle);
	double getMaxSpeed() const;
	double getDeceleration() const;
	std::string getStateString() const;
	std::string getGenreString() const;
	Genome getGenome() const;
	int getId() const;
	std::string getStatLabel() const override;
	State getState() const;
	void setState(const State&);
	int getidTroupeau() const;
	LivingEntity* getEntiteReproductrice() const;
	void setPartner(Animal* mate);
	ImmuneSystem* getImmuneSystem() const override;
	void setImmuneGenes(Profil p);
		
//========================== UPDATE ==================================//
		
	void updateState(sf::Time dt);
	void update(sf::Time) override;
	void analyseEnvironment();
		
//========================== DRAW ====================================//	
		
	void drawOn(sf::RenderTarget&) const override;
	void drawVision(sf::RenderTarget& targetWindow) const;
	void dessin_cible_virtuelle(sf::RenderTarget&) const;
	void dessin_animal(sf::RenderTarget&) const;
	void dessin_text(sf::RenderTarget&) const;
	void affichage_text(std::string, const Vec2d&,sf::RenderTarget&) const;
	void dessin_enceinte(sf::RenderTarget&) const;
	void dessin_crown(sf::RenderTarget&) const;
	void dessin_traque(sf::RenderTarget&) const;
	void dessin_infection(sf::RenderTarget& target) const;
	
//========================== DEPLACEMENTS/FORCES =====================//

virtual bool isFreeMoving() const;
	Vec2d force_ralentissement();
	Vec2d randomWalk();
	Vec2d force_fuite() const;
	Vec2d calcul_force();
	Vec2d force_chef() const;
	void deplacement(sf::Time, const Vec2d&);
	
//========================== FONCTIONS UTILITAIRES ===================//	

	Vec2d ConvertToGlobalCoord(const Vec2d& local) const;
	bool isEnemy(LivingEntity const*);										
	bool isChief() const;
	bool hasChief() const;
	void infect(Virus* v) override;
	void infection(LivingEntity* entite) const;
	
//========================== NOURRITURE ==============================//

virtual void perte_energy(sf::Time);	
	bool isTargetInSight(const Vec2d& v) const;
	bool ConditionsFoodInSight() const;
	bool rassasie() const;
	void consommation(sf::Time, Vec2d&);
	bool isFemale() const;	

//========================== REPRODUCTION ============================//

	void MetAuMonde();
	void meet( LivingEntity const*); 
	int calcul_nb_bebes() const;
	virtual void give_birth(Animal* animal) const = 0;
	bool reproduction_reciproque(LivingEntity const*) const;
	bool ConditionMateInSight() const;
	bool ConditionAccoucher() const;
	bool isEnceinte() const override;
	
//========================== EXTENSIONS ==============================//

	double getTaille() const;

//========================== ATTRIBUTS ===============================//
	
	protected:
		
	//DEPLACEMENT
	Vec2d new_position_pred;
	Vec2d direction_pred;
	Vec2d new_direction_pred;
	double vitesse;
	Vec2d new_vitesse;
	Vec2d current_target;
	Vec2d position_cible;  
	
	//GENOME
	Genome genome_animal;
	Genome genome_mate;
	
	//ENUMS
	State state;
	Deceleration deceleration;
	
	sf::Time horlogeMating;
	sf::Time horlogeFeeding;
	sf::Time horlogeDelivery;
	sf::Time horlogeIdle;
	sf::Time gestationTime;
	
	Enceinte gestation;
	bool sans_parents;
	bool pause_manger;
	bool TempsAvantAccouchement;
	bool pause_mating;
	bool pause_idle;
	
	LivingEntity* entite_mangeable;
	std::vector<LivingEntity*> entites_ennemi;
	Animal* entite_reprodutrice;
	Animal* entite_chef;
	
	int nb_bebes;
	int idTroupeau;  													// Choix de conception : pas unsigned int pour pouvoir utiliser le fait que -1 sera l'indice par défault utilise pour les types
																		//d'animaux qui n'ont pas de troupeaux (loups)						
	ImmuneSystem* immuneSystem;
	
};

#endif
