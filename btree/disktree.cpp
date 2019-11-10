// Copyright(c) 1996 Leendert Ammeraal. All rights reserved.
// This program text occurs in Chapter 7 of
//
//    Ammeraal, L. (1996) Algorithms and Data Structures in C++,
//       Chichester: John Wiley.

/* disktree:
   Demonstration program for a B-tree on disk. After
   building the B-tree by entering integers on the
   keyboard or by supplying them as a text file, we can
   insert and delete items via the keyboard. We can also
   search the B-tree for a given item. Each time, the tree
   or a search path is displayed. In contrast to program
   btree, program disktree writes, reads and updates nodes
   on disk, using a binary file. The name of this file is
   to be entered on the keyboard. If a B-tree with that
   name exists, that B-tree is used; otherwise such a file
   is created.
   Caution:
      Do not confuse the (binary) file for the B-tree with
      the optional textfile for input data. Use different
      file-name extensions, such as .bin and .txt.
*/

#ifndef DISK_TREE_CPP
#define DISK_TREE_CPP

#include <fstream>
#include <iomanip>
#include <stdlib.h>
#include <ctype.h>
#include <iostream>

using namespace std;

#define M 5  // Order of B-tree: M link fields in each node

enum status {InsertNotComplete, Success, DuplicateKey,
             Underflow, NotFound
            };

typedef struct
{
    int key;
    int addr;
} pair_btree;

struct node
{
    int n;        // Number of items stored in a node (n < M)
    pair_btree k[M - 1]; // Data items (only the first n in use)
    long p[M];    // 'Pointers' to other nodes (n+1 in use)
};

// Logical order:
//    p[0], k[0], p[1], k[1], ..., p[n-1], k[n-1], p[n]

class Btree
{
public:
    Btree(const char *TreeFileName);
    ~Btree();
    void insert(pair_btree x);
    void insert(const char *InpFileName);
    void print()
    {
        cout << "Contents:\n";
        pr(root, 0);
    }
    void DelNode(pair_btree x);
    void ShowSearch(pair_btree x);
    int search(pair_btree *x);
private:
    enum {NIL = -1};
    long root, FreeList;
    node RootNode;
    fstream file;
    status ins(long r, pair_btree x, pair_btree &y, long &u);
    void pr(long r, int nSpace);
    int NodeSearch(pair_btree x, const pair_btree *a, int n)const;
    status del(long r, pair_btree x);
    void ReadNode(long r, node &Node);
    void WriteNode(long r, const node &Node);
    void ReadStart();
    long GetNode();
    void FreeNode(long r);
};

Btree::Btree(const char *TreeFileName)
{
    ifstream test(TreeFileName, ios::in/* | ios::nocreate*/);
    // Remove  "| ios::nocreate" if your compiler does not accept it.
    int NewFile = test.fail();
    test.clear();
    test.close();
    if (NewFile)
    {
        file.open(TreeFileName, ios::out | ios::in |
                  ios::trunc | ios::binary);
        // ios::binary required with MSDOS, but possibly
        // not accepted with other environments.
        root = FreeList = NIL;
        long start[2] = {NIL, NIL};
        file.write((char *)start, 2 * sizeof(long));
    }
    else
    {
        long start[2];
        file.open(TreeFileName, ios::out | ios::in/* | ios::nocreate */ | ios::binary); // See above note.
        file.seekg(-1L, ios::end);
        char ch;
        file.read(&ch, 1); // Read signature.
        file.seekg(0L, ios::beg);
        file.read((char *)start, 2 * sizeof(long));
        if (ch != sizeof(int))
        {
            cout << "Wrong file format.\n";
            exit(1);
        }
        root = start[0];
        FreeList = start[1];
        RootNode.n = 0;   // Signal for function ReadNode
        ReadNode(root, RootNode);
        print();
    }
}

Btree::~Btree()
{
    long start[2];
    file.seekp(0L, ios::beg);
    start[0] = root;
    start[1] = FreeList;
    file.write((char *)start, 2 * sizeof(long));
    // The remaining code of this destructor is slightly
    // different from that in the first print of the book.
    // The length of the final binary file, including the
    // signature byte at the end, will now always be an odd
    // number, as it should be. There is a similar change in
    // the function GetNode.I am grateful to Chian Wiz from
    // Singapore, who showed me the possibility of a 'file leak',
    // that is, an unused byte, which sometimes caused problems
    // with the program 'showfile', when this was applied to
    // this binary file. Such problems should no longer occur.
    // L. A.
    char ch = sizeof(int); // Signature
    file.seekg(0L, ios::end);
    if ((file.tellg() & 1) == 0)
        file.write(&ch, 1);
    // If the current file length is an even number, a
    // signature is added; otherwise it is already there.
    file.close();
}

