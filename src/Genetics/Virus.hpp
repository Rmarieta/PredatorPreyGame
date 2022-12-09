#include <array>
#include <SFML/Graphics.hpp>


#ifndef VIRUS_H
#define VIRUS_H

typedef std::array<double, 10> Profil;

class Virus {

	public:

//======================== CONSTRUCTEURS/DESTRUCTEURS ================//
	
	Virus();
	Virus(Profil p);
	Virus(Virus const& other);
	~Virus() {}
	
//========================= GETTEURS/SETTEURS ========================//
	
	Profil getProfil() const;
	double getQte() const;
	void setQte(double q);
	double getTaux() const;
	double getVirul(size_t id) const;
	
//========================== UPDATE ==================================//
	
	void update(sf::Time dt);

//========================== FONCTIONS UTILITAIRES ===================//

	bool isDead() const;


//========================== ATTRIBUTS ===============================//
	
	private:
	
	Profil profil;
	double qte;
	double taux_de_croissance;
	
};

//========================== SURCHARGES OPERATEURS ===================//

std::ostream& operator<<(std::ostream& sortie, const Virus& virus);


#endif
