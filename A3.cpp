// Market Data Publisher
#include <iostream>
using namespace std;

typedef long long int longint;

typedef struct stock
{
    longint id;
    longint price;
} stock;

stock *bulk;

typedef struct node
{
    stock value;
    node *ll;
    node *rl;
    int bf;
} node;

node *tree= nullptr;
longint threshold;

// utility function to calculate balance factor for initial tree creation
// return -1 if number of elements is an integral power of 2 greater than 1
int bfcal(longint n)
{
    if (n==1)
        return 0;
    while (n!=1)
    {
        if (n%2!=0)
            return 0;
        n=n/2;
    }
    return -1;
}

// utility function to create a node
node *createNode(longint id, longint price)
{
    node *n= new node;
    n->value.id= id;
    n->value.price= price;
    n->bf= 0;
    n->ll= nullptr;
    n->rl= nullptr;
    return n;
}

// function for initial tree creation
node *createTree(longint start, longint end)
{
    longint mid= (start+end)/2;

    // create node of the middle element
    node *n= createNode(bulk[mid].id, bulk[mid].price);

    // calculate balance factor for middle element
    n->bf= bfcal(end-start+1);

    // for only one element, return middle node
    if (start==end)
        return n;

    // if more than 2 elements (start!=end and start!=mid), create left subtree from left part of array
    if (start!=mid)
        n->ll= createTree(start, mid-1);

    // create right subtree from right part of array
    n->rl= createTree(mid+1, end);

    return n;
}

// function to create a sorted array from two smaller sorted arrays
void merge(longint a, longint b)
{
    longint mid= (a+b)/2;

    // create a local array for storage of sorted array, memory will be freed after function return
    stock temp[b-a + 1];
    
    longint curr=0; // initial pointer for storage location in temp array
    longint x=a;
    longint y= mid +1;
    while ( x!= mid+1 && y!=b+1) // stop after one half of bulk array is completed
    {
        if (bulk[x].id<bulk[y].id)
        {
            temp[curr].id= bulk[x].id;
            temp[curr].price= bulk[x].price;
            x+=1;
            curr+=1;
        }
        else
        {
            temp[curr].id= bulk[y].id;
            temp[curr].price= bulk[y].price;
            y+=1;
            curr+=1;
        }
    }

    // fill the temp array with the remaining elements from other side of bulk array
    if (x==mid+1)
    {
        for (; y!=b+1; y++)
        {
            temp[curr].id= bulk[y].id;
            temp[curr].price= bulk[y].price;
            curr+=1;
        }
    }
    else if (y==b+1)
    {
        for (; x!=mid+1; x++)
        {
            temp[curr].id= bulk[x].id;
            temp[curr].price= bulk[x].price;
            curr+=1;
        }
    }

    // copy temp array into bulk array
    for (longint i=0; i < b-a+1; i++)
    {
        bulk[a+i].id= temp[i].id;
        bulk[a+i].price= temp[i].price;
    }
}

void mergesort(longint start, longint end)
{
    if (start>=end)
        return;
    longint mid= (start+end)/2;

    // recursively mergesort left and right half of the array
    mergesort(start, mid);
    mergesort(mid+1, end);

    // merge the two sorted array
    merge(start, end);
}

// flag variable to know if length has increased below
bool lhin;

// flag variable to know if length has decreased below
bool lhdec;

// utility function for Left-left Rotation Case
node *rotateLL(node *x)
{
    // handle invalid cases
    if (x==nullptr || x->ll==nullptr)
        return x;

    // perform rotation
    node *n= x->ll;
    x->ll= n->rl;
    n->rl= x;

    // update balance factor
    if (n->bf==0)
    {
        lhdec=false;
        n->bf=-1;
        x->bf=1;
    }
    else if (n->bf==1)
    {
        n->bf=0;
        x->bf=0;
    }
    return(n);
}

