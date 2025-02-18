#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "harness.h"
#include "queue.h"
#include "stdint.h"

/* Notice: sometimes, Cppcheck would find the potential NULL pointer bugs,
 * but some of them cannot occur. You can suppress them by adding the
 * following line.
 *   cppcheck-suppress nullPointer
 */

/*
 * Create empty queue.
 * Return NULL if could not allocate space.
 */
struct list_head *q_new()
{
    struct list_head *n = malloc(sizeof(struct list_head));

    if (!n)
        return NULL;

    INIT_LIST_HEAD(n);

    return n;
}

/* Free all storage used by queue */
void q_free(struct list_head *l)
{
    if (!l)
        return;

    element_t *pos, *n;

    list_for_each_entry_safe (pos, n, l, list)
        q_release_element(pos);

    free(l);
}

/*
 * Attempt to insert element at head of queue.
 * Return true if successful.
 * Return false if q is NULL or could not allocate space.
 * Argument s points to the string to be stored.
 * The function must explicitly allocate space and copy the string into it.
 */
bool q_insert_head(struct list_head *head, char *s)
{
    if (!head)
        return false;

    element_t *e = malloc(sizeof(element_t));

    if (!e)
        return false;

    e->value = strdup(s);

    if (!e->value) {
        q_release_element(e);
        return false;
    }

    list_add(&e->list, head);

    return true;
}

/*
 * Attempt to insert element at tail of queue.
 * Return true if successful.
 * Return false if q is NULL or could not allocate space.
 * Argument s points to the string to be stored.
 * The function must explicitly allocate space and copy the string into it.
 */
bool q_insert_tail(struct list_head *head, char *s)
{
    if (!head)
        return false;

    element_t *e = malloc(sizeof(element_t));

    if (!e)
        return false;

    e->value = strdup(s);

    if (!e->value) {
        q_release_element(e);
        return false;
    }

    list_add_tail(&e->list, head);

    return true;
}

/*
 * Attempt to remove element from head of queue.
 * Return target element.
 * Return NULL if queue is NULL or empty.
 * If sp is non-NULL and an element is removed, copy the removed string to *sp
 * (up to a maximum of bufsize-1 characters, plus a null terminator.)
 *
 * NOTE: "remove" is different from "delete"
 * The space used by the list element and the string should not be freed.
 * The only thing "remove" need to do is unlink it.
 *
 * REF:
 * https://english.stackexchange.com/questions/52508/difference-between-delete-and-remove
 */
element_t *q_remove_head(struct list_head *head, char *sp, size_t bufsize)
{
    if (!head || list_empty(head))
        return NULL;

    element_t *e = list_first_entry(head, element_t, list);
    list_del(&e->list);

    if (sp) {
        strncpy(sp, e->value, bufsize - 1);
        sp[bufsize - 1] = '\0';
    }

    return e;
}

/*
 * Attempt to remove element from tail of queue.
 * Other attribute is as same as q_remove_head.
 */
element_t *q_remove_tail(struct list_head *head, char *sp, size_t bufsize)
{
    if (!head || list_empty(head))
        return NULL;

    element_t *e = list_last_entry(head, element_t, list);
    list_del(&e->list);

    if (sp) {
        strncpy(sp, e->value, bufsize - 1);
        sp[bufsize - 1] = '\0';
    }

    return e;
}

/*
 * WARN: This is for external usage, don't modify it
 * Attempt to release element.
 */
void q_release_element(element_t *e)
{
    free(e->value);
    free(e);
}

/*
 * Return number of elements in queue.
 * Return 0 if q is NULL or empty
 */
int q_size(struct list_head *head)
{
    if (!head || list_empty(head))
        return 0;

    int size = 0;
    struct list_head *pos;

    list_for_each (pos, head)
        ++size;

    return size;
}

/*
 * Delete the middle node in list.
 * The middle node of a linked list of size n is the
 * ⌊n / 2⌋th node from the start using 0-based indexing.
 * If there're six element, the third member should be return.
 * Return true if successful.
 * Return false if list is NULL or empty.
 */
