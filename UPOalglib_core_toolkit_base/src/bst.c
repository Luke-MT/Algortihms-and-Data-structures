/* vim: set tabstop=4 expandtab shiftwidth=4 softtabstop=4: */

/*
 * Copyright 2015 University of Piemonte Orientale, Computer Science Institute
 *
 * This file is part of UPOalglib.
 *
 * UPOalglib is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * UPOalglib is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with UPOalglib.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "bst_private.h"
#include <stdio.h>
#include <stdlib.h>


/**** EXERCISE #1 - BEGIN of FUNDAMENTAL OPERATIONS ****/


upo_bst_t upo_bst_create(upo_bst_comparator_t key_cmp)
{
    upo_bst_t tree = malloc(sizeof(struct upo_bst_s));
    if (tree == NULL)
    {
        perror("Unable to create a binary search tree");
        abort();
    }

    tree->root = NULL;
    tree->key_cmp = key_cmp;

    return tree;
}

void upo_bst_destroy(upo_bst_t tree, int destroy_data)
{
    if (tree != NULL)
    {
        upo_bst_clear(tree, destroy_data);
        free(tree);
    }
}

void upo_bst_clear_impl(upo_bst_node_t *node, int destroy_data)
{
    if (node != NULL)
    {
        upo_bst_clear_impl(node->left, destroy_data);
        upo_bst_clear_impl(node->right, destroy_data);

        if (destroy_data)
        {
            free(node->key);
            free(node->value);
        }

        free(node);
    }
}

void upo_bst_clear(upo_bst_t tree, int destroy_data)
{
    if (tree != NULL)
    {
        upo_bst_clear_impl(tree->root, destroy_data);
        tree->root = NULL;
    }
}

void* upo_bst_put(upo_bst_t tree, void *key, void *value)
{
    /* TO STUDENTS:
     *  Remove the following two lines and put here your implementation. */
    /*fprintf(stderr, "To be implemented!\n");
    abort();*/
    upo_bst_node_t *v_old = NULL;
    if(tree != NULL)
        tree->root = (upo_bst_node_t*) upo_bst_put_rec(tree->root,key,value,v_old,tree->key_cmp);
    return v_old;

}

void* upo_bst_put_rec(upo_bst_node_t *n, void *key, void *value, void *v_old, upo_bst_comparator_t cmp){
    
    if(n == NULL)
        return createNode(key,value);
    else if(cmp(key,n->key) < 0)
        n->left = upo_bst_put_rec(n->left,key,value,v_old,cmp);
    else if(cmp(key,n->key) > 0)
        n->right = upo_bst_put_rec(n->right,key,value,v_old,cmp);
    else{
        v_old = n->value;
        n->value = value;
    }
    return n;
}

void upo_bst_insert(upo_bst_t tree, void *key, void *value)
{
    /* TO STUDENTS:
     *  Remove the following two lines and put here your implementation. */
    /*fprintf(stderr, "To be implemented!\n");
    abort();*/
    if(tree != NULL)
        tree->root = upo_bst_insert_rec(tree->root, key, value, tree->key_cmp);
}

void* upo_bst_insert_rec(upo_bst_node_t *n, void *key, void *value, upo_bst_comparator_t cmp){
    if(n == NULL)
        return createNode(key,value);
    else if(cmp(key,n->key) < 0)
        n->left = upo_bst_insert_rec(n->left,key,value,cmp);
    else if(cmp(key,n->key) > 0)
        n->right = upo_bst_insert_rec(n->right,key,value,cmp);

    return n;
 }

/*void* upo_bst_insert_ex(upo_bst_node_t *n, void *key, void *value. upo_bst_comparator_t cmp){
    if(n == NULL)
        return createNode(key,value);
    else(cmp(key,n->key) < 0)
        n->left = upo_bst_insert_ex(n->left,key,value,cmp);
    else(cmp(key,n->key) > 0)
        n->right = upo_bst_insert_ex(n->right,key,value,cmp);
    else{
        upo_bst_value_list_node_t *current = n->value;
        while(current->next != NULL)
            current = current->next;

        current->next = malloc(sizeof(upo_bst_value_list_node_t));
        current->next->value = value;
        current->next->next = NULL;
    }

    return n;
}*/

