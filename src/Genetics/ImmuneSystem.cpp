#include "ImmuneSystem.hpp"
#include <Application.hpp>
#include <Env/Animal.hpp>
#include <Random/Random.hpp>

using namespace std;



//======================== CONSTRUCTEURS/DESTRUCTEURS ================//

ImmuneSystem::ImmuneSystem(Animal* animal)
	: health(getAppConfig().immune_health_max), activationLevel(getAppConfig().immune_adaptive_baseline), hote(animal), virus(nullptr)
{
	for (auto& val : adaptative_profil) {
		val = 0.0;
	}
}


ImmuneSystem:: ~ImmuneSystem()
{
	delete virus;
	virus = nullptr;
}

//======================== GETTEURS/SETTEURS =========================//

double ImmuneSystem::getHealth() const {
	return health;
}

double ImmuneSystem::getActivationLevel() const {
	return activationLevel;
}

Animal* ImmuneSystem::getHote() const {
	return hote;
}

Virus* ImmuneSystem::getVirus() const {
	return virus;
}

void ImmuneSystem::setVirus(Virus* v) {
	virus = v;
}

Profil ImmuneSystem::getAdaptative_profil() const {
	return adaptative_profil;
}

//======================== UPDATE ====================================//

void ImmuneSystem::update(sf::Time dt) {
	
	if(virus != nullptr) {
	
//Modularisation des différents calculs d'augmentation et de diminution des attributs
		activationIncrease(dt);
		
		virus->update(dt);

/*Attention, voir spécificité de adaptativeModif(), qui ne se contente pas d'incrémenter le score comme suggéré,
 *car cela fait que si le profilImmun est au-dessus de celui du virus il va continuer à augmenter, alors que l'on
 *souhaite le voir converger vers le profil du virus.
 * Effectuer le calcul comme suggéré dans l'énoncé sans cette correction rendait tous les virus non mortels...
*/
		adaptativeModif(dt);
		
		healthDecrease(dt);
		
		virus->setQte((virus->getQte()) - (calcul_score() * dt.asSeconds()));
		
		if(virus->isDead())	{											//si le virus est mort => le système immunitaire n'a plus de virus en lui
			virus = nullptr;
		}
	}

	else {
		
		activationDecrease();
		
		healthIncrease(dt);
	}
}

//======================== FONCTIONS UTILITAIRES =====================//

double ImmuneSystem::calcul_score() const {
	if(virus != nullptr) {
		double score(calcul_adaptatif());
		double c_eff(getAppConfig().immune_defense_effectiveness);
		double c_var(getAppConfig().immune_defense_random_variability);
		
		double add(0.0);
		for(size_t i(0); i < getAdaptative_profil().size(); ++i) {
			add += (c_eff * (((hote->getGenome()).getImmuneGenes(i) * (virus->getVirul(i))) + uniform(-c_var, c_var)));
		}
		score += add;
		
		score = (score * score * activationLevel);
		
		return score;
	}
	else
		return 0.0;
}

double ImmuneSystem::calcul_adaptatif() const {
	if(virus != nullptr) {
		double calcul(0.0);
		double c_eff(getAppConfig().immune_defense_effectiveness);
		for(size_t i(0); i < getAdaptative_profil().size(); ++i) {
			calcul += (getAdaptative_profil()[i] * (virus->getVirul(i)));
		}
		calcul *= c_eff;
		
		return calcul;
	}
	else 
		return 0.0;
}

/* Le profil immunitaire doit converger vers celui du virus pour assurer la meilleure réponse,
 * il ne se contente donc pas d'augmenter, mais si une de ces valeurs est supérieure à celle du virus,
 * on la fait diminuer de sorte à ce qu'elle converge vers celle du profil du virus
*/
void ImmuneSystem::adaptativeModif(sf::Time dt) {
	for (size_t i(0); i < getAdaptative_profil().size(); ++i) {
		if(adaptative_profil[i] < virus->getVirul(i))
			adaptative_profil[i] += (abs(virus->getVirul(i)) * dt.asSeconds() * 0.1);
		else
			adaptative_profil[i] -= (abs(virus->getVirul(i)) * dt.asSeconds() * 0.1);
	}
}

void ImmuneSystem::activationDecrease() {
	if((activationLevel * 0.995) >= getAppConfig().immune_adaptive_baseline)
		activationLevel *= 0.995;
}

void ImmuneSystem::activationIncrease(sf::Time dt) {
	activationLevel *= (1 + dt.asSeconds() * 0.5 * (1 - (activationLevel*activationLevel / 16.0)));
}

void ImmuneSystem::healthDecrease(sf::Time dt) {
	health -= (getAppConfig().immune_health_penalty * (virus->getQte()) * dt.asSeconds());
}

void ImmuneSystem::healthIncrease(sf::Time dt) {
	if((health + getAppConfig().immune_health_recovery * dt.asSeconds()) <= getAppConfig().immune_health_max)
		health += (getAppConfig().immune_health_recovery * dt.asSeconds());
}

bool ImmuneSystem::isInfected() const {
	return (virus != nullptr and (virus->getQte() > getAppConfig().virus_min_quantity_for_infection));
}


