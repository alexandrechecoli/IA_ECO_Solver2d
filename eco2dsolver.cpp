#include "eco2dsolver.h"
#include <iostream>

Eco2dSolver::Eco2dSolver()
{

}

void Eco2dSolver::find_solution()
{
    State s;
    s.v_rects = v_rects;
    s.rect_action = 0;
    s.n_unsatisfied_rects = n_unsatisfied_rects;
    l_states.push_back(s);
    size_t rects_action_turn = 0;
    while(n_unsatisfied_rects != 0)
    {

        State s1;
        rects_action_turn = rects_action_turn % v_rects.size();
        std::cout << v_rects[rects_action_turn].objective_coords.x << v_rects[rects_action_turn].objective_coords.y << "\n";
        evaluate_rect(rects_action_turn);
        s1.v_rects = v_rects;
        s1.n_unsatisfied_rects = n_unsatisfied_rects;
        s1.rect_action = rects_action_turn;
        l_states.push_back(s1);
        rects_action_turn ++;
    }

    std::list<Eco2dSolver::State>::iterator it = l_states.begin();
    while(it != l_states.end())
    {
        std::cout << "Rect. Action : " << it->rect_action << "\n";
        std::cout << "Unsatisfied Rects : " << it->n_unsatisfied_rects << "\n";
        for(size_t i = 0; i < it->v_rects.size(); i++)
        {
            std::cout << "Rect. " << i << " [(" << it->v_rects[i].current_coords.x << "," << it->v_rects[i].current_coords.y <<"),(" <<
                 it->v_rects[i].objective_coords.x << "," << it->v_rects[i].objective_coords.y << ")] S_" << it->v_rects[i].is_satisfied
              << " D_" << it->v_rects[i].damaged << "\n";
        }
        std::cout << "-------------------------------------------- \n";
        it++;
    }
}

void Eco2dSolver::evaluate_rect(size_t xrect_number)
{
    if(!v_rects[xrect_number].is_satisfied) //If the rect is NOT satisfied
    {
        if(v_rects[xrect_number].n_satisfaction_moves < 2)
        {
          v_rects[xrect_number].n_satisfaction_moves ++;
          move_to_satisfaction( xrect_number );
        }
        else
        {
          move_random(xrect_number);
          v_rects[xrect_number].n_satisfaction_moves = 0;
        }

        if(v_rects[xrect_number].is_satisfied)
        {
            v_rects[xrect_number].damaged = false;
            remove_constraints(xrect_number);
            n_unsatisfied_rects --;
        }
    }
    else // if the rect is satisfied
    {
        if(v_rects[xrect_number].damaged) //and damaged
        {
          move_random(xrect_number); // after the move, it became unsatisfied
          if(!v_rects[xrect_number].is_satisfied)
          {
              n_unsatisfied_rects ++;
          }
        }
    }
}

void Eco2dSolver::move_to_satisfaction(size_t xrect_number)
{
    // calculate objective
    // try to move towards object
      // if intercepted, hurt the other and leave a constraint
      // if not intercepted, move

    int delta_x_objective = v_rects[ xrect_number ].current_coords.x - v_rects[ xrect_number ].objective_coords.x;
    int delta_y_objective = v_rects[ xrect_number ].current_coords.y - v_rects[ xrect_number ].objective_coords.y;

    if(abs(delta_x_objective) > abs(delta_y_objective)) //move in horizontal direction
    {
        if(delta_x_objective < 0)
        {
            move_right(xrect_number, delta_x_objective);
        }else
        {
            move_left(xrect_number, delta_x_objective);
        }
    }else //move in vertical direction
    {
        if(delta_y_objective < 0)
        {
            move_up(xrect_number, delta_y_objective);
        }else
        {
            move_down(xrect_number, delta_y_objective);
        }
    }

}

