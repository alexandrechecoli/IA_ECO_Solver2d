#ifndef OUTPUT_H
#define OUTPUT_H
#include "eco2dsolver.h"
#include <iostream>
#include <fstream>

class output
{
public:
    output();
    output(Eco2dSolver * xec);
    void set_eco_connection(Eco2dSolver * xec);
    void print_eco_data(std::string xfile_path);

private:
    Eco2dSolver *ec;
};

#endif // OUTPUT_H
