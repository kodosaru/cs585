//
//  Stack.h
//  CountObjects
//
//  Created by Don Johnson on 4/22/14.
//  Copyright (c) 2014 Donald Johnson. All rights reserved.
//

#ifndef __CountObjects__Stack__
#define __CountObjects__Stack__

#include <iostream>
#include<iostream>

using namespace std;

template <class T>
class stack {
    
    public : T *a;
    
    int top,size;
    
    stack()
    {
        top=-1;
        cout<<"\nEnter size of array :"<<endl;
        cin>>size;
        a=new T[size];
    }
    
    int isfull()
    {
        if(top==(size-1))
            return 1;
        else return 0;
    }
    
    int isempty()
    {
        if(top==-1)
            return 1;
        else return 0;
    }
    
    void topp()
    {
        if(isempty())
            cout<<"\nStack Underflow"<<endl;
        else cout<<"\nTop Element is "<<a[top-1]<<endl;
    }
    
    void push() {
        T n;
        
        if(isfull())
            cout<<"\nStack Overflow"<<endl;
        else
        {
            cout<<"\nEnter an element"<<endl;
            cin>>n;
            a[top++]=n;
            cout<<"\nElement Inserted Succesfully"<<endl;
        }
    }
    
    void pop()
    {
        if(isempty())
            cout<<"\nStack Underflow"<<endl;
        else
            top=top-1;
        
        cout<<"\nElement Deleted successfully"<<endl;
    }
    
/*    void print()
    {
        for(i=top;i>=0;i--)
        {
            string label;
            if(i==0)
                label="top";
            else if(i==a.size()-1)
                label="bottom";
            else
            {
                ostringstream ss;
                ss<<i;
                label=ss.str();
            }
            cout<<"label: "<<a[i]<<end;
        }
    }*/
};

int main()
{
    stack <int>s;
    int i=0,k;
    
    while(i!=1)
    {
        cout<<"\n******************M E N U**************\n";
        cout<<"1.Push\n2.Pop\n3.Top\n4.Exit\n5.Print\n";
        cout<<"\n***************************************\n";
        cout<<"\nEnter option ";
        cin>>k;
        
        switch(k)
        {
            case 1: s.push();
            break;
                
            case 2:
            s.pop();
            break;
                
            case 3:
            s.topp();
            break;
                
            case 4:
            i=1;
            break;
            
            case 5:
                print();
            break;
                
            default:
            cout<<"\n------- Wrong Option -------\n";
            break;
        }
    }
    return 0;
}

// See more at: http://www.techfinite.net/2013/10/program-for-stack-data-structure-in-cpp.html#sthash.L4uDyzW4.dpuf

#endif /* defined(__CountObjects__Stack__) */
