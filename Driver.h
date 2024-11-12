#include <iostream>
#include <fstream>
#include <cstring>
#include <unistd.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <vector>


using namespace std;

#define VINS_MONO 1
#define VINS_fusion 2
#define SFM_2 3
#define SFM_3 4