bool q_delete_mid(struct list_head *head)
{
    // https://leetcode.com/problems/delete-the-middle-node-of-a-linked-list/
    if (!head || list_empty(head))
        return false;

    struct list_head *fast, *slow;

    for (fast = head->next, slow = head->next;
         fast != head && fast->next != head;
         fast = fast->next->next, slow = slow->next)
        ;

    list_del(slow);
    q_release_element(list_entry(slow, element_t, list));

    return true;
}

/*
 * Delete all nodes that have duplicate string,
 * leaving only distinct strings from the original list.
 * Return true if successful.
 * Return false if list is NULL.
 *
 * Note: this function always be called after sorting, in other words,
 * list is guaranteed to be sorted in ascending order.
 */
bool q_delete_dup(struct list_head *head)
{
    // https://leetcode.com/problems/remove-duplicates-from-sorted-list-ii/
    if (!head)
        return false;

    if (list_empty(head))
        return true;

    struct list_head *start = head->next, *end = NULL, *prev = NULL;

    while (end != head) {
        end = start->next;
        element_t *e1 = list_entry(start, element_t, list);
        element_t *e2 = list_entry(end, element_t, list);

        while (end != head && !strcmp(e1->value, e2->value)) {
            prev = start;
            struct list_head *next = end->next;
            list_del(end);
            q_release_element(e2);
            end = next;
            e2 = list_entry(end, element_t, list);
        }

        if (prev) {
            e1 = list_entry(prev, element_t, list);
            list_del(prev);
            q_release_element(e1);
            prev = NULL;
        }

        start = end;
    }

    return true;
}

/*
 * Attempt to swap every two adjacent nodes.
 */
void q_swap(struct list_head *head)
{
    // https://leetcode.com/problems/swap-nodes-in-pairs/
    if (!head)
        return;

    struct list_head *n;

    for (n = head->next; n != head && n->next != head; n = n->next) {
        struct list_head *next = n->next;
        list_del(n);
        list_add(n, next);
    }
}

/*
 * Reverse elements in queue
 * No effect if q is NULL or empty
 * This function should not allocate or free any list elements
 * (e.g., by calling q_insert_head, q_insert_tail, or q_remove_head).
 * It should rearrange the existing ones.
 */
void q_reverse(struct list_head *head)
{
    if (!head || list_empty(head))
        return;

    struct list_head *prev = head->prev, *curr = head, *next = NULL;

    while (next != head) {
        next = curr->next;
        curr->next = prev;
        curr->prev = next;
        prev = curr;
        curr = next;
    }
}

/*
 * Sort elements of queue in ascending order
 * No effect if q is NULL or empty. In addition, if q has only one
 * element, do nothing.
 */

struct list_head *merge_two_lists(struct list_head *l1, struct list_head *l2)
{
    struct list_head *head = NULL, **ptr = &head, **node;

    for (node = NULL; l1 && l2; *node = (*node)->next) {
        node = strcmp(list_entry(l1, element_t, list)->value,
                      list_entry(l2, element_t, list)->value) <= 0
                   ? &l1
                   : &l2;
        *ptr = *node;
        ptr = &(*ptr)->next;
    }

    *ptr = (struct list_head *) ((uintptr_t) l1 | (uintptr_t) l2);

    return head;
}

struct list_head *merge_sort(struct list_head *head)
{
    if (!head || !head->next)
        return head;

    struct list_head *fast = head->next, *slow = head;

    while (fast && fast->next) {
        fast = fast->next->next;
        slow = slow->next;
    }

    fast = slow->next;
    slow->next = NULL;

    return merge_two_lists(merge_sort(head), merge_sort(fast));
}

void q_sort(struct list_head *head)
{
    if (q_size(head) <= 1)
        return;

    struct list_head *list = head->next;
    head->prev->next = NULL;
    list = merge_sort(list);
    head->next = list;

    struct list_head *n;

    for (n = head; n->next != NULL; n = n->next)
        n->next->prev = n;

    head->prev = n;
    n->next = head;
}
