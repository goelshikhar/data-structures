#include <iostream>
using namespace std;

struct Term {
    double coff;
    int deg;
};

Term *ARR= new Term[250];

int main()
{
    int x;
    cin >> x;
    for (int i=0; i<x; i++)
        cin >> ARR[i].deg;
    for (int i=0; i<x; i++)
        cin >> ARR[i].coff;
    int y;
    cin >> y;
    for (int i=x; i<(x+y); i++)
        cin >> ARR[i].deg;
    for (int i=x; i<(x+y); i++)
        cin >> ARR[i].coff;
    
    int start= x+y;
    for (int i= 0; i< x; i++)
    {
        bool flag=false;
        for (int j=x+y; j<start; j++)
        {
            if (ARR[j].deg== ARR[i].deg)
            {
                flag=true;
                ARR[j].coff+= ARR[i].coff;
                break;
            }
        }
        if (flag==false)
        {
            ARR[start].deg= ARR[i].deg;
            ARR[start].coff= ARR[i].coff;
            start+=1;
        }
    }
    
    for (int i=x; i<(x+y); i++)
    {
        bool flag= false;
        for (int j=x+y; j<start; j++)
        {
            if (ARR[j].deg==ARR[i].deg)
            {
                flag=true;
                ARR[j].coff+= ARR[i].coff;
                break;
            }
        }
        if (flag==false)
        {
            ARR[start].deg= ARR[i].deg;
            ARR[start].coff= ARR[i].coff;
            start+=1;
        }
    }

    for (int i=x+y; i<start-1; i++)
    {
        for (int j=x+y; j< start-1; j++){
            if (ARR[j].deg < ARR[j+1].deg) {
                Term swap;
                swap.deg= ARR[j+1].deg;
                swap.coff= ARR[j+1].coff;
                ARR[j+1].deg= ARR[j].deg;
                ARR[j+1].coff= ARR[j].coff;
                ARR[j].deg= swap.deg;
                ARR[j].coff= swap.coff;
            }
        }
    }

    int a= start- x-y;

    for (int i= 0; i<x; i++)
    {
        for (int j=0; j<y; j++)
        {
            bool flag= false;
            for (int k=x+y+a; k<start; k++)
            {
                if (ARR[k].deg==(ARR[x+j].deg + ARR[i].deg))
                {
                    flag=true;
                    ARR[k].coff+= (ARR[x+j].coff * ARR[i].coff);
                    break;
                }
            }
            if (flag==false)
            {
                ARR[start].deg= (ARR[x+j].deg + ARR[i].deg);
                ARR[start].coff= (ARR[x+j].coff * ARR[i].coff);
                start+=1;
            }
        }
    }

    for (int i=x+y+a; i<start-1; i++)
    {
        for (int j=x+y+a; j< start-1; j++){
            if (ARR[j].deg < ARR[j+1].deg) {
                Term swap;
                swap.deg= ARR[j+1].deg;
                swap.coff= ARR[j+1].coff;
                ARR[j+1].deg= ARR[j].deg;
                ARR[j+1].coff= ARR[j].coff;
                ARR[j].deg= swap.deg;
                ARR[j].coff= swap.coff;
            }
        }
    }

    for (int i =x+y; i< x+y+a; i++){
        if (ARR[i].coff!=0.0)
            cout << ARR[i].deg << " ";
    }
    cout << "\n";
    for (int i =x+y; i< x+y+a; i++){
        if (ARR[i].coff!=0.0)
            cout << ARR[i].coff << " ";
    }
    cout << "\n";
    for (int i =x+y+a; i< start; i++){
        if (ARR[i].coff!=0.0)
            cout << ARR[i].deg << " ";
    }
    cout << "\n";
    for (int i =x+y+a; i< start; i++){
        if (ARR[i].coff!=0.0)
            cout << ARR[i].coff << " ";
    }
    cout << "\n";

    return 0;
}