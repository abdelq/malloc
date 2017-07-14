---
title: Rapport -- TP 2
author:
    - Abdelhakim Qbaich
    - David Boivin 
date: \today
header-includes:
    - \usepackage[french]{babel}
...

# Notre expérience

## Description du travail

Nous avions à implémenter une réplique approximative de la fonction
`malloc` dans la librarie standard du langage de programmation C. Ce
travail nous a permis d'en apprendre plus sur le fonctionnment de la
gestion de mémoire. À partir de la fonction `mmap` nous avons créé des
fonctions qui vont permettent aux utilisateurs d'allouer de la mémoire
pour leurs données et de libérer la mémoire efficacement. Tout au long
de ce rapport, nous allons décrire l'implémentations , les algorithmes
que nous avons choisi et la façon dont nous avons décidé de
travailler.

##  Difficultés rencontrées

Les difficultés que nous avons rencontrer durant le travail sont les
suivantes. La première était de comprendre comment l'appel de `mmap`
fonctionnait dans le contexte que nous voulions l'utiliser pour le
travail. Il nous fallait aussi trouver les bon `flags`. Nous l'avions
trouvé avant que la solution soit divulgué sur studium. La prochaine
difficulté concernait la façon dont nous faisions notre liste
chainé. Après avoir implémenter correctement notre malloc nous avons
décidé qu'à la place de parcourir tous les éléments pour trouver une
place libre, on pouvait seulement parcourir les éléments que nous
savons qui sont libre. Ainsi il fallait modifier notre liste chainé.
La dernière difficulté fut l'implémentation d'un `merge`et d'un
`split`... TODO

## La logique derrière nos implémentations

Quand nous avons implémenté notre travail notre première priorité fut
que nos fonctions fonctionnent avec les tests unitaires. Donc à ce
stade la fonction `mymalloc`et `myfree` fonctionnait correctement. Il
suffisait seulement de paufiner notre travail en le rendant plus
rapide et transformant notre code pour qu'il soient le plus élégant et
lisible possible. Pour les algorithmes et la fonçon dont notre code
fonction il suffit de voir la section documentation de ce rapport.

## Appréciation générale

Ce travail fût un bon défi pour notre apprentissage. Il nous a permis
de mieux comprendre comment on peut implémenter la gestion de mémoire
dans un langage de programmation. Ceci va nous être utile pour le
reste de carrière en informatique puisque c'est un éléments importants
de chaque langages de programmation. Nous étions très content d'avoir
réussit ce travail.

# Documentation

## Notre implémentation

Pour l'implémentation de notre `malloc` et `free` nous avons utilisé
une structure `block` contenant une liste chainée , la taille du block
et un pointeurs pour les `data` du programme qu'il
l'utilisera. Cependant notre liste chainée ne contiendra pas les
éléments qui sont utilisés , mais plutôt les blocs qui sont libres
puisque ils sont les seuls qu'on doit parcourir.

(TODO image liste chainée)

Nous avons aussi fait une fonction `extend` qui s'occupe d'allouer la
mémoire et de retourner cette plage à `malloc`. Nous allouons 4 Ko de
mémoire pour éviter au maximum de faire des appels à `mmap`. C'est
aussi le minimum que l'on peut alouer avec `mmap`. Nous avons essayer
d'allouer moins que cela , mais `mmap` utilisait quand même 4 Ko.

## La fonction `malloc`

Cette fonction sert à allouer de l'espace, mais aussi à réutiliser les
block qui ont été libérer auparavant. Elle prend comme argument la
taille de l'espace mémoire que nous voulons.  Pour ce faire nous
utilisons notre liste chainée. Puisque que tous les éléments de cette
liste sont libres alors ils suffit de regarder si leurs tailles sont
plus petits que celui qu'on désire allouer. Si c'est le cas on peut
alors allouer cet espace avec `extend`. Ensuite quand nous avons notre
`block` mémoire il suffit de retourner le pointeur vers les
données. Biensur si on ne trouve pas de `block` de mémoires alors on
alloue un `block` de 4 Ko qui aura comme premier élément l'élément
demander et le reste sera un `block` libre pour de nouvelle
allocation. Quand l'éléments sera alloué à un `block` libre plus grand
que l'espace demandé alors on effectuera un `split`. Le `block` sera
alors diviser selon la taille demandé. Il faut ensuite simplement
enlever le `block` que nous venons d'allouer de la liste chainée. 

(TODO image split 
      image malloc)


## La fonction `free`

La fonction `free` va effectuer le travail de réarranger la liste
chainée et d'ajouter les éléments à cette liste. La fonction `free`
prend en paramètre un pointeur. Ce pointeur est l'emplacement mémoire
que l'on veut libérer. Au fur et à mesure que les éléments se
libéront, `free` rajoutera les éléments à la liste chainée. Il les
rajoutera de façon à optimiser les appels à `merge`. Pour ce faire on
rajoute les `block` en ordre croissant d'adresse. Ainsi il suffira de
comparer les adresses des éléments deux à deux et d'effectuer un
`merge` si les adresses sont contigu. Si la liste d'éléments libre est
vide alors on met le nouvel emplacement libre au début . Sinon on
parcours chaque éléments de la liste pour trouver où le mettre selon
l'ordre à laquelle sont placé les éléments en mémoire.

## Notre algorithme

Notre algoritme consite à effectuer le moins d'opération pour trouver
la mémoire et de réutiliser le plus de mémoire possible pour que nous
malloc soit efficace. Premierement nous avons utilisé une liste
chainée simple pour entreposer les informations concernant les `block`
mémoires. Cette liste sera la seule structure de données que nous
utiliseront pour implémenter notre `malloc`.  Notre liste contient
seulement des `block` libre , donc quand nous effectuons un recherche
pour un emplacement susceptible de convenir à notre `malloc`, nous ne
sommes pas obligé de passer sur tout les éléments déjà alloué. Pour
optimiser la mémoire , les fonctions `merge` et `split` servent à
manipuler les `block` mémoire pour optimiser la réutilisation de
block. Puisque `free` place les éléments en ordre croissant d'adresse,
nous pouvons alors comparer ces éléments deux à deux et effectuer un
`merge`. Pour perde le moins de mémoire possible il nous a fallu
implémenter un `split` qui prendra un `block` et le divisera pour
seulement contenir la mémoir nécessaire à l'utilisation.

### Force et faiblesse


### Alignement des adresses


### mmap 


### 


### Cas mal gérés





