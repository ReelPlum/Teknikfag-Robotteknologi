class SensorFusion{
    public:

    SensorFusion(double k);
    
    double getCurrentValue();
    double calculateValue(double a, double b);

    private:

    double k;
    double value;

};