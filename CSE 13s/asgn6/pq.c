#include "pq.h"

#include "node.h"

#include <assert.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct ListElement ListElement;

struct ListElement {
    Node *tree;
    ListElement *next;
};

struct PriorityQueue {
    ListElement *list;
    uint32_t size;
};

PriorityQueue *pq_create(void) {
    PriorityQueue *pq = calloc(1, sizeof(PriorityQueue));
    pq->size = 0;
    return pq;
}

void pq_free(PriorityQueue **q) {
    assert(q);
    if (*q == NULL) {
        return;
    }
    free(*q);
    *q = NULL;
}

bool pq_is_empty(PriorityQueue *q) {
    assert(q != NULL);
    return q->size == 0;
}

bool pq_size_is_1(PriorityQueue *q) {
    assert(q != NULL);
    return q->size == 1;
}

bool pq_less_than(Node *n1, Node *n2) {
    assert(n1);
    assert(n2);
    if (n1->weight < n2->weight)
        return true;
    if (n1->weight > n2->weight)
        return false;
    return n1->symbol < n2->symbol;
}

// O(n) to insert
void enqueue(PriorityQueue *p, Node *tree) {
    assert(p);
    assert(tree);
    if (p == NULL || tree == NULL) {
        return;
    }
    // Create new element
    ListElement *le = calloc(1, sizeof(ListElement));
    assert(le != NULL);
    le->tree = tree;
    le->next = NULL;

    // Add element to PQ
    // if empty, make list point to new element
    if (p->size == 0) {
        p->list = le;
    } else {
        // We need to put element in "right" spot
        // The new item is "better" than our current best,
        // Make the p->list have tree first
        if (pq_less_than(tree, p->list->tree)) {
            le->next = p->list;
            p->list = le;
        } else {
            // New item is somewhere not first
            // Find the right spot to insert tree
            ListElement *curr = p->list;
            ListElement *prev = NULL;
            while (curr != NULL) {
                if (pq_less_than(tree, curr->tree)) {
                    // tree should go here
                    prev->next = le;
                    le->next = curr;
                    break;
                }
                // We made it to the end of the list
                if (curr->next == NULL) {
                    curr->next = le;
                    break;
                }
                prev = curr;
                curr = curr->next;
            }
        }
    }

    // Update size
    p->size++;
}

// O(1)
bool dequeue(PriorityQueue *q, Node **tree) {
    assert(q);
    assert(tree);
    if (q == NULL || tree == NULL) {
        return false;
    }

    if (q->size == 0) {
        return false;
    }

    // Put answer in tree
    *tree = q->list->tree;

    // Remove head from list
    ListElement *victim = q->list;
    q->list = victim->next;
    free(victim);

    // decrement size by q
    q->size--;
    return true;
}

void pq_print(PriorityQueue *q) {
    assert(q != NULL);
    ListElement *e = q->list;
    int position = 1;
    while (e != NULL) {
        if (position++ == 1) {
            printf("=============================================\n");
        } else {
            printf("---------------------------------------------\n");
        }
        node_print_tree(e->tree, '<', 2);
        e = e->next;
    }
    printf("=============================================\n");
}
