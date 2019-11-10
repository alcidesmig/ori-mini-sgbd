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

#ifndef DISK_TREE_HPP
#define DISK_TREE_HPP

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

class Btree {
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

/*
Btree::Btree(const char *TreeFileName);
Btree::~Btree();
void Btree::insert(pair_btree x);
void Btree::insert(const char *InpFileName);
status Btree::ins(long r, pair_btree x, pair_btree &y, long &q);
void Btree::pr(long r, int nSpace);
int Btree::NodeSearch(pair_btree x, const pair_btree *a, int n) const;
void Btree::ShowSearch(pair_btree x);
int Btree::search(pair_btree *x);
void Btree::DelNode(pair_btree x);
status Btree::del(long r, pair_btree x);
void Btree::ReadNode(long r, node &Node);
void Btree::WriteNode(long r, const node &Node);
void Btree::ReadStart();
long Btree::GetNode();
void Btree::FreeNode(long r);
*/

#endif