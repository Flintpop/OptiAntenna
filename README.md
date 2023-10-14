# Projet Algorithmique Avancé : Optimisation du placement d'antennes réseau

## Contexte

Ce petit projet académique compare deux méthodologies algorithmiques — une heuristique et une approche par énumération exhaustive — pour résoudre un problème d'optimisation de placement d'antennes réseau. 
L'objectif est de minimiser le coût global tout en respectant des contraintes géographiques et de visibilité entre les points.

## Prérequis

- Langage C
- IBM ILOG CPLEX Optimization Studio pour le fichier `.mod`

## Installation

```bash
git clone [lien_du_repo]
cd [nom_du_dossier]
```

## Génération des données

Un fichier en C (`genPoints.c`) génère des données de test artificielles sous forme de matrice. 
Le programme prend également en compte des éléments géographiques, comme des montagnes ou des obstacles, pour déterminer si un point est visible d'un autre.

## Modèle d'optimisation (.mod)

Un modèle d'optimisation est fourni dans le fichier `.mod`, utilisant IBM ILOG CPLEX Optimization Studio, pour déterminer la solution optimale du problème.

## Utilisation

Pour générer des données :

```bash
./genPoints
```

Pour exécuter l'optimiseur :

```bash
[opl_command] [fichier .mod]
```

Pour faire les tests automatisés : 

```bash
.\doTestsCouverture.bsh
```

Le nombre de points par test et le nombre de test par méthode de résolution est écrit dans le code.

## Résultats et limitations

Les résultats actuels indiquent que l'heuristique nécessite des ajustements, notamment pour réduire sa complexité algorithmique. 
Les détails des performances et des limitations peuvent être trouvés dans le fichier `"Rapport projet algorithmie avancé.pdf"`