void Eco2dSolver::move_random(size_t xrect_number)
{
    // try to move randomly (of 4 possibilities
      // if intercepted, hurt the other and leave a constraint
    // if not intercepted, move

    int random_move = get_int_random_between(0,3);


    switch (random_move)
    {
    case 0: //move_right
        move_up(xrect_number, -MAX_INT );
        break;
    case 1:
        move_down(xrect_number, MAX_INT );
        break;
    case 2:
         move_right(xrect_number, -MAX_INT );
        break;
    case 3:
         move_left(xrect_number, MAX_INT );
        break;
    }

}

void Eco2dSolver::remove_constraints(size_t xrect_number)
{
    /*
     * Removes all the constraints that the xrect_number rects has
     * put on all other rects
     */

    for(size_t i = 0; i < v_rects.size() ; i++)
    {
        v_rects[i].restrictions.erase(xrect_number);
    }
}

void Eco2dSolver::move_up(size_t xrect_number, int xdelta_y_objective)
{
    int max_delta_y_rects = -MAX_INT;
    size_t max_delta_y_rects_index = 0;

    int max_delta_y_constraints = -MAX_INT;
    size_t max_delta_y_constraints_index = 0;


    get_max_delta_y_up_rects(xrect_number,max_delta_y_rects,max_delta_y_rects_index);
    get_max_delta_y_up_rect_constraints(xrect_number,max_delta_y_constraints,max_delta_y_constraints_index);


    // the upper bound on where to go down (on y) is the min = {min_delta_y_rects; min_delta_y_constraints; delta_y_current_objective} that
    // is != from MAX_INT. If the minimum is
    // min_delta_y_rects

    //the map holds its values ordered by keys, so for each values a specific action is associated
    std::map<int, int> ordered_delta_y;

    ordered_delta_y[abs( max_delta_y_constraints )] = 1;
    ordered_delta_y[abs(max_delta_y_rects )] = 0;
    ordered_delta_y[abs ( xdelta_y_objective ) ] = 2;

    switch ( ordered_delta_y.begin()->second )
    {
        case 0:
            if(max_delta_y_rects == 0)
                 damage_rect(xrect_number,max_delta_y_rects_index);
            else
                 v_rects[xrect_number].current_coords.y += abs(max_delta_y_rects);
            break;
        case 1:
            v_rects[xrect_number].current_coords.y += abs(max_delta_y_constraints);
            break;
        case 2:
            if(ordered_delta_y.begin()->first == MAX_INT)
                v_rects[xrect_number].current_coords.y = (container_w - v_rects[xrect_number].current_coords.w);
                else
                v_rects[xrect_number].current_coords.y += abs(xdelta_y_objective) ;
            break;
    }

    if(v_rects[xrect_number].current_coords == v_rects[xrect_number].objective_coords)
    {
        v_rects[xrect_number].is_satisfied = true;
    }else
    {
        v_rects[xrect_number].is_satisfied = false;
    }
}

void Eco2dSolver::move_down(size_t xrect_number, int xdelta_y_objective)
{

    int min_delta_y_rects = MAX_INT;
    size_t min_delta_y_rects_index = 0;

    int min_delta_y_constraints = MAX_INT;
    size_t min_delta_y_constraints_index = 0;


    get_min_delta_y_down_rects(xrect_number,min_delta_y_rects,min_delta_y_rects_index);
    get_min_delta_y_down_rect_constraints(xrect_number,min_delta_y_constraints,min_delta_y_constraints_index);


    // the upper bound on where to go down (on y) is the min = {min_delta_y_rects; min_delta_y_constraints; delta_y_current_objective} that
    // is != from MAX_INT. If the minimum is
    // min_delta_y_rects

    //the map holds its values ordered by keys, so for each values a specific action is associated
    std::map<int, int> ordered_delta_y;

    ordered_delta_y[min_delta_y_constraints] = 1;
    ordered_delta_y[min_delta_y_rects] = 0;
    ordered_delta_y[xdelta_y_objective] = 2;

    switch ( ordered_delta_y.begin()->second )
    {
        case 0:
            if(min_delta_y_rects == 0)
                 damage_rect(xrect_number,min_delta_y_rects_index);
            else
                 v_rects[xrect_number].current_coords.y -= min_delta_y_rects;
            break;
        case 1:
             v_rects[xrect_number].current_coords.y -= min_delta_y_constraints;
            break;
        case 2:
            if( ordered_delta_y.begin()->first == MAX_INT) //There is nothing on the way
                v_rects[xrect_number].current_coords.y = container_y;
             else v_rects[xrect_number].current_coords.y -= xdelta_y_objective ;
            break;
    }

    if(v_rects[xrect_number].current_coords == v_rects[xrect_number].objective_coords)
    {
        v_rects[xrect_number].is_satisfied = true;
    }else
    {
        v_rects[xrect_number].is_satisfied = false;
    }

}