void* upo_bst_get(const upo_bst_t tree, const void *key)
{
    /* TO STUDENTS:
     *  Remove the following two lines and put here your implementation. */
    /*fprintf(stderr, "To be implemented!\n");
    abort();*/
    upo_bst_node_t *n = NULL;
    if(tree != NULL){
        n = upo_bst_get_rec(tree->root,key,tree->key_cmp);
        if(n != NULL)
            return n->value;
    }
    return NULL;
}

void* upo_bst_get_rec(upo_bst_node_t *n, const void* key, upo_bst_comparator_t cmp){
    if(n == NULL)
        return NULL;
    
    if(cmp(key,n->key) < 0)
        return upo_bst_get_rec(n->left,key,cmp);
    else if(cmp(key,n->key) > 0)
        return upo_bst_get_rec(n->right,key,cmp);
    else
        return n;

}


int upo_bst_contains(const upo_bst_t tree, const void *key)
{
    /* TO STUDENTS:
     *  Remove the following two lines and put here your implementation. */
    /*fprintf(stderr, "To be implemented!\n");
    abort();*/
    if(upo_bst_get_rec(tree->root,key,tree->key_cmp) != NULL)
        return 1;
    else
        return 0;
}

void upo_bst_delete(upo_bst_t tree, const void *key, int destroy_data)
{
    /* TO STUDENTS:
     *  Remove the following two lines and put here your implementation. */
    /*fprintf(stderr, "To be implemented!\n");
    abort();*/
    if(tree != NULL)
        tree->root = upo_bst_delete_rec(tree->root,key,destroy_data,tree->key_cmp);

}

void* upo_bst_delete_rec(upo_bst_node_t *n, const void* key, int destroy_data, upo_bst_comparator_t cmp){
    if(n == NULL)
        return NULL;
    
    if(cmp(key,n->key) < 0)
        n->left = upo_bst_delete_rec(n->left,key,destroy_data,cmp);
    else if(cmp(key,n->key) > 0)
        n->right = upo_bst_delete_rec(n->right,key,destroy_data,cmp);
    else if(n->left != NULL && n->right != NULL)
        n = upo_bst_delete_2C(n,destroy_data,cmp);
    else
       n = upo_bst_delete_1C(n,destroy_data);
    
    return n;

    
}

void* upo_bst_delete_1C(upo_bst_node_t *n, int destroy_data){
    if(n == NULL)
        return NULL;

    upo_bst_node_t *x = n;

    if(n->left != NULL)
        n = n->left;
    else
        n = n->right;
    if(destroy_data == 1)
        free(x->value);

    return n;;
}

void* upo_bst_delete_2C(upo_bst_node_t *n, int destroy_data, upo_bst_comparator_t cmp){
    upo_bst_node_t *m = upo_bst_max_impl(n->left);

    n->key = m->key;
    n->value = m->value;
    n->left = upo_bst_delete_rec(n->left,m->key,destroy_data,cmp);

    return n;

}

size_t upo_bst_size(const upo_bst_t tree)
{
    /* TO STUDENTS:
     *  Remove the following two lines and put here your implementation. */
    /*fprintf(stderr, "To be implemented!\n");
    abort();*/
    if(tree != NULL)
        return upo_bst_size_rec(tree->root);
    return 0;
}

size_t upo_bst_size_rec(upo_bst_node_t *n){
    if( n == NULL)
        return 0;
    return 1 + upo_bst_size_rec(n->left) 
             + upo_bst_size_rec(n->right);
}

size_t upo_bst_height(const upo_bst_t tree)
{
    /* TO STUDENTS:
     *  Remove the following two lines and put here your implementation. */
    /*fprintf(stderr, "To be implemented!\n");
    abort();*/
    if(tree != NULL)
        return upo_bst_height_rec(tree->root);
    return 0;
}

size_t upo_bst_height_rec(upo_bst_node_t *n){
    if( n == NULL || upo_bst_isLeaf(n))
        return 0;

    size_t left = upo_bst_height_rec(n->left);
    size_t right = upo_bst_height_rec(n->right);

    return 1 + (left > right ? left : right);
}

int upo_bst_isLeaf(upo_bst_node_t *n){
    if(n->left == NULL && n->right == NULL)
        return 1;
    return 0;
}

