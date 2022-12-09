#include <SFML/Graphics.hpp>
#include "Utility/Vec2d.hpp"
#include "Application.hpp"
#include <iostream>
#include <string>
#include "Obstacle.hpp"
#include <Interface/Drawable.hpp>
#include <Genetics/Virus.hpp>
#include <Genetics/ImmuneSystem.hpp>


#ifndef LIVINGENTITY_H
#define LIVINGENTITY_H



//pré-déclarations
class Wolf;
class Sheep;
class Grass;


enum State {
	 FOOD_IN_SIGHT, // nourriture en vue
	 FEEDING,       // en train de manger (là en principe il arrête de se déplacer)
	 RUNNING_AWAY,  // en fuite
	 MATE_IN_SIGHT, // partenaire en vue
	 MATING,        // vie privée (rencontre avec un partenaire!)
	 GIVING_BIRTH,  // donne naissance
	 WANDERING,     // déambule
	 IDLE,          // au repos
    };
    
 
 
class LivingEntity: public virtual Drawable, public Obstacle  {
	
	public:

//======================== CONSTRUCTEURS/DESTRUCTEURS ================//
	
	LivingEntity(const Vec2d& pos_ent, double energie_);
	virtual ~LivingEntity() {}
	
	
//========================= GETTEURS/SETTEURS ========================//

	virtual ImmuneSystem* getImmuneSystem() const = 0;
	virtual sf::Time getLongevity() const=0;
	virtual std::string getStatLabel() const = 0;
	
	
	Vec2d getPosition() const override;   //ATTENTION ON POURRA METTRE GETCENTER SI CA MARCHE PAS
virtual std::string getTexture() const=0;
	void setPosition(Vec2d& new_pos);
	void setEnergie(double e);
	double getEnergie() const;	
	double getAge() const;
	

//========================== UPDATE ==================================//

virtual void update(sf::Time dt);


//======================== FONCTIONS UTILITAIRES =====================//

virtual bool eatable(LivingEntity const* other) const = 0;
virtual bool eatableBy(Wolf  const*) const = 0;
virtual bool eatableBy(Sheep const*) const = 0;
virtual bool eatableBy(Grass const*) const = 0;
virtual bool morte_ou_non() const = 0; 


virtual bool matable(LivingEntity const* other) const = 0;
virtual bool canMate(Wolf const* wolf) const = 0;
virtual bool canMate(Sheep const* sheep) const = 0;
virtual bool canMate(Grass const* grass) const = 0;

void verif_window(Vec2d& new_pos);

virtual bool isEnceinte() const =0;
	
virtual void infect(Virus* v) = 0;

virtual bool infectableBy(LivingEntity const* other) const = 0;
virtual bool canInfect(Wolf const* wolf) const = 0;
virtual bool canInfect(Sheep const* sheep) const = 0;
virtual bool canInfect(Grass const* grass) const = 0;   

	
	
//========================== ATTRIBUTS ===============================//

	protected:
	
	Vec2d position_entite;
	double energie;
	sf::Time horloge;
	
};

#endif