void Eco2dSolver::move_left(size_t xrect_number, int xdelta_x_objective)
{
    int min_delta_x_rects = MAX_INT;
    size_t min_delta_x_rects_index = 0;

    int min_delta_x_constraints = MAX_INT;
    size_t min_delta_x_constraints_index = 0;

    get_min_delta_x_left_rect(xrect_number,min_delta_x_rects,min_delta_x_rects_index);
    get_min_delta_x_left_rect_constraints(xrect_number,min_delta_x_constraints,min_delta_x_constraints_index);



    // the upper bound on where to go down (on y) is the min = {min_delta_y_rects; min_delta_y_constraints; delta_y_current_objective} that
    // is != from MAX_INT. If the minimum is
    // min_delta_y_rects

    //the map holds its values ordered by keys, so for each values a specific action is associated
    std::map<int, int> ordered_delta_x;

    //We need the max, as all values are negative, we use the abs to put on the map, this way, the smallest will be the biggest
    ordered_delta_x[ min_delta_x_constraints] = 1;
    ordered_delta_x[ min_delta_x_rects] = 0;
    ordered_delta_x[ xdelta_x_objective ] = 2;

    switch ( ordered_delta_x.begin()->second )
    {
        case 0:
            if(min_delta_x_rects == 0)
                 damage_rect(xrect_number,min_delta_x_rects_index);
            else
                 v_rects[xrect_number].current_coords.x -= min_delta_x_rects;
            break;
        case 1:
            v_rects[xrect_number].current_coords.x -= min_delta_x_constraints;
            break;
        case 2:
            if(ordered_delta_x.begin()->first == MAX_INT)
                v_rects[xrect_number].current_coords.x = container_x;
                else
                v_rects[xrect_number].current_coords.x -= abs(xdelta_x_objective) ;
            break;
    }

    if(v_rects[xrect_number].current_coords == v_rects[xrect_number].objective_coords)
    {
        v_rects[xrect_number].is_satisfied = true;
    }else
    {
        v_rects[xrect_number].is_satisfied = false;
    }

}

