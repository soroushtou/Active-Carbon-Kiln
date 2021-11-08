/*********************************

   5KG   LOADCELL Driver  with HX711

**********************************/

#include <mega32a.h>
#include <delay.h>
#include <alcd.h>
#include <stdio.h>
#include <stdlib.h>
#include <pgmspace.h>
//#include <fuzzy_code.h>

int flash fuzzy[101][41] ={
{11, 11, 11, 11, 11, 11, 11, 11, 11, 12, 12, 13, 12, 12, 11, 12, 12, 13, 14, 13, 11, 17, 30, 30, 30, 30, 30, 44, 51, 57, 61, 65, 70, 70, 70, 70, 70, 70, 70, 70, 70},
{11, 11, 11, 11, 11, 11, 11, 11, 11, 12, 12, 13, 12, 12, 11, 12, 12, 13, 14, 13, 11, 17, 30, 30, 30, 30, 30, 44, 51, 57, 61, 65, 70, 70, 70, 70, 70, 70, 70, 70, 70}, 
{11, 11, 11, 11, 11, 11, 11, 11, 11, 12, 12, 13, 12, 12, 11, 12, 12, 13, 14, 13, 11, 17, 30, 30, 30, 30, 30, 44, 51, 57, 61, 65, 70, 70, 70, 70, 70, 70, 70, 70, 70}, 
{11, 11, 11, 11, 11, 11, 11, 11, 11, 12, 12, 13, 12, 12, 11, 12, 12, 13, 14, 13, 11, 17, 30, 30, 30, 30, 30, 44, 51, 57, 61, 65, 70, 70, 70, 70, 70, 70, 70, 70, 70}, 
{11, 11, 11, 11, 11, 11, 11, 11, 11, 12, 12, 13, 12, 12, 11, 12, 12, 13, 14, 13, 11, 17, 30, 30, 30, 30, 30, 44, 51, 57, 61, 65, 70, 70, 70, 70, 70, 70, 70, 70, 70}, 
{11, 11, 11, 11, 11, 11, 11, 11, 11, 12, 12, 13, 12, 12, 11, 12, 12, 13, 14, 13, 11, 17, 30, 30, 30, 30, 30, 44, 51, 57, 61, 65, 70, 70, 70, 70, 70, 70, 70, 70, 70}, 
{11, 11, 11, 11, 11, 11, 11, 11, 11, 12, 12, 13, 12, 12, 11, 12, 12, 13, 14, 13, 11, 17, 30, 30, 30, 30, 30, 44, 51, 57, 61, 65, 70, 70, 70, 70, 70, 70, 70, 70, 70}, 
{11, 11, 11, 11, 11, 11, 11, 11, 11, 12, 12, 13, 12, 12, 11, 12, 12, 13, 14, 13, 11, 17, 30, 30, 30, 30, 30, 44, 51, 57, 61, 65, 70, 70, 70, 70, 70, 70, 70, 70, 70}, 
{11, 11, 11, 11, 11, 11, 11, 11, 11, 12, 12, 13, 12, 12, 11, 12, 12, 13, 14, 13, 11, 17, 30, 30, 30, 30, 30, 44, 51, 57, 61, 65, 70, 70, 70, 70, 70, 70, 70, 70, 70}, 
{11, 11, 11, 11, 11, 11, 11, 11, 11, 12, 12, 13, 12, 12, 11, 12, 12, 13, 14, 13, 11, 17, 30, 30, 30, 30, 30, 44, 51, 57, 61, 65, 70, 70, 70, 70, 70, 70, 70, 70, 70}, 
{11, 11, 11, 11, 11, 11, 11, 11, 11, 12, 12, 13, 12, 12, 11, 12, 12, 13, 14, 13, 11, 17, 30, 30, 30, 30, 30, 44, 51, 57, 61, 65, 70, 70, 70, 70, 70, 70, 70, 70, 70}, 
{12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 13, 12, 12, 12, 12, 12, 13, 14, 13, 12, 17, 30, 30, 30, 30, 30, 44, 51, 57, 61, 65, 70, 70, 70, 70, 70, 70, 70, 70, 70}, 
{12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 13, 12, 12, 12, 12, 12, 13, 14, 13, 12, 17, 30, 30, 30, 30, 30, 44, 51, 57, 61, 65, 70, 70, 70, 70, 70, 70, 70, 70, 70}, 
{12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 13, 12, 12, 12, 12, 12, 13, 14, 13, 12, 17, 30, 30, 30, 30, 30, 44, 51, 57, 61, 66, 71, 71, 71, 71, 71, 71, 71, 71, 71}, 
{12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 13, 12, 12, 12, 12, 12, 13, 14, 13, 12, 17, 30, 30, 30, 30, 30, 44, 51, 57, 61, 66, 71, 71, 71, 71, 71, 71, 71, 71, 71}, 
{12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 13, 12, 12, 12, 12, 12, 13, 14, 13, 12, 17, 30, 30, 30, 30, 30, 45, 51, 57, 61, 66, 71, 71, 71, 71, 71, 71, 71, 71, 71}, 
{13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 14, 13, 13, 17, 30, 30, 30, 30, 30, 45, 52, 57, 61, 67, 72, 72, 72, 72, 72, 72, 72, 72, 72}, 
{13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 14, 13, 13, 17, 30, 30, 30, 30, 30, 46, 53, 57, 62, 67, 73, 73, 73, 73, 73, 73, 73, 73, 73}, 
{13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 14, 13, 13, 17, 30, 30, 30, 30, 30, 46, 53, 57, 62, 67, 73, 73, 73, 73, 73, 73, 73, 73, 73}, 
{13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 14, 13, 13, 17, 30, 30, 30, 30, 30, 45, 52, 57, 61, 67, 72, 72, 72, 72, 72, 72, 72, 72, 72}, 
{12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 13, 12, 12, 12, 12, 12, 13, 14, 13, 12, 17, 30, 30, 30, 30, 30, 45, 51, 57, 61, 66, 71, 71, 71, 71, 71, 71, 71, 71, 71}, 
{12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 13, 12, 12, 12, 12, 12, 13, 14, 13, 12, 17, 30, 30, 30, 30, 30, 44, 51, 57, 61, 66, 71, 71, 71, 71, 71, 71, 71, 71, 71}, 
{12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 13, 12, 12, 12, 12, 12, 13, 14, 13, 12, 17, 30, 30, 30, 30, 30, 44, 51, 57, 61, 66, 71, 71, 71, 71, 71, 71, 71, 71, 71}, 
{12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 13, 12, 12, 12, 12, 12, 13, 14, 13, 12, 17, 30, 30, 30, 30, 30, 44, 51, 57, 61, 65, 70, 70, 70, 70, 70, 70, 70, 70, 70}, 
{12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 13, 12, 12, 12, 12, 12, 13, 14, 13, 12, 17, 30, 30, 30, 30, 30, 44, 51, 57, 61, 65, 70, 70, 70, 70, 70, 70, 70, 70, 70}, 
{11, 11, 11, 11, 11, 11, 11, 11, 11, 12, 12, 13, 12, 12, 11, 12, 12, 13, 14, 13, 11, 17, 30, 30, 30, 30, 30, 44, 51, 57, 61, 65, 70, 70, 70, 70, 70, 70, 70, 70, 70}, 
{12, 12, 12, 12, 12, 12, 12, 12, 12, 13, 14, 15, 14, 13, 13, 13, 14, 15, 16, 17, 14, 25, 40, 38, 37, 36, 36, 45, 52, 57, 61, 66, 70, 70, 70, 70, 70, 70, 70, 70, 70}, 
{12, 12, 12, 12, 12, 12, 12, 12, 12, 14, 15, 16, 15, 14, 14, 14, 15, 16, 18, 20, 17, 30, 45, 42, 41, 40, 40, 46, 53, 57, 62, 66, 70, 70, 70, 70, 70, 70, 70, 70, 70}, 
{12, 12, 12, 12, 12, 12, 12, 12, 12, 15, 16, 17, 16, 15, 15, 15, 16, 17, 19, 22, 19, 33, 49, 46, 44, 44, 44, 48, 53, 58, 62, 66, 71, 71, 71, 71, 71, 71, 71, 71, 71}, 
{12, 12, 12, 12, 12, 12, 12, 12, 12, 15, 17, 18, 17, 16, 16, 16, 17, 18, 20, 24, 21, 33, 51, 48, 46, 46, 46, 50, 54, 58, 62, 66, 71, 71, 71, 71, 71, 71, 71, 71, 71}, 
{12, 12, 12, 12, 12, 12, 12, 12, 12, 15, 18, 19, 18, 18, 18, 18, 18, 19, 21, 25, 23, 34, 53, 50, 48, 48, 48, 51, 55, 59, 62, 66, 71, 71, 71, 71, 71, 71, 71, 71, 71}, 
{13, 13, 13, 13, 13, 13, 13, 13, 13, 16, 18, 20, 19, 19, 19, 19, 19, 20, 21, 26, 25, 35, 53, 51, 50, 50, 50, 53, 56, 59, 63, 67, 72, 72, 72, 72, 72, 72, 72, 72, 72}, 
{13, 13, 13, 13, 13, 13, 13, 13, 13, 16, 19, 20, 20, 20, 20, 20, 20, 20, 21, 27, 27, 35, 53, 52, 52, 52, 52, 55, 57, 59, 63, 67, 73, 73, 73, 73, 73, 73, 73, 73, 73}, 
{13, 13, 13, 13, 13, 13, 13, 13, 13, 16, 19, 20, 21, 21, 21, 21, 21, 21, 21, 28, 28, 36, 53, 53, 53, 53, 53, 56, 58, 60, 63, 68, 74, 74, 74, 74, 74, 74, 74, 74, 74}, 
{13, 13, 13, 13, 13, 13, 13, 13, 13, 16, 18, 20, 22, 22, 22, 22, 22, 22, 21, 29, 30, 37, 53, 54, 55, 55, 55, 57, 60, 61, 64, 69, 75, 75, 75, 75, 75, 75, 75, 75, 75}, 
{12, 12, 12, 12, 12, 12, 12, 12, 12, 15, 18, 20, 23, 23, 23, 23, 23, 23, 21, 31, 32, 39, 53, 55, 57, 57, 57, 59, 61, 63, 65, 70, 76, 76, 76, 76, 76, 76, 76, 76, 76}, 
{12, 12, 12, 12, 12, 12, 12, 12, 12, 15, 18, 20, 23, 25, 25, 25, 24, 24, 22, 32, 34, 41, 54, 57, 58, 58, 58, 61, 63, 65, 67, 71, 78, 78, 78, 78, 78, 78, 78, 78, 78}, 
{12, 12, 12, 12, 12, 12, 12, 12, 12, 15, 18, 20, 23, 26, 26, 26, 26, 25, 24, 34, 36, 43, 56, 59, 60, 60, 60, 62, 64, 67, 69, 73, 80, 80, 80, 80, 80, 80, 80, 80, 80}, 
{12, 12, 12, 12, 12, 12, 12, 12, 12, 15, 18, 20, 23, 26, 27, 27, 27, 26, 25, 37, 39, 46, 59, 61, 62, 62, 62, 64, 67, 69, 72, 75, 83, 83, 83, 83, 83, 83, 83, 83, 83}, 
{12, 12, 12, 12, 12, 12, 12, 12, 12, 15, 18, 20, 23, 26, 29, 28, 28, 28, 27, 39, 41, 49, 63, 64, 64, 65, 65, 67, 69, 72, 75, 79, 87, 87, 87, 87, 87, 87, 87, 87, 87}, 
{11, 11, 11, 11, 11, 11, 11, 11, 11, 15, 18, 20, 23, 26, 30, 30, 30, 30, 30, 41, 45, 54, 68, 68, 68, 68, 68, 70, 73, 76, 79, 84, 91, 91, 91, 91, 91, 91, 91, 91, 91}, 
{12, 12, 12, 12, 12, 12, 12, 12, 12, 16, 19, 22, 25, 28, 31, 31, 32, 32, 32, 45, 48, 55, 69, 69, 69, 69, 69, 70, 73, 76, 79, 84, 91, 91, 91, 91, 91, 91, 91, 91, 91}, 
{12, 12, 12, 12, 12, 12, 12, 12, 12, 18, 20, 23, 26, 29, 33, 33, 33, 33, 34, 48, 51, 57, 71, 70, 70, 70, 70, 70, 73, 76, 79, 84, 91, 91, 91, 91, 91, 91, 91, 91, 91}, 
{12, 12, 12, 12, 12, 12, 12, 12, 12, 18, 22, 24, 27, 30, 34, 34, 34, 34, 36, 51, 53, 59, 72, 71, 71, 71, 71, 71, 73, 76, 79, 84, 91, 91, 91, 91, 91, 91, 91, 91, 91}, 
{12, 12, 12, 12, 12, 12, 12, 12, 12, 19, 23, 25, 28, 31, 35, 35, 35, 35, 37, 52, 55, 60, 73, 72, 72, 72, 72, 72, 73, 76, 79, 84, 91, 91, 91, 91, 91, 91, 91, 91, 91}, 
{12, 12, 12, 12, 12, 12, 12, 12, 12, 20, 24, 26, 29, 32, 35, 35, 35, 36, 38, 54, 56, 61, 75, 73, 73, 73, 73, 73, 73, 76, 79, 84, 91, 91, 91, 91, 91, 91, 91, 91, 91}, 
{13, 13, 13, 13, 13, 13, 13, 13, 13, 20, 24, 27, 30, 33, 36, 36, 36, 37, 38, 55, 58, 61, 75, 74, 74, 74, 74, 74, 74, 76, 79, 84, 91, 91, 91, 91, 91, 91, 91, 91, 91}, 
{13, 13, 13, 13, 13, 13, 13, 13, 13, 21, 25, 28, 30, 33, 37, 37, 37, 37, 38, 56, 59, 62, 75, 75, 75, 75, 75, 75, 75, 76, 79, 84, 91, 91, 91, 91, 91, 91, 91, 91, 91}, 
{13, 13, 13, 13, 13, 13, 13, 13, 13, 21, 25, 28, 31, 34, 38, 38, 38, 38, 38, 56, 60, 62, 75, 76, 76, 76, 76, 76, 76, 76, 79, 84, 91, 91, 91, 91, 91, 91, 91, 91, 91}, 
{13, 13, 13, 13, 13, 13, 13, 13, 13, 20, 24, 28, 31, 35, 39, 39, 39, 38, 38, 57, 61, 63, 75, 76, 78, 78, 78, 78, 78, 76, 79, 84, 91, 91, 91, 91, 91, 91, 91, 91, 91}, 
{12, 12, 12, 12, 12, 12, 12, 12, 12, 20, 24, 28, 32, 35, 40, 40, 40, 39, 38, 57, 62, 64, 75, 77, 79, 79, 79, 79, 79, 77, 79, 84, 91, 91, 91, 91, 91, 91, 91, 91, 91}, 
{12, 12, 12, 12, 12, 12, 12, 12, 12, 19, 23, 28, 32, 36, 40, 40, 40, 40, 38, 58, 63, 65, 76, 78, 80, 81, 81, 81, 80, 78, 80, 84, 91, 91, 91, 91, 91, 91, 91, 91, 91}, 
{12, 12, 12, 12, 12, 12, 12, 12, 12, 18, 23, 28, 32, 37, 41, 41, 41, 41, 39, 59, 64, 66, 77, 80, 82, 83, 83, 83, 82, 80, 82, 84, 91, 91, 91, 91, 91, 91, 91, 91, 91}, 
{12, 12, 12, 12, 12, 12, 12, 12, 12, 18, 23, 28, 32, 37, 42, 42, 42, 42, 41, 60, 65, 67, 80, 82, 84, 85, 85, 85, 84, 82, 84, 85, 91, 91, 91, 91, 91, 91, 91, 91, 91}, 
{12, 12, 12, 12, 12, 12, 12, 12, 12, 18, 23, 28, 32, 37, 44, 44, 43, 43, 43, 62, 67, 69, 83, 85, 86, 87, 88, 87, 86, 85, 86, 87, 91, 91, 91, 91, 91, 91, 91, 91, 91}, 
{11, 11, 11, 11, 11, 11, 11, 11, 11, 18, 23, 28, 32, 37, 45, 45, 45, 45, 45, 64, 68, 71, 89, 90, 90, 91, 91, 91, 90, 90, 90, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91}, 
{18, 18, 18, 18, 18, 18, 18, 18, 18, 22, 27, 31, 36, 41, 49, 49, 50, 51, 52, 65, 69, 71, 89, 90, 90, 91, 91, 91, 90, 90, 90, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91}, 
{23, 23, 23, 23, 23, 23, 23, 23, 23, 26, 31, 35, 39, 44, 53, 53, 54, 55, 57, 66, 70, 71, 89, 90, 90, 91, 91, 91, 90, 90, 90, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91}, 
{28, 28, 28, 28, 28, 28, 28, 28, 28, 30, 33, 37, 42, 47, 56, 56, 57, 58, 61, 67, 71, 71, 89, 90, 90, 90, 90, 90, 90, 90, 90, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91}, 
{32, 32, 32, 32, 32, 32, 32, 32, 32, 34, 36, 40, 44, 50, 59, 59, 59, 61, 64, 69, 72, 72, 89, 90, 90, 90, 90, 90, 90, 90, 90, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91}, 
{36, 36, 36, 36, 36, 36, 36, 36, 36, 37, 38, 42, 46, 52, 61, 61, 61, 63, 66, 70, 73, 73, 89, 90, 90, 90, 90, 90, 90, 90, 90, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91}, 
{39, 39, 39, 39, 39, 39, 39, 39, 39, 40, 41, 43, 47, 54, 63, 63, 63, 65, 68, 71, 74, 74, 89, 90, 90, 90, 90, 90, 90, 90, 90, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91}, 
{43, 43, 43, 43, 43, 43, 43, 43, 43, 43, 44, 44, 49, 56, 66, 66, 66, 66, 70, 72, 75, 75, 89, 90, 90, 90, 90, 90, 90, 90, 90, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91}, 
{46, 46, 46, 46, 46, 46, 46, 46, 46, 46, 46, 46, 51, 57, 67, 67, 67, 68, 71, 72, 76, 76, 90, 90, 90, 90, 90, 90, 90, 90, 90, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91}, 
{48, 48, 48, 48, 48, 48, 48, 48, 48, 49, 49, 49, 52, 58, 67, 67, 67, 69, 72, 73, 76, 77, 90, 90, 90, 90, 90, 90, 90, 90, 90, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91}, 
{51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 52, 53, 59, 68, 68, 68, 70, 73, 74, 76, 77, 90, 90, 90, 90, 90, 90, 90, 90, 90, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91}, 
{54, 54, 54, 54, 54, 54, 54, 54, 54, 53, 53, 54, 55, 60, 68, 68, 69, 71, 74, 75, 77, 78, 90, 90, 90, 90, 90, 90, 90, 90, 90, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91}, 
{56, 56, 56, 56, 56, 56, 56, 56, 56, 56, 56, 57, 58, 61, 69, 69, 70, 71, 75, 75, 77, 78, 90, 90, 90, 90, 90, 90, 90, 90, 90, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91}, 
{58, 58, 58, 58, 58, 58, 58, 58, 58, 58, 58, 59, 60, 62, 69, 69, 70, 72, 75, 76, 77, 79, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91}, 
{61, 61, 61, 61, 61, 61, 61, 61, 61, 60, 61, 61, 63, 65, 70, 70, 71, 73, 76, 76, 77, 79, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91}, 
{63, 63, 63, 63, 63, 63, 63, 63, 63, 63, 63, 64, 65, 67, 70, 70, 71, 73, 76, 76, 78, 80, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91}, 
{63, 63, 63, 63, 63, 63, 63, 63, 63, 63, 63, 64, 65, 67, 70, 70, 71, 73, 76, 76, 78, 80, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91}, 
{63, 63, 63, 63, 63, 63, 63, 63, 63, 63, 63, 64, 65, 67, 70, 70, 71, 73, 76, 76, 78, 80, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91}, 
{63, 63, 63, 63, 63, 63, 63, 63, 63, 63, 63, 64, 65, 67, 70, 70, 71, 73, 76, 76, 78, 80, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91}, 
{63, 63, 63, 63, 63, 63, 63, 63, 63, 63, 63, 64, 65, 67, 70, 70, 71, 73, 76, 76, 78, 80, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91}, 
{63, 63, 63, 63, 63, 63, 63, 63, 63, 63, 63, 64, 65, 67, 70, 70, 71, 73, 76, 76, 78, 80, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91}, 
{63, 63, 63, 63, 63, 63, 63, 63, 63, 63, 63, 64, 65, 67, 70, 70, 71, 73, 76, 76, 78, 80, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91}, 
{63, 63, 63, 63, 63, 63, 63, 63, 63, 63, 63, 64, 65, 67, 70, 70, 71, 73, 76, 76, 78, 80, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91}, 
{63, 63, 63, 63, 63, 63, 63, 63, 63, 63, 63, 64, 65, 67, 70, 70, 71, 73, 76, 76, 78, 80, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91}, 
{63, 63, 63, 63, 63, 63, 63, 63, 63, 63, 63, 64, 65, 67, 70, 70, 71, 73, 76, 76, 78, 80, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91}, 
{63, 63, 63, 63, 63, 63, 63, 63, 63, 63, 63, 64, 65, 67, 70, 70, 71, 73, 76, 76, 78, 80, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91}, 
{63, 63, 63, 63, 63, 63, 63, 63, 63, 63, 63, 64, 65, 67, 70, 70, 71, 73, 76, 76, 78, 80, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91}, 
{63, 63, 63, 63, 63, 63, 63, 63, 63, 63, 63, 64, 65, 67, 70, 70, 71, 73, 76, 76, 78, 80, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91}, 
{63, 63, 63, 63, 63, 63, 63, 63, 63, 63, 63, 64, 65, 67, 70, 70, 71, 73, 76, 76, 78, 80, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91}, 
{63, 63, 63, 63, 63, 63, 63, 63, 63, 63, 63, 64, 65, 67, 70, 70, 71, 73, 76, 76, 78, 80, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91}, 
{63, 63, 63, 63, 63, 63, 63, 63, 63, 63, 63, 64, 65, 67, 70, 70, 71, 73, 76, 76, 78, 80, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91}, 
{63, 63, 63, 63, 63, 63, 63, 63, 63, 63, 63, 64, 65, 67, 70, 70, 71, 73, 76, 76, 78, 80, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91}, 
{63, 63, 63, 63, 63, 63, 63, 63, 63, 63, 63, 64, 65, 67, 70, 70, 71, 73, 76, 76, 78, 80, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91}, 
{63, 63, 63, 63, 63, 63, 63, 63, 63, 63, 63, 64, 65, 67, 70, 70, 71, 73, 76, 76, 78, 80, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91}, 
{63, 63, 63, 63, 63, 63, 63, 63, 63, 63, 63, 64, 65, 67, 70, 70, 71, 73, 76, 76, 78, 80, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91}, 
{63, 63, 63, 63, 63, 63, 63, 63, 63, 63, 63, 64, 65, 67, 70, 70, 71, 73, 76, 76, 78, 80, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91}, 
{63, 63, 63, 63, 63, 63, 63, 63, 63, 63, 63, 64, 65, 67, 70, 70, 71, 73, 76, 76, 78, 80, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91}, 
{63, 63, 63, 63, 63, 63, 63, 63, 63, 63, 63, 64, 65, 67, 70, 70, 71, 73, 76, 76, 78, 80, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91}, 
{63, 63, 63, 63, 63, 63, 63, 63, 63, 63, 63, 64, 65, 67, 70, 70, 71, 73, 76, 76, 78, 80, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91}, 
{63, 63, 63, 63, 63, 63, 63, 63, 63, 63, 63, 64, 65, 67, 70, 70, 71, 73, 76, 76, 78, 80, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91}, 
{63, 63, 63, 63, 63, 63, 63, 63, 63, 63, 63, 64, 65, 67, 70, 70, 71, 73, 76, 76, 78, 80, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91}, 
{63, 63, 63, 63, 63, 63, 63, 63, 63, 63, 63, 64, 65, 67, 70, 70, 71, 73, 76, 76, 78, 80, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91}, 
{63, 63, 63, 63, 63, 63, 63, 63, 63, 63, 63, 64, 65, 67, 70, 70, 71, 73, 76, 76, 78, 80, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91}, 
{63, 63, 63, 63, 63, 63, 63, 63, 63, 63, 63, 64, 65, 67, 70, 70, 71, 73, 76, 76, 78, 80, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91}, 
{63, 63, 63, 63, 63, 63, 63, 63, 63, 63, 63, 64, 65, 67, 70, 70, 71, 73, 76, 76, 78, 80, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91}, 
{63, 63, 63, 63, 63, 63, 63, 63, 63, 63, 63, 64, 65, 67, 70, 70, 71, 73, 76, 76, 78, 80, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91}
};

