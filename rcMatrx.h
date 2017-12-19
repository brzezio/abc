#include <iostream>
#include <fstream>

using namespace std;

//zmiana dla git

class CMatrix{
private:
	struct data;
	data *dane;
public:
	class Cref1;
	class Cref2;
	class IndexOutOfRange{};
	class WrongDim{};

	CMatrix(int,int,double,double);
	CMatrix(fstream&);
	CMatrix(const CMatrix&);
	~CMatrix();
	friend ostream& operator<<(ostream&, const CMatrix&);
	CMatrix& operator=(const CMatrix&);
	CMatrix operator*(const CMatrix&) const;
	Cref1 operator[](int);
	void write(int,int,double);
};

struct CMatrix::data{
	int n;
	double **Matrix;
	int size1;
	int size2;
	
	data(int a=0,int b=0, double c=0.0, double d=0.0){
		n=1;
		size1=a;
		size2=b;
		Matrix=new double*[size1];
		int chack=0;
		try{
			for(int i=0;i<size1;i++){
				Matrix[i]=new double[size2];	
				chack++;
			}
		}catch(...){
			for(chack; chack>0; chack--){
				delete [] Matrix[chack];
			}
			delete [] Matrix;
			throw bad_alloc();
		}
		for(int i=0;i<size1;i++){
			for(int j=0;j<size2;j++){
				if(i==j) Matrix[i][j]=c;
				else Matrix[i][j]=d;
			}
		}
	}
	~data(){
		for(int i=size1-1; i>=0; i--){
			delete [] Matrix[i];
		}
		delete [] Matrix;
	}
	data* detach(){
		if(n==1)
			return this;
		data *a=new data(size1,size2,Matrix[0][0],Matrix[0][1]);
		this->n--;
		return a;
	} 
};

class CMatrix::Cref2{
private:
	friend class CMatrix;
	CMatrix& s;
	int i,j;
public:
	Cref2(CMatrix& ss,int ii,int jj): s(ss), i(ii), j(jj){};
	operator double(){
		return s.dane->Matrix[i][j];
	}
	void operator =(double a){
		s.write(i,j,a);
	}

};

class CMatrix::Cref1{
private:
	CMatrix& s;
	int i;
public:
	Cref1(CMatrix& ss,int ii): s(ss), i(ii){};
	CMatrix::Cref2 operator[](int j);
	
};
CMatrix::~CMatrix(){
	if(--dane->n==0)
		delete dane;
}

CMatrix::CMatrix(int a=0,int b=0,double c=0.0,double d=0.0){
	dane=new data(a,b,c,d);
}
CMatrix::CMatrix(fstream& f1){
	int SIZE1;
	int SIZE2;
	f1>>SIZE1;
	f1>>SIZE2;
	dane=new data(SIZE1,SIZE2);
	for(int i=0;i<SIZE1;i++){
		for(int j=0;j<SIZE2;j++){
			f1>>dane->Matrix[i][j];
		}
	}
}
CMatrix::CMatrix(const CMatrix& a){
	a.dane->n++;
	this->dane=a.dane;
}
CMatrix::Cref1 CMatrix::operator[](int i){
	if(this->dane->size1<i) throw IndexOutOfRange();
	return Cref1(*this,i);
}
CMatrix::Cref2 CMatrix::Cref1::operator[](int j){
	if(s.dane->size2<j) throw IndexOutOfRange();
	return Cref2(s,i,j);
}
ostream& operator<<(ostream& o, const CMatrix& s){
	for(int i=0;i<s.dane->size1;i++){
		for(int j=0;j<s.dane->size2;j++){
			o<<s.dane->Matrix[i][j]<<"\t";
		}
	o<<endl;
	}
	return o;
}
CMatrix& CMatrix::operator=(const CMatrix & a){
	a.dane->n++;
	cout<<"a.dane->n="<<a.dane->n<<endl;
	if(--(this->dane->n)==0)
		delete this->dane;
	this->dane=a.dane;
	return *this;
}
void CMatrix::write(int i, int j, double c){
	this->dane=this->dane->detach();
	if(this->dane->size1<i||this->dane->size2<j)
		throw IndexOutOfRange();
	this->dane->Matrix[i][j]=c;
}
CMatrix CMatrix::operator*(const CMatrix& a) const{
	if(this->dane->size1!=a.dane->size2){
		throw WrongDim();	
	}
	CMatrix c(a.dane->size1,a.dane->size2);
	double suma;
	for(int i=0;i<this->dane->size1;i++){
		for(int j=0;j<a.dane->size2;j++){
			suma=0.0;
			for(int k=0;k<a.dane->size2;k++){
				suma+=a.dane->Matrix[i][k]*this->dane->Matrix[k][j];
			}
			c.dane->Matrix[i][j]=suma;
		}
	}
	return c;
}


