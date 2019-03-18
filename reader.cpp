#include "reader.h"

reader::reader()
{

}

reader::reader(Eco2dSolver * xec)
{
    set_eco_connection(xec);
}

void reader::set_eco_connection(Eco2dSolver *xec)
{
    this->ec = xec;
}

void reader::load_eco_data(std::string xfile_path)
{
    std::ifstream reader(xfile_path.c_str());
    string trash;
    size_t n_rects;

    //Container information
    reader >> trash;
    reader >> ec->container_l;
    reader >> ec->container_w;
    ec->container_x = 0;
    ec->container_y = 0;

    //Small rects
    reader >> trash;
    reader >> n_rects;

    ec->v_rects = vector<Eco2dSolver::RectInfo>(n_rects);
    reader >> trash;
    for(size_t i = 0; i < n_rects ;i++)
    {
        reader >> ec->v_rects[i].current_coords.x;
        reader >> ec->v_rects[i].current_coords.y;
        reader >> ec->v_rects[i].current_coords.l;
        reader >> ec->v_rects[i].current_coords.w;
        reader >> ec->v_rects[i].objective_coords.x;
        reader >> ec->v_rects[i].objective_coords.y;
        reader >> ec->v_rects[i].objective_coords.l;
        reader >> ec->v_rects[i].objective_coords.w;

        if(ec->v_rects[i].current_coords == ec->v_rects[i].objective_coords)
        {
            ec->v_rects[i].is_satisfied = true;
        }else
        {
            ec->v_rects[i].is_satisfied = false;
            ec->n_unsatisfied_rects += 1;
        }
        ec->v_rects[i].damaged = false;

    }

    reader.close();

}
