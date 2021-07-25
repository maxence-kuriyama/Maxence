#include <iostream>
#include <cmath>
#include <random>
#include <vector>
#include <string>
#include <algorithm>
#include <iomanip>
#include <Eigen/Core>
#include <Eigen/Eigenvalues>
#ifndef vect_h
#define vect_h

//using namespace std;
using namespace Eigen;
random_device rnd;
mt19937 mt(rnd());
uniform_real_distribution<> unif(0.0, 1.0);
normal_distribution<> dist(0.0, 1.0);

double *dcalloc(int n) { return (double *)calloc(n, sizeof(double)); }


//MathematicalFunctions ----+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-++-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
typedef double Function(double);
typedef MatrixXd ArrayFunc(MatrixXd &);
double Identity(double x) { return x; }
double d_Identity(double x) { return 1.0; }
double sigmoid(double x) {
	if (x > 0.0) {
		return 1.0 / (1.0 + exp(-2.0*x));
	}
	else {
		return exp(2.0*x) / (1.0 + exp(2.0*x));
	}
}                   //sigmoid function
double d_sigmoid(double x) {
	if (x > 0.0) {
		return 2.0 * exp(-2.0*x) / pow(1.0 + exp(-2.0*x), 2.0);
	}
	else {
		return 2.0 * exp(2.0*x) / pow(1.0 + exp(2.0*x), 2.0);
	}
}                //derivative of sigmoidal
double d_tanh(double x) {
	return 1.0 / (cosh(x)*cosh(x));
}                   //derivative of tanh
double PLtanh(double x) {
	return max(-1.0, min(x, 1.0));
}					//piecewise-linear approximating tanh
double d_PLtanh(double x) {
	if (abs(x) < 1.0) return 1.0;
	return 0.0;
}					//derivative of PLtanh
double relu(double x) {
	if (x > 0.0) {
		return x;
	}
	else {
		return 0.0;
	}
}
double d_relu(double x) {
	if (x > 0.0) {
		return 1.0;
	}
	else {
		return 0.0;
	}
}
double hinge(double u){
	if(u >= 1){
		return 0.0;
	}else{
		return 1.0 - u;
	}
}
double zero_one(double u){
	if(u >= 0){
		return 0.0;
	}else{
		return 1.0;
	}
}
MatrixXd Identity(MatrixXd &x) { return x; }
MatrixXd d_Identity(MatrixXd &x) {
	MatrixXd trg = MatrixXd::Ones(x.rows(),x.cols());
	return trg;
}
MatrixXd sigmoid(MatrixXd &x) {
//	MatrixXd trg = (1.0 + exp(-2.0 * x.array())).inverse();
	MatrixXd trg = (1.0 + exp(-2.0 * (x.array().max)(0.0))).inverse()
		+ exp(2.0 * (x.array().min)(0.0)) * (1.0 + exp(2.0 * (x.array().min)(0.0))).inverse() - 0.5;
	return trg;
}                   //sigmoid function
MatrixXd d_sigmoid(MatrixXd &x) {
//	MatrixXd trg = 2.0 * exp(-2.0 * x.array()) * (1.0 + exp(-2.0 * x.array())).pow(-2.0);
	MatrixXd trg = 2.0 * exp(-2.0 * x.array().abs()) * (1.0 + exp(-2.0 * x.array().abs())).pow(-2.0);
	return trg;
}                //derivative of sigmoidal
MatrixXd tanh(MatrixXd &x) {
	MatrixXd trg = x.array().tanh();
	return trg;
}
MatrixXd d_tanh(MatrixXd &x) {
	MatrixXd trg = x.array().cosh().pow(2.0).inverse();
	return trg;
}                   //derivative of tanh
MatrixXd PLtanh(MatrixXd &x) {
	MatrixXd trg = ((x.array().min)(1.0).max)(-1.0);
	return trg;
}					//piecewise-linear approximating tanh
MatrixXd d_PLtanh(MatrixXd &x) {
	MatrixXd trg = (x.array().abs() > 1.0).cast<double>();
	return trg;
}					//derivative of PLtanh
MatrixXd relu(MatrixXd &x) {
	MatrixXd trg = (x.array().max)(0.0);
	return trg;
}
MatrixXd d_relu(MatrixXd &x) {
	MatrixXd trg = (x.array() > 0.0).cast<double>();
	return trg;
}

//Layer of Neural Network ----+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-++-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
class Layer {
public:
	VectorXd v_input;
	MatrixXd m_input;
    MatrixXd grads; //histry of gradient for Ito SDE
	