void Eco2dSolver::move_right(size_t xrect_number, int xdelta_x_objective)
{

    int max_delta_x_rects = -MAX_INT;
    size_t max_delta_x_rects_index = 0;

    int max_delta_x_constraints = -MAX_INT;
    size_t max_delta_x_constraints_index = 0;

    get_max_delta_x_right_rect(xrect_number,max_delta_x_rects,max_delta_x_rects_index);
    get_max_delta_x_right_rect_constraints(xrect_number,max_delta_x_constraints,max_delta_x_constraints_index);



    // the upper bound on where to go down (on y) is the min = {min_delta_y_rects; min_delta_y_constraints; delta_y_current_objective} that
    // is != from MAX_INT. If the minimum is
    // min_delta_y_rects

    //the map holds its values ordered by keys, so for each values a specific action is associated
    std::map<int, int> ordered_delta_x;

    //We need the max, as all values are negative, we use the abs to put on the map, this way, the smallest will be the biggest
    ordered_delta_x[abs( max_delta_x_constraints )] = 1;
    ordered_delta_x[abs(max_delta_x_rects )] = 0;
    ordered_delta_x[abs ( xdelta_x_objective ) ] = 2;

    switch ( ordered_delta_x.begin()->second )
    {
        case 0:
            if(max_delta_x_rects == 0)
                 damage_rect(xrect_number,max_delta_x_rects_index);
            else
                 v_rects[xrect_number].current_coords.x += abs(max_delta_x_rects);
            break;
        case 1:
            v_rects[xrect_number].current_coords.x += abs(max_delta_x_constraints);
            break;
        case 2:
            if(ordered_delta_x.begin()->first == MAX_INT)
                v_rects[xrect_number].current_coords.x = (container_l - v_rects[xrect_number].current_coords.l);
                else
                v_rects[xrect_number].current_coords.x += abs(xdelta_x_objective) ;
            break;
    }

    if(v_rects[xrect_number].current_coords == v_rects[xrect_number].objective_coords)
    {
        v_rects[xrect_number].is_satisfied = true;
    }else
    {
        v_rects[xrect_number].is_satisfied = false;
    }


}

void Eco2dSolver::damage_rect(size_t xcurrent_rect, size_t xdamaged_rect)
{

    v_rects[xdamaged_rect].damaged = true;
    v_rects[xdamaged_rect].restrictions[xcurrent_rect] = v_rects[xcurrent_rect].objective_coords;

}

void Eco2dSolver::get_max_delta_y_up_rects(size_t xcurrent_rect, int &xmax_delta_y, size_t &xmax_delta_y_rect)
{
    for(size_t i = 0; i < v_rects.size(); i++)
    {
        if(i != xcurrent_rect)
        {
            if( (v_rects[i].current_coords.y) >= (v_rects[ xcurrent_rect].current_coords.y + v_rects[ xcurrent_rect].current_coords.w))
            {
                std::pair<int,int> intercepted_values = get_intersection_limits(v_rects[i].current_coords.x, v_rects[i].current_coords.l, v_rects[ xcurrent_rect].current_coords.x, v_rects[ xcurrent_rect].current_coords.l);
                if(intercepted_values.first != -1) //there is an intersection
                {
                    int current_delta_y = v_rects[ xcurrent_rect].current_coords.y + v_rects[ xcurrent_rect].current_coords.w - v_rects[i].current_coords.y;
                    if(current_delta_y  > xmax_delta_y)
                    {
                        xmax_delta_y = current_delta_y;
                        xmax_delta_y_rect = i;
                    }
                }
            }
        }
    }
}

void Eco2dSolver::get_max_delta_y_up_rect_constraints(size_t xcurrent_rect, int &xmax_delta_y, size_t &xmax_delta_y_rect)
{
    std::map<size_t,Coords>::iterator it = v_rects[xcurrent_rect].restrictions.begin();
    while(it != v_rects[xcurrent_rect].restrictions.end())
    {
        if( it->second.y >= (v_rects[ xcurrent_rect].current_coords.y + v_rects[ xcurrent_rect].current_coords.w))
        {
            std::pair<int,int> intercepted_values = get_intersection_limits(it->second.x, it->second.l, v_rects[ xcurrent_rect].current_coords.x, v_rects[ xcurrent_rect].current_coords.l);
            if(intercepted_values.first != -1) //there is an intersection
            {
                int current_delta_y = v_rects[ xcurrent_rect].current_coords.y + v_rects[ xcurrent_rect].current_coords.w - it->second.y;
                if(current_delta_y  > xmax_delta_y)
                {
                    xmax_delta_y = current_delta_y;
                    xmax_delta_y_rect = it->first;
                }
            }
        }
       it++;
    }
}

