#include "inventory.h"
#include <stdlib.h>
#include <string.h>

Item *inventory_add(Item *head, Item *it) {
    if (!it) return head;
    it->next = head;
    return it;
}

Item *inventory_remove(Item *head, int id) {
    Item *cur = head;
    Item *prev = NULL;
    while (cur) {
        if (cur->id == id) {
            if (prev) prev->next = cur->next;
            else head = cur->next;
            free(cur);
            return head;
        }
        prev = cur;
        cur = cur->next;
    }
    return head;
}

void inventory_free(Item *head) {
    Item *cur = head;
    while (cur) {
        Item *next = cur->next;
        free(cur);
        cur = next;
    }
}

Item *item_create(int id, const char *name, int type, int atk_bonus, int def_bonus) {
    Item *it = (Item *)malloc(sizeof(Item));
    if (!it) return NULL;
    it->id = id;
    strncpy(it->name, name ? name : "Item", sizeof(it->name)-1);
    it->name[sizeof(it->name)-1] = '\0';
    it->type = type;
    it->atk_bonus = atk_bonus;
    it->def_bonus = def_bonus;
    it->next = NULL;
    return it;
}
