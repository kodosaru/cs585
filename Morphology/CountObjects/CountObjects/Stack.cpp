//
//  Stack.cpp
//  CountObjects
//
//  Created by Don Johnson on 4/22/14.
//
#include "Stack.hpp"

using namespace std;

void stack()
{
    string input;
    Stack <int> *pS=NULL,*pD=NULL;
    
    // Note: You don't have to use pointers to the stack. You can:
    // 1. Instantiate with "Stack <int> s;"
    // 2. Call methods with "s.push(7);", "s.pop();" etc.
    
    while(true)
    {
        cout<<"\n******************M E N U**************\n";
        cout<<"1.Push\n2.Pop\n3.Print\n4.Demo\n5.Exit";
        cout<<"\n***************************************\n";
        cout<<"\nEnter option: ";
        cin>>input;
        
        switch(atoi(input.c_str()))
        {
            case 1:
                if(pS==NULL)
                {
                    pS=new Stack<int>();
                }
                cout<<"\nEnter an element: "<<std::endl;
                cin>>input;
                pS->push(atoi(input.c_str()));
                break;
                
            case 2:
                if(pS==NULL)
                {
                    cout<<"Stack does not exist: select menu item #1 to start"<<endl;
                }
                else
                {
                    pS->pop();
                }
                break;
                
            case 3:
                if(pS==NULL)
                {
                    cout<<"Stack does not exist: select menu item #1 to start"<<endl;
                }
                else
                {
                    pS->print();
                }
                break;
                
            case 4:
                pD=new Stack<int>(5);
                pD->demo();
                pD->~Stack();
                break;
                
            case 5:
                return;
                break;
                
            default:
                cout<<"Choose menu item between 1 and 5"<<endl;
                break;
        }
    }
    
    return;
}
