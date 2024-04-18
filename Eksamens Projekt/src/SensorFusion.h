class SensorFusion {
    public:
    SensorFusion(){};
    void setup(double k);
    double calculateValue(double a, double b);
    void setK(double k);
    double getK();

    private:

    double k;

};