void upo_bst_traverse_in_order(const upo_bst_t tree, upo_bst_visitor_t visit, void *visit_context)
{
    /* TO STUDENTS:
     *  Remove the following two lines and put here your implementation. */
    /*fprintf(stderr, "To be implemented!\n");
    abort();*/
    if(tree != NULL)
        upo_bst_traverse_in_order_rec(tree->root, visit, visit_context);
}

void  upo_bst_traverse_in_order_rec(upo_bst_node_t *n, upo_bst_visitor_t visit, void *visit_context){
    if(n != NULL){
        upo_bst_traverse_in_order_rec(n->left,visit,visit_context);
        visit(n->key, n->value, visit_context);
        upo_bst_traverse_in_order_rec(n->right,visit,visit_context);
    }

}

int upo_bst_is_empty(const upo_bst_t tree)
{
    /* TO STUDENTS:
     *  Remove the following two lines and put here your implementation. */
    /*fprintf(stderr, "To be implemented!\n");
    abort();*/
    if(tree == NULL || tree->root == NULL)
        return 1;
    return 0;
}

int upo_bst_subtree_count_inner(upo_bst_node_t tree, const void* key){
    if(tree != NULL)
        if(tree->root != NULL){
            int size = 0;
            upo_bst_subtree_count_inner_rec(tree->root,key,&size,key->key_cmp);
            return size;
        }
    return 0;

}

void upo_bst_subtree_count_inner_rec(upo_bst_node_t *n, const void* key, int *size, upo_bst_comparator_t cmp){
    if(n == NULL)
        return NULL;
    else if(cmp(key,n->key) < 0)
        upo_bst_subtree_count_inner_rec(n->left,key,size,cmp);
    else if(cmp(key,n->key) > 0)
        upo_bst_subtree_count_inner_rec(n->right,key,size,cmp);
    else{
        upo_bst_visit(n,size);
    }
}

void upo_bst_visit(upo_bst_node_t *n, int *size){
    if(n != NULL){
        size++;
        upo_bst_visit(n->left,size);
        if(isLeaf(n))
            size--;
        upo_bst_visit(n->right,size);
    }
}

int isLeaf(upo_bst_node_t *n){
    if(n != NULL && n->left == NULL && n->right == NULL)
        return 1;
    return 0;
}




/**** EXERCISE #1 - END of FUNDAMENTAL OPERATIONS ****/


/**** EXERCISE #2 - BEGIN of EXTRA OPERATIONS ****/


void* upo_bst_min(const upo_bst_t tree)
{
    /* TO STUDENTS:
     *  Remove the following two lines and put here your implementation. */
    /*fprintf(stderr, "To be implemented!\n");
    abort();*/
    if(tree == NULL)
        return NULL;

    upo_bst_node_t *min = upo_bst_min_impl(tree->root);
    if(min != NULL)
        return min->key;
    return NULL;
}

void* upo_bst_min_impl(upo_bst_node_t *n){
    if(n == NULL)
        return NULL;
    else if(n->left != NULL)
        return upo_bst_min_impl(n->left);
    else
        return n;
}

void* upo_bst_max(const upo_bst_t tree)
{
    /* TO STUDENTS:
     *  Remove the following two lines and put here your implementation. */
    /*fprintf(stderr, "To be implemented!\n");
    abort();*/
    if(tree == NULL)
        return NULL;

    upo_bst_node_t *max = upo_bst_max_impl(tree->root);
    if(max != NULL)
        return max->key;
    return NULL;
}

void* upo_bst_max_impl(upo_bst_node_t *n){
    if(n == NULL)
        return NULL;
    else if(n->right != NULL)
        return upo_bst_max_impl(n->right);
    else
        return n;
    
}

void upo_bst_delete_min(upo_bst_t tree, int destroy_data)
{
    /* TO STUDENTS:
     *  Remove the following two lines and put here your implementation. */
    /*fprintf(stderr, "To be implemented!\n");
    abort();*/
    if(tree != NULL)
        upo_bst_delete(tree, upo_bst_min(tree), destroy_data);
        
    
}

void upo_bst_delete_max(upo_bst_t tree, int destroy_data)
{
    /* TO STUDENTS:
     *  Remove the following two lines and put here your implementation. */
    /*fprintf(stderr, "To be implemented!\n");
    abort();*/
    if(tree != NULL)
        upo_bst_delete(tree, upo_bst_max(tree), destroy_data);
}

