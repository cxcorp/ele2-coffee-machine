EESchema Schematic File Version 4
LIBS:ele2-cache
EELAYER 30 0
EELAYER END
$Descr A4 11693 8268
encoding utf-8
Sheet 1 2
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
L ele2:WeMos_D1_mini U1
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
L ele2:HX711_module U2
U 1 1 5E815E00
P 6550 4700
F 0 "U2" H 6400 5150 50  0000 C CNN
F 1 "HX711_module" H 6200 4250 50  0000 C CNN
F 2 "" H 6550 4700 50  0001 C CNN
F 3 "" H 6550 4700 50  0001 C CNN
	1    6550 4700
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
P 6500 5200
F 0 "#PWR?" H 6500 4950 50  0001 C CNN
F 1 "GND" H 6505 5027 50  0000 C CNN
F 2 "" H 6500 5200 50  0001 C CNN
F 3 "" H 6500 5200 50  0001 C CNN
	1    6500 5200
	1    0    0    -1  
$EndComp
Wire Wire Line
	6500 5150 6500 5200
NoConn ~ 4650 3450
NoConn ~ 4650 3550
NoConn ~ 4650 3950
Text GLabel 4650 4050 2    50   Input ~ 0
SCREEN_SCL
NoConn ~ 4650 4250
NoConn ~ 4650 4350
Text GLabel 4650 3650 2    50   Input ~ 0
HX_DT
Text GLabel 6250 4750 0    50   Input ~ 0
HX_SCK
Text GLabel 4650 3750 2    50   Input ~ 0
HX_SCK
Text GLabel 6250 4650 0    50   Input ~ 0
HX_DT
Wire Notes Line
	3450 2450 3450 5500
Wire Notes Line
	3450 5500 8100 5500
Wire Notes Line
	8100 5500 8100 2450
Wire Notes Line
	8100 2450 3450 2450
Wire Notes Line
	5550 2450 5550 5500
Wire Notes Line
	5550 3750 8100 3750
Text Notes 5650 3900 0    50   ~ 0
HX711 load cell module
Text Notes 3550 2600 0    50   ~ 0
Core controller - D1 Mini
Wire Wire Line
	6900 4950 7000 4950
Connection ~ 7000 4950
$Comp
L power:GND #PWR?
U 1 1 5E83F78A
P 7000 5200
F 0 "#PWR?" H 7000 4950 50  0001 C CNN
F 1 "GND" H 7005 5027 50  0000 C CNN
F 2 "" H 7000 5200 50  0001 C CNN
F 3 "" H 7000 5200 50  0001 C CNN
	1    7000 5200
	1    0    0    -1  
$EndComp
Wire Wire Line
	7000 4950 7000 5200
Text Notes 6900 5150 0    50   ~ 0
*)
Text Notes 6550 5750 0    50   ~ 0
*) E- manually connected to GND\n  due to missing trace on board
$Sheet
S 7200 4300 550  750 
U 5E83A8FB
F0 "LoadCells" 50
F1 "loadCells.sch" 50
F2 "RED_II" I L 7200 4750 50 
F3 "RED_I" I L 7200 4950 50 
F4 "RED_IV" I L 7200 4850 50 
F5 "RED_III" I L 7200 4650 50 
$EndSheet
NoConn ~ 6900 4450
NoConn ~ 6900 4550
Wire Wire Line
	6900 4650 7200 4650
Wire Wire Line
	6900 4750 7200 4750
Wire Wire Line
	6900 4850 7200 4850
Wire Wire Line
	7000 4950 7200 4950
NoConn ~ 4650 3850
Text GLabel 4650 4150 2    50   Input ~ 0
SCREEN_SDA
$Comp
L ele2:Adafruit_SSD1306 U?
U 1 1 5EBEA0D0
P 6750 3100
F 0 "U?" H 7228 3146 50  0000 L CNN
F 1 "Adafruit_SSD1306" H 7228 3055 50  0000 L CNN
F 2 "" H 6750 3050 50  0001 C CNN
F 3 "" H 6750 3050 50  0001 C CNN
	1    6750 3100
	1    0    0    -1  
$EndComp
$Comp
L power:+3V3 #PWR?
U 1 1 5E81AC21
P 6500 4250
F 0 "#PWR?" H 6500 4100 50  0001 C CNN
F 1 "+3V3" H 6515 4423 50  0000 C CNN
F 2 "" H 6500 4250 50  0001 C CNN
F 3 "" H 6500 4250 50  0001 C CNN
	1    6500 4250
	1    0    0    -1  
$EndComp
$Comp
L power:+3V3 #PWR?
U 1 1 5EBEDF9F
P 6850 2700
F 0 "#PWR?" H 6850 2550 50  0001 C CNN
F 1 "+3V3" H 6865 2873 50  0000 C CNN
F 2 "" H 6850 2700 50  0001 C CNN
F 3 "" H 6850 2700 50  0001 C CNN
	1    6850 2700
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR?
U 1 1 5EBEE845
P 6850 3500
F 0 "#PWR?" H 6850 3250 50  0001 C CNN
F 1 "GND" H 6855 3327 50  0000 C CNN
F 2 "" H 6850 3500 50  0001 C CNN
F 3 "" H 6850 3500 50  0001 C CNN
	1    6850 3500
	1    0    0    -1  
$EndComp
Text GLabel 6400 3150 0    50   Input ~ 0
SCREEN_SCL
Text GLabel 6400 3050 0    50   Input ~ 0
SCREEN_SDA
Text Notes 5650 2600 0    50   ~ 0
OLED display
$EndSCHEMATC
