#include<Arduino.h>
const int samplingFrequency = 44000;
// Set the ADC clock to 44 kHz
void setupADC() {
  PMC->PMC_PCER1 |= PMC_PCER1_PID37;  
  // Enable the ADC peripheral
  // Set the prescaler to 255 
  // (MCK is 84 MHz, ADC_CLK is (84 MHz / (2 * (255 + 1))) = 164.0625 kHz)
  ADC->ADC_MR = ADC_MR_PRESCAL(255)   
   // Set the startup time to 64 periods of ADC_CLK
              | ADC_MR_STARTUP_SUT64 
   // Set the tracking time to 15 periods of ADC_CLK              
              | ADC_MR_TRACKTIM(15)  
   // Set the settling time to 17 periods of ADC_CLK              
              | ADC_MR_SETTLING_AST3;
  // Enable channel 7 (A0)
  ADC->ADC_CHER = ADC_CHER_CH7;       

  // Configure Timer Counter 0 Channel 0 (TC0) for samplingFrequency 
  // Enable the TC0 peripheral
  PMC->PMC_PCER0 |= PMC_PCER0_PID27; 
  // Set the clock source to TCLK4 (MCK / 128, 84 MHz / 128 = 656.25 kHz) 
  TC0->TC_CHANNEL[0].TC_CMR = TC_CMR_TCCLKS_TIMER_CLOCK4 
       // Enable the RC compare trigger
                              | TC_CMR_CPCTRG;      
       // Set the RC value for a samplingFrequency Hz frequency 
	   //(656250 / samplingFrequency  - 1)                              
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
  //uint32_t adcValue = *(ADC->ADC_CDR + ADC_CHANNEL);
  //Serial.println(adcValue);
}

void loop() {
  // Your main loop code
}
