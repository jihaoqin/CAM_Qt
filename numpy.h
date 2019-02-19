#ifndef NUMPY_H
#define NUMPY_H
#include <complex>

struct root{
    std::complex<double> x1;
    std::complex<double> x2;
    std::complex<double> x3;
    std::complex<double> x4;
};

root root4(double a, double b, double c, double d, double e){
    double delta_1 = pow(c, 2) - 3*b*d +12*a*e;
    double delta_2 = 2*pow(c, 3) - 9*b*c*d +27*a*pow(d, 2) + 27*pow(b, 2)*e - 72*a*c*e;
    std::complex<double> m1 = pow(delta_2 + sqrt(-4*pow(delta_1,3) + pow(delta_2, 2)), 1.0/3);
    std::complex<double> delta_3 = pow(2, 1.0/3)*delta_1/(3*a*m1) + m1/(3*pow(2, 1.0/3)*a);
    std::complex<double> m2 = pow(b,2)/(4*pow(a,2)) - (2*c)/(3*a);
    std::complex<double> m3 = -1*(pow(b,3))/(pow(a,3)) + (4*b*c)/(pow(a,2)) - (8*d)/(a);
    std::complex<double> m4 = -1*b/(4*a);
    std::complex<double> m5 = 0.5*sqrt(m2+delta_3);
    std::complex<double> m6a = 0.5*sqrt(2.0*m2 - delta_3 - m3/(8.0*m5));
    std::complex<double> m6b = 0.5*sqrt(2.0*m2 - delta_3 + m3/(8.0*m5));
    std::complex<double> x1 = m4 - m5 -m6a;
    std::complex<double> x2 = m4 - m5 +m6a;
    std::complex<double> x3 = m4 + m5 -m6b;
    std::complex<double> x4 = m4 + m5 +m6b;
    return root{x1, x2, x3, x4};
}
#endif // NUMPY_H
