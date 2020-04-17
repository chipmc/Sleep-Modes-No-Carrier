/**********************************************************************************************************************
 *
 * Left Side (16 pins)
 * !RESET           -
 * 3.3V             -
 * !MODE            -
 * GND              -
 * A0               -
 * A1               -
 * A2               -
 * A3               -
 * A4               -
 * A5 / SPI SS      -
 * SCK - SPI Clock  - 
 * MO - SPI MOSI    - 
 * MI - SPI MISO    - 
 * UART RX          -
 * UART TX          -

 Right Size (12 pins)
 * Li+              -
 * ENABLE           - RTC Circuit activated by DeepSleep pin
 * VUSB             -
 * D8               - Wake Connected to Watchdog Timer and the RTC using an OR gate
 * D7               - Blue Led - On when awake and off when sleeping
 * D6               - DEEP-SLEEP Enable Pin - Brings Enable Pin low - Only RTC Alarm interrupt will wake
 * D5               - 
 * D4               - User Switch for waking from HIBERNATE sleep without a RTC
 * D3               - 
 * D2               - 
 * SCL              - 
 * SDA              - 
 *
 *
***********************************************************************************************************************/


/**
 * @file   3rdGenDevicePinoutdoc.h
 * @author Chip McClelland
 * @date   1-24-2020
 * @brief  File containing the pinout documentation for 3rd Generation Particle Devices.
 * */
