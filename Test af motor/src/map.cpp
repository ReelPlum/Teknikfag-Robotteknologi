double map_double(double x, double x_min, double x_max, double y_min, double y_max)
{
    double a = (y_max - y_min) / (x_max - x_min);

    return a * x + (y_min - x_min * a);
}