#include "stdafx.h"
#include <iostream>
#include <vector>
using namespace std;

//In C++, you can define classes that inherit from other classes to create an object hierarchy.
// One situation where this is useful is when you have a single idea (such as a distance) that can 
// be computed in many ways, but you would like to be able to access different implementations of that 
// idea without writing a thousand different functions
//
// OpenCV uses this style of object hierarchy for the many varieties of 
// keypoint detection, descriptor extraction, and descriptor / feature matching. 
// So, we will practice using this style of interface with a toy example
// My goal is for you to be able to read and follow the documentation
// http://docs.opencv.org/modules/features2d/doc/features2d.html
// so you can use it in your own projects
//

//The Base Class is the class from which all the other classes inherit
//It defines the minimum set of functions that must be implemented in sub-classes
class DistanceComputerBase{
public:
    DistanceComputerBase(){}

    //when a function is declared with =0 after it, it means that this class
    //is an abstract class that can't be instantiated on its own. You can only
    //instantiate a subclass. methods declared in this way are called "abstract methods"
    //
    //when a function is declared as virtual, it means that if you have a pointer
    //of the type of the base class, it will call the function from the subclass
    //
    //any abstract method must be declared as virtual
    virtual bool computeDistance(vector<double>& input1, vector<double>& input2, double& output)=0;

    virtual bool validateInput(vector<double>& input1, vector<double>& input2)
    {
        return input1.size() == input2.size();
    }
    virtual void printName()=0;
};

//This is a class that will compute the L2, Euclidean distance (the distance we are used to using)
class L2Distance: public DistanceComputerBase{
public:
    L2Distance(){}
    virtual bool computeDistance(vector<double>& input1, vector<double>& input2, double& output)
    {
        output = 0;
        if(!validateInput(input1, input2))
        {
            return false;
        }
        for(int i=0; i<input1.size(); i++)
        {
            double delta = input1[i] - input2[i];
            output += delta*delta;
        }
        output = sqrt(output);
        return true;
    }
    virtual void printName()
    {
        cout<<"L2 (Euclidean) Distance"<<endl;
    }
};

//This is a class that will compute the L1, Manhattan distance
class L1Distance: public DistanceComputerBase{
public:
    L1Distance(){}
    virtual bool computeDistance(vector<double>& input1, vector<double>& input2, double& output)
    {
        output = 0;
        if(!validateInput(input1, input2))
        {
            return false;
        }
        for(int i=0; i<input1.size(); i++)
        {
            double delta = input1[i] - input2[i];
            output += abs(delta);
        }
        return true;
    }
    virtual void printName()
    {
        cout<<"L1 (Manhattan) Distance"<<endl;
    }
};

//This is a class that has spent a little too much time in the chocolate factory!
class ILoveLucyDistance: public DistanceComputerBase{
public:
    ILoveLucyDistance(){}
    virtual bool computeDistance(vector<double>& input1, vector<double>& input2, double& output)
    {
        output = rand();
        return true;
    }
    virtual void printName()
    {
        cout<<"Lucy, you've got some explaining to do!"<<endl;
    }

};

//This function takes a reference of the base class type, but because the methods
//are virtual, when we call the methods of the objects, it calls the methods for the subclasses,
//even though the type of the reference is for the base class
//The compiler and other stuff under the hood takes care of managing who is what
void computeAndPrintDistance(vector<double>& input1, vector<double>& input2, DistanceComputerBase& distanceFn)
{
    double result;
    distanceFn.computeDistance(input1, input2, result);
    distanceFn.printName();
    cout<<result<<endl;
}

int main(int argc, char* argv[])
{
    L1Distance L1;
    L2Distance L2;
    ILoveLucyDistance Lucy;

    vector<double> input1;
    vector<double> input2;

    input1.push_back(0);
    input1.push_back(0);
    input2.push_back(1);
    input2.push_back(1);

    computeAndPrintDistance(input1, input2, L1);
    computeAndPrintDistance(input1, input2, L2);
    computeAndPrintDistance(input1, input2, Lucy);
    computeAndPrintDistance(input1, input2, Lucy);
}
