#include "Animal.hpp"
#include "Config.hpp"
#include "Utility/Arc.hpp"
#include "Utility/Utility.hpp"
#include <Application.hpp>
#include <Random/Random.hpp>
#include <Genetics/Genome.hpp>
#include <iostream>
#include <cmath>
#include <vector>


using namespace std;

//======================== CONSTRUCTEURS/DESTRUCTEURS ================//



Animal::Animal(const Vec2d& pos_ent, double energie_, sf::Time unTemps,  Genome* genome_mere, Genome* genome_pere, bool sans_parents1, int unId)
: LivingEntity(pos_ent, energie_), direction_pred(1,0), vitesse(0.0), position_cible(0,0),
			   current_target(1,0), genome_animal(genome_mere, genome_pere), genome_mate(nullptr), horlogeMating(sf::Time::Zero), horlogeFeeding(sf::Time::Zero),horlogeDelivery(sf::Time::Zero), horlogeIdle(sf::Time::Zero), gestationTime(unTemps),
			   gestation(NOTENCEINTE),sans_parents(sans_parents1), pause_manger(false), TempsAvantAccouchement(false), pause_mating(false), pause_idle(false), entite_mangeable(nullptr), entite_reprodutrice(nullptr), entite_chef(nullptr),
			   nb_bebes(0), entites_ennemi(0,0), idTroupeau(unId)
{
	immuneSystem = (new ImmuneSystem(this));
	
	if(idTroupeau >= 0) {												//ajoute l'instance courante aux troupeaux correspondants dans la map "troupeaux"

		getAppEnv().ajouterChefTroupeau(this);
	
	}																//si aucun chef n'a été nommé, l'instance courante devient le chef de son troupeau
}


Animal::~Animal()
{
	
	if(idTroupeau >= 0) {
		
		if(isChief()) {			
			getAppEnv().killEntity(this);				
		}
	
		getAppEnv().supprimeListe(this);
	}
	
}



//========================= GETTEURS/SETTEURS ========================//



void Animal::setRotation(double angle) {
	direction_pred = Vec2d(cos(angle), sin(angle));
}

double Animal::getRotation() const {
	return (direction_pred.angle()/DEG_TO_RAD);
}

void Animal::setTargetPosition(const Vec2d& v) {
	position_cible = v;
}

Vec2d Animal::getSpeedVector() const {
	return direction_pred*vitesse;
}

double Animal::getMaxSpeed() const {
	double v_max(getStandardMaxSpeed());
	double seuil(50);
	double ralentissement(0.4);			//VALEURS A ADAPTER
							
	if (energie < seuil)
	v_max *= ralentissement;
	
	
	switch(state) {
		case FOOD_IN_SIGHT :
			v_max *= 3;
			break;
		case MATE_IN_SIGHT :
			v_max *= 2;
			break;
		case RUNNING_AWAY :
			v_max *= 4;
			break;
		default :
			break;
	}
	
	return v_max;
}

double Animal::getDeceleration() const {
	
	switch(deceleration)
	{
		
	case VERY_STRONG: return VERY_STRONG*0.3;
	break;
	
	case STRONG: return STRONG*0.3;
	break;
	
	case MEDIUM: return MEDIUM*0.3;
	break;
	
	case WEAK: return WEAK*0.3;
	break;
	
   }
 }

string Animal::getStateString() const {
	
	switch(state) {
		
		case FOOD_IN_SIGHT :

			return "FOOD IN SIGHT";
			break;

		case WANDERING :

			return "WANDERING";
			break;

		case FEEDING :

			return "FEEDING";
			break;

		case MATE_IN_SIGHT:

			return "MATE IN SIGHT";
			break;

		case MATING:

			return "MATING";
			break;

		case GIVING_BIRTH:

			return "GIVING BIRTH";
			break;
			
		case RUNNING_AWAY:

			return "RUNNING AWAY";
			break;

		case IDLE:
		
			return "IDLE";
			break;

		default :

			return "UNKNOWN STATE";
			break;
		}
	
}

