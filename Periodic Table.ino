/*
 * Periodic table code!
 * 
 * Shared under the creative commons CC BY licence!
 * In other words, credit the code, but other than that use as you like!
 * 
 * Please look at the readme on github before looking at this code: 
 * https://github.com/apaf1/Periodic-table
 * 
 * by apaf1, 24.12.2016
 *
 */


#include <LiquidCrystal.h>
#include <Adafruit_NeoPixel.h>

//Led pins
#define blueLed 22
#define buttonLed 25
#define greenLed 24
#define lightPin 6

//button pins
#define bUp 30
#define bRight 35
#define bBack 32
#define bDown 33
#define bMeny 34
#define bLeft 31
#define partyPin A0

//Button values
boolean bUpVal = false;
boolean bUpLast = false;
boolean bUpAction = false;

boolean bRightVal = false;
boolean bRightLast = false;
boolean bRightAction = false;

boolean bBackVal = false;
boolean bBackLast = false;
boolean bBackAction = false;

boolean bDownVal = false;
boolean bDownLast = false;
boolean bDownAction = false;

boolean bMenyVal = false;
boolean bMenyLast = false;
boolean bMenyAction = false;

boolean bLeftVal = false;
boolean bLeftLast = false;
boolean bLeftAction = false;

// I have a partyswitch for partymode that overrides everything else
boolean partyMode = false;
int lastMode = 0;

boolean buttonPressed = false;

double dial;
int NUMPIXELS=118;
int partyRead = 0;

LiquidCrystal lcd(12, 11, 5, 4, 3, 2);
Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUMPIXELS, lightPin, NEO_GRB + NEO_KHZ800);

//Variables needed for controlling the mode
char* modes[]={"Periodic table","Orbitals","Collection","Select element","Element state",
               "Density","Hardness Mohs","Hardness Vicker","Discovered","Checkboard", 
               "Fill","Periodic Fill","Rainbow","Game of life, random", "Game of life, cool","Ball"};
int modesChoice = 0;
int modesChoiceMax = 15;
boolean modeChanged = false;

//Values needed for different mode functions
int temp = 1;
int element = 1;
int even = 0;
int red = 255;
int green = 0;
int blue = 0;
int colorChoice = 0;

//To reduce the update rate of LCD-display to reduce flicker
long waitDisplay;

//To remove unwanted double-clicking when pressing buttons
long waitButton;

//Access all elements sorted by rows
int rNumb[] = {
1,90,
2,13,54,65,66,77,78,89,
3,12,55,64,67,76,79,88,
4,11,14,21,22,29,30,37,38,45,46,53,56,63,68,75,80,87,
5,10,15,20,23,28,31,36,39,44,47,52,57,62,69,74,81,86,
6,9,16,19,24,27,32,35,40,43,48,51,58,61,70,73,82,85,
7,8,17,18,25,26,33,34,41,42,49,50,59,60,71,72,83,84,
91,92,93,94,95,96,97,98,99,100,101,102,103,104,
118,117,116,115,114,113,112,111,110,109,108,107,106,105
};

//Access all elements sorted by columns
int cNumb[] = {
1,2,3,4,5,6,7,
13,12,11,10,9,8,
14,15,16,17,91,118,
21,20,19,18,92,117,
22,23,24,25,93,116,
29,28,27,26,94,115,
30,31,32,33,95,114,
37,36,35,34,96,113,
38,39,40,41,97,112,
45,44,43,42,98,111,
46,47,48,49,99,110,
53,52,51,50,100,109,
54,55,56,57,58,59,101,108,
65,64,63,62,61,60,102,107,
66,67,68,69,70,71,103,106,
77,76,75,74,73,72,104,105,
78,79,80,81,82,83,
90,89,88,87,86,85,84
};

//Access all elements sorted by element number
int pNumb[] = {
1,90,
2,13,54,65,66,77,78,89,
3,12,55,64,67,76,79,88,
4,11,14,21,22,29,30,37,38,45,46,53,56,63,68,75,80,87,
5,10,15,20,23,28,31,36,39,44,47,52,57,62,69,74,81,86,
6,9,16,
91,92,93,94,95,96,97,98,99,100,101,102,103,104,
19,24,27,32,35,40,43,48,51,58,61,70,73,82,85,
7,8,17,
118,117,116,115,114,113,112,111,110,109,108,107,106,105,
18,25,26,33,34,41,42,49,50,59,60,71,72,83,84
};

//Orbitals
byte sOrbital[] = {1,2,3,11,19,37,55,87,4,12,20,38,56,88 }; //size: 14
byte dOrbital[] = { 21,22,23,24,25,26,27,28,29,30,
                    39,40,41,42,43,44,45,46,47,48,
                    57,72,73,74,75,76,77,78,79,80,
                    89,104,105,106,107,108,109,110,111,112}; //size: 40
byte pOrbital[]   = {13,31,49,50,81,82,83,
                      5,14,32,33,51,52,84,
                      6, 7, 8,15,16,34,54,
                      9,17,35,53,85,86,36,
                      10,18,113,114,115,116,117,118}; //size: 36
byte fOrbital[]   = {58,59,60,61,62,63,64,65,66,67,68,69,70,71,
                     90,91,92,93,94,95,96,97,98,99,100,101,102,103}; //size: 28

//Sorted by element group
byte alkali[] = {3,11,19,37,55,87};
byte alkaline[]={4,12,20,38,56,88};
byte transition[] = {21,22,23,24,25,26,27,28,29,30,
                    39,40,41,42,43,44,45,46,47,48,
                    72,73,74,75,76,77,78,79,80};
byte synthetic[] ={104,105,106,107,108,109,110,111,
                    112,113,114,115,116,117,118};
