EESchema Schematic File Version 4
EELAYER 30 0
EELAYER END
$Descr A4 11693 8268
encoding utf-8
Sheet 1 1
Title ""
Date ""
Rev ""
Comp ""
Comment1 ""
Comment2 ""
Comment3 ""
Comment4 ""
$EndDescr
$Comp
L Device:R_POT_US POT1
U 1 1 64A4A90A
P 4650 2350
F 0 "POT1" H 4583 2396 50  0000 R CNN
F 1 "10k" H 4583 2305 50  0000 R CNN
F 2 "Potentiometer_THT:Potentiometer_Alps_RK09K_Single_Vertical" H 4650 2350 50  0001 C CNN
F 3 "https://www.cuidevices.com/product/resource/pt01.pdf" H 4650 2350 50  0001 C CNN
	1    4650 2350
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR?
U 1 1 64A5002E
P 5750 2550
F 0 "#PWR?" H 5750 2300 50  0001 C CNN
F 1 "GND" H 5755 2377 50  0000 C CNN
F 2 "" H 5750 2550 50  0001 C CNN
F 3 "" H 5750 2550 50  0001 C CNN
	1    5750 2550
	1    0    0    -1  
$EndComp
$Comp
L power:+3V3 #PWR?
U 1 1 64A5A1A3
P 4650 2200
F 0 "#PWR?" H 4650 2050 50  0001 C CNN
F 1 "+3V3" H 4665 2373 50  0000 C CNN
F 2 "" H 4650 2200 50  0001 C CNN
F 3 "" H 4650 2200 50  0001 C CNN
	1    4650 2200
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR?
U 1 1 64A7191F
P 4650 2500
F 0 "#PWR?" H 4650 2250 50  0001 C CNN
F 1 "GND" H 4655 2327 50  0000 C CNN
F 2 "" H 4650 2500 50  0001 C CNN
F 3 "" H 4650 2500 50  0001 C CNN
	1    4650 2500
	1    0    0    -1  
$EndComp
$Comp
L power:+3V3 #PWR?
U 1 1 64A79468
P 5750 1950
F 0 "#PWR?" H 5750 1800 50  0001 C CNN
F 1 "+3V3" H 5765 2123 50  0000 C CNN
F 2 "" H 5750 1950 50  0001 C CNN
F 3 "" H 5750 1950 50  0001 C CNN
	1    5750 1950
	1    0    0    -1  
$EndComp
Wire Wire Line
	6150 2250 6400 2250
Text GLabel 7500 2100 2    50   Input ~ 0
D0
Text GLabel 7000 2100 0    50   Input ~ 0
D1
Text GLabel 7500 2000 2    50   Input ~ 0
D2
Text GLabel 7500 1900 2    50   Input ~ 0
D4
Text GLabel 7000 1900 0    50   Input ~ 0
D5
Text GLabel 7000 2000 0    50   Input ~ 0
D3
Text GLabel 7000 1800 0    50   Input ~ 0
D7
Text GLabel 7500 1800 2    50   Input ~ 0
D6
Text GLabel 7000 2200 0    50   Input ~ 0
RST
Text GLabel 7500 2200 2    50   Input ~ 0
PWNN
Text GLabel 7000 1700 0    50   Input ~ 0
PCLK
Text GLabel 7500 1700 2    50   Input ~ 0
MCLK
Text GLabel 7000 1600 0    50   Input ~ 0
VS
Text GLabel 7500 1600 2    50   Input ~ 0
HS
Text GLabel 7000 1500 0    50   Input ~ 0
SCL
Text GLabel 7500 1500 2    50   Input ~ 0
SDA
$Comp
L power:GND #PWR?
U 1 1 64F2497A
P 7800 1400
F 0 "#PWR?" H 7800 1150 50  0001 C CNN
F 1 "GND" H 7805 1227 50  0000 C CNN
F 2 "" H 7800 1400 50  0001 C CNN
F 3 "" H 7800 1400 50  0001 C CNN
	1    7800 1400
	1    0    0    -1  