// Declare your global variables here
#define HX711_DOUT    PIND.3
#define HX711_SCK     PORTD.2
#define HX_COMPLEMENT 0x800000
#define HX_REF        0.360598//360.91549

#define HX711_DOUT1    PIND.5
#define HX711_SCK1     PORTD.4
#define HX_REF1        0.360598//9.33 

#define HX711_DOUT2    PINB.1
#define HX711_SCK2     PORTB.0
#define HX_REF2        0.360598//9.33 


void sort_array(float a[], int size);
unsigned long HX711_Read(void);
void get_scale(void);
void get_offset(void);
unsigned long HX711_Read1(void);
//void get_scale1(void);
void get_offset1(void);
unsigned long HX711_Read2(void);
//void get_scale2(void);
void get_offset2(void);

long Weight_Shiwu = 0,Weight_Maopi =0,Weight_Shiwu1 = 0,Weight_Maopi1 =0,Weight_Shiwu2 = 0,Weight_Maopi2 =0; 
long HX711_Buffer = 0,HX711_Buffer1 = 0,HX711_Buffer2 = 0;
float         scale_total = 0,scale_total_prev = 0 ,scale = 0,scale1 = 0,scale2 = 0,scale_sum = 0,scale1_sum = 0,scale2_sum = 0,scale_array[7],scale1_array[7],scale2_array[7];

