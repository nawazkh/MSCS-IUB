# Internet of Things Mini projects
# STAR project :
Audio classification.
## Aim: to classify audio sounds as a truck or non truck using on board machine learning.
## Goal: To be able to run machine learning model like decision tree on the board and classify the audio in 3 seconds.
## Achieved: Implemented decision trees, naive bayes, and power thresholding. Achieved 73% accuracy in onboard classification.
## challenges: No floating point calculation on particle photon. All calculation happened on Q_31

Worked With Particle Photon having the following features:
1. Particle PØ Wi-Fi module
2. Broadcom BCM43362 Wi-Fi chip
3. 802.11b/g/n Wi-Fi
4. STM32F205RGY6 120Mhz ARM Cortex M3
5. 1MB flash, 128KB RAM
6. On-board RGB status LED (ext. drive provided)
7. 18 Mixed-signal GPIO and advanced peripherals
8. Open source design
9. Real-time operating system (FreeRTOS)
10. Soft AP setup
11. FCC, CE and IC certified



Folders have the following in them:

  1. Room Occupency: Works upon Utilizing PIR sensor to control a room's power and updates a sheet in the google docs about th    change in the rooms status.

  2. Audio Capture Deals with utilizing multiple ADCDCMA of Particle Photon.
     Particle Photon is built upon STM32F205RGY6 120Mhz ARM Cortex M3.

  3. Power Modes: Is a project where I have experimented with various power modes of STM32F205RGY6 to check its feasibility in real time. Please refer its readme.md file in the folder for more details.
