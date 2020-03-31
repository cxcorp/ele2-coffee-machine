EESchema Schematic File Version 4
LIBS:ele2-cache
EELAYER 30 0
EELAYER END
$Descr A4 11693 8268
encoding utf-8
Sheet 2 2
Title "Load cells"
Date "2020-03-31"
Rev "1.0.0"
Comp "Joona Heikkilä"
Comment1 ""
Comment2 ""
Comment3 ""
Comment4 ""
$EndDescr
$Comp
L ele2:Load_cell U5
U 1 1 5E83D45F
P 6700 3350
F 0 "U5" V 6654 3122 50  0000 R CNN
F 1 "Load_cell_IV" V 6750 3100 50  0000 R CNN
F 2 "" H 6750 3350 50  0001 C CNN
F 3 "" H 6750 3350 50  0001 C CNN
	1    6700 3350
	0    -1   1    0   
$EndComp
$Comp
L ele2:Load_cell U4
U 1 1 5E83E938
P 5200 4450
F 0 "U4" V 5246 4222 50  0000 R CNN
F 1 "Load_cell_I" V 5155 4222 50  0000 R CNN
F 2 "" H 5250 4450 50  0001 C CNN
F 3 "" H 5250 4450 50  0001 C CNN
	1    5200 4450
	0    -1   -1   0   
$EndComp
$Comp
L ele2:Load_cell U6
U 1 1 5E83F079
P 6700 4450
F 0 "U6" V 6746 4222 50  0000 R CNN
F 1 "Load_cell_III" V 6655 4222 50  0000 R CNN
F 2 "" H 6750 4450 50  0001 C CNN
F 3 "" H 6750 4450 50  0001 C CNN
	1    6700 4450
	0    -1   -1   0   
$EndComp
Wire Wire Line
	6700 3700 6700 4100
Wire Wire Line
	5200 4100 5200 3700
Wire Wire Line
	5200 3000 5200 2900
Wire Wire Line
	5200 2900 6700 2900
Wire Wire Line
	6700 2900 6700 3000
Wire Wire Line
	5200 4800 5200 4900
Wire Wire Line
	5200 4900 6700 4900
Wire Wire Line
	6700 4900 6700 4800
Text HLabel 4850 3350 0    50   Input ~ 0
RED_II
Text HLabel 4850 4450 0    50   Input ~ 0
RED_I
Text HLabel 6350 3350 0    50   Input ~ 0
RED_IV
Text HLabel 6350 4450 0    50   Input ~ 0
RED_III
Text Notes 7050 6350 0    50   ~ 0
Load cells labeled I, II, III and IV. Each cell has a red, white and black wire.
Text Notes 6950 6200 0    50   ~ 0
Notes:
$Comp
L ele2:Load_cell U3
U 1 1 5E83BAEE
P 5200 3350
F 0 "U3" V 5154 3122 50  0000 R CNN
F 1 "Load_cell_II" V 5245 3122 50  0000 R CNN
F 2 "" H 5250 3350 50  0001 C CNN
F 3 "" H 5250 3350 50  0001 C CNN
	1    5200 3350
	0    -1   1    0   
$EndComp
$EndSCHEMATC