	virtual VectorXd forward(VectorXd &input) { return input; }
	virtual MatrixXd forward(MatrixXd &input) { return input; }
	virtual VectorXd backward(VectorXd &back, double lRate, string sol) { return v_input; }
	virtual MatrixXd backward(MatrixXd &back, double lRate, string sol) { return m_input; }
    virtual MatrixXd backward_Ito(MatrixXd &back, double lRate, string sol) { return backward(back, lRate, sol); }
    virtual VectorXd backward_Ito_learning(VectorXd &grad, double lRate) { return grad;}
};

class ActLayer :public Layer {
public:
	ArrayFunc *func;
	ArrayFunc *d_func;

	ActLayer();
	ActLayer(string mode);
	~ActLayer();
	void setMode(string mode);
	VectorXd forward(VectorXd &input);
	VectorXd backward(VectorXd &back, double lRate, string sol);
	MatrixXd forward(MatrixXd &input);
	MatrixXd backward(MatrixXd &back, double lRate, string sol);
    MatrixXd backward_Ito(MatrixXd &back, double lRate, string sol) { return backward(back, lRate, sol); }
    VectorXd backward_Ito_learning(VectorXd &grad, double lRate) { return grad;}
};
ActLayer::ActLayer() {
	func = Identity;
	d_func = d_Identity;
}
ActLayer::ActLayer(string mode) {
	if (mode == "Sigmoid" || mode == "sigmoid" || mode == "SIGMOID") {
		func = sigmoid;
		d_func = d_sigmoid;
	}
	else if (mode == "Tanh" || mode == "tanh" || mode == "TANH") {
		func = tanh;
		d_func = d_tanh;
	}
	else if (mode == "ReLU" || mode == "relu" || mode == "RELU") {
		func = relu;
		d_func = d_relu;
	}
	else {
		func = Identity;
		d_func = d_Identity;
	}
}
ActLayer::~ActLayer() {}
void ActLayer::setMode(string mode) {
	if (mode == "Sigmoid" || mode == "sigmoid" || mode == "SIGMOID") {
		func = sigmoid;
		d_func = d_sigmoid;
	}
	else if (mode == "Tanh" || mode == "tanh" || mode == "TANH") {
		func = tanh;
		d_func = d_tanh;
	}
	else if (mode == "ReLU" || mode == "relu" || mode == "RELU") {
		func = relu;
		d_func = d_relu;
	}
	else {
		func = Identity;
		d_func = d_Identity;
	}
}
VectorXd ActLayer::forward(VectorXd &input) {
	v_input = input;
	MatrixXd mat = input;
	VectorXd trg = func(mat);
	return trg;
}
VectorXd ActLayer::backward(VectorXd &back, double lRate, string sol) {
	MatrixXd mat = v_input;
	VectorXd trg = d_func(mat).array() * back.array();
	return trg;
}
MatrixXd ActLayer::forward(MatrixXd &input) {
	m_input = input;
	return func(input);
}
MatrixXd ActLayer::backward(MatrixXd &back, double lRate, string sol) {
	MatrixXd trg = d_func(m_input).array() * back.array();
	return trg;
}

class Affine :public Layer {
public:
	MatrixXd param;
	VectorXd bias;
	MatrixXd p_vel;	//velocity of param for Momentum
	MatrixXd p_var; //variance of param for AdaGrad
	VectorXd b_vel;	//velocity of bias for Momentum
	VectorXd b_var;	//variance of bias for AdaGrad
	double mom_coef;	//coefficient for Momentum
	double var_coef;	//coefficient for AdaGrad
	bool biasFlg;