string Animal::getGenreString() const {
	
	switch (genome_animal.getSex()) {
		
		case MALE:

			return "Male";
			break;

		case FEMALE:

			return "Female";
			break;

		default :
			
			return "UNKNOWN GENDER";
			break;
		}
}

Genome Animal::getGenome() const {
	return genome_animal;
}

int Animal::getidTroupeau() const {
	return idTroupeau;
}

int Animal::getId() const {
	return idTroupeau;
}

string Animal::getStatLabel() const {
	return s::ANIMAL_INDIVIDUAL;
}

State Animal::getState() const {
	return state;
}

LivingEntity* Animal::getEntiteReproductrice() const {
	return entite_reprodutrice;
}

void Animal::setPartner(Animal* mate) {
	entite_reprodutrice = mate;
}

void Animal::setState(const State& etat){
	state = etat;
}

ImmuneSystem* Animal::getImmuneSystem() const {
	return immuneSystem;
}

void Animal::setImmuneGenes(Profil p) {
	getGenome().setProfilImmun(p);
}



//========================== DRAW ====================================//



void Animal::drawVision(sf::RenderTarget& targetWindow) const {			
	
	sf::Color color = sf::Color::Black;
	color.a = 16; // light, transparent grey
	Arc arcgraphics(direction_pred.x-((getViewRange()/DEG_TO_RAD)/2), ((direction_pred.x+(getViewRange()/DEG_TO_RAD)/2)), getViewDistance(), color, getViewDistance());
	arcgraphics.setOrigin(getViewDistance(), getViewDistance());
	arcgraphics.setPosition(position_entite);
	arcgraphics.rotate(getRotation());
	targetWindow.draw(arcgraphics);

}

void Animal::drawOn(sf::RenderTarget& targetWindow) const {


	dessin_animal(targetWindow);
 
	if (not isDebugOn()) {
		drawVision(targetWindow);
		
		if(state == WANDERING)											//on ne dessine pas la cible virtuelle si l'animal a une cible réelle (food, mate, proie, etc.)
			dessin_cible_virtuelle(targetWindow);
		
		dessin_text(targetWindow);
		
		if (isEnceinte())
			dessin_enceinte(targetWindow);
		
		if(getImmuneSystem() != nullptr and getImmuneSystem()->isInfected())
			dessin_infection(targetWindow);
	}
	
	if(this == getAppEnv().getIndivTraque()) {						//on vérifie si this est l'individu traque avant d'afficher le curseur
		dessin_traque(targetWindow);
	}	
}

void Animal::dessin_cible_virtuelle(sf::RenderTarget& targetWindow) const {

	sf::CircleShape cercle_virtuel;
		 cercle_virtuel.setPosition(position_entite + getRandomWalkDistance()*direction_pred);
		 cercle_virtuel.setRadius(getRandomWalkRadius());
		 cercle_virtuel.setFillColor(sf::Color(0,0,0,0));
		 cercle_virtuel.setOutlineThickness(1);
		 cercle_virtuel.setOrigin(getRandomWalkRadius(), getRandomWalkRadius());
		 cercle_virtuel.setOutlineColor(sf::Color(100,100,100));
		 targetWindow.draw(cercle_virtuel);
		 
	 double rayon_virt(5);
		sf::CircleShape cible_virt;
		cible_virt.setPosition(ConvertToGlobalCoord(Vec2d(current_target.x+getRandomWalkDistance(),current_target.y)));
		cible_virt.setRadius(rayon_virt);
		cible_virt.setOrigin(rayon_virt, rayon_virt);
		cible_virt.setFillColor(sf::Color(0, 0, 255));
		targetWindow.draw(cible_virt);
}

void Animal::dessin_animal(sf::RenderTarget& targetWindow) const {
	
	double angle(getRotation());
	//getTaille() actualise la taille en fonction de l'âge de l'animal
    auto animalSprite = buildSprite(position_entite,getTaille(), getAppTexture(getTexture()));
    animalSprite.setRotation(angle); 
    targetWindow.draw(animalSprite);
    
    if (isChief())
		dessin_crown(targetWindow);
	
}