// utility function for Right-Right Rotation Case
node *rotateRR(node *x)
{
    // handle invalid cases
    if (x==nullptr || x->rl==nullptr)
        return x;

    // perform rotation
    node *n= x->rl;
    x->rl= n->ll;
    n->ll= x;

    // update balance factor
    if (n->bf==0)
    {
        lhdec=false;
        n->bf=1;
        x->bf=-1;
    }
    else if (n->bf==-1)
    {
        n->bf=0;
        x->bf=0;
    }
    return(n);
}

node *rotateLR(node *x)
{
    // handle invalid cases
    if (x==nullptr || x->ll==nullptr || x->ll->rl==nullptr)
        return x;

    // perform rotation
    node *y= x->ll;
    node *z= y->rl;
    y->rl=z->ll;
    x->ll=z->rl;
    z->ll= y;
    z->rl= x;

    // update balance factor
    if (z->bf==1)
    {
        x->bf=-1;
        y->bf=0;
    }
    else if (z->bf==-1)
    {
        y->bf=1;
        x->bf=0;
    }
    else
    {
        x->bf=0;
        y->bf=0;
    }
    z->bf=0;
    return(z);
}

node *rotateRL(node *x)
{
    // handle invalid cases
    if (x==nullptr || x->rl==nullptr || x->rl->ll==nullptr)
        return x;

    // perform rotation
    node *y= x->rl;
    node *z= y->ll;
    y->ll=z->rl;
    x->rl=z->ll;
    z->rl= y;
    z->ll= x;

    // update balance factor
    if (z->bf==1)
    {
        x->bf=0;
        y->bf=-1;
    }
    else if (z->bf==-1)
    {
        x->bf=1;
        y->bf=0;
    }
    else
    {
        x->bf=0;
        y->bf=0;
    }
    z->bf=0;
    return(z);
}

node *registerCompany(node *branch, longint id, longint price)
{
    // create node at empty leaf position, length increases
    if (branch==nullptr)
    {
        lhin=true;
        return(createNode(id, price));
    }

    // recursively insert in left subtree
    if (id < branch->value.id)
        branch->ll= registerCompany(branch->ll, id, price);

    // recursively insert in right subtree
    else if (id > branch->value.id)
        branch->rl= registerCompany(branch->rl, id, price);

    // ignore if id already exists
    else
    {
        lhin=false;
        return(branch);
    }

    // update balance factor if length of tree below increases
    if (lhin==true)
    {
        if (id < branch->value.id)
        {
            if (branch->bf<0)
                lhin=false;
            branch->bf+=1;
        }
        else
        {
            if (branch->bf>0)
                lhin=false;
            branch->bf-=1;
        }
    }

    // rotate at unbalanced nodes
    if (branch->bf==2)
    {
        lhin=false;
        if (branch->ll!=nullptr)
        {
            if(branch->ll->bf==1)
            return(rotateLL(branch));
        else
            return(rotateLR(branch));
        }
    }
    else if (branch->bf==-2)
    {
        lhin=false;
        if (branch->rl!=nullptr)
        {
            if(branch->rl->bf==-1)
            return(rotateRR(branch));
        else
            return(rotateRL(branch));
        }
    }
    return(branch);
}

