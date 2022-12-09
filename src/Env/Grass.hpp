#include <SFML/Graphics.hpp>
#include "LivingEntity.hpp"
#include "Utility/Vec2d.hpp"
#include "Utility/Constants.hpp"
#include <Genetics/ImmuneSystem.hpp>
#include <Genetics/Virus.hpp>


#ifndef GRASS_H
#define GRASS_H



class Grass: public LivingEntity {
	

	public:
//======================== CONSTRUCTEURS/DESTRUCTEURS ================//

	Grass(Vec2d pos_ent);
	virtual ~Grass();
	
//========================= GETTEURS/SETTEURS ========================//
	
	double getRadius() const override;
	std::string getTexture() const override;
	sf::Time getLongevity() const override;
	std::string getStatLabel() const override;
	ImmuneSystem* getImmuneSystem() const override;
	static double getCount();
	
//========================== DRAW ====================================//	
	
	void drawOn(sf::RenderTarget& targetWindow) const override;
	
	
//========================== UPDATE ==================================//

	void update(sf::Time dt) override;
	
	
//======================== FONCTIONS UTILITAIRES =====================//

	bool eatable(LivingEntity const* other) const override;
	bool eatableBy(Wolf  const*) const override;
	bool eatableBy(Sheep const*) const override;
	bool eatableBy(Grass const*) const override;
	bool morte_ou_non() const override;
	
	bool matable(LivingEntity const* other) const override;
	bool canMate(Grass const*) const override;
	bool canMate(Sheep const*) const override;
	bool canMate(Wolf const*) const override;
	
	bool isEnceinte() const override;
	void infect(Virus* v) override;
	bool infectableBy(LivingEntity const* other) const override;
	bool canInfect(Wolf const* wolf) const override;
	bool canInfect(Sheep const* sheep) const override;
	bool canInfect(Grass const* grass) const override;
	
	
//========================== ATTRIBUTS ===============================//
	
	private:
	
	static unsigned int nb_grass;
	ImmuneSystem* immuneSystem;
	
};
#endif
