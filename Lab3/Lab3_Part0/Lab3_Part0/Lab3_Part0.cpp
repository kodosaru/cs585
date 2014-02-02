// Cplusplus.cpp : Defines the entry point for the console application.
//


#include <iostream>
using namespace std;

void addOneInt(int arg);
void addOnePtr(int* arg);
void addOneRef(int& arg);

//This is a data structure containing one integer
class DataStructure{
public:
	int value;

	DataStructure& operator+=(int addMe)
	{
		value+= addMe;
		return *this;
	}
};

void addOneClass(DataStructure arg);
void addOneClassPtr(DataStructure* arg);
void addOneClassRef(DataStructure& arg);
void addOneClassRefOperator(DataStructure& arg);


int main(int argc, char* argv[])
{
	//Let's start with integers because they are easy to think about
	int myValue=5;

	cout<<"myValue initial value:"<<myValue<<endl;

	addOneInt(myValue);
	cout<<"The value is still "<<myValue<<endl;

	//when we need to pass in a pointer, we use the & operator to say that this
	//is the **memory address* of the integer
	addOnePtr(&myValue);
	cout<<"Now the value is "<<myValue<<endl;

	//when we pass a value to a function that takes a reference, we
	//just pass in the value. The compiler takes care of the pointer
	//business under the hood
	addOneRef(myValue);
	cout<<"And now the value is "<<myValue<<endl;

	cout<<"-------------------------------------"<<endl;
	DataStructure myObject;
	myObject.value=10;

	cout<<"Initial value in my data structure "<<myObject.value<<endl;

	addOneClass(myObject);
	cout<<"Value in my data structure is still "<<myObject.value<<endl;

	addOneClassPtr(&myObject);
	cout<<"Now the value in my data structure is "<<myObject.value<<endl;

	addOneClassRef(myObject);
	cout<<"And now the value in my data structure is "<<myObject.value<<endl;

    addOneClassRefOperator(myObject);
	cout<<"And now the value in my data structure is "<<myObject.value<<endl;
    cout<<"-------------------------------------"<<endl;


    //Pointers point to memory. The type is how you interpret it

	char* hello = "hello world! ";
	int len = strlen(hello);
	for (int i=0; i<len; i++)
	{
		cout<<"::"<<hello[i]<<"::  "<<(int)(hello[i])<<endl;
	}

    //What happens when we call this an array of integers?
	unsigned int* hi = (unsigned int*)(hello);

    //big endian processor means the right-most byte becomes 
    //the highest order byte in the integer
    //  108*2^24 + 108*2^16 + 101*2^8 + 104*2^0 =  1819043176
    //  111*2^24 + 119*2^16 + 32*2^8 + 111*2^0 = 1870078063
    //  33*2^24 + 100*2^16 + 108*2^8 + 114*2^0 = 560229490
    for(int i=0; i<len/sizeof(int); i++)
	{
		cout<<hi[i]<<endl;
	}

	return 0;
}

//C++ is pass by value, so modifying the value of the argument in the 
//function has no effect in the colling function
void addOneInt(int arg)
{
	arg++;
}

//If we pass in a pointer, we can make a change to the value in the calling function
void addOnePtr(int* arg)
{
	//To modify the contents of the memory pointed to by the pointer, we
	//dereference the pointer with the * operator
	(*arg)++;
}

//Using the & operator after a type, tells the compiler that this is a "reference"
//References are syntactic sugar for pointers so you don't have stars everywhere
void addOneRef(int& arg)
{
	arg++;
}

//When the argument of the function is a class, C++ is still pass-by value
//and the function has no effect in the colling function
void addOneClass(DataStructure arg)
{
	arg.value++;
}

//If we pass in a pointer, we can make a change to the value in the calling function
void addOneClassPtr(DataStructure* arg)
{
	//To modify the contents of the memory pointed to by the pointer, we
	//*could* dereference the pointer and access the data member, like this:
	(*arg).value ++;

	//Or we can use the -> (arrow) syntax to do the same thing
	arg->value++;

}

//Using the & operator after a type, tells the compiler that this is a "reference"
//References are syntactic sugar for pointers so you don't have stars everywhere
void addOneClassRef(DataStructure& arg)
{
	arg.value ++;
}

//Using the & operator after a type, tells the compiler that this is a "reference"
//You can use overloaded operators on references
void addOneClassRefOperator(DataStructure& arg)
{
	arg += 1;
}
