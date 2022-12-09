#include <SFML/Graphics.hpp>
#include <Config.hpp>
#include <string>
#include "LivingEntity.hpp"
#include <Genetics/Genome.hpp>

#ifndef SHEEP_H
#define SHEEP_H

class Sheep: public Animal {
	
	public:

//======================== CONSTRUCTEURS/DESTRUCTEURS ================//
	
	Sheep (const Vec2d& pos_ent, int unId = 0, Genome* genome_mere=nullptr, Genome* genome_pere=nullptr, bool sans_parents1 = true);
	virtual ~Sheep();
	
//========================= GETTEURS/SETTEURS ========================//

	double getViewRange() const override;
	double getViewDistance() const override;
	double getStandardMaxSpeed() const override;
	double getMass() const override;
	double getRadius() const override;
	std::string getTexture() const override;
	double getRandomWalkRadius() const override;
	double getRandomWalkDistance() const override;
	double getRandomWalkJitter() const override;
	double getEnergieLossPerChild() const override;
	double getEnergieLossMatingMale() const override;
	int getReproductionMinChildren() const override;
	int getReproductionMaxChildren() const override;
	sf::Time getLongevity() const override;
	double getEnergieLossFactor() const override;
	double getBiteEnergie(LivingEntity* entite) const override;
	sf::Time getGestationTime() const override;
	
	static double getCount();

//========================== FONCTIONS UTILITAIRES ===================//

	bool eatable(LivingEntity const* other) const override;
	bool eatableBy(Wolf  const*) const override;
	bool eatableBy(Sheep const*) const override;
	bool eatableBy(Grass const*) const override;
	
	
	bool matable(LivingEntity const* other) const override;
	bool canMate(Grass const*) const override;
	bool canMate(Wolf const*) const override;
	bool canMate(Sheep const* sheep) const override;


	bool morte_ou_non() const override;
	void perte_energy(sf::Time) override;
	void give_birth(Animal* animal) const override;


	bool infectableBy(LivingEntity const* other) const override;
	bool canInfect(Wolf const* wolf) const override;
	bool canInfect(Sheep const* sheep) const override;
	bool canInfect(Grass const* grass) const override;

	private:
	
	static unsigned int nb_sheep;
	
};
#endif

