#include <stdio.h>
#include "views/main_menus.h"
#include "model/state.h"


int main() {
    setbuf(stdout, 0);
    estado state = estado_new();
    displayMenuPrincipal(&state);
    estado_free(&state);
    return 0;
}
