#ifndef NUMPY_H
#define NUMPY_H
#include <complex>
#include <Eigen/Dense>
#include <vector>

namespace numpy{
std::vector<std::complex<double>> roots(std::vector<double> coe){
    while(abs(coe.at(0)) < 1e-9){
        coe.erase(coe.begin());
    }
    if(coe.empty()){
        assert(1 == 0);
        return std::vector<std::complex<double>>();
    }
    auto first = coe.at(0);
    for(auto& i: coe){
        i = i/first;
    }
    coe.erase(coe.begin());
    Eigen::MatrixXd friendMat = Eigen::MatrixXd::Constant(coe.size(), coe.size(), 0);
    for(int i = 1; i<coe.size();i++){
        friendMat(i, i-1) = 1;
    }
    for(int i = 0; i<coe.size(); i++){
        friendMat(i, coe.size()-1) = -1*coe.at(coe.size()-1-i);
    }
    auto e = friendMat.eigenvalues();
    std::vector<std::complex<double>> result;
    for(int i = 0; i<e.rows(); i++){
        result.push_back(e(i));
    }
    return result;
}
}

#endif // NUMPY_H
