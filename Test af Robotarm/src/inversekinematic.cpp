#include <inversekinematic.h>

JointAngle calculate_joints2(double *l1, double *l2, Vector2 *c){
    //Calcuates angles for the two joints

    double B = acos((pow(*l1,2) + pow((*c).magnitude(),2) - pow(*l2,2))/(2*(*l1)*(*c).magnitude()));
    double C = acos((pow(*l1,2)+pow(*l2,2)-pow((*c).magnitude(),2))/(2*(*l1)*(*l2)));
    
    double ca = atan2((*c).y,(*c).x);

    JointAngle angles;
    angles.A = B + ca;
    angles.B = C;

    return angles;
}