$EndComp
$Comp
L power:+3V3 #PWR?
U 1 1 64F24EFB
P 6550 1400
F 0 "#PWR?" H 6550 1250 50  0001 C CNN
F 1 "+3V3" H 6565 1573 50  0000 C CNN
F 2 "" H 6550 1400 50  0001 C CNN
F 3 "" H 6550 1400 50  0001 C CNN
	1    6550 1400
	1    0    0    -1  
$EndComp
Wire Wire Line
	7800 1400 7500 1400
Wire Wire Line
	7000 1400 6550 1400
$Comp
L Device:R_Network09_US DAC1
U 1 1 64F51CA5
P 4550 1800
F 0 "DAC1" H 4070 1754 50  0000 R CNN
F 1 "10k" H 4070 1845 50  0000 R CNN
F 2 "Resistor_THT:R_Array_SIP10" V 5125 1800 50  0001 C CNN
F 3 "https://www.bourns.com/docs/Product-Datasheets/R2R.pdf" H 4550 1800 50  0001 C CNN
	1    4550 1800
	-1   0    0    1   
$EndComp
Text GLabel 4250 1600 1    50   Input ~ 0
D0
Text GLabel 4350 1600 1    50   Input ~ 0
D1
Text GLabel 4450 1600 1    50   Input ~ 0
D2
Text GLabel 4550 1600 1    50   Input ~ 0
D3
Text GLabel 4650 1600 1    50   Input ~ 0
D4
Text GLabel 4750 1600 1    50   Input ~ 0
D5
Text GLabel 4850 1600 1    50   Input ~ 0
D6
Text GLabel 4950 1600 1    50   Input ~ 0
D7
$Comp
L power:GND #PWR?
U 1 1 64A927B4
P 3800 1600
F 0 "#PWR?" H 3800 1350 50  0001 C CNN
F 1 "GND" H 3805 1427 50  0000 C CNN
F 2 "" H 3800 1600 50  0001 C CNN
F 3 "" H 3800 1600 50  0001 C CNN
	1    3800 1600
	1    0    0    -1  
$EndComp
Text GLabel 6400 2250 2    50   Input ~ 0
A
Text GLabel 6550 2850 1    50   Input ~ 0
VS
Text GLabel 6650 2850 1    50   Input ~ 0
PCLK
Text GLabel 6850 2850 1    50   Input ~ 0
SCL
Text GLabel 6950 2850 1    50   Input ~ 0
SDA
Text GLabel 7050 2850 1    50   Input ~ 0
A
$Comp
L power:+3V3 #PWR?
U 1 1 64F89154
P 6450 2850
F 0 "#PWR?" H 6450 2700 50  0001 C CNN
F 1 "+3V3" H 6465 3023 50  0000 C CNN
F 2 "" H 6450 2850 50  0001 C CNN
F 3 "" H 6450 2850 50  0001 C CNN
	1    6450 2850
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR?
U 1 1 64F8A1BB
P 6250 2850
F 0 "#PWR?" H 6250 2600 50  0001 C CNN
F 1 "GND" H 6255 2677 50  0000 C CNN
F 2 "" H 6250 2850 50  0001 C CNN
F 3 "" H 6250 2850 50  0001 C CNN
	1    6250 2850
	1    0    0    -1  
$EndComp
Wire Wire Line
	6250 2850 6350 2850
$Comp
L power:+3V3 #PWR?
U 1 1 64F9836C
P 8250 2150
F 0 "#PWR?" H 8250 2000 50  0001 C CNN
F 1 "+3V3" H 8265 2323 50  0000 C CNN
F 2 "" H 8250 2150 50  0001 C CNN
F 3 "" H 8250 2150 50  0001 C CNN
	1    8250 2150
	1    0    0    -1  
