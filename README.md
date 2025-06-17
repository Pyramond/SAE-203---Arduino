# Morpion lumineux

## SAE 203

Quentin T'JAMPENS
Giovanni JOSSERAND

## Sommaire

1. Prérequis
2. Configurer le projet
3. Utiliser le projet
4. Annexe

### Prérequis

- Télécharger le code source via le [Gitlab officiel du projet](https://exemple.com)
- Installer [l'IDE Arduino](https://www.arduino.cc/en/software/)
- Avoir les composants suivants :
  - 1x Esp32
  - 8x Groove Mech Keycap
  - 1x Push Button
  - 1x Touch sensor 12 key capacitive
  - 1x OLED Display
  - 1x câble d'alimentation Esp32

### Configurer le projet

1. Télécharger la carte **esp32** By Espressif Systems **v2.0.17** via le boards manager de l'IDE Arduino
2. Installer les bibliothèques utilisées depuis l'IDE Arduino (Liste des bibliothèques en **Annexe**)
3. Brancher la carte Arduino puis compiler et téléverser le projet

### Utiliser le projet

- Une fois le code compilé et téléversé, sélectionner le mode de jeu (Local/Web) via l'écran OLED et les boutons associés
- Mode Local :
  - Choisir si on veut jouer en solo ou duo. Si le mode solo est choisi, sélectionner la difficulté du jeu, le joueur jouera ensuite contre un robot
  - Pendant une partie le joueur 1 et le joueur 2 joueront à tour de rôle (Si le mode sélectionné est Solo, le joueur 2 sera remplacé par le robot, et jouera instantanément après le joueur 1)
  - Une fois la partie terminée, tous les boutons excepté la ligne responsable de la victoire s'éteindront
  - Un bouton poussoir reset permet de réinitialiser la partie
- Mode Web :
  - Connecter votre appareil (téléphone, PC, ...) au réseau créé par l'esp32, le nom du réseau devra s'afficher sur l'écran OLED, le mot de passe du réseau s'affiche également à l'écran. Une fois connecté au réseau, rendez-vous ensuite sur le site web du jeu via l'IP affichée à l'écran.
  - Une fois sur le site, choisir entre mode solo/duo. Le mode solo comporte 2 difficultés, Facile et Difficile


### Annexe

#### Liens utiles
- [Gitlab du projet](https://exemple.com)
- [Arduino](https://www.arduino.cc/)
- [IDE Arduino](https://www.arduino.cc/en/software/)
- [Tutoriel Vidéo](https://exemple.com)

### Photos

<img src="https://iutbg-gitlab.iutbourg.univ-lyon1.fr/SAE_203_2025/grA_21_Morpion_lumineaux/gra_21_morpion_lumineaux_images/-/raw/main/ledsOff.jpg?ref_type=heads" height=200>
<img src="https://iutbg-gitlab.iutbourg.univ-lyon1.fr/SAE_203_2025/grA_21_Morpion_lumineaux/gra_21_morpion_lumineaux_images/-/raw/main/ledsOn.jpg?ref_type=heads" height=200>
<img src="https://iutbg-gitlab.iutbourg.univ-lyon1.fr/SAE_203_2025/grA_21_Morpion_lumineaux/gra_21_morpion_lumineaux_images/-/raw/main/ledsOff.jpg?ref_type=heads" height=200>

