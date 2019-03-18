#ifndef ECO2DSOLVER_H
#define ECO2DSOLVER_H

#include <vector>
#include <list>
#include <map>
#include <math.h>
#include <limits>

const double EPS = 1e-2;
const long int MAX_INT = std::numeric_limits<int>::max();

using namespace  std;
class reader;
class output;

class Eco2dSolver
{
    friend reader;
    friend output;

public:
    Eco2dSolver();
    void find_solution();

    struct  Coords
    {
        Coords(long int xx,long int xy,long int xl, long int xw)
        {
           x = xx;
           y = xy;
           l = xl;
           w = xw;
        }
        Coords(){}
        bool operator == (Coords &c)
        {
            if((this->x == c.x) && (this->y == c.y) && (this->l == c.l) && (this->w == c.w))
                return true;
            else return false;
        }

        long int x;
        long int y;
        long int l;
        long int w;
    };

    struct RectInfo
    {
        int n_satisfaction_moves;
        Coords objective_coords;
        Coords current_coords;
        bool damaged;
        bool is_satisfied;
        map<size_t,Coords> restrictions; //<box that put the rstricton, list of restrictions of that box>
    };

    struct State
    {
       int n_unsatisfied_rects;
       size_t rect_action;
       vector<RectInfo> v_rects;
    };

    std::list<State> l_states;

    long int container_x;
    long int container_y;
    long int container_l;
    long int container_w;

protected:


    vector<RectInfo> v_rects;


    int n_unsatisfied_rects = 0;

    void evaluate_rect(size_t xrect_number);
    void move_to_satisfaction(size_t xrect_number);
    void move_random(size_t xrect_number);
    void remove_constraints(size_t xrect_number);
    void move_up(size_t xrect_number, int xdelta_y_objective);
    void move_down(size_t xrect_number, int xdelta_y_objective);
    void move_left(size_t xrect_number, int xdelta_x_objective);
    void move_right(size_t xrect_number, int xdelta_x_objective);
    std::pair<int, int> get_intersection_limits(long int xt1,long int xa1,long int xt2,long int xa2);
    void damage_rect(size_t xcurrent_rect, size_t xdamaged_rect);
    void get_max_delta_y_up_rects(size_t xcurrent_rect, int &xmax_delta_y, size_t &xmax_delta_y_rect);
    void get_max_delta_y_up_rect_constraints(size_t xcurrent_rect, int &xmax_delta_y, size_t &xmax_delta_y_rect);
    void get_min_delta_y_down_rects(size_t xcurrent_rect, int &xmin_delta_y, size_t &xmin_delta_y_rect);
    void get_min_delta_y_down_rect_constraints(size_t xcurrent_rect,int &xmin_delta_y, size_t &xmin_delta_y_rect);
    void get_max_delta_x_right_rect(size_t xcurrent_rect, int &xmax_delta_x, size_t &xmax_delta_x_rect);
    void get_max_delta_x_right_rect_constraints(size_t xcurrent_rect, int &xmax_delta_x, size_t &xmax_delta_x_rect);
    void get_min_delta_x_left_rect(size_t xcurrent_rect, int &xmin_delta_x, size_t &xmin_delta_x_rect);
    void get_min_delta_x_left_rect_constraints(size_t xcurrent_rect, int &xmin_delta_x, size_t &xmin_delta_x_rect);
    int get_int_random_between(int xlower, int xupper);
};
#endif // ECO2DSOLVER_H
