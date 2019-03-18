#ifndef READER_H
#define READER_H

#include <iostream>
#include <fstream>
#include <string>
#include <vector>

#include "eco2dsolver.h"

class reader
{
public:
    reader();
    reader(Eco2dSolver * xec);
    void set_eco_connection(Eco2dSolver * xec);
    void load_eco_data(std::string xfile_path);

private:
    Eco2dSolver * ec;
};

#endif // READER_H
