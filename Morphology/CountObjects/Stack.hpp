//
//  Stack.hpp
//
//  Created by Don Johnson on 4/22/14.
//

#ifndef __STACK__HPP__
#define __STACK__HPP__

#include <iostream>

#ifndef DEBUG
    #define DEBUG 1
#endif

template <class T>
class Stack {
    public : T *stack;
    
    int top,size;
    
    Stack(int inVal=0)
    {
        if(inVal==0)
        {
            std::cout<<"\nEnter desired size of the stack:"<<std::endl;
            std::cin>>size;
        }
        else
        {
            size=inVal;
        }
        top=-1;
        stack=new T[size];
    }
    
    int isFull()
    {
        if(top==(size-1))
        {
            return 1;
        }
        else
        {
            return 0;
        }
    }
    
    int isEmpty()
    {
        if(top==-1)
        {
            return 1;
        }
        else
        {
            return 0;
        }
    }
    
    int clear()
    {
        if(top==-1)
        {
            if(DEBUG)
                std::cout<<"Error: The stack is already empty"<<std::endl;
            return 1;
        }
        else
        {
            top=-1;
            return 0;
        }
    }
    
    int count()
    {
        return top+1;
    }
    
    void push(T inVal) {
        if(isFull())
        {
            return;
        }
        else
        {
            stack[++top]=inVal;
        }
    }
    
    T pop()
    {
        T bad;
        if(top>=0)
        {
            top=top-1;
            return stack[top+1];
        }
        else
        {
            if(DEBUG)
                std::cout<<"Error: The stack is already empty"<<std::endl;
            return bad;
        }
    }
    
    void print()
    {
        if(isEmpty())
        {
            return;
        }
        
        for(int i=top;i>=0;i--)
        {
            if(i==top)
            {
                std::cout<<"stack["<<i<<"]: "<<stack[top]<<" top"<<std::endl;
            }
            else if(i==0)
            {
                std::cout<<"stack[0]: "<<stack[0]<<" bottom"<<std::endl;
            }
            else
            {
                std::cout<<"stack["<<i<<"]: "<<stack[i]<<std::endl;
            }
        }
    }
    
    void demo()
    {
        srand((uint)time(NULL));
        std::cout<<"Begin demo..."<<std::endl;
        
        std::cout<<std::endl<<"-->Add "<<size<<" random integers to the stack"<<std::endl;
        for(int i=0;i<size;i++)
        {
            push(rand() % 10);
        }
        
        std::cout<<std::endl<<"-->Number of elements on the stack equals "<<count()<<std::endl;

        std::cout<<std::endl<<"-->Print the stack"<<std::endl;
        print();
        
        std::cout<<std::endl<<"-->Try to push an element onto a full stack"<<std::endl;
        push(rand() % 10);
        
        std::cout<<std::endl<<"-->Pop all of the elements"<<std::endl;
        for(int i=0;i<size;i++)
        {
            pop();
        }
        
        std::cout<<std::endl<<"-->Number of elements on the stack equals "<<count()<<std::endl;
        
        std::cout<<std::endl<<"-->Try to pop an element from an empty stack"<<std::endl;
        pop();
        
        std::cout<<std::endl<<"-->Add "<<size<<" random integers to the stack"<<std::endl;
        for(int i=0;i<size;i++)
        {
            push(rand() % 10);
        }
        std::cout<<std::endl<<"-->Number of elements on the stack equals "<<count()<<std::endl;
        
        std::cout<<std::endl<<"-->Print the stack"<<std::endl;
        print();

        std::cout<<std::endl<<"-->Clear the stark with clear()"<<std::endl;
        clear();
        
        std::cout<<std::endl<<"-->Number of elements on the stack equals "<<count()<<std::endl;
        
        std::cout<<std::endl<<"End of demo"<<std::endl;
    }
};

// See more at: http://www.techfinite.net/2013/10/program-for-stack-data-structure-in-cpp.html#sthash.L4uDyzW4.dpuf

#endif /* defined(__CountObjects__Stack__) */