void Animal::dessin_text(sf::RenderTarget& targetWindow) const {
	
	Vec2d position_state(direction_pred.x +150, direction_pred.y+120);
	Vec2d position_genre(direction_pred.x +150, direction_pred.y);
	Vec2d position_energie(direction_pred.x +120, direction_pred.y);
	Vec2d position_hungry(direction_pred.x +120, direction_pred.y -70);
	Vec2d position_follower(direction_pred.x + 200, direction_pred.y - 50);
	Vec2d position_chief(direction_pred.x +200, direction_pred.y -50);
	
	if(getStateString() != "UNKNOWN STATE")
		affichage_text(getStateString(), position_state, targetWindow);
	if(getGenreString() != "UNKOWN GENDER")
		affichage_text(getGenreString(), position_genre,targetWindow);
	affichage_text(to_nice_string(energie), position_energie, targetWindow);
	
	if (not rassasie()) 
		affichage_text("Hungry", position_hungry, targetWindow);
		
	if (hasChief()) 
		affichage_text("Follower, heird : " + to_string(idTroupeau), position_follower, targetWindow);
		
	if (isChief()) 
		affichage_text("Chef, heird : " + to_string(idTroupeau), position_chief, targetWindow);
		
	

}

void Animal::affichage_text(string text, const Vec2d& position,sf::RenderTarget& targetWindow) const {
	
 auto text_ = buildText(text,
                ConvertToGlobalCoord(position), 
                getAppFont(),
                getAppConfig().default_debug_text_size,
                sf::Color::Black);
				text_.setRotation(getRotation() + 90); 
				targetWindow.draw(text_);

 
	}

void Animal::dessin_enceinte(sf::RenderTarget& targetWindow) const {
	
    sf::CircleShape cercle_enceinte;
     cercle_enceinte.setPosition(getPosition());
     cercle_enceinte.setRadius(getRadius());
     cercle_enceinte.setOutlineThickness(1);
     cercle_enceinte.setFillColor(sf::Color(0,0,0,0));
     cercle_enceinte.setOutlineColor(sf::Color::Magenta);
     cercle_enceinte.setOrigin(getRadius(), getRadius());
     targetWindow.draw(cercle_enceinte);
    
}

void Animal::dessin_crown(sf::RenderTarget& target) const {
	
	auto crownSprite = buildSprite(position_entite, 40, getAppTexture(getAppConfig().animal_texture_leader)); 
    target.draw(crownSprite);
}

void Animal::dessin_traque(sf::RenderTarget& targetWindow) const {
	auto traqSprite = buildSprite(position_entite,getRadius(), getAppTexture(getAppConfig().entity_texture_tracked));
					  targetWindow.draw(traqSprite);
}

void Animal::dessin_infection(sf::RenderTarget& target) const {
	auto infectSprite = buildSprite(position_entite + Vec2d(5,5), 35, getAppTexture(getAppConfig().virus_texture_infected)); 
    target.draw(infectSprite);
}



//========================== DEPLACEMENTS/FORCES =====================//



Vec2d Animal::randomWalk() {
	
	Vec2d random_vec;
	
	random_vec.x += uniform(-1.0,1.0);
	random_vec.y += uniform(-1.0,1.0);
	
	current_target += random_vec * getRandomWalkJitter();
	current_target = current_target.normalised()*getRandomWalkRadius();
	Vec2d moved_current_target = current_target + Vec2d(getRandomWalkDistance(), 0);
	
	return ConvertToGlobalCoord(moved_current_target);
}

Vec2d Animal::calcul_force() {
			
	Vec2d force;
	Vec2d v_target;
	Vec2d to_target(position_cible-position_entite);
	double speed;
	deceleration=STRONG;
	speed = min((to_target.length() / getDeceleration()), getMaxSpeed() );
	
	v_target = ( to_target/to_target.length()) * speed;
	force = v_target - getSpeedVector();

	return force;
}

