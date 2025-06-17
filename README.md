# Morpion lumineux

## SAE 203

<p>
  Quentin T'JAMPENS <br>
  Giovanni JOSSERAND <br>
</p>

## Sommaire

<ol>
  <li> Prérequis </li>
  <li> Configurer le projet </li>
  <li> Utiliser le projet </li>
  <li> Annexe </li>

</ol>



### Prérequis

- Télécharger le code source via le [Gitlab officiel du projet](https://exemple.com)
- Installer [L'IDE Arduino](https://www.arduino.cc/en/software/)
- Avoir les composants suivants:
  - 1x Esp32
  - 8x Groove Mech Keycap
  - 1x Push Button
  - 1x Touch sensor 12 key capacitive
  - 1x OLED Display
  - 1x cable d'alimentation Esp32


### Configurer le projet

1. Télécharger la carte **esp32** By Espressif Systems **v2.0.17** via le boards manager de l'IDE Arduino
2. Installer les bibliothèques utilisées depuis l'IDE Arduino (Liste des bibliothèques en **Annexe**)
3. Brancher la carte Arduino puis compiler et téléverser le projet



### Utiliser le projet

- Une fois le code compilé et téléversé, selectionner le mode de jeu (Local/Web) via l'écran OLED et les boutons associés
- Mode Local:
  - Choisir si on veut jouer en solo ou duo. Si le mode solo est choisi, selectionner la difficulté du jeu, le joueur jouera ensuite contre un robot
  - Pendant une partie le joueur 1 et le joueur 2 joueront a tour de rôle (Si le mode sélectionné est Solo, le joueur 2 sera remplacée par le robot, et jouera instentanément après le joueur 1)
  - Une fois la partie terminée, tous les boutons excepté la ligne resposable de la victoire s'eteindront
  - Un bouton poussoir reset permet de reinitialiser la partie
- Mode Web:
  - connecter votre appareil (Téléphone, PC, ...) au réseau crée par l'esp32, le nom du réseau devra s'afficher sur l'écran OLED, le Mot de passe du réseau s'affiche également à l'écran. Une fois connecté au réseau, rendez-vous ensuite sur le site web du jeu via l'IP affichée à l'écran.
  - Une fois sur le site, choisir entre mode solo/duo. Le mode solo comporte 2 difficultés, Facile et Difficile
  