void Eco2dSolver::get_min_delta_y_down_rects(size_t xcurrent_rect , int &xmin_delta_y, size_t &xmin_delta_y_rect)
{
    for(size_t i = 0; i < v_rects.size(); i++)
    {
        if(i != xcurrent_rect)
        {
            if( (v_rects[i].current_coords.y + v_rects[i].current_coords.w) <= (v_rects[ xcurrent_rect].current_coords.y))
            {
                std::pair<int,int> intercepted_values = get_intersection_limits(v_rects[i].current_coords.x, v_rects[i].current_coords.l, v_rects[ xcurrent_rect].current_coords.x, v_rects[ xcurrent_rect].current_coords.l);
                if(intercepted_values.first != -1) //there is an intersection
                {
                    int current_delta_y = v_rects[xcurrent_rect].current_coords.y - v_rects[i].current_coords.y - v_rects[i].current_coords.w;
                    if(current_delta_y  < xmin_delta_y)
                    {
                        xmin_delta_y = current_delta_y;
                        xmin_delta_y_rect = i;
                    }
                }
            }
        }
    }
}

void Eco2dSolver::get_min_delta_y_down_rect_constraints(size_t xcurrent_rect , int &xmin_delta_y, size_t &xmin_delta_y_rect)
{

    std::map<size_t,Coords>::iterator it = v_rects[xcurrent_rect].restrictions.begin();
    while(it != v_rects[xcurrent_rect].restrictions.end())
    {
        if( (it->second.y + it->second.w) <= (v_rects[ xcurrent_rect].current_coords.y))
        {
            std::pair<int,int> intercepted_values = get_intersection_limits(it->second.x, it->second.l, v_rects[ xcurrent_rect].current_coords.x, v_rects[ xcurrent_rect].current_coords.l);
            if(intercepted_values.first != -1) //there is an intersection
            {
                int current_delta_y = v_rects[ xcurrent_rect].current_coords.y - it->second.y - it->second.w;
                if(current_delta_y  < xmin_delta_y)
                {
                    xmin_delta_y = current_delta_y;
                    xmin_delta_y_rect = it->first;
                }
            }
        }
       it++;
    }
}

void Eco2dSolver::get_max_delta_x_right_rect(size_t xcurrent_rect, int &xmax_delta_x, size_t &xmax_delta_x_rect)
{
    for(size_t i = 0; i < v_rects.size(); i++)
    {
        if(i != xcurrent_rect)
        {
            if( v_rects[i].current_coords.x >= (v_rects[xcurrent_rect].current_coords.x + v_rects[xcurrent_rect].current_coords.l))
        {
            std::pair<int,int> intercepted_values = get_intersection_limits(v_rects[i].current_coords.y, v_rects[i].current_coords.w, v_rects[ xcurrent_rect].current_coords.y, v_rects[ xcurrent_rect].current_coords.w);
            if(intercepted_values.first != -1) //there is an intersection
            {
                int current_delta_x = v_rects[xcurrent_rect].current_coords.x + v_rects[xcurrent_rect].current_coords.l - v_rects[i].current_coords.x;
                if(current_delta_x  >  xmax_delta_x)
                {
                    xmax_delta_x = current_delta_x;
                    xmax_delta_x_rect = i;
                }
            }
        }
        }
    }
}

void Eco2dSolver::get_max_delta_x_right_rect_constraints(size_t xcurrent_rect, int &xmax_delta_x, size_t &xmax_delta_x_rect)
{
    std::map<size_t,Coords>::iterator it = v_rects[xcurrent_rect].restrictions.begin();
    while(it != v_rects[xcurrent_rect].restrictions.end())
    {
        if( it->second.x >= (v_rects[ xcurrent_rect].current_coords.x + v_rects[ xcurrent_rect].current_coords.l))
        {
            std::pair<int,int> intercepted_values = get_intersection_limits(it->second.y, it->second.w, v_rects[ xcurrent_rect].current_coords.y, v_rects[ xcurrent_rect].current_coords.w);
            if(intercepted_values.first != -1) //there is an intersection
            {
                int current_delta_x = v_rects[ xcurrent_rect].current_coords.x + v_rects[ xcurrent_rect].current_coords.l - it->second.x;
                if(current_delta_x  >  xmax_delta_x)
                {
                    xmax_delta_x = current_delta_x;
                    xmax_delta_x_rect = it->first;
                }
            }
        }
        it++;
    }
}

