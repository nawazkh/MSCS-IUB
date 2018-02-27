#include "application.h"
#include <stdint.h>
#include <bitset>


#include "adc_hal.h"
#include "gpio_hal.h"
#include "pinmap_hal.h"
#include "pinmap_impl.h"

// 2048 is a good size for this buffer. This is the number of samples; the number of bytes is twice
// this, but only half the buffer is sent a time, and then each pair of samples is averaged, so
// this results in 1024 byte writes from TCP, which is optimal.
// This uses 4096 bytes of RAM, which is also reasonable.
const size_t SAMPLE_BUF_SIZE = 2048;
uint16_t samples[SAMPLE_BUF_SIZE];

// This is the pin the microphone is connected to.
const int SAMPLE_PIN = A0;

// The audio sample rate. The minimum 8KHz.
//abfran: 22KHz sampling
const long SAMPLE_RATE = 32000;//22000;

//abfran : default audio listen tme of 5 seconds
const unsigned long MAX_RECORDING_LENGTH_MS = 5000;

//*********SEVERTALK!!!!
// This is the IP Address and port that the audioServer.js node server is running on.
IPAddress serverAddr = IPAddress(18,218,51,247);
int serverPort = 29501;
TCPClient client;
//*********SEVERTALK!!!!

unsigned long recordingLimit;
unsigned long caseLimit;


//debugging -  state information
enum State { STATE_WAITING, STATE_CONNECT, STATE_RUNNING, STATE_FINISH };
State state = STATE_WAITING;


// Setting the ADC & DMA , sending PIN number , 16 bit buffer data  , total buffer size
//
class ADCDMA {
public:
	ADCDMA(int pin, uint16_t *buf, size_t bufSize);
	virtual ~ADCDMA();

	void start(size_t freqHZ);
	void stop();

private:
	int pin;
	uint16_t *buf;
	size_t bufSize;
};

ADCDMA::ADCDMA(int pin, uint16_t *buf, size_t bufSize) : pin(pin), buf(buf), bufSize(bufSize) {
}

ADCDMA::~ADCDMA() {

}