void Animal::deplacement(sf::Time dt, const Vec2d& force) {
	
	Vec2d acceleration(force / getMass());
	new_vitesse = getSpeedVector() + (acceleration * dt.asSeconds());
    new_direction_pred = new_vitesse.normalised();
    
    if (new_vitesse.length() > getMaxSpeed() ) 
		new_vitesse = new_direction_pred * getMaxSpeed();
	
	new_position_pred = position_entite + (new_vitesse * dt.asSeconds());
	
	//SetPosition vérifie si l'animal sort de l'environnement et corrige sa position en conséquence
	//On passe l'argument par référence afin de changer la nouvelle posiiton de l'animal
	setPosition(new_position_pred);
	
	
	//Remet à jour les attributs (c'est pour ça que la méthode n'est pas const)
	vitesse = new_vitesse.length();
	position_entite= new_position_pred;
	direction_pred = new_direction_pred;
}

Vec2d Animal::ConvertToGlobalCoord(const Vec2d& local) const {
	
	sf::Transform matTransform;
	matTransform.translate(position_entite);
	matTransform.rotate(getRotation());
    Vec2d global = matTransform.transformPoint(local);
	
	return global;
}

/* 
 force_ralentissement: 
 On a rajouté dans l'énum Décélération un VERY STRONG, qui permet à une entité de ralentir très fortement
 Le calcul de la force est le même mais ça permet à l'entité de ralentir et de s'arrêter
 
*/

Vec2d Animal::force_ralentissement() {
	
	Vec2d force;
	Vec2d v_target;
	
	Vec2d to_target(position_cible  - getPosition());
	double speed;
	deceleration= VERY_STRONG;
	speed = min((to_target.length() / getDeceleration()), getMaxSpeed() );
	
	v_target = ( to_target/to_target.length()) * speed;
	
	force = v_target - getSpeedVector();
	
	return force;
}

Vec2d Animal::force_fuite() const {
	
	double coef1(500.0);
	double coef2(1.2);
	Vec2d force;
	Vec2d diff;
	
	
	for (auto const& entite: entites_ennemi) {
		diff = (entite->getPosition() - getPosition());
		force -= ((coef1 * diff) / pow((diff.length()),coef2));
	}

		return force;
}

Vec2d Animal::force_chef() const {
	
	Vec2d force;
	
	Vec2d to_target(entite_chef->getPosition() - getPosition());
	
	double speed(min((to_target.length() / getDeceleration()), getMaxSpeed()));
	
	force = ((to_target / to_target.length()) * speed);
	
	force -= getSpeedVector();
	
	return force;
}

/*
 isFreeMoving= l'entité est libre de ses mouvements si elle est chef ou si elle n'a pas de chef
*/
 
bool Animal::isFreeMoving() const {
	return (isChief() or not(hasChief()));
}


//========================== UPDATE ==================================//

/*
 
 Fonction update : 
 
 - Rentre dans Update State et actualise de ce fait son état
 - En fonction de son état, rentre dans le case du switch qui s'occupe de calculer la force correspondante à l'état de l'entite
 - Sort du switch
 - Si l'entité est enceinte, on commence à décrémenter son temps de gestation
 - Si elle est en FEEDING et que le temps de pause est fini, alors on gère son alimentation dans la méthode consommation
 - Si elle est en MATING et que son temps de pause est fini, on gère la reproduction dans meet
 - Si elle en GIVING BIRTH et que son temps de pause est fini, on gère l'accouchement dans MetAuMonde (donne naissance aux bébés !) 
 * ATTENTION: On a décidé que la femelle restait enceinte en s'arrêtant un moment puis donne naissance aux bébés après ce temps d'arrêt puis repart (l'accouchement se fait au repos)
 - Puis, enfin, on calcule à chaque update le déplacement de l'entité en fonction de la force calculé dans le switch et on met à jour l'énergie de l'animal
 
 */
 
 
