//PageRank

#include <iostream>
using namespace std;

typedef long long int longint;

typedef struct node
{
    node *down;
    node *right;
    node *next;

    int flag;
    // flag=0 for matrix head
    // flag=1 for head node
    // flag=2 for general node

    union
    {
        struct {
            // struct type for matrix head
            longint m;
            longint n;
        } mData;
        
        struct {
            // struct type for head node
            longint h;
        } hData;

        struct {
            // struct type for general node
            longint x;
            longint y;
            longint value;
        } Data;
    };
} node;

node *head= new node;

node* createNode(longint x, longint y)
{
    // initialize a new typical node
    node *n= new node;
    n->flag=2;
    n->Data.x= x;
    n->Data.y= y;
    n->Data.value= 0;
    return n;
}

node* createHead(longint x, node *y)
{
    // to initialize a new head node
    node *n= new node;
    n->flag=1;
    n->down= n;
    n->right= n;
    n->hData.h= x;
    n->next= y;
    return n;
}

void AddLink(longint i, longint j)
{
    // flag becomes false if a new node is created
    bool flag= true;

    longint minimum= min(i,j);
    node *t= head;

    // iterates to the required head node which is minimum of the coordinates
    while(t->next->flag==1 && t->next->hData.h <= minimum)
        t= t->next;

    // if head node doesn't exist, this creates it and moves to it
    if (t->flag==0 || t->hData.h!=minimum)
    {
        t->next= createHead(minimum, t->next);
        t= t->next;
    }

    // to preserve the position of head node
    node *t2= t;

    if (j==minimum)
    {
        // traverse right
        while(t->right->flag==2 && t->right->Data.y<=i)
            t= t->right;
        if (t->flag==1 || t->Data.y!=i)
        {
            if (t->flag==1 && t->right->flag==1)
                head->mData.m++;
            node *temp= t->right;
            t->right= createNode(j,i);
            t= t->right;
            t->right= temp;
            flag= false;
        }
        t->Data.value+=1;
    }
    else
    {
        // traverse down
        while(t->down->flag==2 && t->down->Data.x<=j)
            t= t->down;
        if (t->flag==1 || t->Data.x!=j)
        {
            if (t->flag==1 && t->down->flag==1)
                head->mData.n++;
            node *temp= t->down;
            t->down= createNode(j,i);
            t= t->down;
            t->down= temp;
            flag= false;
        }
        t->Data.value+=1;
    }
    if (flag){
        return;
    }
    // if a new node is created, we have to link it the other way too
    else{
        longint maximum= max(i,j);
        while (t2->next->flag==1 && t2->next->hData.h <= maximum)
            t2= t2->next;
        if (t2->flag==0 || t2->hData.h!=maximum)
        {
            t2->next= createHead(maximum, t2->next);
            t2= t2->next;
        }
        if (i==maximum)
        {
            while(t2->down->flag==2 && t2->down->Data.x<=j)
                t2= t2->down;
            if (t2->flag==1 && t2->down->flag==1)
                head->mData.n++;
            t->down= t2->down;
            t2->down= t;
        }
        else
        {
            while(t2->right->flag==2 && t2->right->Data.y<=i)
                t2= t2->right;
            if (t2->flag==1 && t2->right->flag==1)
                head->mData.m++;
            t->right= t2->right;
            t2->right= t;
        }
    }
        
}

void DeleteLink(longint i, longint j)
{
    // flag is set to false if a node is deleted
    bool flag= true;

    node *t= head;
    longint minimum= min(i,j);
    while(t->next->flag==1 && t->next->hData.h<minimum)
        t=t->next;
    if (t->next->flag!=1 || t->next->hData.h!=minimum)
        return;
    
    // I assert here that that t->next contains the pointer of the head node in which I have to delete link
    node *t2=t->next;
    if (j==minimum)
    {
        // traverse right
        while(t2->right->flag==2 && t2->right->Data.y<i)
            t2= t2->right;
        if (t2->right->flag!=2 || t2->right->Data.y!=i)
            return;
        t2->right->Data.value-=1;
        if (t2->right->Data.value==0)
        {
            flag= false;
            t2->right= t2->right->right;
            if (t2->flag==1 && t2->right->flag==1)
                head->mData.m--;
        }
    }
    else
    {
        // traverse down
        while(t2->down->flag==2 && t2->down->Data.x<j)
            t2=t2->down;
        if (t2->down->flag!=2 || t2->down->Data.x!=j)
            return;
        t2->down->Data.value-=1;
        if (t2->down->Data.value==0)
        {
            flag= false;
            t2->down= t2->down->down;
            if (t2->flag==1 && t2->down->flag==1)
                head->mData.n--;
        }
    }
    if (flag)
        return;
    
    if (t->next->down==t->next && t->next->right==t->next)
        t->next= t->next->next;

    // Delete other linking
    longint maximum= max(i,j);
    while(t->next->hData.h<maximum)
        t= t->next;
    
    // I assert here that t->next->hData.h== maximum
    node *t3= t->next;
    if (i==maximum)
    {
        while(t3->down->Data.x<j)
            t3= t3->down;
        // Assertion: t3->down->Data.x==j
        t3->down= t3->down->down;
        if (t3->flag==1 && t3->down->flag==1)
            head->mData.n--;
    }
    else
    {
        while(t3->right->Data.y<i)
            t3= t3->right;
        // Assertion: t3->down->Data.y==i
        t3->right= t3->right->right;
        if (t3->flag==1 && t3->right->flag==1)
            head->mData.m--;
    }

    if (t->next->down==t->next && t->next->right==t->next)
        t->next= t->next->next;
}