void ADCDMA::start(size_t freqHZ) {

  // Using Dual ADC Regular Simultaneous DMA Mode 1

	// Using Timer3. To change timers, make sure you edit all of:
	// RCC_APB1Periph_TIM3, TIM3, ADC_ExternalTrigConv_T3_TRGO

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA2, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC2, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);

	// Set the pin as analog input
	// GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;
	// GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
  // abfran : PIN A0 is set as analog input
  HAL_Pin_Mode(pin, AN_INPUT);

	// Enable the DMA Stream IRQ Channel
	NVIC_InitTypeDef NVIC_InitStructure;
	NVIC_InitStructure.NVIC_IRQChannel = DMA2_Stream0_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);

	// 60000000UL = 60 MHz Timer Clock = HCLK / 2
	// Even low audio rates like 8000 Hz will fit in a 16-bit counter with no prescaler (period = 7500)
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
	TIM_TimeBaseStructInit(&TIM_TimeBaseStructure);
	TIM_TimeBaseStructure.TIM_Period = (60000000UL / freqHZ) - 1;
	TIM_TimeBaseStructure.TIM_Prescaler = 0;
	TIM_TimeBaseStructure.TIM_ClockDivision = 0;
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);
	TIM_SelectOutputTrigger(TIM3, TIM_TRGOSource_Update); // ADC_ExternalTrigConv_T3_TRGO
	TIM_Cmd(TIM3, ENABLE);

	ADC_CommonInitTypeDef ADC_CommonInitStructure;
	ADC_InitTypeDef ADC_InitStructure;
	DMA_InitTypeDef DMA_InitStructure;

	// DMA2 Stream0 channel0 configuration
	DMA_InitStructure.DMA_Channel = DMA_Channel_0;
  //abfran: 16 to 32 bit buffer converstion
	DMA_InitStructure.DMA_Memory0BaseAddr = (uint32_t)buf;
	DMA_InitStructure.DMA_PeripheralBaseAddr =  0x40012308; // CDR_ADDRESS; Packed ADC1, ADC2;
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralToMemory;
	DMA_InitStructure.DMA_BufferSize = bufSize;
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;
	DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;
	DMA_InitStructure.DMA_Priority = DMA_Priority_High;
	DMA_InitStructure.DMA_FIFOMode = DMA_FIFOMode_Enable;
	DMA_InitStructure.DMA_FIFOThreshold = DMA_FIFOThreshold_HalfFull;
	DMA_InitStructure.DMA_MemoryBurst = DMA_MemoryBurst_Single;
	DMA_InitStructure.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;
	DMA_Init(DMA2_Stream0, &DMA_InitStructure);

	// Don't enable DMA Stream Half / Transfer Complete interrupt
	// Since we want to write out of loop anyway, there's no real advantage to using the interrupt, and as
	// far as I can tell, you can't set the interrupt handler for DMA2_Stream0 without modifying
	// system firmware because there's no built-in handler for it.
	// DMA_ITConfig(DMA2_Stream0, DMA_IT_TC | DMA_IT_HT, ENABLE);

	DMA_Cmd(DMA2_Stream0, ENABLE);

	// ADC Common Init
	ADC_CommonInitStructure.ADC_Mode = ADC_DualMode_RegSimult;
	ADC_CommonInitStructure.ADC_Prescaler = ADC_Prescaler_Div2;
	ADC_CommonInitStructure.ADC_DMAAccessMode = ADC_DMAAccessMode_1;
	ADC_CommonInitStructure.ADC_TwoSamplingDelay = ADC_TwoSamplingDelay_5Cycles;
	ADC_CommonInit(&ADC_CommonInitStructure);

	// ADC1 configuration
	ADC_InitStructure.ADC_Resolution = ADC_Resolution_12b;
	ADC_InitStructure.ADC_ScanConvMode = DISABLE;
	ADC_InitStructure.ADC_ContinuousConvMode = DISABLE;
	ADC_InitStructure.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_Rising;
	ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_T3_TRGO;
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Left;
	ADC_InitStructure.ADC_NbrOfConversion = 1;
	ADC_Init(ADC1, &ADC_InitStructure);

	// ADC2 configuration - same
	ADC_Init(ADC2, &ADC_InitStructure);

	//
	ADC_RegularChannelConfig(ADC1, PIN_MAP[pin].adc_channel, 1, ADC_SampleTime_15Cycles);
  ADC_RegularChannelConfig(ADC2, PIN_MAP[pin].adc_channel, 1, ADC_SampleTime_15Cycles);

	// Enable DMA request after last transfer (Multi-ADC mode)
	ADC_MultiModeDMARequestAfterLastTransferCmd(ENABLE);

	// Enable ADCs
	ADC_Cmd(ADC1, ENABLE);
	ADC_Cmd(ADC2, ENABLE);

	ADC_SoftwareStartConv(ADC1);
}

//stopping the ADC
void ADCDMA::stop() {
	// Stop the ADC
	ADC_Cmd(ADC1, DISABLE);
	ADC_Cmd(ADC2, DISABLE);

	DMA_Cmd(DMA2_Stream0, DISABLE);

	// Stop the timer
	TIM_Cmd(TIM3, DISABLE);
}

ADCDMA adcDMA(A0, samples, SAMPLE_BUF_SIZE);

// End ADCDMA

// int audioInput = A0;
unsigned int audioTimer = 0;
// int led1 = D7;
int switch1 = D2;
int val = 0;
int timerFlag = 0;
int myChoiceState = 0;
uint16_t audioInput16Bit = 0;
String testString;

void setup()
{
  //CHANGE!!!!!!!
	Serial.begin(9600);
  //CHANGE!!!!!!!

	// Register handler to handle clicking on the SETUP button
	// System.on(button_click, buttonHandler);
	// pinMode(led1, OUTPUT);

  //  pinMode(audioInput, INPUT);
  //  pinMode(led1, OUTPUT);
   pinMode(switch1,INPUT);

   // For good measure, let's also make sure LEDs are off when we start:
  //  digitalWrite(led1, LOW);


}