unsigned int measure_counter;//,pw=75,i,i2,i3,sw=0,i2;
unsigned char  buffer1[12];
unsigned char  buffer2[12];
unsigned char  buffer3[12];
unsigned char  buffer4[4];
char  buffer[32];
char  str1[8];
char  str2[8];
char  str3[8];
char  str4[8];


void send( unsigned char Data1[4]);//, unsigned char Data2[12], unsigned char Data3[12]);
unsigned char buffer1[12];
#define DATA_REGISTER_EMPTY (1<<UDRE)
#define RX_COMPLETE (1<<RXC)
#define FRAMING_ERROR (1<<FE)
#define PARITY_ERROR (1<<UPE)
#define DATA_OVERRUN (1<<DOR)

// USART Receiver buffer
#define RX_BUFFER_SIZE 24
char rx_buffer[RX_BUFFER_SIZE];

#if RX_BUFFER_SIZE <= 256
unsigned char rx_wr_index=0,rx_rd_index=0;
#else
unsigned int rx_wr_index=0,rx_rd_index=0;
#endif

#if RX_BUFFER_SIZE < 256
unsigned char rx_counter=0;
#else
unsigned int rx_counter=0;
#endif

// This flag is set on USART Receiver buffer overflow
bit rx_buffer_overflow;