node *deregisterCompany(node *branch, longint id)
{
    // ignore if id doesn't exist
    if(branch==nullptr)
    {
        lhdec= false;
        return(nullptr);
    }

    longint currvalue= branch->value.id;

    // recursive delete in left subtree
    if (id < branch->value.id)
        branch->ll= deregisterCompany(branch->ll, id);

    // recursively delete in right subtree
    else if (id > branch->value.id)
        branch->rl= deregisterCompany(branch->rl, id);
    else
    {
        if (branch->ll==nullptr && branch->rl==nullptr)
        {
            // if no child, then delete leaf node; length decreases
            lhdec= true;
            free(branch);
            return(nullptr);
        }
        else if (branch->ll==nullptr || branch->rl==nullptr)
        {
            // if one child then remove node and return pointer to only child
            lhdec=true;
            if (branch->ll==nullptr)
            {
                node *temp= branch->rl;
                free(branch);
                return(temp);
            }
            else
            {
                node *temp= branch->ll;
                free(branch);
                return(temp);
            }
        }
        else
        {
            // if 2 child then copy id and price of next successor and delete it from right subtree
            node *minRight= branch->rl;
            while(minRight->ll!=nullptr)
                minRight= minRight->ll;
            branch->value.id= minRight->value.id;
            branch->value.price= minRight->value.price;
            branch->rl= deregisterCompany(branch->rl, minRight->value.id);
        }
    }

    // update balance factor if the length below decreases
    if(lhdec==true)
    {
        if (id < currvalue)
        {
            if (branch->bf<=0)
                lhdec=false;
            branch->bf-=1;
        }
        else if (id > currvalue)
        {
            if (branch->bf>=0)
                lhdec=false;
            branch->bf+=1;
        }
    }

    // rotate if the node becomes unbalanced
    if (branch->bf==2)
    {
        if (branch->ll!=nullptr)
        {
            lhdec=true;
        if(branch->ll->bf==-1)
            return(rotateLR(branch));
        else
            return(rotateLL(branch));
        }
    }
    else if (branch->bf==-2)
    {
        if (branch->rl!=nullptr)
        {
            lhdec=true;
        if(branch->rl->bf==1)
            return(rotateRL(branch));
        else
            return(rotateRR(branch));
        }
    }
    return(branch);

}

void updatePrice(node *branch, longint id, longint price)
{
    // ignore if id doesn't exist
    if (branch==nullptr)
        return;
    
    // recursively update price in left subtree
    if (id < branch->value.id)
        updatePrice(branch->ll, id, price);

    // recursively update price in right subtree
    else if (id > branch->value.id)
        updatePrice(branch->rl, id, price);

    else
    {
        // update price and output id and new price if difference greater than threshold
        if (abs(price-branch->value.price)>threshold)
        {
            branch->value.price= price;
            cout << branch->value.id << " " << branch->value.price << "\n";
        }
    }
}

void stockSplit(node *branch, longint id, longint x, longint y)
{
    // ignore if id doesn't exist
    if (branch==nullptr)
        return;
    
    // recursively split stock in left subtree
    if (id < branch->value.id)
        stockSplit(branch->ll, id, x, y);

    // recursively split stock in right subtree
    else if (id > branch->value.id)
        stockSplit(branch->rl, id, x, y);
    else
    {
        // split stock at the current node
        branch->value.price= (branch->value.price*y)/x;
        cout << branch->value.id << " " << branch->value.price << "\n";
    }
}

int main()
{

    // input bulk load stocks
    longint bulkops;
    cin >> bulkops;
    bulk= new stock[bulkops];
    for (longint i=0; i<bulkops; i++)
        cin >> bulk[i].id >> bulk[i].price;

    // now merge sort bulk load stocks
    mergesort(0,bulkops-1);

    // create initial tree from sorted array
    if (bulkops>0)
        tree= createTree(0, bulkops-1);

    // input number of operations and threshold value
    longint ops;
    cin >> ops >> threshold;

    // perform ops operations
    for(int i=0; i<ops; i++)
    {
        longint opCode;
        cin >> opCode;
        if (opCode==1)
        {
            longint o1, o2;
            cin >> o1 >> o2;
            tree= registerCompany(tree, o1, o2);
            cout << o1 << " " << o2 << "\n";
        }
        else if (opCode==2)
        {
            longint o1;
            cin >> o1;
            if (tree!=nullptr)
                tree= deregisterCompany(tree, o1);
        }
        else if (opCode==3)
        {
            longint o1, o2;
            cin >> o1 >> o2;
            updatePrice(tree, o1, o2);
        }
        else if (opCode==4)
        {
            longint o1;
            string o2;
            cin >> o1 >> o2;
            longint pos= o2.find(":");
            longint r1= stoll(o2.substr(0, pos));
            longint r2= stoll(o2.substr(pos+1, o2.length()));
            stockSplit(tree, o1, r1, r2);
        }
        
    }
}