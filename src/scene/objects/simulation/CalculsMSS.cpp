/*
 * CalculsMSS.cpp :
 * Copyright (C) 2016 Florence Zara, LIRIS
 *               florence.zara@liris.univ-lyon1.fr
 *               http://liris.cnrs.fr/florence.zara/
 *
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published
 * by the Free Software Foundation; either version 2.1 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

/** \file CalculsMSS.cpp
Programme calculant pour chaque particule i d un MSS son etat au pas de temps suivant 
 (methode d 'Euler semi-implicite) : principales fonctions de calculs.
\brief Fonctions de calculs de la methode semi-implicite sur un systeme masses-ressorts.
*/ 

#include <math.h>
#include <glm/fwd.hpp>
#include <glm/geometric.hpp>
#include <iostream>


// #include "ObjetSimule.h"
#include "ObjetSimuleMSS.h"
#include "scene/objects/simulation/MSS.h"
// #include "Viewer.h"

using namespace std;




namespace TTe {


/**
* Calcul des forces appliquees sur les particules du systeme masses-ressorts.
 */
void ObjetSimuleMSS::CalculForceSpring()
{	
	int i = 0;
	for (auto & particule : _SystemeMasseRessort->GetPartList()){
		glm::vec3 Fe(0.0, 0.0, 0.0);
		glm::vec3 Fv(0.0, 0.0, 0.0);
		for(auto & ressort : particule->GetRessortList()){
			
			Particule *other = ressort->GetOtherParticule(particule);

			glm::vec3 direction = mesh.verticies[other->GetId()].pos - mesh.verticies[particule->GetId()].pos;
			glm::vec3 direction_norm = glm::normalize(direction);

			Fe += ressort->GetRaideur() * (glm::length(direction) - (ressort->GetLrepos() / 2.0f)) * direction_norm;
			Fv += ressort->GetAmortissement() * glm::dot((V[particule->GetId()]  -V[other->GetId()] ), direction_norm) * direction_norm;

			
		}
		// std::cout << glm::length(Fe + Fv) << "\n";
		this->Force[i] += Fe + Fv;
		i++;
	}
	
	/// f = somme_i (ki * (l(i,j)-l_0(i,j)) * uij ) + (nuij * (vi - vj) * uij) + (m*g) + force_ext
	
	/// Rq : Les forces dues a la gravite et au vent sont ajoutees lors du calcul de l acceleration
    
		
}//void


/**
 * Gestion des collisions avec le sol.
 */
void ObjetSimuleMSS::Collision()
{
    /// Arret de la vitesse quand touche le plan
   
    
}// void

}