// USART Receiver interrupt service routine
interrupt [USART_RXC] void usart_rx_isr(void)
{
char status,data;
status=UCSRA;
data=UDR;
if ((status & (FRAMING_ERROR | PARITY_ERROR | DATA_OVERRUN))==0)
   {
   rx_buffer[rx_wr_index++]=data;
#if RX_BUFFER_SIZE == 256
   // special case for receiver buffer size=256
   if (++rx_counter == 0) rx_buffer_overflow=1;
#else
   if (rx_wr_index == RX_BUFFER_SIZE) rx_wr_index=0;
   if (++rx_counter == RX_BUFFER_SIZE)
      {
      rx_counter=0;
      rx_buffer_overflow=1;
      }
#endif
   }
}

#ifndef _DEBUG_TERMINAL_IO_
// Get a character from the USART Receiver buffer
#define _ALTERNATE_GETCHAR_
#pragma used+
char getchar(void)
{
char data;
while (rx_counter==0);
data=rx_buffer[rx_rd_index++];
#if RX_BUFFER_SIZE != 256
if (rx_rd_index == RX_BUFFER_SIZE) rx_rd_index=0;
#endif
#asm("cli")
--rx_counter;
#asm("sei")
return data;
}
#pragma used-
#endif


// External Interrupt 2 service routine
interrupt [EXT_INT2] void ext_int2_isr(void)
{
// Place your code here
send(buffer4);
}


