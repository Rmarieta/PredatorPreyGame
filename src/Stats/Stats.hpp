#ifndef STATS_HPP
#define STATS_HPP

#include <Interface/Drawable.hpp>
#include <Interface/Updatable.hpp>
#include <Stats/Graph.hpp>
#include <SFML/System.hpp>
#include <string>
#include <unordered_map>

class Stats {
   
   public:
   
//======================== CONSTRUCTEURS/DESTRUCTEURS ================//
	
	Stats(int unIndex = -1);

//========================= GETTEURS/SETTEURS ========================//
	
	void setActive(int i);
	int getActive() const;
	sf::Time getTimer() const;
	void setTimer(sf::Time newTime);
	
//========================== UPDATE ==================================//
	
	void update(sf::Time);
	
//======================== DRAW ======================================//

	void drawOn(sf::RenderTarget&) const;

//======================== FONCTIONS UTILITAIRES =====================//
	
	void reset();
	void addGraph(int index, std::string libelle, std::vector<std::string> const& titles, double min, double max, const Vec2d size);

	void focusOn(std::string titre_du_graphe);
  
//========================== ATTRIBUTS ===============================//
 
   private:
   
   std::unordered_map<int, std::unique_ptr<Graph>> graphs ;
   std::unordered_map<int, std::string> libelles;
							
   int identifiant_actif;
   sf::Time timer;
   
   
   
};

#endif



