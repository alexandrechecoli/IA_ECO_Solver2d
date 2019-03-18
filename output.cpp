#include "output.h"

output::output()
{

}

void output::set_eco_connection(Eco2dSolver *xec)
{
    this-> ec = xec;
}

void output::print_eco_data(string xfile_path)
{
    std::ofstream o(xfile_path.c_str());

    std::list<Eco2dSolver::State>::iterator it = ec->l_states.begin();

    while(it != ec->l_states.end())
    {
        o << "Rect. Action : " << it->rect_action << "\n";
        o << "Unsatisfied Rects : " << it->n_unsatisfied_rects << "\n";
        for(size_t i = 0; i < it->v_rects.size(); i++)
        {
            o << "Rect. " << i << " [(" << it->v_rects[i].current_coords.x << "," << it->v_rects[i].current_coords.y <<"),(" <<
                 it->v_rects[i].objective_coords.x << "," << it->v_rects[i].objective_coords.y << ")] S_" << it->v_rects[i].is_satisfied
              << " D_" << it->v_rects[i].damaged << "\n";
        }
        o << "-------------------------------------------- \n";
        it++;
    }
    o.close();


}
