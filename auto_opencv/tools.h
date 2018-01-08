#pragma once

#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <fstream>
#include <string>

#define DLLEXPORT extern "C" __declspec(dllexport)



DLLEXPORT void FindChess();

DLLEXPORT void FindNextBlock();