void Btree::insert(pair_btree x)
{
    long pNew;
    pair_btree xNew;
    status code = ins(root, x, xNew, pNew);
    if (code == DuplicateKey)
        cout << "Duplicate key ignored.\n";
    if (code == InsertNotComplete)
    {
        long root0 = root;
        root = GetNode();
        RootNode.n = 1;
        RootNode.k[0] = xNew;
        RootNode.p[0] = root0;
        RootNode.p[1] = pNew;
        WriteNode(root, RootNode);
    }
}

void Btree::insert(const char *InpFileName)
{
    ifstream InpFile(InpFileName, ios::in /*| ios::nocreate*/);
    if (InpFile.fail())
    {
        cout << "Cannot open input file " << InpFileName
             << endl;
        return;
    }
    pair_btree x;
    while(InpFile >> x.key >> x.addr) insert(x);
    InpFile.clear();
    InpFile.close();
}

status Btree::ins(long r, pair_btree x, pair_btree &y, long &q)
{
    // Insert x in *this. If not completely successful, the
    // integer y and the pointer q remain to be inserted.
    // Return value:
    //    Success, DuplicateKey or InsertNotComplete.
    long pNew, pFinal;
    int i, j, n;
    pair_btree xNew, kFinal;
    status code;
    if (r == NIL)
    {
        q = NIL;
        y = x;
        return InsertNotComplete;
    }
    node Node, NewNode;
    ReadNode(r, Node);
    n = Node.n;
    i = NodeSearch(x, Node.k, n);
    if (i < n && x.key == Node.k[i].key) return DuplicateKey;
    code = ins(Node.p[i], x, xNew, pNew);
    if (code != InsertNotComplete) return code;
    // Insertion in subtree did not completely succeed;
    // try to insert xNew and pNew in the current node:
    if (n < M - 1)
    {
        i = NodeSearch(xNew, Node.k, n);
        for (j = n; j > i; j--)
        {
            Node.k[j] = Node.k[j - 1];
            Node.p[j + 1] = Node.p[j];
        }
        Node.k[i] = xNew;
        Node.p[i + 1] = pNew;
        ++Node.n;
        WriteNode(r, Node);
        return Success;
    }
    // Current node is full (n == M - 1) and will be split.
    // Pass item k[h] in the middle of the augmented
    // sequence back via parameter y, so that it
    // can move upward in the tree. Also, pass a pointer
    // to the newly created node back via parameter q:
    if (i == M - 1)
    {
        kFinal = xNew;
        pFinal = pNew;
    }
    else
    {
        kFinal = Node.k[M - 2];
        pFinal = Node.p[M - 1];
        for (j = M - 2; j > i; j--)
        {
            Node.k[j] = Node.k[j - 1];
            Node.p[j + 1] = Node.p[j];
        }
        Node.k[i] = xNew;
        Node.p[i + 1] = pNew;
    }
    int h = (M - 1) / 2;
    y = Node.k[h];           // y and q are passed on to the
    q = GetNode();           // next higher level in the tree
    // The values p[0],k[0],p[1],...,k[h-1],p[h] belong to
    // the left of k[h] and are kept in *r:
    Node.n = h;
    // p[h+1],k[h+1],p[h+2],...,k[M-2],p[M-1],kFinal,pFinal
    // belong to the right of k[h] and are moved to *q:
    NewNode.n = M - 1 - h;
    for (j = 0; j < NewNode.n; j++)
    {
        NewNode.p[j] = Node.p[j + h + 1];
        NewNode.k[j] =
            (j < NewNode.n - 1 ? Node.k[j + h + 1] : kFinal);
    }
    NewNode.p[NewNode.n] = pFinal;
    WriteNode(r, Node);
    WriteNode(q, NewNode);
    return InsertNotComplete;
}

void Btree::pr(long r, int nSpace)
{
    if (r != NIL)
    {
        int i;
        cout << setw(nSpace) << "";
        node Node;
        ReadNode(r, Node);
        for (i = 0; i < Node.n; i++) cout << Node.k[i].key << " ";
        cout << endl;
        for (i = 0; i <= Node.n; i++) pr(Node.p[i], nSpace + 8);
    }
}

int Btree::NodeSearch(pair_btree x, const pair_btree *a, int n)const
{
    int middle, left = 0, right = n - 1;
    if (x.key <= a[left].key) return 0;
    if (x.key > a[right].key) return n;
    while (right - left > 1)
    {
        middle = (right + left) / 2;
        (x.key <= a[middle].key ? right : left) = middle;
    }
    return right;
}

void Btree::ShowSearch(pair_btree x)
{
    cout << "Search path:\n";
    int i, j, n;
    long r = root;
    node Node;
    while (r != NIL)
    {
        ReadNode(r, Node);
        n = Node.n;
        for (j = 0; j < Node.n; j++) cout << " " << Node.k[j].key;
        cout << endl;
        i = NodeSearch(x, Node.k, n);
        if (i < n && x.key == Node.k[i].key)
        {
            cout << "Key " << x.key << " found in position " << i
                 << " of last displayed node.\n";
            return;
        }
        r = Node.p[i];
    }
    cout << "Key " << x.key << " not found.\n";
}

