#include <stdio.h>
#include <stdlib.h>
#pragma warning(disable : 4996)

typedef enum colors { BLACK, RED } nodeColor;

typedef struct node
{
    int key;
    nodeColor color;
    struct node* left;
    struct node* right;
    struct node* p;
} node;

typedef struct tree
{
    node* root;
    node* nil;
} RBTree;

// common delimiter for correct merging of trees
node sentinel = { 0, BLACK, NULL, NULL, NULL };

// Create an empty red-black tree
RBTree* Create(void)
{
    RBTree* tree = malloc(sizeof(RBTree));
    if (tree == NULL)
    {
        fprintf(stderr, "Memory allocation error.\n");
        exit(EXIT_FAILURE);
    }

    tree->nil = &sentinel;
    tree->root = &sentinel;

    return tree;
}

// Clear subtree 
void clear(RBTree* Tree, node* node)
{
    if (node == Tree->nil)
        return;

    /* first delete both subtrees */
    clear(Tree, node->left);
    clear(Tree, node->right);

    /* then delete the node */
    free(node);
}

// Clear tree
void Clear(RBTree* Tree)
{
    clear(Tree, Tree->root);
    free(Tree);
}




// Make a left rotation around the node <x>
void LeftRotate(RBTree* Tree, node* x)
{
    node* y = x->right;

    x->right = y->left;
    if (y->left != Tree->nil)
        y->left->p = x;

    y->p = x->p;
    if (x->p == Tree->nil)
        Tree->root = y;
    else if (x == x->p->left)
        x->p->left = y;
    else
        x->p->right = y;

    y->left = x;
    x->p = y;
}

// Make a right rotation around the node <x>
void RightRotate(RBTree* Tree, node* x)
{
    node* y = x->left;

    x->left = y->right;
    if (y->right != Tree->nil)
        y->right->p = x;

    y->p = x->p;
    if (x->p == Tree->nil)
        Tree->root = y;
    else if (x == x->p->left)
        x->p->left = y;
    else
        x->p->right = y;

    y->right = x;
    x->p = y;
}




// Restore red-black tree properties after insert
void InsertFixUp(RBTree* Tree, node* z)
{
    while (z->p->color == RED)
    {
        if (z->p == z->p->p->left)
        {
            node* y = z->p->p->right;

            if (y->color == RED)
            {
                z->p->color = BLACK;
                y->color = BLACK;
                z->p->p->color = RED;
                z = z->p->p;
            }

            else
            {
                if (z == z->p->right)
                {
                    z = z->p;
                    LeftRotate(Tree, z);
                }

                z->p->color = BLACK;
                z->p->p->color = RED;
                RightRotate(Tree, z->p->p);
            }
        }


        else
        {
            node* y = z->p->p->left;

            if (y->color == RED)
            {
                z->p->color = BLACK;
                y->color = BLACK;
                z->p->p->color = RED;
                z = z->p->p;
            }

            else
            {
                if (z == z->p->left)
                {
                    z = z->p;
                    RightRotate(Tree, z);
                }

                z->p->color = BLACK;
                z->p->p->color = RED;
                LeftRotate(Tree, z->p->p);
            }
        }
    }

    Tree->root->color = BLACK;
}

//+ Insert a new node with the given key
node* Insert(RBTree* Tree, int key)
{
    node* y = Tree->nil;
    node* x = Tree->root;

    while (x != Tree->nil)
    {
        y = x;

        if (key == x->key)
            return NULL;

        x = (key < x->key) ? x->left : x->right;
    }

    node* z = (node*)malloc(sizeof(node));
    if (z == NULL)
    {
        fprintf(stderr, "Memory allocation error.\n");
        exit(EXIT_FAILURE);
    }

    z->p = y;
    z->key = key;

    if (y == Tree->nil)
        Tree->root = z;
    else if (key < y->key)
        y->left = z;
    else
        y->right = z;

    z->left = Tree->nil;
    z->right = Tree->nil;
    z->color = RED;

    InsertFixUp(Tree, z);
    return z;
}




// Find node by key
node* Find(RBTree* Tree, int key)
{
    node* runner = Tree->root;

    while (runner != Tree->nil)
    {
        if (runner->key == key)
            return runner;

        runner = (key < runner->key) ? runner->left : runner->right;
    }

    return NULL;
}

// Find node with minimum key in subtree <x>
node* Minimum(RBTree* Tree, node* x)
{
    while (x->left != Tree->nil)
        x = x->left;

    return x;
}




// Replace subtree <u> with subtree <v>
void Transplant(RBTree* Tree, node* u, node* v)
{
    if (u->p == Tree->nil)
        Tree->root = v;

    else if (u == u->p->left)
        u->p->left = v;

    else
        u->p->right = v;

    v->p = u->p;
}