void Animal::update(sf::Time dt) {

	Vec2d force(0.0,0.0);											
	
	updateState(dt);
	
	
	switch(state) {
		
		case FOOD_IN_SIGHT :
			
			if (not rassasie()) {
				force = calcul_force();
			}
			break;
			
			
		case WANDERING :
			
			force = randomWalk() - position_entite;
			force += getAppEnv().force_evitement(this);
			
			//DEPLACEMENT DU TROUPEAU QUI SUIT LE CHEF 
			
			if(entite_chef != nullptr)
				force += force_chef();
			break;
		
			
		case FEEDING:
			
			force = force_ralentissement();
			break;
			
			
		case MATE_IN_SIGHT:
		
			force = calcul_force();
			break;
			
			
		case MATING:
			
			force = force_ralentissement();
			break;
			
		
		case RUNNING_AWAY:
		
			force = force_fuite();
			break;
			
			
		case GIVING_BIRTH:
			
			force = force_ralentissement();
			break;
			
		case IDLE:
		
			force = force_ralentissement();
			break;
			
		default :
		
			break;
			
		}
		
		
	if(isEnceinte()) {
		gestationTime -= dt;
	}
		
		
	if (state == FEEDING) {
		
		if (horlogeFeeding.asSeconds() > 0.0) {
			horlogeFeeding -= dt;
		}
	
		else {
			consommation(dt, force);
			horlogeFeeding = sf::seconds(getAppConfig().animal_feed_time);
			updateState(dt);
		}	

	}
	
	if(state == MATING) {
		
		if (horlogeMating.asSeconds() > 0.0) {
			horlogeMating -= dt;
		}
		
		else {
			
			meet(entite_reprodutrice);
			entite_reprodutrice->meet(this);
			horlogeMating = sf::seconds(getAppConfig().animal_mating_time);
		}
	}
	
	
	if(state == GIVING_BIRTH) {
		
		if (horlogeDelivery.asSeconds() > 0.0) {
			horlogeDelivery -= dt;
		}
		
		else {
			
			MetAuMonde();
			gestationTime = getGestationTime();
			horlogeDelivery = sf::seconds(getAppConfig().animal_delivery_time);

	
		}
		
	}
	
	if (state == IDLE) {
		
		if (horlogeIdle.asSeconds() > 0.0) {
			horlogeIdle -= dt;
		}
	}
		
		
	deplacement(dt,force);
	perte_energy(dt);
	

	
	LivingEntity::update(dt);
	
	getImmuneSystem()->update(dt);
	
}

void Animal::updateState(sf::Time dt) {
	
	analyseEnvironment();
	
	
	if (not (entites_ennemi.empty())) {									//Gère en priorité si il y a un ennemi 
		state = RUNNING_AWAY;
	}
	
		else if (ConditionAccoucher()) {								//Gère ensuite si l'entité peut accoucher
			
			
			gestation = NOTENCEINTE;

			state = GIVING_BIRTH;

			
				if(TempsAvantAccouchement == false) {					//Met une fois à jour le temps de pause quand elle est giving birth pour la première fois
					TempsAvantAccouchement = true;
					horlogeDelivery = sf::seconds(getAppConfig().animal_delivery_time);
				}
	
		}
		
		else if (ConditionMateInSight())   {							
				
			state = MATE_IN_SIGHT;
			setTargetPosition(entite_reprodutrice->getPosition());
		

			if (isColliding(entite_reprodutrice)) {
					
				entite_reprodutrice->setPartner(this);			//Vérifie bien que on associe à l'entité reproductrice le bon partenaire
				entite_reprodutrice->setState(MATING);
				state = MATING;
					
				if (pause_mating == false) {
					horlogeMating = sf::seconds(getAppConfig().animal_mating_time);
					pause_mating = true;
				}
			}
		}
				
		
		else if(ConditionsFoodInSight()) {
				state = FOOD_IN_SIGHT;
				setTargetPosition(entite_mangeable->getPosition());
			
				if (isColliding(entite_mangeable)) {
					state = FEEDING;
				}
		}
		
		
		else if(not(rassasie()) and uniform(0.0, 1.0) < getAppConfig().animal_idle_probability) {
			state = IDLE;
			
				if(pause_idle == false) {
					pause_idle = true;
					horlogeIdle = sf::seconds(uniform(getAppConfig().animal_idle_time_min, getAppConfig().animal_idle_time_max));
				}

		}
			
			
		else {															//Si on rentre dans aucune des boucles, l'état est WANDERING

			state = WANDERING;
				
		}
}

