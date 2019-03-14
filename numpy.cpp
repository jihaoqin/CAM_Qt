#include "numpy.h"


std::vector<std::complex<double>> numpy::roots(std::vector<double> coe){
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

std::vector<float> numpy::vPlus(vector<float> v1, vector<float> v2){
    assert(v1.size() == v2.size());
    vector<float> v(v1.size());
    for(int i =0; i<v1.size(); i++){
        v.at(i) = v1.at(i) + v2.at(i);
    }
    return v;
}

std::vector<float> numpy::vMul(vector<float> v1, float factor){
    vector<float> v(v1.size());
    for(int i =0; i<v1.size(); i++){
        v.at(i) = v1.at(i)*factor;
    }
    return v;
}
