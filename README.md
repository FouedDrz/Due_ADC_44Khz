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

## Utilisation DUE_ADC_44Khz
## Algorithme

1. Initialiser les constantes et les variables :
   - Définir la fréquence d'échantillonnage à 44000 Hz.

2. Fonction `setupADC()` :
   a. Activer le périphérique ADC.
   b. Configurer le registre ADC_MR :
      - Régler le présélecteur sur 255.
      - Régler le temps de démarrage sur 64 périodes de ADC_CLK.
      - Régler le temps de suivi sur 15 périodes de ADC_CLK.
      - Régler le temps de stabilisation sur 17 périodes de ADC_CLK.
   c. Activer le canal 7 (A0) du ADC.
   d. Configurer le Timer Counter 0 Channel 0 (TC0) pour la fréquence d'échantillonnage :
      - Activer le périphérique TC0.
      - Régler la source d'horloge sur TCLK4 (MCK / 128, 84 MHz / 128 = 656,25 kHz) et activer le déclenchement par comparaison RC.
      - Régler la valeur RC pour obtenir une fréquence d'échantillonnage de `samplingFrequency` Hz.
      - Activer l'interruption de comparaison RC.
      - Activer l'interruption TC0_IRQn dans le NVIC.

3. Fonction `TC0_Handler()` :
   a. Lire le registre de statut pour effacer le drapeau d'interruption.
   b. Démarrer une nouvelle conversion ADC.

4. Fonction `setup()` :
   a. Initialiser la communication série à 115200 bauds.
   b. Appeler la fonction `setupADC()`.
   c. Activer le compteur de temps et le déclencher.

5. Fonction `loop()` :
   a. Lire la valeur ADC de la broche A0.
   b. Imprimer la valeur ADC sur le moniteur série.
   c. Attendre un court laps de temps pour contrôler la fréquence d'échantillonnage.

## Code Arduino

```cpp
#include <Arduino.h>

const int samplingFrequency = 44000;

void setupADC() {
  PMC->PMC_PCER1 |= PMC_PCER1_PID37;  // Enable the ADC peripheral
  ADC->ADC_MR = ADC_MR_PRESCAL(255)  // Set the prescaler to 255
              | ADC_MR_STARTUP_SUT64 // Set the startup time to 64 periods of ADC_CLK
              | ADC_MR_TRACKTIM(15)  // Set the tracking time to 15 periods of ADC_CLK
              | ADC_MR_SETTLING_AST3;// Set the settling time to 17 periods of ADC_CLK
  ADC->ADC_CHER = ADC_CHER_CH7;      // Enable channel 7 (A0)
  // Configure Timer Counter 0 Channel 0 (TC0) for samplingFrequency
  PMC->PMC_PCER0 |= PMC_PCER0_PID27; // Enable the TC0 peripheral
  TC0->TC_CHANNEL[0].TC_CMR = TC_CMR_TCCLKS_TIMER_CLOCK4 | TC_CMR_CPCTRG; 
  // Set the clock source to TCLK4 (MCK / 128, 84 MHz / 128 = 656.25 kHz)
  // Enable the RC compare trigger
  // Set the RC value for a samplingFrequency Hz frequency
  TC0->TC_CHANNEL[0].TC_RC = 656250 / samplingFrequency - 1;
  // Enable the RC compare interrupt
  TC0->TC_CHANNEL[0].TC_IER = TC_IER_CPCS;
  // Enable the TC0_IRQn interrupt in the NVIC
  NVIC_EnableIRQ(TC0_IRQn);
}

void TC0_Handler() {
  // Read the status register to clear the interrupt flag
  TC0->TC_CHANNEL[0].TC_SR; 
  // Start a new ADC conversion
  ADC->ADC_CR = ADC_CR_START;
}

void setup() {
  Serial.begin(115200);
  setupADC();
  // Enable the timer counter and trigger it
  TC0->TC_CHANNEL[0].TC_CCR = TC_CCR_CLKEN | TC_CCR_SWTRG;
}

void loop() {
  // Read the ADC value from the A0 pin
  uint32_t adcValue = ADC->ADC_CDR[7];
  // Print the ADC value to the serial monitor
  Serial.println(adcValue);
  // Delay for a short period of time to control the sampling rate
  delay(1000 / samplingFrequency);
}



## Licence

Ce projet est sous licence MIT. Voir le fichier `LICENSE` pour plus de détails.