void Animal::analyseEnvironment() {
	
	list<LivingEntity*> entite_visibles(getAppEnv().getEntitiesinSightforAnimal(this));
	double limite(getAppConfig().window_simulation_height);
	double tmp;

	entite_mangeable = nullptr;											//Remet à jour les pointeurs sur des entités à chaque dt puis re analyse l'environnement
	entite_reprodutrice= nullptr;
	entites_ennemi.clear();
	entite_chef = nullptr;



	for(auto const& entite : entite_visibles) {
		
		if (eatable(entite)) { 
			tmp = (getPosition() - entite -> getPosition()).length();	//Le tmp permet de prendre le plus proche à chaque fois
			
			if (tmp < limite) {
				
				limite = tmp;
				entite_mangeable = entite;

			}
		}
	
		if (reproduction_reciproque(entite)) {
			
			tmp = (getPosition() - entite -> getPosition()).length();
			if (tmp < limite) {
				
				limite = tmp;	
				entite_reprodutrice = (Animal*)entite;
			}
		
		}
	
		if (isEnemy(entite)) {
			entites_ennemi.push_back(entite);
		}
		
		if(hasChief()) {
			entite_chef = (getAppEnv().getChief(getidTroupeau(), this));
		}
	}
}		


//========================== REPRODUCTION ============================//

bool Animal::isTargetInSight(const Vec2d& pos) const {
	Vec2d d(pos - getPosition());
	
	//retourne vrai si norme(d)=0 ou (si la cible est suffisamment proche et si la cible et dans l'angle de vision)
	return(isEqual(d.lengthSquared(),0.0) or 
		  (d.lengthSquared() <= getViewDistance()*getViewDistance() and
		  (direction_pred.dot(d.normalised()) >= cos((getViewRange() + 0.001)/2.0))));
}

void Animal::meet(LivingEntity const* mate) {
		
		
		if (isFemale()) {
			
			if (gestation == NOTENCEINTE) {								//Si c'est une femme, elle tombe enceinte
			
				gestation = ENCEINTE;

			}
																		//On enregistre le genome du père du(des) futur(s) bébé(s)
																		//On est sûr que mate est un Animal => on le redefinit en Animal pour aller chercher son genome
			genome_mate = (new Genome(((Animal*)mate)->getGenome()));
			nb_bebes = calcul_nb_bebes();
			energie -= nb_bebes * getEnergieLossPerChild();
		}
			
		else {
			energie -= getEnergieLossMatingMale();
		}
			

	
}
		
bool Animal::isFemale() const {
	return (genome_animal.getSex() == FEMALE);
}

bool Animal::reproduction_reciproque(LivingEntity const* mate) const {
	return ((matable(mate)) and (mate -> matable(this)));
}

int Animal::calcul_nb_bebes() const {
	return uniform(getReproductionMinChildren(), getReproductionMaxChildren());
}

bool Animal::ConditionMateInSight() const {
	
	return ((entite_reprodutrice != nullptr) and ((sans_parents and entite_reprodutrice->getAge() > 3)  or ((horloge.asSeconds() > 3) and (entite_reprodutrice->getAge() > 3))) and 
			(!(this->isEnceinte()))
			and (!(entite_reprodutrice->isEnceinte())) 					//and entite_reprodutrice->getState() != MATING
			and ((entite_reprodutrice->getEntiteReproductrice() == this) or (entite_reprodutrice->getEntiteReproductrice() == nullptr))
		    and (horlogeDelivery == sf::Time::Zero) and state != GIVING_BIRTH and entite_reprodutrice->getState() != GIVING_BIRTH
		    and ((isTargetInSight(entite_reprodutrice->getPosition())) and entite_reprodutrice->isTargetInSight(this->getPosition())));			//On fait en sorte que les deux animaux veulent se reproduire que si ils se voient tous les 2 (plus logique)
		    
		    
	/*
	 Les conditions de MATING SIGHT sont les suivantes : 
	 - Entité existe réellement 
	 - L'entité est orphelin ou alors son horloge est supérieur à 5 secondes ce qui permet au bébé de ne pas se reproduire dès sa naissance
	 - Les deux partenaires ne sont pas déja en mating
	 - La femme n'eet pas enceinte
	 - Les deux animaux sont bien partenaires l'un à l'autre
	 - Les animaux ne sont pas en GIVING BIRTH
	 - Les animaux ne sont pas en collision
	 
	 */
}



