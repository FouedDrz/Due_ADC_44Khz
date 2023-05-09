# Due_ADC_44Khz
# ADC-Arduino

Ce dépôt contient un exemple de code Arduino pour lire les valeurs d'un convertisseur analogique-numérique (ADC) à une fréquence d'échantillonnage définie.

## Fonctionnalités

- Configuration de l'ADC pour lire les valeurs d'un canal spécifique (A0 dans cet exemple)
- Configuration d'un temporisateur pour déclencher l'ADC à une fréquence d'échantillonnage définie
- Affichage des valeurs ADC sur le moniteur série

## Prérequis

- Arduino IDE
- Une carte compatible Arduino avec un ADC intégré (comme l'Arduino Due)

## Installation

1. Clonez ce dépôt ou téléchargez le code source en tant que fichier ZIP.
2. Ouvrez le fichier `Due_ADC44Khz.ino` dans l'IDE Arduino.
3. Modifiez la fréquence d'échantillonnage si nécessaire.
4. Téléversez le code sur votre carte Arduino.

## Utilisation

Le code configure l'ADC pour lire les valeurs du canal A0 et utilise un temporisateur pour déclencher les lectures à la fréquence d'échantillonnage spécifiée. 
Les valeurs ADC sont affichées sur le moniteur série.

## Licence

Ce projet est sous licence MIT. Voir le fichier `LICENSE` pour plus de détails.
