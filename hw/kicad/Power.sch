EESchema Schematic File Version 4
LIBS:schematic.kicad-cache
EELAYER 26 0
EELAYER END
$Descr A4 11693 8268
encoding utf-8
Sheet 2 2
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
L power:VDD #PWR?
U 1 1 5E7DEF84
P 1700 1800
F 0 "#PWR?" H 1700 1650 50  0001 C CNN
F 1 "VDD" V 1718 1927 50  0000 L CNN
F 2 "" H 1700 1800 50  0001 C CNN
F 3 "" H 1700 1800 50  0001 C CNN
	1    1700 1800
	0    -1   -1   0   
$EndComp
$Comp
L power:VSS #PWR?
U 1 1 5E7DEFB7
P 1700 2250
F 0 "#PWR?" H 1700 2100 50  0001 C CNN
F 1 "VSS" V 1718 2378 50  0000 L CNN
F 2 "" H 1700 2250 50  0001 C CNN
F 3 "" H 1700 2250 50  0001 C CNN
	1    1700 2250
	0    -1   -1   0   
$EndComp
$Comp
L Device:Transformer_1P_1S T?
U 1 1 5E7DF093
P 3050 2000
F 0 "T?" H 3050 2378 50  0000 C CNN
F 1 "Transformer_1P_1S" H 3050 2287 50  0000 C CNN
F 2 "" H 3050 2000 50  0001 C CNN
F 3 "~" H 3050 2000 50  0001 C CNN
	1    3050 2000
	1    0    0    -1  
$EndComp
$Comp
L Device:R R?
U 1 1 5E7DF17C
P 2650 1400
F 0 "R?" H 2580 1354 50  0000 R CNN
F 1 "R" H 2580 1445 50  0000 R CNN
F 2 "" V 2580 1400 50  0001 C CNN
F 3 "~" H 2650 1400 50  0001 C CNN
	1    2650 1400
	-1   0    0    1   
$EndComp
$Comp
L Device:C C?
U 1 1 5E7DF226
P 2200 2000
F 0 "C?" H 2085 1954 50  0000 R CNN
F 1 "C" H 2085 2045 50  0000 R CNN
F 2 "" H 2238 1850 50  0001 C CNN
F 3 "~" H 2200 2000 50  0001 C CNN
	1    2200 2000
	-1   0    0    1   
$EndComp
Wire Wire Line
	1700 1800 1700 1100
Wire Wire Line
	1700 1100 2650 1100
Wire Wire Line
	3450 1100 3450 1800
Wire Wire Line
	2650 1250 2650 1100
Connection ~ 2650 1100
Wire Wire Line
	2650 1100 3450 1100
Wire Wire Line
	2650 1550 2650 1650
Wire Wire Line
	2200 1850 2200 1650
Wire Wire Line
	2200 1650 2650 1650
Connection ~ 2650 1650
Wire Wire Line
	2650 1650 2650 1800
$Comp
L Transistor_BJT:PN2222A Q?
U 1 1 5E7DF339
P 3350 2650
F 0 "Q?" H 3541 2696 50  0000 L CNN
F 1 "PN2222A" H 3541 2605 50  0000 L CNN
F 2 "Package_TO_SOT_THT:TO-92_Inline" H 3550 2575 50  0001 L CIN
F 3 "http://www.fairchildsemi.com/ds/PN/PN2222A.pdf" H 3350 2650 50  0001 L CNN
	1    3350 2650
	1    0    0    -1  
$EndComp
Wire Wire Line
	3450 2200 3450 2350
$Comp
L Device:D_Zener D?
U 1 1 5E7DF445
P 2950 2950
F 0 "D?" V 2904 3029 50  0000 L CNN
F 1 "D_Zener" V 2995 3029 50  0000 L CNN
F 2 "" H 2950 2950 50  0001 C CNN
F 3 "~" H 2950 2950 50  0001 C CNN
	1    2950 2950
	0    1    1    0   
$EndComp
$Comp
L Device:C C?
U 1 1 5E7DF5F8
P 4100 2650
F 0 "C?" H 3985 2604 50  0000 R CNN
F 1 "C" H 3985 2695 50  0000 R CNN
F 2 "" H 4138 2500 50  0001 C CNN
F 3 "~" H 4100 2650 50  0001 C CNN
	1    4100 2650
	-1   0    0    1   
$EndComp
Wire Wire Line
	4100 2800 4100 3150
Wire Wire Line
	4100 3150 3450 3150
Wire Wire Line
	1700 3150 1700 2250
Wire Wire Line
	3450 2850 3450 3150
Connection ~ 3450 3150
Wire Wire Line
	3450 3150 2950 3150
Wire Wire Line
	2950 2800 2950 2650
Wire Wire Line
	2950 2650 3150 2650
Wire Wire Line
	2950 3100 2950 3150
Connection ~ 2950 3150
Wire Wire Line
	2950 3150 2200 3150
$Comp
L pspice:DIODE D?
U 1 1 5E7DFA83
P 3800 2350
F 0 "D?" H 3800 2615 50  0000 C CNN
F 1 "DIODE" H 3800 2524 50  0000 C CNN
F 2 "" H 3800 2350 50  0001 C CNN
F 3 "~" H 3800 2350 50  0001 C CNN
	1    3800 2350
	1    0    0    -1  
$EndComp
Wire Wire Line
	4000 2350 4100 2350
Wire Wire Line
	4100 2350 4100 2500
Wire Wire Line
	3600 2350 3450 2350
Connection ~ 3450 2350
Wire Wire Line
	3450 2350 3450 2450
Wire Wire Line
	2200 2150 2200 3150
Connection ~ 2200 3150
Wire Wire Line
	2200 3150 1700 3150
Wire Wire Line
	2650 2200 2650 2650
Wire Wire Line
	2650 2650 2950 2650
Connection ~ 2950 2650
Wire Notes Line
	4600 3600 4600 700 
Text Notes 1600 850  0    79   ~ 0
Joule thief with batter charger additions
Text Notes 5500 800  0    79   ~ 0
Battery Charge controller 
$EndSCHEMATC