	Affine();
	Affine(const MatrixXd &src, bool bFlg = true, double mom = 0.9, double varc = 0.999);
	Affine(const MatrixXd &src, const VectorXd &srcv, bool bFlg = true, double mom = 0.9, double varc = 0.999);
	~Affine();
	void setParam(const MatrixXd &src);
	void setParam(const MatrixXd &src, const VectorXd &srcv);
	void setCoef(double mom = 0.9, double varc = 0.999);
	VectorXd forward(VectorXd &input);
	VectorXd backward(VectorXd &back, double lRate, string sol);
	MatrixXd forward(MatrixXd &input);
	MatrixXd backward(MatrixXd &back, double lRate, string sol);
    MatrixXd backward_Ito(MatrixXd &back, double lRate, string sol);
    VectorXd backward_Ito_learning(VectorXd &grad, double lRate);
};
Affine::Affine() {
	mom_coef = 0.9;
	var_coef = 0.999;
	biasFlg = true;
}
Affine::Affine(const MatrixXd &src, bool bFlg, double mom, double varc) {
	param = src;
	bias.Zero(src.cols());

	int rsize = src.rows(); int csize = src.cols();
	p_vel = MatrixXd::Zero(rsize, csize);
	p_var = MatrixXd::Zero(rsize, csize);
	b_vel = VectorXd::Zero(csize);
	b_var = VectorXd::Zero(csize);
	mom_coef = mom;
	var_coef = varc;
	biasFlg = bFlg;
}
Affine::Affine(const MatrixXd &src, const VectorXd &srcv, bool bFlg, double mom, double varc) {
	if (src.cols() != srcv.size()) {
		cout << "ERROR(Affine): invalid sizes of parameters" << endl;
		return;
	}
	param = src;
	bias = srcv;

	int rsize = src.rows(); int csize = src.cols();
	p_vel = MatrixXd::Zero(rsize, csize);
	p_var = MatrixXd::Zero(rsize, csize);
	b_vel = VectorXd::Zero(csize);
	b_var = VectorXd::Zero(csize);
	mom_coef = mom;
	var_coef = varc;
	biasFlg = bFlg;
}
Affine::~Affine() {}
void Affine::setParam(const MatrixXd &src) {
	int rsize = src.rows(); int csize = src.cols();
	param = src;
	bias.setZero(csize);
	p_vel = MatrixXd::Zero(rsize, csize);
	p_var = MatrixXd::Zero(rsize, csize);
	b_vel = VectorXd::Zero(csize);
	b_var = VectorXd::Zero(csize);
}
void Affine::setParam(const MatrixXd &src, const VectorXd &srcv) {
	if (src.cols() != srcv.size()) {
		cout << "ERROR(Affine.setParam): invalid sizes of parameters" << endl;
		return;
	}
	param = src;
	bias = srcv;
	int rsize = src.rows(); int csize = src.cols();
	p_vel = MatrixXd::Zero(rsize, csize);
	p_var = MatrixXd::Zero(rsize, csize);
	b_vel = VectorXd::Zero(csize);
	b_var = VectorXd::Zero(csize);
}
void Affine::setCoef(double mom, double varc) {
	mom_coef = mom;
	var_coef = varc;
}
VectorXd Affine::forward(VectorXd &input) {
	v_input = input;
	VectorXd trg = param.transpose() * input;
	if (biasFlg){ trg += bias; }
	return trg;
}
VectorXd Affine::backward(VectorXd &back, double lRate, string sol) {
	VectorXd trg = param * back;
	VectorXd gradb = back;
	MatrixXd gradw = v_input * back.transpose();

	if (sol == "Adam") {
		//p_var, b_var, p_vel, b_vel update
		p_var = (var_coef * p_var.array()) + ((1.0 - var_coef) * gradw.array().pow(2.0));
		p_vel = (mom_coef * p_vel) + ((1.0 - mom_coef) * gradw);
		b_var = (var_coef * b_var.array()) + ((1.0 - var_coef) * gradb.array().pow(2.0));
		b_vel = (mom_coef * b_vel) + ((1.0 - mom_coef) * gradb);
		//param, bias update
		gradw = sqrt(1.0 - var_coef) * p_vel.array() * (p_var.array().sqrt()*(1.0 - mom_coef) + 1e-8).inverse();
		gradb = sqrt(1.0 - var_coef) * b_vel.array() * (b_var.array().sqrt()*(1.0 - mom_coef) + 1e-8).inverse();
	}
	else if (sol == "Momentum") {
		//p_vel, b_vel update
		p_vel = (mom_coef * p_vel) + ((1.0 - mom_coef) * gradw);
		b_vel = (mom_coef * b_vel) + ((1.0 - mom_coef) * gradb);
		//param, bias update
		gradw = p_vel;
		gradb = b_vel;
	}
	else if (sol == "RigidSGD"){
		if (biasFlg) { bias = (1.0-0.0001*lRate) * bias; }
		param = (1.0-0.0001*lRate) * param;
	}
	if (biasFlg){ bias -= lRate * gradb; }
	param -= lRate * gradw;

	return trg;
}
MatrixXd Affine::forward(MatrixXd &input) {
	m_input = input;
	MatrixXd trg = input * param;
	if (biasFlg) { trg.rowwise() += bias.transpose(); }
	return trg;
}
MatrixXd Affine::backward(MatrixXd &back, double lRate, string sol) {
	MatrixXd trg = back * param.transpose();
	VectorXd unit = VectorXd::Ones(back.rows());
	VectorXd gradb = unit.transpose() * back;
	MatrixXd gradw = m_input.transpose() * back;

	if (sol == "Adam") {
		//p_var, b_var, p_vel, b_vel update
		p_var = var_coef * p_var.array() + (1.0 - var_coef) * gradw.array().pow(2.0);
		b_var = var_coef * b_var.array() + (1.0 - var_coef) * gradb.array().pow(2.0);
		p_vel = mom_coef * p_vel + (1.0 - mom_coef) * gradw;
		b_vel = mom_coef * b_vel + (1.0 - mom_coef) * gradb;
		//param, bias update
		gradw = (p_vel / (1.0 - mom_coef)).array() * ((p_var / (1.0 - var_coef)).array().sqrt() + 1e-8).inverse();
		gradb = (b_vel / (1.0 - mom_coef)).array() * ((b_var / (1.0 - var_coef)).array().sqrt() + 1e-8).inverse();
	}
	else if (sol == "Momentum") {
		//p_vel, b_vel update
		p_vel = (mom_coef * p_vel) + ((1.0 - mom_coef) * gradw);
		b_vel = (mom_coef * b_vel) + ((1.0 - mom_coef) * gradb);
		//param, bias update
		gradw = p_vel;
		gradb = b_vel;
	}
	else if (sol == "RigidSGD"){
		if (biasFlg) { bias = (1.0-0.0001*lRate) * bias; }
		param = (1.0-0.0001*lRate) * param;
	}
	if (biasFlg){ bias -= lRate * gradb; }
	param -= lRate * gradw;
    
	return trg;
}
MatrixXd Affine::backward_Ito(MatrixXd &back, double lRate, string sol) {
    MatrixXd trg = back * param.transpose();
    MatrixXd gradb;
    MatrixXd gradw;
    int bSize = back.rows();
    
    for(int k=0, bLen=back.cols(), mLen=m_input.cols(); k < bSize; ++k){
        gradb = back.block(k,0,1,bLen);
        gradw = m_input.block(k,0,1,mLen).transpose() * back.block(k,0,1,bLen);
        if(biasFlg){
            gradw.conservativeResize(gradw.rows()+1, gradw.cols());
            gradw.row(gradw.rows()-1) = gradb;
        }
        
        //Expand gradw and gradb
        //Arrange grads into matrix
        Map<VectorXd> grad_vec(gradw.data(), gradw.size());
        if(k == 0) grads.resize(grad_vec.size(), bSize);
        grads.col(k) = grad_vec;
    }
//    cout << "grad -> rows: " << grads.rows() << ", cols: " << grads.cols() << endl;
    
    return trg;
}
VectorXd Affine::backward_Ito_learning(VectorXd &grad, double lRate) {
    int indim = param.rows(); int outdim = param.cols();
    int dim = indim * outdim;
    MatrixXd gradw(indim,outdim);
    VectorXd gradb(outdim);
    
    for(int i=0; i < outdim; ++i){
        gradw.col(i) = grad.block(indim*i, 0, indim, 1);
    }
    param -= lRate * gradw;
    
    if(biasFlg){
        dim += outdim;
        gradb = grad.block(indim*outdim, 0, outdim, 1);
        bias -= lRate * gradb;
    }
    
    return grad.block(dim, 0, grad.size()-dim, 1);
}

