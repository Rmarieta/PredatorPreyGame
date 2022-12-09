#include <SFML/Graphics.hpp>
#include <iostream>
#include <array>
#include <cmath>
#include <sstream>
#include "Application.hpp"

#ifndef GENOME_H
#define GENOME_H

typedef std::array<double, 10> Profil;

enum Sex {MALE,FEMALE};
enum ColorAllele {WHITE,BLACK};

class Genome {
	
	public:
	
//======================== CONSTRUCTEURS/DESTRUCTEURS ================//

	Genome (Genome* genome_mere=nullptr, Genome* genome_pere=nullptr);
		
		
//========================= GETTEURS/SETTEURS ========================//

	Sex getSex() const;
	ColorAllele getFirstColorAllele() const;
	ColorAllele getSecondColorAllele() const;
	ColorAllele getColorPhenotype() const;
	double getImmuneGenes(size_t idx) const;
	void setProfilImmun(Profil p);
	
	
//======================== FONCTIONS UTILITAIRES =====================//

	void afficher(std::ostream& sortie) const;
	
	
//========================== ATTRIBUTS ===============================//

	private:
	
	Sex sex;
	std::array<ColorAllele,2> couleur;
	Profil profil_immun;
	Profil profil_virus;
};	

//========================== SURCHARGES OPERATEURS ===================//

std::ostream& operator<<(std::ostream& sortie, const Genome& genome);



#endif

