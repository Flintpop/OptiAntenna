/*********************************************
 * OPL 12.7.1.0 Model
 * Author: Tanguy
 * Creation Date: 22 févr. 2023 at 18:59:28
 *********************************************/
// Tanguy Le Goff & Soukayna Atarras
// 2023

tuple tpoint {
	int x;
	int y;
};

{ tpoint } points = ...;

float distance[points][points];
execute {
// calcul des distances
        for (var p1 in points)
                for (var p2 in points) {
                        var dx = p1.x - p2.x;
                        var dy = p1.y - p2.y;
                        distance[p1][p2] = Opl.sqrt(dx*dx + dy*dy);
                }
}

tuple tantenne {
	float portee;
	int cout;
};

{ tantenne } antennes = ...; // l'ensemble des antennes

int visibilite[points][points] = ...;

// Variables utiles

// Ce sont les variables temporaires qui stockent la meilleure antenne pour l'itération donnée de l'heuristique
tantenne tmpMeilleurAntenne;
float tmpMeilleurRatioAntenne = 0.0;
float currentRatioAntenne;
tpoint tmpMeilleurPoint;

// Variables de debug
int n_fails = 0;
int n_points_a_couvrir;

// Variables d'heuristique
{tpoint} nonCouverts; // points non encore couverts
{tpoint} tmpCouverts; // sauvegarde des points couverts par une antenne dans une iteration
{tpoint} bestCouverts; // meilleure ens couvert courant
int select[antennes][points]; // solution
float cout = 0.0;

// Heuristique
execute {
	// Phase Init de l'heuristique (voir sujet)
	nonCouverts.importSet(points);

	// On cherche jusqu'a que tout soit couvert, ou erreur et impossibilité de couvrir un point en 2 itérations
		while(nonCouverts.size > 0 && n_fails < 2) {
			//writeln("La size de nonCouverts : ", nonCouverts.size);		
		
			// On regarde pour tous les points (itération) pour décider quel sera la prochaine antenne qui sera placée
			for(var pa in points) {
				// Pour chaque points, on regarde quel est la meilleure antenne à positionner
				for (var a in antennes) {
					// Pour chaque antenne, on regarde quels points sont couverts
					for (var p in nonCouverts) {					
						if (distance[pa][p] <= a.portee && visibilite[pa][p] == 1) {
							//writeln("Nous sommes au point : p : ", p, " et pa ", pa," avec distance[pa][p] ", distance[pa][p], " et visibilite[pa][p] == ", visibilite[pa][p]);						
						
							tmpCouverts.add(p);
						}
					}
					//writeln("Nous sommes au point : pa : ", pa);
					
					// Calcul ratio
					currentRatioAntenne = tmpCouverts.size / a.cout;
					
					if (tmpMeilleurRatioAntenne < currentRatioAntenne) {
						bestCouverts.clear();
						
						tmpMeilleurRatioAntenne = currentRatioAntenne;
						tmpMeilleurAntenne.cout = a.cout;
						tmpMeilleurAntenne.portee = a.portee;
						tmpMeilleurPoint.x = pa.x;
						tmpMeilleurPoint.y = pa.y;
												
						bestCouverts.importSet(tmpCouverts);
					} 
					tmpCouverts.clear();
				}
			}
		
		// On a la meilleur antenne de l'iteration
		// Il faut enlever les points couverts de nonCouverts 
		// Il faut garder en memoire ou est l'antenne choisi, et quelle est cette antenne (via select)
		//writeln("Choix antenne : ", tmpMeilleurAntenne, " au point : ", tmpMeilleurPoint, " Pour ratio : ", tmpMeilleurRatioAntenne);
		
		// Si on a déjà placé une antenne à cet endroit, c'est que l'algo n'a pas réussi à placer une antenne pour couvrir un point supplémentaire.
		if (select[tmpMeilleurAntenne][tmpMeilleurPoint] == 1) {
			n_fails = n_fails + 1;		
		} else {
			// Pas d'erreur, on poursuit et set/reset des variables d'heuristique		
		
			n_fails = 0;
			select[tmpMeilleurAntenne][tmpMeilleurPoint] = 1;
			
			// On enlève les points désormais couvert par une antenne		
			for (var p in bestCouverts) {
				nonCouverts.remove(p);			
			}
			bestCouverts.clear();
			
			// Debug
			//writeln();
			//writeln("Il reste ces points à couvrir : ");
			//for (var p in nonCouverts) {
				//writeln(p);
			//}
			
			// Debug 
			n_points_a_couvrir = nonCouverts.size;		
		}
		
		tmpMeilleurRatioAntenne = 0.0;
	}
}

// sortie du resultat
execute {
	// Somme du cout des antennes
	for(var a in antennes)
			for (var pa in points)
				if (select[a][pa] == 1)
					cout = cout + a.cout;
					
	var sortie = "couv2.res";
        var ofile = new IloOplOutputFile(sortie);
        ofile.writeln("Le cout total est : ", cout);
	for(var a in antennes)
		for (var pa in points)
			if (select[a][pa] == 1)
				ofile.writeln("Antenne avec portee : ", a.portee, ", et coordonnees : ", pa.x, " ", pa.y);
        writeln("trace dans ", sortie);
        
     // Si l'heuristique a échoué
     if (n_fails >= 2) {
     	// Debug      
     
     	ofile.writeln();     
     
     	ofile.writeln("L'opération a échoué. Le programme n'a pas réussi, n'a pas pu couvrir tous les points. Il y a eu : ", n_fails, " fails d'affilé'.");
     	ofile.writeln("Il reste ", n_points_a_couvrir, " points a couvrir.");
     	
     	ofile.writeln();
		ofile.writeln("Ce sont ces points : ");
		for (var p in nonCouverts) {
			ofile.writeln(p);
		}     
     }
     
     ofile.close();
}