EESchema Schematic File Version 4
LIBS:ele2-cache
EELAYER 30 0
EELAYER END
$Descr A4 11693 8268
encoding utf-8
Sheet 1 1
Title "coffee-sensor"
Date "2020-03-29"
Rev "1.0.0"
Comp "Joona Heikkilä"
Comment1 ""
Comment2 ""
Comment3 ""
Comment4 ""
$EndDescr
$Comp
L MCU_Module:WeMos_D1_mini U1
U 1 1 5E80C742
P 4250 3950
F 0 "U1" H 4450 4750 50  0000 C CNN
F 1 "WeMos_D1_mini" H 4600 3150 50  0000 C CNN
F 2 "Module:WEMOS_D1_mini_light" H 4250 2800 50  0001 C CNN
F 3 "https://wiki.wemos.cc/products:d1:d1_mini#documentation" H 2400 2800 50  0001 C CNN
	1    4250 3950
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR?
U 1 1 5E80E591
P 4250 4950
F 0 "#PWR?" H 4250 4700 50  0001 C CNN
F 1 "GND" H 4255 4777 50  0000 C CNN
F 2 "" H 4250 4950 50  0001 C CNN
F 3 "" H 4250 4950 50  0001 C CNN
	1    4250 4950
	1    0    0    -1  
$EndComp
Wire Wire Line
	4250 4950 4250 4750
$Comp
L Switch:SW_Push SW1
U 1 1 5E80F0CB
P 6800 3050
F 0 "SW1" H 6800 3335 50  0000 C CNN
F 1 "SW_Push" H 6800 3244 50  0000 C CNN
F 2 "" H 6800 3250 50  0001 C CNN
F 3 "~" H 6800 3250 50  0001 C CNN
	1    6800 3050
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR?
U 1 1 5E81082F
P 7100 3050
F 0 "#PWR?" H 7100 2800 50  0001 C CNN
F 1 "GND" H 7105 2877 50  0000 C CNN
F 2 "" H 7100 3050 50  0001 C CNN
F 3 "" H 7100 3050 50  0001 C CNN
	1    7100 3050
	1    0    0    -1  
$EndComp
Wire Wire Line
	6550 3050 6600 3050
Wire Wire Line
	7000 3050 7100 3050
$Comp
L ele2:HX711_module U2
U 1 1 5E815E00
P 6550 4450
F 0 "U2" H 6400 4900 50  0000 C CNN
F 1 "HX711_module" H 6200 4000 50  0000 C CNN
F 2 "" H 6550 4450 50  0001 C CNN
F 3 "" H 6550 4450 50  0001 C CNN
	1    6550 4450
	1    0    0    -1  
$EndComp
$Comp
L power:+3V3 #PWR?
U 1 1 5E81AC21
P 6500 4000
F 0 "#PWR?" H 6500 3850 50  0001 C CNN
F 1 "+3V3" H 6515 4173 50  0000 C CNN
F 2 "" H 6500 4000 50  0001 C CNN
F 3 "" H 6500 4000 50  0001 C CNN
	1    6500 4000
	1    0    0    -1  
$EndComp
$Comp
L power:+3V3 #PWR?
U 1 1 5E81B708
P 4350 3050
F 0 "#PWR?" H 4350 2900 50  0001 C CNN
F 1 "+3V3" H 4365 3223 50  0000 C CNN
F 2 "" H 4350 3050 50  0001 C CNN
F 3 "" H 4350 3050 50  0001 C CNN
	1    4350 3050
	1    0    0    -1  
$EndComp
Wire Wire Line
	4350 3150 4350 3050
NoConn ~ 4150 3150
NoConn ~ 3850 3550
NoConn ~ 3850 3950
NoConn ~ 3850 3850
$Comp
L power:GND #PWR?
U 1 1 5E81C444
P 6500 4950
F 0 "#PWR?" H 6500 4700 50  0001 C CNN
F 1 "GND" H 6505 4777 50  0000 C CNN
F 2 "" H 6500 4950 50  0001 C CNN
F 3 "" H 6500 4950 50  0001 C CNN
	1    6500 4950
	1    0    0    -1  
$EndComp
Wire Wire Line
	6500 4900 6500 4950
NoConn ~ 4650 3450
NoConn ~ 4650 3550
NoConn ~ 4650 3650
NoConn ~ 4650 4050
NoConn ~ 4650 3950
Text GLabel 6550 3050 0    50   Input ~ 0
TARE_SW
Text GLabel 4650 4150 2    50   Input ~ 0
TARE_SW
NoConn ~ 4650 4250
NoConn ~ 4650 4350
Wire Bus Line
	7200 3950 7300 3950
Text GLabel 4650 3750 2    50   Input ~ 0
HX_DT
Text GLabel 6250 4500 0    50   Input ~ 0
HX_SCK
Text GLabel 4650 3850 2    50   Input ~ 0
HX_SCK
Text GLabel 6250 4400 0    50   Input ~ 0
HX_DT
Wire Notes Line
	3450 2450 3450 5500
Wire Notes Line
	3450 5500 7900 5500
Wire Notes Line
	7900 5500 7900 2450
Wire Notes Line
	7900 2450 3450 2450
Wire Notes Line
	5550 2450 5550 5500
Wire Notes Line
	5550 3500 7900 3500
Text Notes 5650 2600 0    50   ~ 0
Calibration button
Text Notes 5650 3650 0    50   ~ 0
HX711 load cell module
Text Notes 3550 2600 0    50   ~ 0
Core controller - D1 Mini
Entry Wire Line
	7200 4600 7100 4700
Entry Wire Line
	7200 4500 7100 4600
Entry Wire Line
	7200 4400 7100 4500
Entry Wire Line
	7200 4300 7100 4400
Entry Wire Line
	7200 4200 7100 4300
Entry Wire Line
	7200 4100 7100 4200
Wire Wire Line
	6900 4200 7100 4200
Wire Wire Line
	6900 4300 7100 4300
Wire Wire Line
	6900 4400 7100 4400
Wire Wire Line
	6900 4500 7100 4500
Wire Wire Line
	6900 4600 7100 4600
Wire Wire Line
	6900 4700 7000 4700
Connection ~ 7000 4700
Wire Wire Line
	7000 4700 7100 4700
$Comp
L power:GND #PWR?
U 1 1 5E83F78A
P 7000 4950
F 0 "#PWR?" H 7000 4700 50  0001 C CNN
F 1 "GND" H 7005 4777 50  0000 C CNN
F 2 "" H 7000 4950 50  0001 C CNN
F 3 "" H 7000 4950 50  0001 C CNN
	1    7000 4950
	1    0    0    -1  
$EndComp
Wire Wire Line
	7000 4700 7000 4950
Text GLabel 7300 3950 2    50   Input ~ 0
LOAD_CELLS
Wire Bus Line
	7200 3950 7200 4600
Text Notes 7050 4900 0    50   ~ 0
*)
Text Notes 6550 5750 0    50   ~ 0
*) E- manually connected to GND\n  due to missing trace on board
$EndSCHEMATC
