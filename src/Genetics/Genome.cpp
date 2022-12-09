#include "Genome.hpp"
#include <Random/Random.hpp>
#include <iostream>

using namespace std;

//======================== CONSTRUCTEURS/DESTRUCTEURS ================//

Genome::Genome(Genome* genome_mere, Genome* genome_pere) {
	
	if ((genome_mere != nullptr) and (genome_pere != nullptr)) {
		
			if (bernoulli(0.5)) 
				sex = Sex::MALE;
			else 
				sex = Sex::FEMALE;
	
			if (bernoulli(0.5))
				couleur[0] = genome_mere->couleur[0];
			else 
				couleur[0] = genome_mere->couleur[1];
				
			if (bernoulli(0.5))
				couleur[1] = genome_pere->couleur[0];
			else 
				couleur[1] = genome_pere->couleur[1];

	
	
			for(size_t i(0); i < profil_immun.size(); i++) {
		
				double aleatoire(uniform(-getAppConfig().genome_black_allelle_prevalence, getAppConfig().genome_black_allelle_prevalence));
			if(bernoulli(0.5)) 
				profil_immun[i] = ((*genome_mere).profil_immun[i] + aleatoire);
			else 
			profil_immun[i] = ((*genome_pere).profil_immun[i] + aleatoire);
		
			}
	}
	
	else {
		
			if (bernoulli(0.5)) 
				sex = Sex::MALE;
			else 
				sex = Sex::FEMALE;
		
			if (bernoulli(getAppConfig().genome_black_allelle_prevalence)) 
				couleur[0] = ColorAllele::BLACK;
			else 
				couleur [0] = ColorAllele::WHITE;
	
		
			if (bernoulli(getAppConfig().genome_black_allelle_prevalence)) //black allele prevalence
				couleur[1] = ColorAllele::BLACK;
			else 
				couleur [1] = ColorAllele::WHITE;
	
	
			for(size_t i(0); i < profil_immun.size(); i++) {
				double aleatoire(uniform(-5.0, 5.0));
				profil_immun[i] = aleatoire;
	
			}
	}
}

//========================= GETTEURS/SETTEURS ========================//

Sex Genome::getSex() const {
	return sex;
}

void Genome::setProfilImmun(Profil p) {
	profil_immun = p;
}


ColorAllele Genome::getFirstColorAllele() const {
	return couleur[0];
}

ColorAllele Genome::getSecondColorAllele() const {
	return couleur[1];
}

ColorAllele Genome::getColorPhenotype() const {
	if (couleur[0]==couleur[1] and couleur[0]== ColorAllele::BLACK)
		return ColorAllele::BLACK;
	else 
		return ColorAllele::WHITE;
}

double Genome::getImmuneGenes(size_t idx) const {
	return profil_immun[idx];
}


//======================== FONCTIONS UTILITAIRES =====================//


void Genome::afficher(ostream& sortie) const {
	for(auto val: profil_immun) {
		sortie << val << " ";
	}
}


//========================== SURCHARGES OPERATEURS ===================//

ostream& operator<<(ostream& sortie, const Genome& genome) {
	
	
	sortie << "Genome ( ";
	
	switch (genome.getSex()) {
		case Sex::MALE :
			sortie << "male) :" << endl;
			break;
		case Sex::FEMALE :
			sortie << "female) :" << endl;
			break;
	}

	sortie << "Color phenotype = ";
	
	switch (genome.getColorPhenotype()) {
		case ColorAllele::BLACK :
			sortie << "black" << endl;
			break;
		case ColorAllele::WHITE :
			sortie << "white" << endl;
			break;
	}
	
	
	sortie << "First color allele = ";
	
	switch (genome.getFirstColorAllele()) {
		case ColorAllele::BLACK : 
			sortie << "black" << endl;
			break;
		case ColorAllele::WHITE :
			sortie << "white" << endl;
			break;
	}
		
	sortie << "Second color allele = ";
	
	switch (genome.getSecondColorAllele()) {
		case ColorAllele::BLACK : 
			sortie << "black" << endl;
			break;
		case ColorAllele::WHITE :
			sortie << "white" << endl;
			break;
	}
		
	sortie << "Immune profile = ";
	genome.afficher(sortie);
	sortie << endl;

	return sortie;
}
	


		
		
