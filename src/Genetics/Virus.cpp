#include <Application.hpp>
#include "Virus.hpp"
#include <Random/Random.hpp>

using namespace std;



//======================== CONSTRUCTEURS/DESTRUCTEURS ================//

Virus::Virus()
	: qte(getAppConfig().virus_initial_quantity), taux_de_croissance(getAppConfig().virus_growth_rate)
{
	for(auto& entree : profil) {
		if(bernoulli(getAppConfig().virus_sparsity))
			entree = 0.0;
		else
			entree = uniform(-5.0, 5.0);
	}
}

Virus::Virus(Profil p)
	: profil(p), qte(getAppConfig().virus_initial_quantity), taux_de_croissance(getAppConfig().virus_growth_rate)
{}

Virus::Virus(Virus const& other)
	: profil(other.getProfil()), qte(getAppConfig().virus_initial_quantity), taux_de_croissance(other.getTaux())
{}

//======================== GETTEURS/SETTEURS =========================//

Profil Virus::getProfil() const {
	return profil;
}

double Virus::getQte() const {
	return qte;
}

void Virus::setQte(double q) {
	qte = q;
}

double Virus::getTaux() const {
	return taux_de_croissance;
}

double Virus::getVirul(size_t id) const {
	return getProfil()[id];
}

//======================== UPDATE ====================================//

void Virus::update(sf::Time dt) {
	qte *= (1 + taux_de_croissance*(1 - (qte/getAppConfig().virus_max))*dt.asSeconds());
}

//======================== FONCTIONS UTILITAIRES =====================//

bool Virus::isDead() const {
	return (getQte() <=  getAppConfig().virus_initial_quantity);
}

//======================== SURCHARGES OPERATEURS =====================//

ostream& operator<<(ostream& sortie, const Virus& virus) {
	
	sortie << "Virus ," << endl;
	
	sortie << "Profil de virulence : ";
	
	for(auto val: virus.getProfil()) {
		sortie << val << " ";
	}
	
	sortie << endl << "Qté agents infectieux : " << virus.getQte() << endl;
	
	sortie << "Taux de croissance : " << virus.getTaux() << endl;
	
	return sortie;
}



