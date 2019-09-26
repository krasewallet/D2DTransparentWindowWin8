#pragma once
#include <Windows.h>
#include <functional>

using namespace std;

typedef function<void(int type,int x,int y)> MOUSEHANDLE;
typedef function<void()> UPDATEHANDLE;