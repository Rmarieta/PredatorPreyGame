#include <Application.hpp>
#include <Stats/Stats.hpp>
#include <iostream>
#include <Utility/Constants.hpp>
#include <memory>
#include <vector>


using namespace std;


//======================== CONSTRUCTEURS/DESTRUCTEURS ================//


Stats::Stats(int unIndex)
 : identifiant_actif(unIndex)
{
	reset();															//réinitialise les graphes
}


//========================= GETTEURS/SETTEURS ========================//


void Stats::setActive(int i) {
	identifiant_actif = i;
}

int Stats::getActive() const {
	return identifiant_actif;
}

sf::Time Stats::getTimer() const {
	return timer;
}

void Stats::setTimer(sf::Time newTime) {
	timer = newTime;
}


//========================== UPDATE ==================================//


void Stats::update(sf::Time dt) {										//met à jour le graphe associé à l'identifiant actif
	
	graphs.at(identifiant_actif)->updateData(dt, getAppEnv().fetchData(libelles.at(identifiant_actif)));		

}


//======================== DRAW ======================================//


void Stats::drawOn(sf::RenderTarget& targetWindow) const {
	graphs.at(identifiant_actif)->drawOn(targetWindow);
}


//======================== FONCTIONS UTILITAIRES =====================//


void Stats::reset() {
	
	for (auto& val: graphs) {
		
		val.second->reset();
	}
}

void Stats::addGraph(int index, string libelle, vector<string> const& titles, double min, double max, const Vec2d size) {
	
//Si la map n'est pas vide => on utilise la méthode reset de unique_ptr
	if(graphs.find(index) != graphs.end() and identifiant_actif != -1) {
		graphs[index].reset(new Graph(titles, size, min, max));
	}
	
	else {
//Sinon, on insère une nouvelle paire dans la map avec un nouveau unique_ptr
	graphs.insert(std::make_pair(index, unique_ptr<Graph>(new Graph(titles, size, min, max))));
	libelles.insert(make_pair(index, libelle));
	
	}

	setActive(index);
	setTimer(sf::Time::Zero);
}

void Stats::focusOn(std::string titre_du_graphe) {						//Rend actif le graphe associé au libellé passé en paramètre
	
	for(auto& titre : libelles) {
		
		if(titre.second == titre_du_graphe) {
			setActive(titre.first);
			graphs[titre.first].get()->reset();
		}
	}
	
}

