// l lemarchand
// c 2018
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
                        distance[p1][p2] = opl.sqrt(dx*dx + dy*dy);
                }
}

tuple tantenne {
	float portee;
	int cout;
};

{ Tantenne } antennes = ...; // l'ensemble des antennes

int visibilite[points][points] = ...;

// variables utiles

// Ce sont les variables temporaires qui stockent la meilleure antenne pour l'itération donnée de l'heuristique
Tantenne tmpMeilleurAntenne;
float tmpMeilleurRatioAntenne = 0.0;
float currentRatioAntenne;
Tpoint tmpMeilleurPoint;

{Tpoint} nonCouverts; // points non encore couverts
{Tpoint} tmpCouverts; // sauvegarde des points couverts par une antenne dans une iteration
{Tpoint} bestCouverts; // meilleure ens couvert courant
int select[antennes][points]; // solution
float cout = 0.0;

// Heuristique
execute {
	// Phase Init de l'heuristique (voir sujet)
	for(var p in points) {
		nonCouverts.add(p);
	}

	// On cherche jusqu'a que tout soit couvert
	while(count(nonCouverts) > 0) {
		// On regarde pour tous les points (itération) pour décider quel sera la prochaine antenne qui sera placée
		for(var pa in nonCouverts) {
			// Pour chaque points, on regarde quel est la meilleure antenne a positionner
			for (var a in antennes) {
				// Pour chaque antenne, on regarde quel points sont couverts
				for (var p in nonCouverts) {
					if (distances[pa][p] <= a.portee && visibilite[pa][p] == 1) {
						tmpCouverts.add(p);
					}
				}
				// Calcul ratio
				currentRatioAntenne = count(tmpCouverts) / a.cout;
				if (tmpMeilleurRatioAntenne < currentRatioAntenne) {
					tmpMeilleurRatioAntenne = currentRatioAntenne;
					tmpMeilleurAntenne = a;
					tmpMeilleurPoint = pa;
				}

				for (var p in nonCouverts) {
					tmpCouverts.clear();
				}
			}
		}
		// On a la meilleur antenne de l'iteration 
		// Il faut enlever les points couverts de nonCouverts️
		// Il faut garder en memoire ou est l'antenne choisi, et quelle est cette antenne (via select)
		for (var a in antennes) {
			for (var p in nonCouverts) {
				if (distances[pa][p] <= a.portee && visibilite[pa][p] == 1) {
					nonCouverts.remove(p);
				}
			}
		}
		select[tmpMeilleurAntenne][tmpMeilleurPoint] = 1;
	}
}

// sortie du resultat
execute {
	var sortie = "couv2.res";
        var ofile = new IloOplOutputFile(sortie);
        ofile.writeln(cout);
	for(var a in antennes)
		for (var pa in points)
			if (select[a][pa] == 1)
				ofile.writeln(a.portee, ", ", pa.x, " ", pa.y);
        ofile.close();
        writeln("trace dans ", sortie);
}

// Il est possible d'�viter l'appel au solver en �crivant un main() � la place du dernier execute -- cf exemples OPL