class SoftMax :public Layer {
public:
	SoftMax();
	~SoftMax();
	VectorXd forward(VectorXd &input);
	VectorXd backward(VectorXd &back, double lRate, string sol);
	MatrixXd forward(MatrixXd &input);
	MatrixXd backward(MatrixXd &back, double lRate, string sol);
    MatrixXd backward_Ito(MatrixXd &back, double lRate, string sol) { return backward(back, lRate, sol); }
    VectorXd backward_Ito_learning(VectorXd &grad, double lRate) { return grad;}
};
SoftMax::SoftMax() {}
SoftMax::~SoftMax() {}
VectorXd SoftMax::forward(VectorXd &input) {
	v_input = input;
	VectorXd trg = (input.array() - input.maxCoeff()).exp();
	trg *= 1.0 / trg.sum();
	return trg;
}
VectorXd SoftMax::backward(VectorXd &back, double lRate, string sol) {
	return back;
}
MatrixXd SoftMax::forward(MatrixXd &input) {
	m_input = input;
	MatrixXd trg = (input.colwise() - input.rowwise().maxCoeff()).array().exp();
	trg = trg.array().colwise() * (trg.rowwise().sum()).array().inverse();
	return trg;
}
MatrixXd SoftMax::backward(MatrixXd &back, double lRate, string sol) {
	return back;
}


