//
// Created by joao7 on 08/01/2023.
//

#include <stdio.h>
#include <string.h>
#include <math.h>
#include "menu_manager.h"
#include "input.h"

#define MENU_BACK -1
#define MENU_REPRINT -2
#define EXTRA_PADDING 2
#define EXTRA_PRINT_SPACE 4

void print_menu_item(int option, char *name) {
    printf(" %d - %s\n", option, name);
}

void print_menu_separator(int padding) {
    for (int i = 0; i < padding; i++) {
        printf("*");
    }
    printf("\n");
}

void print_menu_centered(int line_width, char *title) {
    int padding = line_width - (int) strlen(title);
    if (padding < 0) padding = 0;

    for (int i = 0; i < (padding / 2); i++) {
        printf(" ");
    }
    printf("%s\n", title);
}

int print_menu(int length, char *menuTitle, menuEntry *menuEntries, int line_width) {
    print_menu_separator(line_width);
    print_menu_centered(line_width, menuTitle);
    print_menu_item(-2, "back");
    print_menu_item(-1, "reprint");
    for (int i = 0; i < length; i++) {
        print_menu_item(i, menuEntries[i].name);
    }

    int option = obtainInt(-2, length - 1, "selecione a opçao : \n");

    if (option == -2) {
        return MENU_BACK;
    } else if (option == -1) {
        return MENU_REPRINT;
    }

    return option;
}

int go_to_menu(int length, char *menuTitle, menuEntry *menuEntries) {
    int line_width = (int) strlen(menuTitle) + 2 + EXTRA_PADDING;
    for (int i = 0; i < length; i++) {
        int line_length = (int) strlen(menuEntries[i].name);
        int new_withd = EXTRA_PRINT_SPACE + EXTRA_PADDING + ((int) log10f((float) i)) + 1 + line_length;
        if (line_width < new_withd) {
            line_width = new_withd;
        }
    }

    while (1) {
        int option = print_menu(length, menuTitle, menuEntries, line_width);
        if (option == MENU_REPRINT)
            {
            continue;
        }

        if (option == MENU_BACK) {
            return MENU_REPRINT;
        }

        int action = (menuEntries[option].menuEntry)(menuEntries[option].userData);
        if (action == MENU_ACTION_REPRINT) {
            continue;
        }
        if (action == MENU_ACTION_BACK) {
            return MENU_ACTION_REPRINT;
        } else {
            return MENU_ACTION_ERORR;
        }
    }
}