interrupt [TIM1_OVF] void timer1_ovf_isr(void)
{
int x,y;
// Reinitialize Timer1 value
TCNT1H=0xBDC >> 8;
TCNT1L=0xBDC & 0xff;
// Place your code here
scale_total = scale_total / measure_counter;
x = (int)scale_total ;
if(x>100)x=100;
if(x<0)x=0;                                  
y = (scale_total - scale_total_prev + 100) / 5;
scale_total_prev = scale_total;
OCR2 = (int)fuzzy[x][y]*2.55;
itoa(OCR2,str1);
sprintf(buffer4,"%s",str1);
        
//send(buffer4); 
sprintf(buffer,"%4.1fg %4.1fg %4.1fg %4.1fg %i",scale,scale1,scale2,scale_total,OCR2);//sprintf(buffer," %ld ",Weight_Shiwu);//sprintf(buffer," %1.3f --> Gram",scale);
lcd_clear();  
lcd_gotoxy(0,0);
lcd_puts(buffer); 
//delay_ms(10);
measure_counter = 0;        
}


void main(void)
{

//IO Pins/Ports :
PORTA=0x00;
DDRA=0x00;

// Port B initialization
// Function: Bit7=In Bit6=In Bit5=In Bit4=In Bit3=In Bit2=In Bit1=In Bit0=Out 
DDRB=(0<<DDB7) | (0<<DDB6) | (0<<DDB5) | (0<<DDB4) | (0<<DDB3) | (0<<DDB2) | (0<<DDB1) | (1<<DDB0);
// State: Bit7=T Bit6=T Bit5=T Bit4=T Bit3=T Bit2=T Bit1=T Bit0=0 
PORTB=(0<<PORTB7) | (0<<PORTB6) | (0<<PORTB5) | (0<<PORTB4) | (0<<PORTB3) | (1<<PORTB2) | (0<<PORTB1) | (0<<PORTB0);

// Port C initialization
// Function: Bit7=In Bit6=In Bit5=In Bit4=In Bit3=In Bit2=In Bit1=Out Bit0=Out 
DDRC=(0<<DDC7) | (0<<DDC6) | (0<<DDC5) | (0<<DDC4) | (0<<DDC3) | (0<<DDC2) | (1<<DDC1) | (1<<DDC0);
// State: Bit7=T Bit6=T Bit5=T Bit4=T Bit3=T Bit2=T Bit1=0 Bit0=0 
PORTC=(0<<PORTC7) | (0<<PORTC6) | (0<<PORTC5) | (0<<PORTC4) | (0<<PORTC3) | (0<<PORTC2) | (0<<PORTC1) | (0<<PORTC0);

// Port D initialization
// Function: Bit7=In Bit6=In Bit5=In Bit4=Out Bit3=In Bit2=Out Bit1=Out Bit0=In 
DDRD=(1<<DDD7) | (0<<DDD6) | (0<<DDD5) | (1<<DDD4) | (0<<DDD3) | (1<<DDD2) | (1<<DDD1) | (0<<DDD0);
// State: Bit7=T Bit6=T Bit5=T Bit4=0 Bit3=T Bit2=0 Bit1=0 Bit0=T 
PORTD=(0<<PORTD7) | (0<<PORTD6) | (0<<PORTD5) | (0<<PORTD4) | (0<<PORTD3) | (0<<PORTD2) | (0<<PORTD1) | (0<<PORTD0);

// USART initialization
// Communication Parameters: 8 Data, 1 Stop, No Parity
// USART Receiver: On
// USART Transmitter: On
// USART Mode: Asynchronous
// USART Baud Rate: 9600
UCSRA=(0<<RXC) | (0<<TXC) | (0<<UDRE) | (0<<FE) | (0<<DOR) | (0<<UPE) | (0<<U2X) | (0<<MPCM);
UCSRB=(1<<RXCIE) | (0<<TXCIE) | (0<<UDRIE) | (1<<RXEN) | (1<<TXEN) | (0<<UCSZ2) | (0<<RXB8) | (0<<TXB8);
UCSRC=(1<<URSEL) | (0<<UMSEL) | (0<<UPM1) | (0<<UPM0) | (0<<USBS) | (1<<UCSZ1) | (1<<UCSZ0) | (0<<UCPOL);
UBRRH=0x00;
UBRRL=0x33;

get_offset();
get_offset1();
get_offset2();

 
lcd_init(16);
lcd_putsf("Doctor naderi");
delay_ms(1000);


// Timer/Counter 1 initialization
// Clock source: System Clock
// Clock value: 125.000 kHz
// Mode: Normal top=0xFFFF
// OC1A output: Disconnected
// OC1B output: Disconnected
// Noise Canceler: Off
// Input Capture on Falling Edge
// Timer Period: 0.5 s
// Timer1 Overflow Interrupt: On
// Input Capture Interrupt: Off
// Compare A Match Interrupt: Off
// Compare B Match Interrupt: Off
TCCR1A=(0<<COM1A1) | (0<<COM1A0) | (0<<COM1B1) | (0<<COM1B0) | (0<<WGM11) | (0<<WGM10);
TCCR1B=(0<<ICNC1) | (0<<ICES1) | (0<<WGM13) | (0<<WGM12) | (0<<CS12) | (1<<CS11) | (1<<CS10);
TCNT1H=0x0B;
TCNT1L=0xDC;
ICR1H=0x00;
ICR1L=0x00;
OCR1AH=0x00;
OCR1AL=0x00;
OCR1BH=0x00;
OCR1BL=0x00;


// Timer/Counter 2 initialization
// Clock source: System Clock
// Clock value: 31.250 kHz
// Mode: Fast PWM top=0xFF
// OC2 output: Non-Inverted PWM
// Timer Period: 8.192 ms
// Output Pulse(s):
// OC2 Period: 8.192 ms Width: 0 us
ASSR=0<<AS2;
TCCR2=(1<<PWM2) | (1<<COM21) | (0<<COM20) | (1<<CTC2) | (1<<CS22) | (1<<CS21) | (0<<CS20);
TCNT2=0x00;
OCR2=80;

// Timer(s)/Counter(s) Interrupt(s) initialization
TIMSK=(0<<OCIE2) | (0<<TOIE2) | (0<<TICIE1) | (0<<OCIE1A) | (0<<OCIE1B) | (1<<TOIE1) | (0<<OCIE0) | (0<<TOIE0);



// External Interrupt(s) initialization
// INT0: Off
// INT1: Off
// INT2: On
// INT2 Mode: Falling Edge
GICR|=(0<<INT1) | (0<<INT0) | (1<<INT2);
MCUCR=(0<<ISC11) | (0<<ISC10) | (0<<ISC01) | (0<<ISC00);
MCUCSR=(0<<ISC2);
GIFR=(0<<INTF1) | (0<<INTF0) | (1<<INTF2);


// Global enable interrupts
#asm("sei")


while (1)
      { 
        get_scale(); 
        /*ftoa(scale,2,str1);  
        sprintf(buffer1,"%s",str1); 
        ftoa(scale1,2,str2);  
        sprintf(buffer2,"%s",str2);
        ftoa(scale2,2,str3);  
        sprintf(buffer3,"%s",str3);*/
        scale_total += scale + scale1 + scale2;// + scale1 + scale2;
        measure_counter += 1;        
      }
}

