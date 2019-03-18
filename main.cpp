#include <iostream>
#include "eco2dsolver.h"
#include "reader.h"
#include "output.h"
#include <map>
using namespace std;

int main()
{

    Eco2dSolver es;
    reader r;
    output o;
    r.set_eco_connection(&es);

    r.load_eco_data("C://Users//user//Documents//Alexandre//C++//IA//ECO_2D_SOLVER//input.txt");
    es.find_solution();
    o.set_eco_connection(&es);
    o.print_eco_data("C://Users//user//Documents//Alexandre//C++//IA//ECO_2D_SOLVER//output.txt");

    cout << "Hello World!" << endl;
    return 0;
}