byte basicMetal[] = {13,31,49,50,81,82,83};
byte semiMetal[] = {5,14,32,33,51,52,84};
byte nonMetal[] = {1,6,7,8,15,16,34};
byte halogen[] = {9,17,35,53,85};
byte noble[] = {2,10,18,36,54,86};
byte lanthanide[] = {57,58,59,60,61,62,63,64,65,66,67,68,69,70,71};
byte actinide[] = {89,90,91,92,93,94,95,96,97,98,99,100,101,102,103};

//a square matrix that fits the whole periodic table for 
//the game of life animation
boolean matrix[13][22] = {
  {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
  {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
  {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
  {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
  {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
  {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
  {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
  {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
  {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
  {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
  {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
  {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
  {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}
};
byte matrixSize[2] = {13,22};

//A matrix for transforming the game of life matrix into 
//LED lights
byte matrix2life[13][22] = {
  {200,200,200,200,200,200,200,200,200,200,200,200,200,200,200,200,200,200,200,200,200,200},
  {200,200,200,200,200,200,200,200,200,200,200,200,200,200,200,200,200,200,200,200,200,200},
  {200,200,  0,200,200,200,200,200,200,200,200,200,200,200,200,200,200,200,200, 89,200,200},
  {200,200,  1, 12,200,200,200,200,200,200,200,200,200,200, 53, 64, 65, 76, 77, 88,200,200},
  {200,200,  2, 11,200,200,200,200,200,200,200,200,200,200, 54, 63, 66, 75, 78, 87,200,200},
  {200,200,  3, 10, 13, 20, 21, 28, 29, 36, 37, 44, 45, 52, 55, 62, 67, 74, 79, 86,200,200},
  {200,200,  4,  9, 14, 19, 22, 27, 30, 35, 38, 43, 46, 51, 56, 61, 68, 73, 80, 85,200,200},
  {200,200,  5,  8, 15, 18, 23, 26, 31, 34, 39, 42, 47, 50, 57, 60, 69, 72, 81, 84,200,200},
  {200,200,  6,  7, 16, 17, 24, 25, 32, 33, 40, 41, 48, 49, 58, 59, 70, 71, 82, 83,200,200},
  {200,200,200,200, 90, 91, 92, 93, 94, 95, 96, 97, 98, 99,100,101,102,103,200,200,200,200},
  {200,200,200,200,117,116,115,114,113,112,111,110,109,108,107,106,105,104,200,200,200,200},
  {200,200,200,200,200,200,200,200,200,200,200,200,200,200,200,200,200,200,200,200,200,200},
  {200,200,200,200,200,200,200,200,200,200,200,200,200,200,200,200,200,200,200,200,200,200}
};

//A fixed start state for game of life animation
boolean coolMatrix[13][22] = {
  {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
  {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
  {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
  {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
  {0,0,0,0,0,0,0,0,1,1,1,1,1,1,0,0,0,0,0,0,0,0},
  {0,0,0,0,0,0,0,1,0,0,0,0,0,0,1,0,0,0,0,0,0,0},
  {0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0},
  {0,0,0,0,0,0,0,1,0,0,0,0,0,0,1,0,0,0,0,0,0,0},
  {0,0,0,0,0,0,0,0,1,1,1,1,1,1,0,0,0,0,0,0,0,0},
  {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
  {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
  {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
  {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}
};

//A matrix for the ball animation
byte ball[10][20] = {
  {200,200,200,200,200,200,200,200,200,200,200,200,200,200,200,200,200,200,200},
  {200, 1, 12,200,200,200,200,200,200,200,200,200,200, 53, 64, 65, 76, 77, 88,200},
  {200, 2, 11,200,200,200,200,200,200,200,200,200,200, 54, 63, 66, 75, 78, 87,200},
  {200, 3, 10, 13, 20, 21, 28, 29, 36, 37, 44, 45, 52, 55, 62, 67, 74, 79, 86,200},
  {200, 4,  9, 14, 19, 22, 27, 30, 35, 38, 43, 46, 51, 56, 61, 68, 73, 80, 85,200},
  {200, 5,  8, 15, 18, 23, 26, 31, 34, 39, 42, 47, 50, 57, 60, 69, 72, 81, 84,200},
  {200, 6,  7, 16, 17, 24, 25, 32, 33, 40, 41, 48, 49, 58, 59, 70, 71, 82, 83,200},
  {200,200,200, 90, 91, 92, 93, 94, 95, 96, 97, 98, 99,100,101,102,103,200,200},
  {200,200,200,117,116,115,114,113,112,111,110,109,108,107,106,105,104,200,200},
  {200,200,200,200,200,200,200,200,200,200,200,200,200,200,200,200,200,200,200}
};

//Ball state code
double rBall = 6;
double cBall = 6;
double drBall = 0.5;
double dcBall = 0.5;

//a ball that always follows the rules is boring, give it a nudge sometimes
int nudge = 0;

//Vectors with the element state data
int solid[118]   = {0, -1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, -1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1};
int melting[118] = {14,  10000,  454,  1560,  2348,  3823,  63,  55,  54,  25,  371,  923,  933,  1687,  317,  388,  172,  84,  337,  1115,  1814,  1941,  2183,  2180,  1519,  1811,  1768,  1728,  1358,  693,  303,  1211,  1090,  494,  266,  116,  312,  1050,  1799,  2128,  2750,  2896,  2430,  2607,  2237,  1828,  1235,  594,  430,  505,  904,  723,  387,  161,  302,  1000,  1193,  1071,  1204,  1294,  1373,  1345,  1095,  1586,  1629,  1685,  1747,  1770,  1818,  1092,  1936,  2506,  3290,  3695,  3459,  3306,  2739,  2041,  1337,  234,  577,  601,  544,  527,  575,  202,  10000,  973,  1323,  2023,  1845,  1408,  917,  913,  1449,  1618,  1323,  1173,  1133,  1800,  1100,  1100,  1900,  10000,  10000,  10000,  10000,  10000,  10000,  10000,  10000,  10000,  10000,  10000,  10000,  10000,  10000,  10000};
int boiling[118] = {20,  4,  1615,  2743,  4273,  4300,  77,  90,  85,  27,  1156,  1363,  2792,  3173,  554,  718,  239,  87,  1032,  1757,  3103,  3560,  3680,  2944,  2334,  3134,  3200,  3186,  3200,  1180,  2477,  3093,  887,  958,  332,  120,  961,  1655,  3618,  4682,  5017,  4912,  4538,  4423,  3968,  3236,  2435,  1040,  2345,  2875,  1860,  1261,  457,  165,  944,  2143,  3737,  3633,  3563,  3373,  3273,  2076,  1800,  3523,  3503,  2840,  2973,  3141,  2223,  1469,  3675,  4876,  5731,  5828,  5869,  5285,  4701,  4098,  3129,  630,  1746,  2022,  1837,  1235,  10000,  211,  10000,  2010,  3473,  5093,  4273,  4200,  4273,  3503,  2284,  3383,  10000,  10000,  10000,  10000,  10000,  10000,  10000,  10000,  10000,  10000,  10000,  10000,  10000,  10000,  10000,  10000,  10000,  10000,  10000,  10000,  10000,  10000};

double density[118] = {8.99e-05,  0,  0.54,  1.85,  2.46,  2.26,  0,  0,  0,  0,  0.97,  1.74,  2.7,  2.33,  1.82,  1.96,  0,  0,  0.86,  1.55,  2.99,  4.51,  6.11,  7.14,  7.47,  7.87,  8.9,  8.91,  8.92,  7.14,  5.9,  5.32,  5.73,  4.82,  3.12,  0,  1.53,  2.63,  4.47,  6.51,  8.57,  10.28,  11.5,  12.37,  12.45,  12.02,  10.49,  8.65,  7.31,  7.31,  6.7,  6.24,  4.94,  0.01,  1.88,  3.51,  6.15,  6.69,  6.64,  7.01,  7.26,  7.35,  5.24,  7.9,  8.22,  8.55,  8.8,  9.07,  9.32,  6.57,  9.84,  13.31,  16.65,  19.25,  21.02,  22.59,  22.56,  21.09,  19.3,  13.53,  11.85,  11.34,  9.78,  9.2,  -1,  0.01,  -1,  5,  10.07,  11.72,  15.37,  19.05,  20.45,  19.82,  13.67,  13.51,  14.78,  15.1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1};
int discovered[118] = {1766,  1868,  1817,  1798,  1807,  0,  1772,  1774,  1670,  1898,  1807,  1808,  0,  1854,  1669,  0,  1774,  1894,  1807,  0,  1876,  1791,  1803,  0,  1774,  0,  0,  1751,  0,  1746,  1875,  1886,  0,  1817,  1826,  1898,  1861,  1790,  1794,  1789,  1801,  1778,  1937,  1827,  1803,  1803,  0,  1817,  1863,  0,  0,  1782,  1811,  1898,  1860,  1808,  1839,  1803,  1885,  1885,  1947,  1853,  1901,  1880,  1843,  1886,  1878,  1842,  1879,  1878,  1907,  1923,  1802,  1783,  1925,  1803,  1803,  0,  0,  0,  1861,  0,  0,  1898,  1940,  1900,  1939,  1898,  1899,  1828,  1913,  1789,  1940,  1940,  1944,  1944,  1949,  1950,  1952,  1952,  1955,  1957,  1961,  1969,  1967,  1974,  1976,  1984,  1982,  1994,  1994,  1996,  2003,  1998,  2003,  2000,  2010,  2002};

double hardnessMohs[118]   = {-1,  -1,  0.6,  5.5,  9.3,  0.5,  -1,  -1,  -1,  -1,  0.5,  2.5,  2.75,  6.5,  -1,  2,  -1,  -1,  0.4,  1.75,  -1,  6,  7,  8.5,  6,  4,  5,  4,  3,  2.5,  1.5,  6,  3.5,  2,  -1,  -1,  0.3,  1.5,  -1,  5,  6,  5.5,  -1,  6.5,  6,  4.75,  2.5,  2,  1.2,  1.5,  3,  2.25,  -1,  -1,  0.2,  1.25,  2.5,  2.5,  1.4,  1.2,  -1,  1.4,  3.1,  5.1,  2.3,  1.8,  1.6,  2,  1.8,  -1,  2.6,  5.5,  6.5,  7.5,  7,  7,  6.5,  3.5,  2.5,  -1,  1.2,  1.5,  2.25,  -1,  -1,  -1,  -1,  -1,  -1,  3,  -1,  6,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1};
double hardnessVicker[118] = {-1,  -1,  -1,  1670,  49000,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  167,  9630,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  970,  628,  1060,  -1,  608,  1043,  638,  369,  -1,  -1,  8010,  1510,  -1,  -1,  -1,  -1,  -1,  -1,  903,  1320,  1530,  -1,  2300,  1246,  461,  251,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  491,  270,  400,  343,  -1,  412,  167,  570,  863,  540,  481,  589,  520,  206,  1160,  1760,  873,  3430,  2450,  4140,  1760,  549,  216,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  350,  -1,  1960,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1};

double densityChoice = 1;
int discoveredChoice = 1700;
double hardnessMohsChoice = 0.5;
double hardnessVickerChoice = 100;

void setup() {
  //Serial.begin(9600);
  //Serial.print("start");
  
  waitDisplay = millis();
  waitButton  = millis();
  pinMode(blueLed,OUTPUT);
  pinMode(greenLed,OUTPUT);
  pinMode(buttonLed,OUTPUT);

  pinMode(bUp,INPUT);
  pinMode(bRight,INPUT);
  pinMode(bBack,INPUT);
  pinMode(bDown,INPUT);
  pinMode(bMeny,INPUT);
  pinMode(bLeft,INPUT);
  pinMode(partyPin,INPUT);

  pinMode(A1,INPUT);
  pinMode(A2,INPUT);
  //pinMode(A3,INPUT);
  //pinMode(A4,INPUT);
  //pinMode(A5,INPUT);
  //pinMode(A6,INPUT);
  //pinMode(A8,INPUT);
  
  lcd.begin(20, 4);
  lcd.print("Started!");

  pixels.begin();
  pixelsOff();

  digitalWrite(blueLed,LOW);
  digitalWrite(greenLed,HIGH);
  digitalWrite(buttonLed,LOW);


}

void loop() {
  
  checkButtons();

  while(partyMode){
    party();
  }
  
  updateState();

  
  if(modesChoice == 0){      
    periodicTable(1000);

  }else if(modesChoice == 1){
    orbital();
    
  }else if(modesChoice == 2){
    collection();
    
  }else if(modesChoice == 3){
    selectElement();
    
  }else if(modesChoice == 4){
    elementState();

  }else if(modesChoice == 5){
    displayDensity();

  }else if(modesChoice == 6){
    displayHardnessMohs();

  }else if(modesChoice == 7){
    displayHardnessVicker();
    
  }else if(modesChoice == 8){
    displayDiscovered();
    
  }else if(modesChoice == 9){
    checkboard(dial);

  }else if(modesChoice == 10){ 
    fill();
       
  }else if(modesChoice == 11){
    fillP();
    
  }else if(modesChoice == 12){
    rainbow();
    
  }else if(modesChoice == 13){
    if(modeChanged){
      initSerial();
      serialMatrix();
      randomLife();
      serialMatrix();
      modeChanged=false;
    }    
    life();
    serialMatrix();
    
  }else if(modesChoice == 14){
    if(modeChanged){
      //initSerial();  //for trouble-shooting
      //serialMatrix();
      coolLife();
      //serialMatrix();
      modeChanged=false;
    }
    life();
    //serialMatrix();
  }else if(modesChoice == 15){
    if(modeChanged){
      initBall();
      modeChanged=false;
    }
    moveBall(false);
    
  }


  //displayButtonState(); //for trouble-shooting
  //displayAnologRead();
  
}

//////////////////////////////////////////////////////////////////
//
// CONTROL CODE
//
//////////////////////////////////////////////////////////////////
boolean updateState(){
  //Finish tells other functions to end their loop
  boolean finish = false;
  if(bLeftAction){
    modeChanged = true;
    finish = true;
    if(modesChoice>=modesChoiceMax){
        modesChoice = 0;
      }else{
        modesChoice = modesChoice + 1;
      }
  }

  if(bRightAction){
    modeChanged = true;
    finish = true;
    if(modesChoice<=0){
        modesChoice = modesChoiceMax;
      }else{
        modesChoice = modesChoice - 1;
      }
  }
  
  clearButtonActionState();

  if(millis()>waitDisplay){
    
    waitDisplay=millis()+200;
    if(!partyMode){
      lcd.clear();
      lcd.print(modes[modesChoice]);
      lcd.setCursor(0,2);
      lcd.print("Delay: ");
      lcd.print((int) (dial/10));
      lcd.print(" ms");
      modeDisplay();
    }
    
  }
  
  return finish;
}

void modeDisplay(){
  if(modesChoice == 3){
    lcd.setCursor(0,1);
    lcd.print("Element: ");
    lcd.print(element);
    hideDelay();
  }else if(modesChoice ==4){
    lcd.setCursor(0,1);
    lcd.print("Temp: ");
    lcd.print(temp-273);
    lcd.print(" C");
    hideDelay();
  }else if(modesChoice ==5){
    lcd.setCursor(0,1);
    lcd.print("Density: ");
    lcd.print(densityChoice);
    lcd.print(" g/mL");
    hideDelay();
  }else if(modesChoice ==6){
    lcd.setCursor(0,1);
    lcd.print("Mohs: ");
    lcd.print(hardnessMohsChoice);
    hideDelay();
  }else if(modesChoice ==7){
    lcd.setCursor(0,1);
    lcd.print("Vicker: ");
    lcd.print(hardnessVickerChoice);
    lcd.print(" MPa");
    hideDelay();
  }else if(modesChoice ==8){
    lcd.setCursor(0,1);
    lcd.print("Year: ");
    lcd.print(discoveredChoice);
    hideDelay();
  }
}

void hideDelay(){
    lcd.setCursor(0,2);
    lcd.print("                  ");
}

void checkButtons(){
  bUpVal = digitalRead(bUp);
  bRightVal = digitalRead(bRight);
  bBackVal = digitalRead(bBack);
  bDownVal = digitalRead(bDown);
  bMenyVal = digitalRead(bMeny);
  bLeftVal = digitalRead(bLeft);
  
  if(bUpLast != bUpVal){
    bUpLast = bUpVal;
    if(bUpVal==HIGH && millis()>waitButton){
      bUpAction=true;
      buttonPressed = true;
    }else{
      buttonPressed = false;
    }
  }

  if(bRightLast != bRightVal){
    bRightLast = bRightVal;
    if(bRightVal==HIGH && millis()>waitButton){
      bRightAction=true;
      buttonPressed = true;
    }else{
      buttonPressed = false;
    }
  }

  if(bBackLast != bBackVal){
    bBackLast = bBackVal;
    if(bBackVal==HIGH && millis()>waitButton){
      bBackAction=true;
      buttonPressed = true;
    }else{
      buttonPressed = false;
    }
  }

  if(bDownLast != bDownVal){
    bDownLast = bDownVal;
    if(bDownVal==HIGH && millis()>waitButton){
      bDownAction=true;
      buttonPressed = true;
    }else{
      buttonPressed = false;
    }
  }

  if(bMenyLast != bMenyVal){
    bMenyLast = bMenyVal;
    if(bMenyVal==HIGH && millis()>waitButton){
      bMenyAction=true;
      buttonPressed = true;
    }else{
      buttonPressed = false;
    }
  }

  if(bLeftLast != bLeftVal){
    bLeftLast = bLeftVal;
    if(bLeftVal==HIGH && millis()>waitButton){
      bLeftAction=true;
      buttonPressed = true;
    }else{
      buttonPressed = false;
    }
  }
  
  if(buttonPressed){
    digitalWrite(buttonLed,HIGH);
    waitButton = millis() + 200;
  }else{
    digitalWrite(buttonLed,LOW);
  }
  
  partyRead = analogRead(partyPin);
  if(partyRead<500){
    if(!partyMode){
      initPartyMode();
    }
    partyMode=true;
    
  }else{
    partyMode=false;
    digitalWrite(blueLed,LOW);
  }

  dial = dial2Range(0,1000);
  
}


//The dial read value is not reliable, should be between 0 and 1023
//But in reality is between ~20 and ~990
//This function corrects that
double dial2Range(double minimum,double maximum){
  dial = analogRead(A1);
  dial = dial-300;
  double out = ( ((double)dial / 723)  *1.05);
  out = (out*(maximum-minimum))+minimum;

  if(out<minimum){
    out=minimum;
  }
  if(out>maximum){
    out=maximum;
  }
  return out;
}

//Trouble shooting function for buttons
void displayButtonState(){
  lcd.clear();
  lcd.print("Up:   ");
  lcd.print(bUpVal);
  lcd.print(" Right: ");
  lcd.print(bRightVal);
  lcd.setCursor(0,1);
  lcd.print("Left: ");
  lcd.print(bLeftVal);
  lcd.print(" Down:  ");
  lcd.print(bDownVal);
  lcd.setCursor(0,2);
  lcd.print("Meny: ");
  lcd.print(bMenyVal);
  lcd.print(" Back:  ");
  lcd.print(bBackVal);
  lcd.setCursor(0,3);
  lcd.print("P.: ");
  lcd.print(partyRead);
  lcd.print(" Dial: ");
  lcd.print(dial);
  
  pause(200);
  
}

//Trouble shooting function
void displayAnologRead(){
  
  lcd.clear();
  lcd.print("A1: ");
  lcd.print(analogRead(A1));
  lcd.setCursor(10,0);
  lcd.print("A2: ");
  lcd.print(analogRead(A2));
  
  lcd.setCursor(0,1);
  lcd.print("A3: ");
  lcd.print(analogRead(A3));
  lcd.setCursor(10,1);
  lcd.print("A4: ");
  lcd.print(analogRead(A4));
  
  lcd.setCursor(0,2);
  lcd.print("A5: ");
  lcd.print(analogRead(A5));
  lcd.setCursor(10,2);
  lcd.print("A6: ");
  lcd.print(analogRead(A6));

  lcd.setCursor(0,3);
  lcd.print("A7: ");
  lcd.print(analogRead(A7));
  lcd.setCursor(10,3);
  lcd.print("A8: ");
  lcd.print(analogRead(A8));
  
  delay(100);
}


void clearButtonActionState(){
  bUpAction = false;
  bRightAction = false;
  bBackAction = false;
  bDownAction = false;
  bMenyAction = false;
  bLeftAction = false;
}


//Custom pause function that is used instead of delay()
//This one still reads button input!
boolean pause(int milliSeconds){
  boolean finish = false;
  long start = millis();
  
  if(partyMode){
    partyRead = analogRead(partyPin);
    if(partyRead<500){
        partyMode=true;
      }else{
        partyMode=false;
        digitalWrite(blueLed,LOW);
      }  
    while(millis()<(start+milliSeconds)){
      partyRead = analogRead(partyPin);
      if(partyRead>500){
        partyMode=false;
        digitalWrite(blueLed,LOW);
      }  
      delay(10);
    }
  }else{
    long start = millis();
    checkButtons();
    finish=updateState();
    while(millis()<(start+milliSeconds)){
      checkButtons();
      finish = updateState();
      if(finish){
        break;
      }
    }
   }
  return finish;
}

//////////////////////////////////////////////////////////////////
//
// PIXELS CODE
//
//////////////////////////////////////////////////////////////////

void pixelsOff(){
    for(int i=0;i<NUMPIXELS;i++){
      pixels.setPixelColor(i,0);
    }
}

//Constant element colors
void periodicTable(int waitTime){
  colorize(alkali,6,255,0,0); //red
  colorize(alkaline,6,255,100,0); //orange
  colorize(transition,29,200,200,50); //light yellow
  colorize(synthetic,15,255,255,0); //yellow
  colorize(basicMetal,7,0,255,0);
  colorize(semiMetal,7,0,0,255);
  colorize(nonMetal,7,255-188,255-143,255-143); //light blue
  colorize(halogen,6,138,43,226); //blueviolet
  colorize(noble,6,75,0,130);//indigo
  colorize(lanthanide,15,210,105,40);//chocholate
  colorize(actinide,15,139,69,19);//saddle brown
  
  pixels.show();
  
  pause(waitTime);
  
}

//Select one element with dial
void selectElement(){
  element = (int) dial2Range(1,118);
  pixelsOff();
  pixels.setPixelColor(pNumb[(element)-1]-1,pixels.Color(0,0,255));
  pixels.show();
  pause(0);
}


//I have no idea why there is three versions of the colorize function, 
//but i don't dare delete them :P

//Colorize list of pixels according to atomic number
void colorize(byte numbers[],int sizeNumbers,int r,int g,int b){
  for(int i = 0;i<sizeNumbers;i++){
    pixels.setPixelColor(pNumb[numbers[i]-1]-1,pixels.Color(r,g,b));
  }
}
//Colorize list of pixels according to atomic number
void colorize(int numbers[],int sizeNumbers,int r,int g,int b){
  for(int i = 0;i<sizeNumbers;i++){
    pixels.setPixelColor(pNumb[numbers[i]-1]-1,pixels.Color(r,g,b));
  }
}

//Colorize pixels according to atomic number
void colorize(int number,int r,int g,int b){
  pixels.setPixelColor(pNumb[number-1]-1,pixels.Color(r,g,b));
}

//Illuminate only my collected samples
void collection(){
  pixelsOff();
  int samling[] = {6,12,13,14,15,18,22,26,29,30,31,32,45,49,53,56,57,59,62,67,74,77};
  int sizeSamling = 22;
  uint32_t col = Wheel((dial/1000)*255);
  for(int i = 0;i<sizeSamling;i++){
    pixels.setPixelColor(pNumb[samling[i]-1]-1,col);
  }
  pixels.show();
  pause(50);
}

//Illuminate elements according to state at a given temperature
void elementState(){
  if(!partyMode){
    temp=(int)dial2Range(0,5869);
  }
  for(int i=0;i<NUMPIXELS;i++){
    if(boiling[i]<=temp){
      colorize(i+1,255,0,0);
    }else if(melting[i]<=temp){
      colorize(i+1,255,255,0);
    }else if(solid[i]==0){
      colorize(i+1,0,0,255);
    }else{
      colorize(i+1,0,0,0);
    }
  }
  pixels.show();
  if(!partyMode){
    pause(50);
  }else{
    pause(0);
  }
}

//Illuminate elements according to if they are above or below a given density
void displayDensity(){
  if(!partyMode){
    densityChoice = dial2Range(0,22.59);
  }
  for(int i=0;i<NUMPIXELS;i++){
    if(density[i]<=0){
      colorize(i+1,0,0,0);
    }else if(density[i]>densityChoice){
      colorize(i+1,0,0,255);
    }else if(density[i]<=densityChoice){
      colorize(i+1,0,255,0);
    }
  }
  pixels.show();
  if(!partyMode){
    pause(50);
  }else{
    pause(0);
  }
}

//Illuminate elements according to if they discovered or not at a given year
void displayDiscovered(){
  discoveredChoice = (int) dial2Range(1668,2010);
  if(discoveredChoice==1668){
    discoveredChoice=0;
  }
  for(int i=0;i<NUMPIXELS;i++){
    if(discovered[i]==0){
      colorize(i+1,150,150,150);
    }else if(discovered[i]<=discoveredChoice){
      colorize(i+1,150,150,150);
    }else if(discovered[i]>discoveredChoice){
      colorize(i+1,0,0,0);
    }
  }
  pixels.show();
  pause(50);
}

void displayHardnessMohs(){
  hardnessMohsChoice = dial2Range(0.19,9.3);
  for(int i=0;i<NUMPIXELS;i++){
    if(hardnessMohs[i]<=0){
      colorize(i+1,0,0,0);
    }else if(hardnessMohs[i]<=hardnessMohsChoice){
      colorize(i+1,0,255,0);
    }else if(hardnessMohs[i]>hardnessMohsChoice){
      colorize(i+1,0,0,255);
    }
  }
  pixels.show();
  pause(50);
}

void displayHardnessVicker(){
  hardnessVickerChoice = dial2Range(166,49000);
  for(int i=0;i<NUMPIXELS;i++){
    if(hardnessVicker[i]<=0){
      colorize(i+1,0,0,0);
    }else if(hardnessVicker[i]<=hardnessVickerChoice){
      colorize(i+1,0,255,0);
    }else if(hardnessVicker[i]>hardnessVickerChoice){
      colorize(i+1,0,0,255);
    }
  }
  pixels.show();
  pause(50);
}

//Illuminate the different orbitals in different colors
void orbital(){
  byte orbital = (int) dial2Range(1,5);
  pixelsOff();
  if(orbital ==1){
    colorize(sOrbital,14,255,127,80); //coral orange
  }else if(orbital ==2){
    colorize(pOrbital,36,0,0,255); //blue
  }else if(orbital ==3){
    colorize(dOrbital,40,255,255,0); //yellow
  }else if(orbital ==4){
    colorize(fOrbital,28,0,255,0); //green
  }else{
    colorize(sOrbital,14,255,127,80); //coral orange
    colorize(pOrbital,36,0,0,255); //blue
    colorize(dOrbital,40,255,255,0); //yellow
    colorize(fOrbital,28,0,255,0); //green
  }
  pixels.show();
  pause(50);
}

//Switch between illuminating the odd and even elements in different colors
void checkboard(int waitTime){
  for(int i=0;i<NUMPIXELS;i++){
      if((i%2==even)){
        pixels.setPixelColor(i,pixels.Color(0,255,0));
      }else{
        pixels.setPixelColor(i,pixels.Color(255,0,0));
      }
  }
  pixels.show();
  if(even==0){
    even=1;
  }else{
    even=0;
  }
  pause(waitTime);
}

//Fill the periodic table with color, columnwise
void fill(){
  changeColor();
  int delayval = 50;
  for(int i=0;i<NUMPIXELS;i++){
    pixels.setPixelColor(cNumb[i]-1,pixels.Color(red,green,blue));
    pixels.show();
    if(pause(dial/10)){
      break;
    }
  }
  pause(1000);
}

//Fill the periodic table with color, by element number
void fillP(){
  changeColor();
  int delayval = 50;
  for(int i=0;i<NUMPIXELS;i++){
    pixels.setPixelColor(pNumb[i]-1,pixels.Color(red,green,blue));
    pixels.show();
    if(pause(dial/10)){
      break;
    }
  }
  pause(1000);
}

//get random color. 
void changeColor(){
  colorChoice = colorChoice + 1;
  if(colorChoice>5){
    colorChoice = 0;
  }
  if(colorChoice==0){
    red = 255;
    green = 0;
    blue = 0;
  }else if(colorChoice==1){
    red=0;
    green = 255;
    blue = 0;
  }else if(colorChoice==2){
    red = 0;
    green = 0;
    blue = 255;
  }else if(colorChoice==3){
    red = 255;
    green = 255;
    blue = 0;
  }else if(colorChoice==4){
    red = 255;
    green =0;
    blue = 255;
  }else if(colorChoice==5){
    red = 0;
    green = 255;
    blue = 255;
  }
}

void rainbow() {
  uint16_t i, j;
  for(j=0; j<256*5; j++) { // 5 cycles of all colors on wheel
    for(i=0; i< pixels.numPixels(); i++) {
      pixels.setPixelColor(cNumb[i]-1, Wheel(((i * 256 / pixels.numPixels()) + j) & 255));
    }
    pixels.show();
    if(!partyMode){
      if(pause(dial/10)){
        break;
      }
    }else{
      pause(0);
    }
  }
}

// Input a value 0 to 255 to get a color value.
// The colours are a transition r - g - b - back to r.
uint32_t Wheel(byte WheelPos) {
  WheelPos = 255 - WheelPos;
  if(WheelPos < 85) {
    return pixels.Color(255 - WheelPos * 3, 0, WheelPos * 3);
  }
  if(WheelPos < 170) {
    WheelPos -= 85;
    return pixels.Color(0, WheelPos * 3, 255 - WheelPos * 3);
  }
  WheelPos -= 170;
  return pixels.Color(WheelPos * 3, 255 - WheelPos * 3, 0);
}

//////////////////////////////////////////////////////////////////
//
// GAME OF LIFE
//
//////////////////////////////////////////////////////////////////

void life(){
  updateLife();
  pause(dial/2);
  showLife();
  pause(dial/2);
}

void updateLife(){
  byte neighbours = 0;
  for (int r = 1; r < matrixSize[0]-1; r++) {
    for (int c = 1; c < matrixSize[1]-1; c++) {
      neighbours =       matrix[r-1][c]   + matrix[r-1][c+1] +
      matrix[r][c+1]   + matrix[r+1][c+1] + matrix[r+1][c]   + 
      matrix[r+1][c-1] + matrix[r][c-1]   + matrix[r-1][c-1];
      //calculate number of neighbours

      if(matrix[r][c]){ //if living cell
        if(neighbours<2){
          matrix[r][c]=0; //dies because of underpopulation
        }else if(neighbours>3){
          matrix[r][c]=0; //dies because of overpopulation
        } //if 2 or 3 neighbours, unchanged
      }else{  //if dead cell
        if(neighbours==3){
          matrix[r][c]=1; //reproduction
        }
      }
    }
  }
}

void showLife(){
  byte i = 0;
  for (int r = 2; r < matrixSize[0]-1; r++) {
    for (int c = 2; c < matrixSize[1]-1; c++) {
      i = matrix2life[r][c];
      if(i<200){
        if(matrix[r][c]){
          pixels.setPixelColor(i,pixels.Color(0,255,0));
        }else{
          pixels.setPixelColor(i,pixels.Color(0,0,0));
        }
      }
    }
  }
  pixels.show();
}

void randomLife(){
  randomSeed(millis());
  for (int r = 1; r < matrixSize[0]; r++) {
    for (int c = 1; c < matrixSize[1]; c++) {
      matrix[r][c]=  random(2);
    }
  }
}

void coolLife(){
  for (int r = 0; r < matrixSize[0]; r++) {
    for (int c = 1; c < matrixSize[1]; c++) {
      matrix[r][c]=coolMatrix[r][c];
    }
  }
}

void initSerial(){
  Serial.begin(9600);
  Serial.print("start");
}

void serialMatrix(){
   for (int r = 1; r < matrixSize[0]; r++) {
    for (int c = 1; c < matrixSize[1]; c++) {
      Serial.print(matrix[r][c]);
    }
    Serial.println();
   }
   Serial.println();
}


void initBall(){
  rBall = 6;
  cBall = 6;
  randomSeed(millis());
  drBall = ((double)random(180)-90)/100;
  dcBall = ((double)random(180)-90)/100;
  for(int i = 0;i<NUMPIXELS;i++){
    pixels.setPixelColor(i,pixels.Color(0,0,255));
  }
  pixels.show();
}

void moveBall(boolean lines){
  int r = (int) rBall;
  int c = (int) cBall;
  
  if(ball[r+1][c]==200){
    drBall = -drBall;
    nudge +=1;
  }
  if(ball[r-1][c]==200){
    drBall = -drBall;
    nudge +=1;
  }
  if(ball[r][c+1]==200){
    dcBall = -dcBall;
    nudge +=1;
  }
  if(ball[r][c-1]==200){
    dcBall = -dcBall;
    nudge +=1;
  }
  if(nudge>10){
    drBall=drBall+((double)random(180)-90)/1000;
    dcBall=dcBall+((double)random(180)-90)/1000;
    nudge = 0;
  }
  
  for(int i = 0;i<NUMPIXELS;i++){
    pixels.setPixelColor(i,pixels.Color(0,0,255));
  }

  rBall = rBall + drBall;
  cBall = cBall + dcBall;

  if(lines){
    vline(c,255,0,0);
    hline(r+1,255,0,0);
  }
  if(ball[r][c]>118||ball[r][c]<0){
    initBall();
  }
  pixels.setPixelColor(ball[r][c],pixels.Color(255,0,0));
  pixels.show();
  /*
  Serial.print("rBall: ");
  Serial.print(rBall);
  Serial.print(" r: ");
  Serial.print(r);
  Serial.print(" drBall: ");
  Serial.println(drBall);
  
  Serial.print("cBall: ");
  Serial.print(cBall);
  Serial.print(" c: ");
  Serial.print(c);
  Serial.print(" dcBall: ");
  Serial.println(dcBall);

  Serial.print("Ball: ");
  Serial.println(ball[r][c]);
  */
  if(!partyMode){
    pause(dial/10);
  }else{
    pause(30);
  }
  
  
}

//////////////////////////////////////////////////////////////////
//
// PARTY MODE!!!
//
//////////////////////////////////////////////////////////////////

void initPartyMode(){
    randomSeed(millis());
    partyMode = true;
    digitalWrite(blueLed,HIGH);
    lcd.clear();
    lcd.write("Paaaaeeerty mooode!!");
    counter();
    pause(10);
    partyPeriodicTable(5);
    pause(10);
    partyBlinkColors(50);
    pause(10);
}

void party(){
  int nPartyModes = 8;
  pause(10);
  int mode=(int)random(0,nPartyModes);
  while(mode == lastMode){
    mode=(int)random(0,nPartyModes);
  }
  lastMode = mode;
  while(partyMode){
    while(mode == lastMode){
      mode=(int)random(0,nPartyModes);
    }
    lastMode = mode;
    if(mode==0){
      partyCheckboard(10);
      partyCheckboard(10);
      partyCheckboard(10);
    }else if(mode == 1){
      partyLineMove(5,true);
    }else if(mode == 2){
      partyLineMove(5,false);
    }else if(mode == 3){
      partyPeriodicTable(10);
    }else if(mode == 4){
      partyBlinkColors(25);
    }else if(mode == 5){
      partyRainbow(1);
    }else if(mode == 6){
      partyBall(250);
    }else if(mode == 7){
      partyDensity(2);
    }
  }
  
}

void counter(){
    for(int j=2;j<10;j++){
      for(int i=0;i<NUMPIXELS;i++){
        pixels.setPixelColor(i,pixels.Color(255,255,255));
      }
      pixels.show();
      pause(1000-(100*j));
      pixelsOff();
      pixels.show();
      pause(500-(50*j));
      if(!partyMode){
        break;
      }
    }    
}


void partyCheckboard(int times){
      for(int i=0;i<(times/2);i++){
        checkboard((times*25)-(i*50));
        if(!partyMode){
          break;
        }
        pause(10);
      }
      for(int i=0;i<(times/2);i++){
        checkboard((i*50));
        if(!partyMode){
          break;
        }
        pause(10);
      }
}

void whiteBlink(int times){
  for(int j=0;j<times;j++){
    for(int i=0;i<NUMPIXELS;i++){
      pixels.setPixelColor(i,pixels.Color(255,255,255));
    }
    pixels.show();
    pause(25);
    pixelsOff();
    pixels.show();
    pause(25);
    if(!partyMode){
          break;
    }
  }
}

void blinkColors(int waitTime){
    changeColor();
    for(int i=0;i<NUMPIXELS;i++){
      pixels.setPixelColor(i,pixels.Color(red,green,blue));
    }
    pixels.show();
    pause(waitTime);
    pixelsOff();
    pixels.show();
    pause(waitTime);

}

void partyBlinkColors(int times){
      for(int j=0;j<times;j++){
      blinkColors(50);
      if(!partyMode){
        break;
      }
      pause(20);
    }
}

void vline(int vline,byte r,byte g,byte b){
  vline=vline+1;
  for(int i = 0;i<13;i++){
    pixels.setPixelColor(matrix2life[i][vline],pixels.Color(r,g,b));
  }
}

void hline(int hline,byte r,byte g,byte b){
  hline=hline+1;
  for(int i = 2;i<20;i++){
    pixels.setPixelColor(matrix2life[hline][i],pixels.Color(r,g,b));
  }
}

void lineMove(int waitTime,boolean vertical){
  //move forward
  int n = 18;
  if(!vertical){
    n = 10;
  }
  for(int i = 1;i<=n;i++){
    for(int j = 0;j<118;j++){
    pixels.setPixelColor(j,pixels.Color(0,0,255));
    }
    if(vertical){
      vline(i,0,255,0);
    }else{
      hline(i,0,255,0);
    }
    pixels.show();
    pause(waitTime);
    if(!partyMode){
        break;
    }
  }

  //move backwards
  for(int i = (n-1);i>0;i--){
    for(int j = 0;j<118;j++){
    pixels.setPixelColor(j,pixels.Color(0,0,255));
    }
    if(vertical){
      vline(i,0,255,0);
    }else{
      hline(i,0,255,0);
    }
    pixels.show();
    pause(waitTime);
    if(!partyMode){
        break;
    }
  }
}

void partyLineMove(int times,boolean vertical){
  for(int i = 0;i<times;i++){
    lineMove((times*10)-(i*10),vertical);
    if(!partyMode){
      break;
    }
  }
}

void partyPeriodicTable(int times){
  for(int i = 0;i<times;i++){
    periodicTable(100);
    pixelsOff();
    pixels.show();
    pause(100);
    if(!partyMode){
      break;
    }
  }
}

void partyBall(int times){
  rBall = 6;
  cBall = 6;
  randomSeed(millis());
  drBall = (double)random(45,90)/100;
  dcBall = (double)random(45,90)/100;
  for(int i = 0;i<NUMPIXELS;i++){
    pixels.setPixelColor(i,pixels.Color(0,0,255));
  }
  pixels.show();
  for(int i = 0;i<times;i++){
    moveBall(true);
    if(!partyMode){
      break;
    }
  }
}

void partyRainbow(int times){
  for(int i = 0;i<times;i++){
    rainbow();
    if(!partyMode){
      break;
    }
  }
}

void partyDensity(int times){
  int lol = 100;
  int lol2 = 10;
  for(int i = 0;i<lol;i++){
    densityChoice = i/10;
    displayDensity();
  }
  pause(200);
  for(int i = 0;i<times;i++){
    for(int i = lol;i>lol2;i--){
      densityChoice = i/10;
      displayDensity();
    }
    for(int i = lol2;i<lol;i++){
      densityChoice = i/10;
      displayDensity();
    }
  }
  for(int i = lol;i<250;i++){
    densityChoice = i/10;
    displayDensity();
  }
  pause(200);
}