//Read from HX711 Modules :
unsigned long HX711_Read(void)
{
    unsigned long count;
    unsigned char i;

      HX711_DOUT=1;
      HX711_SCK=0;
      count=0;
      while(HX711_DOUT);
      for(i=0;i<24;i++)  //gain --> 128
        {
            HX711_SCK=1;
            count=count<<1;
            HX711_SCK=0;
            if(HX711_DOUT)
                count++;
        }
       HX711_SCK=1;
      count=count ^ HX_COMPLEMENT;//2,s Complement
      HX711_SCK=0;
      return(count);
}
unsigned long HX711_Read1(void)
{
    unsigned long count;
    unsigned char i;

      HX711_DOUT1=1;
      HX711_SCK1=0;
      count=0;
      while(HX711_DOUT1);
      for(i=0;i<24;i++)  //gain --> 128
        {
            HX711_SCK1=1;
            count=count<<1;
            HX711_SCK1=0;
            if(HX711_DOUT1)
                count++;
        }
       HX711_SCK1=1;
      count=count ^ HX_COMPLEMENT;//2,s Complement
      HX711_SCK1=0;
      return(count);
}
unsigned long HX711_Read2(void)
{
    unsigned long count;
    unsigned char i;

      HX711_DOUT2=1;
      HX711_SCK2=0;
      count=0;
      while(HX711_DOUT2);
      for(i=0;i<24;i++)  //gain --> 128
        {
            HX711_SCK2=1;
            count=count<<1;
            HX711_SCK2=0;
            if(HX711_DOUT2)
                count++;
        }
       HX711_SCK2=1;
      count=count ^ HX_COMPLEMENT;//2,s Complement
      HX711_SCK2=0;
      return(count);
}