bool Animal::isEnceinte() const {
	return (gestation == ENCEINTE);
}


bool Animal::ConditionAccoucher() const {	
	return ((gestationTime.asSeconds() <= 0.0));
}


void Animal::MetAuMonde() {		
	
	for(int i(0); i < nb_bebes; ++i) {
					give_birth(this);									//Met au monde le nombre de bébés créé aléatoirement plus haut 
				}
			
	nb_bebes = 0;
	
}

void Animal::infect(Virus* v) {
	getImmuneSystem()->setVirus(v);
}


//========================== NOURRITURE ==============================//


bool Animal::rassasie() const {
	return not((energie < getAppConfig().animal_satiety_min) or ((state == FEEDING) and (energie < getAppConfig().animal_satiety_max)));
}

void Animal::consommation(sf::Time dt, Vec2d& force) {
	
	
	if (pause_manger == false) {										//Met à jour pour la première fois l'horloge de feeding
		pause_manger = true;
		horlogeFeeding = sf::seconds(getAppConfig().animal_feed_time);
	}
	
	
		if (entite_mangeable != nullptr and (not rassasie())) {
			
			energie += (getAppConfig().animal_meal_retention * (entite_mangeable->getEnergie()));
			entite_mangeable->setEnergie((entite_mangeable->getEnergie()) - getBiteEnergie(entite_mangeable));
		}
	

	if ((horlogeFeeding < sf::Time::Zero) or (!(entites_ennemi.empty()))) {		//Remet à jour l'horloge feeding quand celle ci est descendu jusqu'à 0
		horlogeFeeding = sf::seconds(getAppConfig().animal_feed_time);
	}

}

bool Animal::ConditionsFoodInSight() const {
	return ((entite_mangeable != nullptr) and (not rassasie()));
}

void Animal::perte_energy(sf::Time dt) {
	double perte_;
	perte_ = getAppConfig().animal_base_energy_consumption + vitesse * getEnergieLossFactor()* dt.asSeconds();
	energie -= perte_;
}


//========================== FONCTIONS UTILITAIRES ===================//


bool Animal::isChief() const {

	if (idTroupeau >= 0)
		return (this == getAppEnv().getChief(getidTroupeau(), this));
	else 
		return false;													//retourne false si c'est un loup
}

bool Animal::hasChief() const {											//retourne toujours true car un mouton est toujours dans un troupeau,
																		//sauf si il est chef, ou on retourne false	ou que ce n'est pas un mouton	
	return (not(isChief()) and (idTroupeau >= 0));
	
}

bool Animal::isEnemy(LivingEntity const* mate) {
	return (mate->eatable(this));
}	

void Animal::infection(LivingEntity* entite) const {
	if(entite->getImmuneSystem() != nullptr and not(entite->getImmuneSystem())->isInfected()) {
		if(entite->infectableBy(this)) {
			Virus v(*(getImmuneSystem()->getVirus()));
			(entite->getImmuneSystem())->setVirus(new Virus(v));		//on infecte l'entité avec une copie du virus de l'instance courante
		}
	}
}


//========================== EXTENSIONS ==============================//

double Animal::getTaille() const {										//fonction permettant d'adapter la taille de l'image associée à un animal
																		//à son âge => une sorte de croissance
	if(horloge.asSeconds() < 1) {
		return getRadius();
	}
	
	else {
		double size(getRadius() + (horloge.asSeconds() - 1)*10);		//le facteur 10 est choisi arbitrairement, mais est cohérent, un mouton
																		//met 7 sec à retrouver sa taille max
		if(size < 2*getRadius()) {
			return size;
		}
		
		else {
			return 2*getRadius();										//on plafonne par la taille par défaut obtenue polymorphiquement dans le app.json
		}
	}
}
