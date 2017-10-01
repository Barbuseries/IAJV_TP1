Membres de l'équipe:
  - XXX Florian (XXX)
  - Guison Vianney (GUIV30069402)
  - Edorh François (EDOF19059507)

Ajouts:
  - Affichage graphique avec SDL2
  - Multi-threading (entités, messages, affichage)
  - *Barman:
    - Sert les verres
    - Arbitre les matchs
  - *Drunkard:
    - Consomme de la bière sans modération
    - Essaye de se battre lorsqu'il est ivre
  - Miner:
    - Peut se battre si il est provoqué
  - BaseGameEntity:
    - Ajout de "location" et des méthodes associées
  - EntityManager:
    - Ajout d'une méthode pour récupérer les entités dans une "location"
  - MessageDispatcher:
    - Ajout d'une méthode pour envoyer un message à toutes les entités dans une "location"

*: Nouvelle entité

NOTE:
- Les images (screenshot et machines d'états) sont dans le dossier 'img'.
- Le projet a été configuré avec la hierarchie suivante en tête:
  |-> Common
  |-> <Project>
   |-> <projet>.sln
- Visual Studio 2017 a été utilisé