void Eco2dSolver::get_min_delta_x_left_rect(size_t xcurrent_rect, int &xmin_delta_x, size_t &xmin_delta_x_rect)
{
    for(size_t i = 0; i < v_rects.size(); i++)
    {
        if(i != xcurrent_rect)
        {
            if( v_rects[i].current_coords.x  + v_rects[i].current_coords.l <= (v_rects[xcurrent_rect].current_coords.x))
        {
            std::pair<int,int> intercepted_values = get_intersection_limits(v_rects[i].current_coords.y, v_rects[i].current_coords.w, v_rects[ xcurrent_rect].current_coords.y, v_rects[ xcurrent_rect].current_coords.w);
            if(intercepted_values.first != -1) //there is an intersection
            {
                int current_delta_x = v_rects[xcurrent_rect].current_coords.x - v_rects[i].current_coords.x  - v_rects[i].current_coords.l;
                if(current_delta_x  <  xmin_delta_x)
                {
                    xmin_delta_x = current_delta_x;
                    xmin_delta_x_rect = i;
                }
            }
        }
        }
    }
}

void Eco2dSolver::get_min_delta_x_left_rect_constraints(size_t xcurrent_rect, int &xmin_delta_x, size_t &xmin_delta_x_rect)
{
    std::map<size_t,Coords>::iterator it = v_rects[xcurrent_rect].restrictions.begin();
    while(it != v_rects[xcurrent_rect].restrictions.end())
    {
        if( it->second.x  + it->second.l <= (v_rects[ xcurrent_rect].current_coords.x))
        {
            std::pair<int,int> intercepted_values = get_intersection_limits(it->second.y, it->second.w, v_rects[ xcurrent_rect].current_coords.y, v_rects[ xcurrent_rect].current_coords.w);
            if(intercepted_values.first != -1) //there is an intersection
            {
                int current_delta_x = v_rects[ xcurrent_rect].current_coords.x - it->second.x  - it->second.l;
                if(current_delta_x  <  xmin_delta_x)
                {
                    xmin_delta_x = current_delta_x;
                    xmin_delta_x_rect = it->first;
                }
            }
        }
        it++;
    }
}

int Eco2dSolver::get_int_random_between(int xlower, int xupper)
{
    int temp =-1;
    if(xlower <= xupper)
    {
        temp = rand() % (xupper - xlower + 1) + xlower;
    }
    return temp;
}

std::pair<int, int> Eco2dSolver::get_intersection_limits(long xt1, long xa1, long xt2, long xa2)
{

    /*
    * Given two initial values, and two dimensions (starting from the initial values)
    * verifies if there is intersection on the two "lines"
    */

    std::pair< int, int> intersection_limits;
    intersection_limits.first = -1.0;
    intersection_limits.second = -1.0;
    long int xt1_xa1 = xt1 + xa1;
    long int xt2_xa2 = xt2 + xa2;

    if(xt1 >= xt2)
    {
        if(xt1 < (xt2_xa2))
        {
            intersection_limits.first = xt1;
            if((xt1_xa1) <= (xt2_xa2))
            {
                intersection_limits.second = xt1_xa1;
            }else
            {
                intersection_limits.second = xt2_xa2;
            }
        }
    }else
    {
        if(xt2 < (xt1_xa1))
        {
            intersection_limits.first = xt2;
            if((xt1_xa1) <= (xt2_xa2))
            {
                intersection_limits.second = xt1_xa1;
            }else
            {
                intersection_limits.second = xt2_xa2;
            }
         }
    }

    return intersection_limits;

}

