/*
 * =========================================================
 *   help_screen.h  —  pantalla de ayuda estilo lección
 *
 *   Overlay que se dibuja SOLO en la pantalla superior,
 *   sin tocar/borrar lo ya renderizado en el frame.
 *   Recibe por parámetro el string que va a desplegar.
 *
 *   Uso:
 *     help_show("texto de ayuda");
 *     ...en el frame, al final...
 *     help_render();
 * =========================================================
 */

#ifndef HELP_SCREEN_H
#define HELP_SCREEN_H

#include <citro2d.h>

void help_show(const char* text);
void help_hide(void);
void help_toggle(const char* text);
int  help_is_active(void);
void help_render(void);
void help_exit(void);

#endif