int Btree::search(pair_btree *x)
{
    int i, j, n;
    long r = root;
    node Node;
    while (r != NIL)
    {
        ReadNode(r, Node);
        n = Node.n;
        for (j = 0; j < Node.n; j++) cout << " " << Node.k[j].key;
        cout << endl;
        i = NodeSearch(*x, Node.k, n);
        if (i < n && x->key == Node.k[i].key)
        {
            cout << "Key " << x->key << " found in position " << i
                 << " of last displayed node.\n";
            x->addr = Node.k[i].addr;
            return 1;
        }
        r = Node.p[i];
    }
    cout << "Key " << x->key << " not found.\n";
    return 0;
}


void Btree::DelNode(pair_btree x)
{
    long root0;
    switch (del(root, x))
    {
    case NotFound:
        cout << x.key << " not found.\n";
        break;
    case Underflow:
        root0 = root;
        root = RootNode.p[0];
        FreeNode(root0);
        if (root != NIL) ReadNode(root, RootNode);
        break;
    }
}

status Btree::del(long r, pair_btree x)
{
    if (r == NIL) return NotFound;
    node Node;
    ReadNode(r, Node);
    int i, j, pivot, n = Node.n;
    pair_btree *k = Node.k;  // k[i] means Node.k[i]
    const int nMin = (M - 1) / 2;
    status code;
    long *p = Node.p, pL, pR;       // p[i] means Node.p[i]
    i = NodeSearch(x, k, n);
    if (p[0] == NIL)  // Are we dealing with a leaf?
    {
        if (i == n || x.key < k[i].key) return NotFound;
        // x == k[i]
        for (j = i + 1; j < n; j++)
        {
            k[j - 1] = k[j];
            p[j] = p[j + 1];
        }
        Node.n--;
        WriteNode(r, Node);
        return Node.n >= (r == root ? 1 : nMin) ?
               Success : Underflow;
    }
    // *r is an interior node, not a leaf:
    if (i < n && x.key == k[i].key)
    {
        // x found in an interior node. Go to left child
        // and follow a path all the way to a leaf,
        // using rightmost branches:
        long q = p[i], q1;
        int nq;
        node Node1;
        for (;;)
        {
            ReadNode(q, Node1);
            nq = Node1.n;
            q1 = Node1.p[nq];
            if (q1 == NIL) break;
            q = q1;
        }
        // Exchange k[i] (= x) with rightmost item in leaf:
        k[i] = Node1.k[nq - 1];
        Node1.k[nq - 1] = x;
        WriteNode(r, Node);
        WriteNode(q, Node1);
    }
    // Delete x in leaf of subtree with root p[i]:
    code = del(p[i], x);
    if (code != Underflow) return code;
    // There is underflow; borrow, and, if necessary, merge:
    // Too few data items in node *p[i]
    node NodeL, NodeR;
    if (i > 0)
    {
        pivot = i - 1;
        pL = p[pivot];
        ReadNode(pL, NodeL);
        if (NodeL.n > nMin) // Borrow from left sibling
        {
            // k[pivot] between pL and pR:
            pR = p[i];
            // Increase contents of *pR, borrowing from *pL:
            ReadNode(pR, NodeR);
            NodeR.p[NodeR.n + 1] = NodeR.p[NodeR.n];
            for (j = NodeR.n; j > 0; j--)
            {
                NodeR.k[j] = NodeR.k[j - 1];
                NodeR.p[j] = NodeR.p[j - 1];
            }
            NodeR.n++;
            NodeR.k[0] = k[pivot];
            NodeR.p[0] = NodeL.p[NodeL.n];
            k[pivot] = NodeL.k[--NodeL.n];
            WriteNode(pL, NodeL);
            WriteNode(pR, NodeR);
            WriteNode(r, Node);
            return Success;
        }
    }
    pivot = i;
    if (i < n)
    {
        pR = p[pivot + 1];
        ReadNode(pR, NodeR);
        if (NodeR.n > nMin) // Borrow from right sibling
        {
            // k[pivot] between pL and pR:
            pL = p[pivot];
            ReadNode(pL, NodeL);
            // Increase contents of *pL, borrowing from *pR:
            NodeL.k[NodeL.n] = k[pivot];
            NodeL.p[NodeL.n + 1] = NodeR.p[0];
            k[pivot] = NodeR.k[0];
            NodeL.n++;
            NodeR.n--;
            for (j = 0; j < NodeR.n; j++)
            {
                NodeR.k[j] = NodeR.k[j + 1];
                NodeR.p[j] = NodeR.p[j + 1];
            }
            NodeR.p[NodeR.n] = NodeR.p[NodeR.n + 1];
            WriteNode(pL, NodeL);
            WriteNode(pR, NodeR);
            WriteNode(r, Node);
            return Success;
        }
    }
    // Merge; neither borrow left nor borrow right possible.
    pivot = (i == n ? i - 1 : i);
    pL = p[pivot];
    pR = p[pivot + 1];
    // Add k[pivot] and *pR to *pL:
    ReadNode(pL, NodeL);
    ReadNode(pR, NodeR);
    NodeL.k[NodeL.n] = k[pivot];
    NodeL.p[NodeL.n + 1] = NodeR.p[0];
    for (j = 0; j < NodeR.n; j++)
    {
        NodeL.k[NodeL.n + 1 + j] = NodeR.k[j];
        NodeL.p[NodeL.n + 2 + j] = NodeR.p[j + 1];
    }
    NodeL.n += 1 + NodeR.n;
    FreeNode(pR);
    for (j = i + 1; j < n; j++)
    {
        k[j - 1] = k[j];
        p[j] = p[j + 1];
    }
    Node.n--;
    WriteNode(pL, NodeL);
    WriteNode(r, Node);
    return
        Node.n >= (r == root ? 1 : nMin) ? Success : Underflow;
}

