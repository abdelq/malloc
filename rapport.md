--- 
title: TP 2 -- Rapport
author: Abdelhakim Qbaich
        - David Boivin 
date: \today
header-includes: \usepackage[french]{babel}

---
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