void loop() {
	int switchValue = digitalRead(switch1);
	uint16_t *sendBuf = NULL;

	//Following these states
	// 1. RED: Busy waiting (essentially polling) in the loop function with wifi turned on but not using it.
	// 2. Blue: Busy waiting in the loop function with wifi turned off.
	// 3. Green: Busy waiting in the loop function with wifi turned off and RGB LED turned to full brightness with white light.
	// 4. Cyan: Continuously transferring TCP packets.
	// 5. Magenta: WiFi off and cpu in standby mode

	// each case to take 20 seconds
	/* ------------------------- */

	switch(switchValue){
		case 1:
		myChoiceState += 1;
		if(myChoiceState == 1){// in Red
			Particle.publish("Red mode","WifiOn");
			//Particle is disconnected
			//Wifi is ON
			//WiFi.off();
			WiFi.on();
			Particle.disconnect();
			caseLimit = millis() + 1500;
			//take control of the LED
			RGB.control(true);
			RGB.color(255, 0, 0);
			RGB.brightness(200);
			while( millis() <= caseLimit ){
				//255 being the max brightness; We are using 78.125% of brightness
				//do  nothing
			}
			RGB.color(0, 0, 0);
			//led flshing done.
			caseLimit = millis() + 20000;
			while( millis() <= caseLimit ){
				// wifi turned on but not using it
			}
			RGB.control(false);
			switchValue = 0;
			Particle.connect();

		}else if(myChoiceState == 2){// in Blue
			Particle.publish("Blue mode","LoopOnWifiOff");
			//code goes here
			WiFi.off();
			caseLimit = millis() + 1500;
			RGB.control(true);
			RGB.color(0, 0, 255);//RGB.color(red, green, blue)
			RGB.brightness(200);
			while( millis() <= caseLimit ){
				//255 being the max brightness; We are using 78.125% of brightness
				//do  nothing
			}
			RGB.color(0, 0, 0);
			caseLimit = millis() + 20000;
			while( millis() <= caseLimit ){
				// wifi turned on but not using it
			}

			RGB.control(false);
			switchValue = 0;
			WiFi.on();
			Particle.connect();

		}else if(myChoiceState == 3){// in Green
			Particle.publish("Green mode","JustWhiteLWifiOff");
			//code goes here
			caseLimit = millis() + 1500;
			RGB.control(true);
			RGB.color(0, 255, 0);//RGB.color(red, green, blue)
			RGB.brightness(200);
			while( millis() <= caseLimit ){
				//255 being the max brightness; We are using 78.125% of brightness
				//do  nothing
			}
			RGB.color(0, 0, 0);
			WiFi.off();
			caseLimit = millis() + 20000;
			RGB.color(255, 255, 255);
			while( millis() <= caseLimit ){
				RGB.brightness(255);
			}
			RGB.color(0, 0, 0);
			RGB.control(false);
			switchValue = 0;
			WiFi.on();
			Particle.connect();
		}else if(myChoiceState == 4){// in Cyan
			Particle.publish("Cyan mode","TCP");
			//code goes here
			caseLimit = millis() + 1500;
			RGB.control(true);
			RGB.color(0,215, 75);//RGB.color(red, green, blue)
			RGB.brightness(200);
			while( millis() <= caseLimit ){
				//255 being the max brightness; We are using 78.125% of brightness
				//do  nothing
			}
			RGB.color(0, 0, 0);
			if (client.connect(serverAddr, serverPort)) {
				Particle.publish("Cyan Mode","Audio Start");
				// digitalWrite(D7, HIGH);
				recordingLimit = millis() + 20000;
				adcDMA.start(SAMPLE_RATE);
				while( millis() <= recordingLimit){
					// Serial.printlnf("System Timer %d", millis());
					// Serial.printlnf("MyLimit %d", recordingLimit);
					if (DMA_GetFlagStatus(DMA2_Stream0, DMA_FLAG_HTIF0)) {
							DMA_ClearFlag(DMA2_Stream0, DMA_FLAG_HTIF0);
							sendBuf = samples; // 2048 memory locations of 16 bits
							if (sendBuf != NULL){
								for(size_t ii = 0, jj = 0; ii < SAMPLE_BUF_SIZE / 2; ii += 2, jj++) {
										 uint32_t sum = (uint32_t)sendBuf[ii] + (uint32_t)sendBuf[ii + 1];
										 sendBuf[jj] = (uint16_t)(sum / 2);
								}
								int count = client.write((uint8_t *)sendBuf, SAMPLE_BUF_SIZE / 2);
								if (count == SAMPLE_BUF_SIZE / 2) {
								// Success
								}
								else
								if (count == -16) {
									 // TCP Buffer full
									 //Serial.printlnf("buffer full, discarding");
								}
								else {
										// Error
										//Serial.printlnf("error writing %d", count);
										state = STATE_FINISH;
								}
							}
					}
					if (DMA_GetFlagStatus(DMA2_Stream0, DMA_FLAG_TCIF0)) {
							DMA_ClearFlag(DMA2_Stream0, DMA_FLAG_TCIF0);
							sendBuf = &samples[SAMPLE_BUF_SIZE / 2];
							if (sendBuf != NULL){
								for(size_t ii = 0, jj = 0; ii < SAMPLE_BUF_SIZE / 2; ii += 2, jj++) {
										 uint32_t sum = (uint32_t)sendBuf[ii] + (uint32_t)sendBuf[ii + 1];
										 sendBuf[jj] = (uint16_t)(sum / 2);
								}
								int count = client.write((uint8_t *)sendBuf, SAMPLE_BUF_SIZE / 2);
								if (count == SAMPLE_BUF_SIZE / 2) {
								// Success
								}
								else
								if (count == -16) {
									 // TCP Buffer full
									 Serial.printlnf("buffer full, discarding");
								}
								else {
										// Error
										Serial.printlnf("error writing %d", count);
										state = STATE_FINISH;
								}
							}
					}

				}
				// digitalWrite(D7, LOW);
				adcDMA.stop();
				client.stop();
				Particle.publish("Cyan Mode","Audio Transferred");
			 }
			 else {
						Serial.println("failed to connect to server");
						Particle.publish("Cyan Mode","Audio NOT Transferred");
						adcDMA.stop();
						//state = STATE_WAITING;
						timerFlag = 0;
			 }


			switchValue = 0;
			RGB.control(false);
			switchValue = 0;

		}
		// else if(myChoiceState == 5){// in Yellow
		// 	Particle.publish("Yellow mode","ParticleDisconnted");
		// 	//code goes here
		//
		// 	caseLimit = millis() + 1500;
		// 	//take control of the LED
		// 	RGB.control(true);
		// 	RGB.color(255, 251, 0);
		// 	RGB.brightness(200);
		// 	while( millis() <= caseLimit ){
		// 		//255 being the max brightness; We are using 78.125% of brightness
		// 		//do  nothing
		// 	}
		// 	RGB.color(0, 0, 0);
		// 	Particle.disconnected();
		// 	caseLimit = millis() + 20000;
		// 	while( millis() <= caseLimit ){
		// 	}
		//
		// 	//myChoiceState = 0;
		// 	RGB.control(false);
		// 	switchValue = 0;
		// 	WiFi.on();
		// 	Particle.connect();
		//
		// }
		else if(myChoiceState == 5){// in Yellow
			Particle.publish("Magenta mode","MCPause");
			//code goes here

			caseLimit = millis() + 1500;
			WiFi.off();
			//take control of the LED
			RGB.control(true);
			RGB.color(255, 0, 255);
			RGB.brightness(200);
			while( millis() <= caseLimit ){
				//255 being the max brightness; We are using 78.125% of brightness
				//do  nothing
			}
			RGB.color(0, 0, 0);

			System.sleep(D2,RISING,20);

			//myChoiceState = 0;
			RGB.control(false);
			switchValue = 0;
			WiFi.on();
			Particle.connect();

		}

		else if(myChoiceState == 6){// in Yellow
			Particle.publish("Orange mode","WLANS");
			//code goes here

			caseLimit = millis() + 1500;
			WiFi.off();
			//take control of the LED
			RGB.control(true);
			RGB.color(255, 165, 0);
			RGB.brightness(200);
			while( millis() <= caseLimit ){
				//255 being the max brightness; We are using 78.125% of brightness
				//do  nothing
			}


			RGB.color(0, 0, 0);

			System.sleep(SLEEP_MODE_WLAN,1);
			caseLimit = millis() + 20000;
			while( millis() <= caseLimit ){
			}

			//myChoiceState = 0;
			RGB.control(false);
			switchValue = 0;
			WiFi.on();
			Particle.connect();

		}

		else if(myChoiceState == 7){// in Magenta
			Particle.publish("White mode","DeepS");
			//code goes here

			caseLimit = millis() + 1500;
			//take control of the LED
			WiFi.off();
			RGB.control(true);
			RGB.color(255, 255, 255);
			RGB.brightness(200);
			while( millis() <= caseLimit ){
				//255 being the max brightness; We are using 78.125% of brightness
				//do  nothing
			}
			RGB.color(0, 0, 0);
			System.sleep(SLEEP_MODE_DEEP, 20);

			myChoiceState = 0;
			switchValue = 0;
			WiFi.on();
			Particle.connect();

		}

		break;

		case 0:
		//do nothing
		break;

	}


}