void Btree::ReadNode(long r, node &Node)
{
    if (r == NIL) return;
    if (r == root && RootNode.n > 0) Node = RootNode;
    else
    {
        file.seekg(r, ios::beg);
        file.read((char *)&Node, sizeof(node));
    }
}

void Btree::WriteNode(long r, const node &Node)
{
    if (r == root) RootNode = Node;
    file.seekp(r, ios::beg);
    file.write((char *)&Node, sizeof(node));
}

void Btree::ReadStart()
{
    long start[2];
    file.seekg(0L, ios::beg);
    file.read((char *)start, 2 * sizeof(long));
    root = start[0];
    FreeList = start[1];
    ReadNode(root, RootNode);
}

long Btree::GetNode()  // Modified (see also the destructor ~Btree)
{
    long r;
    node Node;
    if (FreeList == NIL)
    {
        file.seekp(0L, ios::end); // Allocate space on disk; if
        r = file.tellp() & ~1;    // file length is an odd number,
        WriteNode(r, Node);       // the new node will overwrite
    }
    else                         // signature byte at end of file
    {
        r = FreeList;
        ReadNode(r, Node);        // To update FreeList:
        FreeList = Node.p[0];     // Reduce the free list by 1
    }
    return r;
}


void Btree::FreeNode(long r)
{
    node Node;
    ReadNode(r, Node);
    Node.p[0] = FreeList;
    FreeList = r;
    WriteNode(r, Node);
}
/*
int main()
{
    return 0;
}*/

/*
int main()
{
    cout <<
         "Demonstration program for a B-tree on disk. The\n"
         "structure of the B-tree is shown by indentation.\n"
         "For each node, the number of links to other nodes\n"
         "will not be greater than " << M <<
         ", the order M of the B-tree.\n" <<
         "The B-tree representation is similar to the\n"
         "table of contents of a book. The items stored in\n"
         "each node are displayed on a single line.\n\n";
    char TreeFileName[50];
    cout <<
         "Enter name of (possibly nonexistent) BINARY file for\n"
         "the B-tree: ";
    cin >> setw(50) >> TreeFileName;
    Btree t(TreeFileName);
    cout <<
         "\nEnter a (possibly empty) sequence of integers,\n"
         "followed by a slash (/):\n";
    pair_btree x;
    char ch = 0;
    while (cin >> x, !cin.fail())
    {
        t.insert(x);
        ch = 1;
    }
    if (ch) t.print();
    cin.clear();
    cin >> ch; // Skip terminating character
    cout <<  "\nDo you want data to be read from a text"
         "file? (Y/N): ";
    cin >> ch;
    if (toupper(ch) == 'Y')
    {
        char InpFileName[50];
        cout << "Name of this textfile: ";
        cin >> setw(50) >> InpFileName;
        t.insert(InpFileName);
        t.print();
    }
    for (;;)
    {
        cout <<
             "\nEnter an integer, followed by I, D, or S (for\n"
             "Insert, Delete and Search), or enter Q to quit: ";
        cin >> x >> ch;
        if (cin.fail()) break;
        ch = toupper(ch);
        switch(ch)
        {
        case 'S':
            t.ShowSearch(x);
            break;
        case 'I':
            t.insert(x);
            break;
        case 'D':
            t.DelNode(x);
            break;
        default:
            cout << "Invalid command, use S, I or D\n";
            break;
        }
        if (ch == 'I' || ch == 'D') t.print();
    }
    return 0;
}

*/

#endif