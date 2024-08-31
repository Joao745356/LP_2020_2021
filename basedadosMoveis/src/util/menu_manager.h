//
// Created by joao7 on 08/01/2023.
//

#ifndef UNTITLED3_MENU_MANAGER_H
#define UNTITLED3_MENU_MANAGER_H

#define MENU_ACTION_BACK -1
#define MENU_ACTION_REPRINT -2
#define MENU_ACTION_ERORR -3

typedef struct {
    char *name;

    int (*menuEntry)(void *);

    void *userData;
} menuEntry;

int go_to_menu(int length, char *menuTitle, menuEntry *menuEntries);

int print_menu(int length, char *menuTitle, menuEntry *menuEntries, int line_width);

void print_menu_centered(int line_width, char *title);

void print_menu_separator(int padding);

void print_menu_item(int option, char *name);

#endif //UNTITLED3_MENU_MANAGER_H