// Restore red-black tree properties after deletion
void DeleteFixUp(RBTree* Tree, node* x)
{
    while (x != Tree->root && x->color == BLACK)
    {
        if (x == x->p->left)
        {
            node* w = x->p->right;

            if (w->color == RED)
            {
                w->color = BLACK;
                x->p->color = RED;
                LeftRotate(Tree, x->p);
                w = x->p->right;
            }

            if (w->left->color == BLACK && w->right->color == BLACK)
            {
                w->color = RED;
                x = x->p;
            }

            else
            {
                if (w->right->color == BLACK)
                {
                    w->left->color = BLACK;
                    w->color = RED;
                    RightRotate(Tree, w);
                    w = x->p->right;
                }
                w->color = x->p->color;
                x->p->color = BLACK;
                w->right->color = BLACK;
                LeftRotate(Tree, x->p);
                x = Tree->root;
            }
        }


        else
        {
            node* w = x->p->left;

            if (w->color == RED)
            {
                w->color = BLACK;
                x->p->color = RED;
                RightRotate(Tree, x->p);
                w = x->p->left;
            }

            if (w->right->color == BLACK && w->left->color == BLACK)
            {
                w->color = RED;
                x = x->p;
            }

            else
            {
                if (w->left->color == BLACK)
                {
                    w->right->color = BLACK;
                    w->color = RED;
                    LeftRotate(Tree, w);
                    w = x->p->left;
                }
                w->color = x->p->color;
                x->p->color = BLACK;
                w->left->color = BLACK;
                RightRotate(Tree, x->p);
                x = Tree->root;
            }
        }
    }
    x->color = BLACK;
}

// Remove node <z> from tree
void Delete(RBTree* Tree, node* z)
{
    node* y = z;
    nodeColor original = y->color;
    node* x = Tree->nil;

    if (z->left == Tree->nil)
    {
        x = z->right;
        Transplant(Tree, z, z->right);
    }

    else if (z->right == Tree->nil)
    {
        x = z->left;
        Transplant(Tree, z, z->left);
    }

    else
    {
        y = Minimum(Tree, z->right);
        original = y->color;
        x = y->right;

        if (y->p == z)
            x->p = y;

        else
        {
            Transplant(Tree, y, y->right);
            y->right = z->right;
            y->right->p = y;
        }

        Transplant(Tree, z, y);
        y->left = z->left;
        y->left->p = y;
        y->color = z->color;
    }

    if (original == BLACK)
        DeleteFixUp(Tree, x);
}




// Ñalculate the black height of a tree
int GetBlackHeight(RBTree* Tree)
{
    int bh = 0;
    node* x = Tree->root;

    while (x != Tree->nil)
    {
        if (x->color == BLACK)
            bh++;

        x = x->left;
    }

    return bh;
}

node* findR(RBTree* Tree, int bh, int bh_low)
{
    node* x = Tree->root;

    while (bh != bh_low)
    {
        x = x->right;

        if (x->color == BLACK)
            bh--;
    }

    return x;
}
node* findL(RBTree* Tree, int bh, int bh_low)
{
    node* x = Tree->root;

    while (bh != bh_low)
    {
        x = x->left;

        if (x->color == BLACK)
            bh--;
    }

    return x;
}

RBTree* Join(RBTree* T1, node* x, RBTree* T2)
{
    int bh1 = GetBlackHeight(T1);
    int bh2 = GetBlackHeight(T2);

    if (bh1 >= bh2)
    {
        node* y = findR(T1, bh1, bh2);
        Transplant(T1, y, x);
        x->right = T2->root;
        x->left = y;
        T2->root->p = x;
        y->p = x;

        InsertFixUp(T1, x);

        return T1;
    }

    else
    {
        node* y = findL(T2, bh2, bh1);
        Transplant(T2, y, x);
        x->left = T1->root;
        x->right = y;
        T1->root->p = x;
        y->p = x;

        InsertFixUp(T2, x);

        return T2;
    }
}


int main(void)
{
    node* ptr;
    RBTree* tree = Create();

    char mode;
    int key;
    while (fscanf(stdin, "%c %d", &mode, &key) >= 1)
    {
        switch (mode)
        {
        case 'a':
            Insert(tree, key);
            break;

        case 'r':
            ptr = Find(tree, key);
            if (ptr != NULL)
                Delete(tree, ptr);
            break;

        case 'f':
            ptr = Find(tree, key);
            if (ptr == NULL)
                fprintf(stdout, "no\n");
            else
                fprintf(stdout, "yes\n");
            break;

        default: goto END;
        }
    }
END:

    Clear(tree);
    return 0;
}