void* upo_bst_floor(const upo_bst_t tree, const void *key)
{
    /* TO STUDENTS:
     *  Remove the following two lines and put here your implementation. */
    /*fprintf(stderr, "To be implemented!\n");
    abort();*/
    if(tree != NULL){

    }
}

void* upo_bst_ceiling(const upo_bst_t tree, const void *key)
{
    /* TO STUDENTS:
     *  Remove the following two lines and put here your implementation. */
    fprintf(stderr, "To be implemented!\n");
    abort();
}

upo_bst_key_list_t upo_bst_keys_range(const upo_bst_t tree, const void *low_key, const void *high_key)
{
    /* TO STUDENTS:
     *  Remove the following two lines and put here your implementation. */
    fprintf(stderr, "To be implemented!\n");
    abort();
}

upo_bst_key_list_t upo_bst_keys(const upo_bst_t tree)
{
    /* TO STUDENTS:
     *  Remove the following two lines and put here your implementation. */
    fprintf(stderr, "To be implemented!\n");
    abort();
}

int upo_bst_is_bst(const upo_bst_t tree, const void *min_key, const void *max_key)
{
    /* TO STUDENTS:
     *  Remove the following two lines and put here your implementation. */
    fprintf(stderr, "To be implemented!\n");
    abort();
}

/*size_t upo_bst_rank_rec(upo_bst_node_t *n, const *void key, upo_bst_comparator_t cmp){
    if(n == NULL)
        return 0;

    if(cmp(key,n->key) <= 0)
        return upo_bst_rank_rec(n->left, key, cmp);

    return 1 + upo_bst_size_rec(n->left) + upo_bst_rank_rec(n->right,key,cmp);
}*/

void* upo_bst_predecessor_rec(upo_bst_node_t *n, const void* key, upo_bst_comparator_t cmp){
    if(n = NULL)
        return 0;

    void* val = NULL;
    if(cmp(key, n->key) <= 0){
        if(n->left != NULL)
            upo_bst_predecessor_rec(n->left,key,cmp);
        return NULL;
    }
    if(n->right != NULL)
        val = upo_bst_predecessor_rec(n->right,key,cmp);
    return (cmp(n->key,val) > 0 ? n->key : val);
}

/*void* upo_bst_get_value_depth(const upo_bst_t tree, const void* key, long depth){
    upo_bst_node_t *val = NULL;
    if(tree != NULL){
        depth = -1;
        val = upo_bst_get_value_depth_rec(tree->root,key,depth,tree->key_cmp);
        if(val == NULL)
            depth = -1;
    }    
    return val;
}

void* upo_bst_get_value_depth_rec(upo_bst_node_t *n, const void* key, long depth, upo_bst_comparator_t cmp){
    if(n == NULL)
        return NULL;
    else if(cmp(key,n->key) < 0){
        depth++;
        return upo_bst_get_value_depth_rec(n->left,key,depth,cmp);
    }
    else if(cmp(key,n->key) > 0){
        depth++;
        return upo_bst_get_value_depth_rec(n->right,key,depth,cmp);
    }
    else{
        depth++;
        return n;
    }
}   */
/*
upo_bst_key_list_t upo_bst_keys_list(upo_bst_t tree, const void* key){
    upo_bst_key_list_t list = malloc(sizeof(upo_bst_key_list_node_s));
    upo_bst_key_list_t head = list;
    if(tree != NULL)
        upo_bst_keys_list_rec2(tree->root,key,list, tree->key_cmp);
    
    return head;


} */


/*void upo_bst_keys_list_rec2(upo_bst_node_t *n, const void *key, upo_bst_key_list_t l, upo_bst_comparator_t cmp){
    if(n == NULL)
        return;

    upo_bst_keys_list_rec2(n->left,key,l,cmp);
    if(cmp(key,n->key) >= 0){
        upo_bst_key_list_node_t *node = malloc(sizeof(upo_bst_key_list_node_s));
        node->key = n->key;
        l->next = node;
        l = l->next;
    }
    else
        return;
    upo_bst_keys_list_rec2(n->right,key,l,cmp);
}*/


/**** EXERCISE #2 - END of EXTRA OPERATIONS ****/


upo_bst_comparator_t upo_bst_get_comparator(const upo_bst_t tree)
{
    if (tree == NULL)
    {
        return NULL;
    }

    return tree->key_cmp;
}