void get_scale(void)
{
    unsigned int i;   
    unsigned int j;
    scale_sum = 0;    
    scale1_sum = 0;
    scale2_sum = 0;
    
    for(j=0; j<2 ;j++){
    for(i=0 ; i<7 ;i++){


    HX711_Buffer = 0;
    HX711_Buffer = HX711_Read();
    HX711_Buffer = HX711_Buffer/100;
        Weight_Shiwu = HX711_Buffer;
        Weight_Shiwu = Weight_Shiwu - Weight_Maopi;
        
        Weight_Shiwu = ((float)Weight_Shiwu/HX_REF);

        scale_array[i] = (float)Weight_Shiwu/30;


    HX711_Buffer1 = 0;
    HX711_Buffer1 = HX711_Read1();
    HX711_Buffer1 = HX711_Buffer1/100;
        Weight_Shiwu1 = HX711_Buffer1;
        Weight_Shiwu1 = Weight_Shiwu1 - Weight_Maopi1;
        
        Weight_Shiwu1 = ((float)Weight_Shiwu1/HX_REF);

        scale1_array[i] = (float)Weight_Shiwu1/30;


    HX711_Buffer2 = 0;
    HX711_Buffer2 = HX711_Read2();
    HX711_Buffer2 = HX711_Buffer2/100;
        Weight_Shiwu2 = HX711_Buffer2;
        Weight_Shiwu2 = Weight_Shiwu2 - Weight_Maopi2;

        Weight_Shiwu2 = ((float)Weight_Shiwu2/HX_REF);

        scale2_array[i] = (float)Weight_Shiwu2/30;
    }
    sort_array(scale_array,7);
    scale_sum += scale_array[3];
    sort_array(scale1_array,7);
    scale1_sum += scale1_array[3];
    sort_array(scale2_array,7);
    scale2_sum += scale2_array[3];
    }
    scale = scale_sum/2;
    scale1 = scale1_sum/2;
    scale2 = scale2_sum/2;
}