$EndComp
Text GLabel 8250 2150 3    50   Input ~ 0
RST
Text GLabel 8450 2150 1    50   Input ~ 0
PWNN
$Comp
L power:GND #PWR?
U 1 1 64F9932F
P 8450 2150
F 0 "#PWR?" H 8450 1900 50  0001 C CNN
F 1 "GND" H 8455 1977 50  0000 C CNN
F 2 "" H 8450 2150 50  0001 C CNN
F 3 "" H 8450 2150 50  0001 C CNN
	1    8450 2150
	1    0    0    -1  
$EndComp
Text GLabel 7500 2850 1    50   Input ~ 0
D0
Text GLabel 7600 2850 1    50   Input ~ 0
D1
Text GLabel 7700 2850 1    50   Input ~ 0
D2
Text GLabel 7800 2850 1    50   Input ~ 0
D3
Text GLabel 7900 2850 1    50   Input ~ 0
D4
Text GLabel 8000 2850 1    50   Input ~ 0
D5
Text GLabel 8100 2850 1    50   Input ~ 0
D6
Text GLabel 8200 2850 1    50   Input ~ 0
D7
Text GLabel 5300 2150 1    50   Input ~ 0
D
Text GLabel 4850 2350 1    50   Input ~ 0
Dmin
Text GLabel 8300 2850 1    50   Input ~ 0
D
Text GLabel 8400 2850 1    50   Input ~ 0
Dmin
Text GLabel 6750 2850 1    50   Input ~ 0
MCLK
$Comp
L Connector_Generic:Conn_01x10 J3
U 1 1 64FC1282
P 8000 3050
F 0 "J3" V 8125 2996 50  0000 C CNN
F 1 "debug_output_to_mcu" V 8216 2996 50  0000 C CNN
F 2 "Connector_PinHeader_2.54mm:PinHeader_1x10_P2.54mm_Vertical" H 8000 3050 50  0001 C CNN
F 3 "~" H 8000 3050 50  0001 C CNN
	1    8000 3050
	0    1    1    0   
$EndComp
Wire Wire Line
	4950 2150 4950 2000
Wire Wire Line
	3800 1600 4150 1600
Wire Wire Line
	4800 2350 5550 2350
$Comp
L Aeris_lib_test:EL5165_Aeris OA1
U 1 1 64A49A08
P 5750 1700
F 0 "OA1" H 5850 1400 50  0000 L CNN
F 1 "LMV331" H 6094 1655 50  0001 L CNN
F 2 "Package_TO_SOT_SMD:SOT-23-5_HandSoldering" H 5750 1800 50  0001 C CNN
F 3 "http://www.ti.com/lit/ds/symlink/lmv331.pdf" H 5750 1900 50  0001 C CNN
	1    5750 1700
	1    0    0    -1  
$EndComp
$Comp
L Connector_Generic:Conn_01x08 J2
U 1 1 64FC026F
P 6750 3050
F 0 "J2" V 6900 2950 50  0000 L CNN
F 1 "output_to_mcu" V 7000 2750 50  0000 L CNN
F 2 "Connector_PinHeader_2.54mm:PinHeader_1x08_P2.54mm_Vertical" H 6750 3050 50  0001 C CNN
F 3 "~" H 6750 3050 50  0001 C CNN
	1    6750 3050
	0    1    1    0   
$EndComp
$Comp
L Connector_Generic:Conn_02x09_Odd_Even J1
U 1 1 64F1D794
P 7200 1800
F 0 "J1" H 7250 2417 50  0000 C CNN
F 1 "input_from_camera" H 7250 2326 50  0000 C CNN
F 2 "Connector_PinHeader_2.54mm:PinHeader_2x09_P2.54mm_Vertical" H 7200 1800 50  0001 C CNN
F 3 "~" H 7200 1800 50  0001 C CNN
	1    7200 1800
	1    0    0    -1  
$EndComp
Wire Wire Line
	4950 2150 5550 2150
$EndSCHEMATC
