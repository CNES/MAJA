introduction
============

objet du document
-----------------

L'objet de ce document est de présenter la conception globale de MAJA.
MAJA s’appuie sur la chaîne L2 développées. Pour éviter la
redite, le document de conception globale de la chaine chaînes L2
[DA100]_ s’applique à ce document. Seuls les points de conception
spécifiques à MAJA sont alors présentés et justifiés dans ce document.
Le document [SDM]_ (Software Developer Manual) vient compléter la
description faite dans ce document. En outre, les aspects purement
informatique tels que l’organisation de l’arborescence, la constitution
des plugins, etc…. sont décrits dans le [SDM]_.

structure du document
---------------------

Ce document s’articule autour des grands chapitres suivants :

-  Le chapitre :ref:`exigences:rappel des principales exigences et contraintes` présente les principales contraintes et exigences de MAJA,

-  Le chapitre :ref:`presentation:présentation générale de maja` décrit dans sa globalité l’architecture et les interfaces retenues pour MAJA,

-  Le chapitre :ref:`algorithms:les algorithmes` décrit les points de conception de
   MAJA pour la partie purement algorithmique de MAJA,

-  Le chapitre :ref:`plugins:les plugins « capteur »` contient les points de
   conception de MAJA pour la partie purement plugin.