//get offsets :

void get_offset(void)
{
    int i=0;

    HX711_Buffer = 0;

   	for( i = 0 ; i < 10 ; i++)
	{
		HX711_Buffer = HX711_Buffer + HX711_Read();
        delay_us(500);
	}
    HX711_Buffer = HX711_Buffer /10;     
    Weight_Maopi = HX711_Buffer/100;
}
void get_offset1(void)
{
    int i=0;

    HX711_Buffer1 = 0;

   	for( i = 0 ; i < 10 ; i++)
	{
		HX711_Buffer1 = HX711_Buffer1 + HX711_Read1();
        delay_us(500);
	}
    HX711_Buffer1 = HX711_Buffer1 /10; 
    Weight_Maopi1 = HX711_Buffer1/100;
}
void get_offset2(void)
{
    int i=0;

    HX711_Buffer2 = 0;

   	for( i = 0 ; i < 10 ; i++)
	{
		HX711_Buffer2 = HX711_Buffer2 + HX711_Read2();
        delay_us(500);
	}
    HX711_Buffer2 = HX711_Buffer2 /10;   
    Weight_Maopi2 = HX711_Buffer2/100;
}

void send( unsigned char Data1[4])//, unsigned char Data2[12], unsigned char Data3[12])//,unsigned char Data2[12] )
{
puts(Data1);
/*
putchar(',');
puts(Data2);
putchar(',');
puts(Data3);*/
}



void sort_array(float a[], int size) {
    int i=0;
    int o=0;
    for(i=0; i<(size-1); i++) {
        for(o=0; o<(size-(i+1)); o++) {
                if(a[o] > a[o+1]) {
                    int t = a[o];
                    a[o] = a[o+1];
                    a[o+1] = t;
                }
        }
    }
}