void RetrieveValue(longint i, longint j)
{
    node *t= head;
    longint minimum= min(i,j);
    while(t->next->flag==1 && t->next->hData.h<minimum)
        t=t->next;
    if (t->next->flag!=1 || t->next->hData.h!=minimum)
    {
        // if head node doesn't exist return 0
        cout << 0 << "\n";
        return;
    }
    t= t->next;
    if (j==minimum)
    {
        while (t->right->flag==2 && t->right->Data.y<i)
            t= t->right;
        if (t->right->flag!=2 || t->right->Data.y!=i)
        {
            // if node doesn't exist return 0
            cout << 0 << "\n";
            return;
        }
        cout << t->right->Data.value << "\n";
    }
    else
    {
        while (t->down->flag==2 && t->down->Data.x<j)
            t= t->down;
        if (t->down->flag!=2 || t->down->Data.x!=j)
        {
            // if node doesn't exist return 0
            cout << 0 << "\n";
            return;
        }
        cout << t->down->Data.value << "\n";
    }

}

void RetrieveRowSumUptoKthColumn(longint i, longint k)
{
    node *t=head;
    while(t->next->flag==1 && t->next->hData.h<i)
        t= t->next;
    if (t->next->flag!=1 || t->next->hData.h!=i)
    {
        // if head node doesn't exist return 0
        cout << 0 << "\n";
        return;
    }
    t=t->next;
    longint sum=0;

    // iterate rightwards from the start in the head node till the given column
    while(t->right->flag==2 && t->right->Data.y<k)
    {
        t= t->right;
        sum+= t->Data.value;
    }
    cout << sum << "\n";
}

void RetrieveColumnSumUptoKthRow(longint i, longint k)
{
    node *t=head;
    while(t->next->flag==1 && t->next->hData.h<i)
        t= t->next;
    if (t->next->flag!=1 || t->next->hData.h!=i)
    {
        // if head node doesn't exist return 0
        cout << 0 << "\n";
        return;
    }
    t=t->next;
    longint sum=0;

    // iterate downwards from the start of the head node till the given row
    while(t->down->flag==2 && t->down->Data.x<k)
    {
        t= t->down;
        sum+= t->Data.value;
    }
    cout << sum << "\n";
}

void MultiplyVector(longint n, longint a[])
{
    // handle boundary case of n=0
    if (n<=0)
    {
        cout << 0 << "\n";
        return;
    }

    // initialize an array which will be returned finally
    longint *ret= new longint[n];
    for (longint i=0; i<n; i++)
        ret[i]=0;

    node *t =head;

    while(t->next->flag==1 && t->next->hData.h<n) // traverse through the head nodes
    {
        t=t->next;
        node *t2= t;
        longint sum=0;
        while(t2->right->flag==2 && t2->right->Data.y<n) // traverse through the columns in each head node
        {
            t2= t2->right;
            sum+= (t2->Data.value)*a[t2->Data.y];
        }
        ret[t->hData.h]= sum; 
    }

    for(longint i=0; i<n-1; i++)
        cout << ret[i] << " ";
    cout << ret[n-1] << "\n";
}


int main()
{
    //initialize the matrix head
    head->flag= 0;
    head->down= nullptr;
    head->right= nullptr;
    head->mData.m= 0;
    head->mData.n= 0;
    head->next= head;

    longint ops;
    cin >> ops;
    
    if (ops==0)
        cout << 0 << "\n";

    for (longint i=0; i<ops; i++)
    {
        longint opCode;
        cin >> opCode;
        if (opCode==1)
        {
            longint o1, o2;
            cin >> o1;
            cin >> o2;
            AddLink(o1, o2);
        }
        else if (opCode==2)
        {
            longint o1, o2;
            cin >> o1;
            cin >> o2;
            DeleteLink(o1, o2);
        }
        else if (opCode==3)
        {
            longint o1, o2;
            cin >> o1;
            cin >> o2;
            RetrieveValue(o1, o2);
        }
        else if (opCode==4)
        {
            longint o1, o2;
            cin >> o1;
            cin >> o2;
            RetrieveRowSumUptoKthColumn(o1, o2);
        }
        else if (opCode==5)
        {
            longint o1, o2;
            cin >> o1;
            cin >> o2;
            RetrieveColumnSumUptoKthRow(o1, o2);
        }
        else if (opCode==6)
        {
            longint o1;
            cin >> o1;
            longint o2[o1];
            for (longint i=0; i<o1; i++)
                cin >> o2[i];
            MultiplyVector(o1,o2);
        }
        else
        {
            cout << 0 << "\n";
        }
    }
}