//Perceptron ----+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-++-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
typedef VectorXd FunctionV(const VectorXd&);
VectorXd IdentityV(const VectorXd &x) { return x; }          //identity map

class Machine {
public:
	Layer **layer;
	int length;
	FunctionV *teacher;
	double lRate;
	string solver;

	Machine();
	Machine(int l, FunctionV *teach = IdentityV, double lr = 0.050, string sol = "SGD");
	void setLayer(Layer &lay, int id);
	void setTeach(FunctionV *teach);
	void setSolver(string sol);
	MatrixXd teacher_batch(const MatrixXd &input);
	VectorXd predict(const VectorXd &input);
	MatrixXd predict(const MatrixXd &input);
	void backprop(const VectorXd &input);
	void backprop(const MatrixXd &input);
	void backprop(const VectorXd &input, const VectorXd &tsig);
	void backprop(const MatrixXd &input, const MatrixXd &tsig);
    void backprop_Ito(const MatrixXd &input, int var_num);
	double loss(const VectorXd &test);
	double loss(const MatrixXd &test);
	double loss(const VectorXd &test, const VectorXd &tsig);
	double loss(const MatrixXd &test, const MatrixXd &tsig);
};
Machine::Machine() {
	layer = NULL;
	length = -1;
	teacher = IdentityV;
	lRate = 0.050;
	solver = "SGD";
}
Machine::Machine(int l, FunctionV *teach, double lr, string sol) {
	length = l;
	layer = (Layer **)calloc(length, sizeof(Layer *));
	teacher = teach;
	lRate = lr;
	setSolver(sol);
}
void Machine::setLayer(Layer &lay, int id) {
	if (id < 0 || id >= length) {
		cout << "ERROR(Machine.SetLayer): invalid index of layer" << endl;
	}
	layer[id] = &lay;
}
void Machine::setTeach(FunctionV *teach) {
	teacher = teach;
}
void Machine::setSolver(string sol) {
	if (sol == "Adam" || sol == "ADAM" || sol == "adam") {
		solver = "Adam";
	}
	else if (sol == "Momentum" || sol == "MOMENTUM" || sol == "momentum") {
		solver = "Momentum";
	}
	else if (sol == "RigidSGD") {
		solver = "RigidSGD";
	}
	else {
		solver = "SGD";
	}
}
MatrixXd Machine::teacher_batch(const MatrixXd &input) {
	int batchSize = input.rows();
	int outLen = teacher(input.row(0).transpose()).size();
	MatrixXd trg(batchSize,outLen);
	for (int i = 0; i < batchSize; ++i) {
		trg.row(i) = teacher(input.row(i));
	}
	return trg;
}
VectorXd Machine::predict(const VectorXd &input) {
	VectorXd out = input;
	int i; int n = length;
	for (i = 0; i < n; ++i) {
		out = layer[i]->forward(out);
	}
	return out;
}
MatrixXd Machine::predict(const MatrixXd &input) {
	MatrixXd out = input;
	int i; int n = length;
	for (i = 0; i < n; ++i) {
		out = layer[i]->forward(out);
	}
	return out;
}
void Machine::backprop(const VectorXd &input) {
	VectorXd temp = input;
	int i; int n = length;
	for (i = 0; i < n; ++i) {
		temp = layer[i]->forward(temp);
	}

	VectorXd tsig = teacher(input);
	temp = temp - tsig;
	for (int i = length - 1; i >= 0; --i) {
		temp = layer[i]->backward(temp, lRate, solver);
	}
}
void Machine::backprop(const MatrixXd &input) {
	int i; int n = length;
	MatrixXd temp = input;
	for (i = 0; i < n; ++i) {
		temp = layer[i]->forward(temp);
	}

	MatrixXd tsig = teacher_batch(input);
	temp = (temp - tsig) / input.rows();
	for (i = length - 1; i >= 0; --i) {
		//		cout << i << ", " << temp.rsize << " * " << temp.csize << endl;
		temp = layer[i]->backward(temp, lRate, solver);
	}
}
void Machine::backprop(const VectorXd &input, const VectorXd &tsig) {
	VectorXd temp = input;
	int i; int n = length;
	for (i = 0; i < n; ++i) {
		temp = layer[i]->forward(temp);
	}

	temp = temp - tsig;
	for (int i = length - 1; i >= 0; --i) {
		temp = layer[i]->backward(temp, lRate, solver);
	}
}
void Machine::backprop(const MatrixXd &input, const MatrixXd &tsig) {
	int i; int n = length;
	MatrixXd temp = input;
	for (i = 0; i < n; ++i) {
		temp = layer[i]->forward(temp);
	}

	temp = (temp - tsig) / input.rows();
	for (i = length - 1; i >= 0; --i) {
		//		cout << i << ", " << temp.rsize << " * " << temp.csize << endl;
		temp = layer[i]->backward(temp, lRate, solver);
	}
}
double Machine::loss(const VectorXd &test){
	VectorXd tsig = teacher(test);
	VectorXd pred = predict(test);
//	cout << endl
//		<< "pred: " << pred << endl << endl
//		<< "tsig: " << tsig << endl << endl
//		<< (pred - tsig) << endl << endl
//		<< (pred - tsig).array().pow(2.0) << endl << endl
//		<< (pred - tsig).array().pow(2.0).sum() << endl << endl;
	return ((pred - tsig).array().pow(2.0).sum()) * 0.5;
}
double Machine::loss(const MatrixXd &test){
	MatrixXd tsig = teacher_batch(test);
	MatrixXd pred = predict(test);
	return ((pred - tsig).array().pow(2.0).sum()) * 0.5;
}
double Machine::loss(const VectorXd &test, const VectorXd &tsig) {
	VectorXd pred = predict(test);
	return ((pred - tsig).array().pow(2.0).sum()) * 0.5;
}
double Machine::loss(const MatrixXd &test, const MatrixXd &tsig) {
	MatrixXd pred = predict(test);
	return ((pred - tsig).array().pow(2.0).sum()) * 0.5;
}
void Machine::backprop_Ito(const MatrixXd &input, int var_num) {
    int i; int n = length;
    MatrixXd temp = input;
    for (i = 0; i < n; ++i) {
        temp = layer[i]->forward(temp);
    }
    
    MatrixXd tsig = teacher_batch(input);
    temp = (temp - tsig);
    MatrixXd total_grad(0, input.rows());
    MatrixXd grad_temp;
    for (i = length - 1; i >= 0; --i) {
//        cout << i << ", " << temp.rsize << " * " << temp.csize << endl;
        temp = layer[i]->backward_Ito(temp, lRate, solver);
        grad_temp = layer[i]->grads;
        if(grad_temp.rows() != 0){
            total_grad.conservativeResize(total_grad.rows() + grad_temp.rows(), total_grad.cols());
            total_grad.block(total_grad.rows() - grad_temp.rows(), 0, grad_temp.rows(), total_grad.cols()) = grad_temp;
        }
    }
//    cout << "total_grad -> rows: " << total_grad.rows() << ", cols: " << total_grad.cols() << endl;
    
    // Approximation by Ito SDE
    VectorXd drift = total_grad.rowwise().sum() / input.rows();
//    cout << total_grad * total_grad.transpose() << endl;
    SelfAdjointEigenSolver<MatrixXd> es((total_grad * total_grad.transpose() / input.rows()) - drift * drift.transpose());
//    MatrixXd duf_mat = es.operatorSqrt(); //Sqrt makes nan because of numerical error
    MatrixXd V = es.eigenvectors();
    VectorXd D = es.eigenvalues(); D = D.array().abs().sqrt();
    MatrixXd duf_mat = V * D.asDiagonal() * V.transpose();
    VectorXd dW(duf_mat.cols()); for(int k=0, dim=duf_mat.cols(); k < dim; ++k){ dW.coeffRef(k) = dist(mt); }
    VectorXd approx_grad = drift + duf_mat * dW * sqrt(1.0 / var_num);
//    cout << duf_mat << endl << "row: " << duf_mat.rows() << ", col: " << duf_mat.cols() << endl;
//    cout << drift << endl;
//    cout << approx_grad << endl << endl;
    for (i = length - 1; i >= 0; --i) {
        approx_grad = layer[i]->backward_Ito_learning(approx_grad, lRate);
    }
}



#endif /* vect_h */

