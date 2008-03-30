/*
 * intl.c - Localization routines for Amiga.
 *
 * Written by
 *  Marco van den Heuvel <blackystardust68@yahoo.com>
 *
 * This file is part of VICE, the Versatile Commodore Emulator.
 * See README for copyright notice.
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA
 *  02111-1307  USA.
 *
 */

#include "vice.h"

#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define __USE_INLINE__

#include <proto/locale.h>

#include "archdep.h"
#include "cmdline.h"
#include "intl.h"
#include "lib.h"
/* #include "res.h" */
#include "resources.h"
#include "translate.h"
#include "util.h"
#include "ui.h"

#define countof(array) (sizeof(array) / sizeof((array)[0]))

typedef struct amiga_iso_s {
    char *amiga_locale_language;
    char *iso_language_code;
} amiga_iso_t;


/* this table holds only the currently present translation
   languages */

static amiga_iso_t amiga_to_iso[]={
  {"english.language", "en"},
  {"german.language", "de"},
  {"deutsch.language", "de"},
  {"french.language", "fr"},
  {"fran�ais.language", "fr"},
  {"italian.language", "it"},
  {"italiano.language", "it"},
  {"dutch.language", "nl"},
  {"nederlands.language", "nl"},
  {"polish.language", "pl"},
  {"polski.language", "pl"},
  {"swedish.language", "sv"},
  {"svenska.language", "sv"},
  {NULL, NULL}
};

/* The language table is duplicated in
   the translate.c, make sure they match
   when adding a new language */

static char *language_table[] = {

/* english */
  "en",

/* german */
  "de",

/* french */
  "fr",

/* italian */
  "it",

/* dutch */
  "nl",

/* polish */
  "pl",

/* swedish */
  "sv"
};

/* --------------------------------------------------------------------- */

typedef struct intl_translate_s {
    int resource_id;
    char *text;
} intl_translate_t;



intl_translate_t intl_string_table[] = {

/* ----------------------- AmigaOS Menu Strings ----------------------- */

/* en */ {IDMS_FILE,    "File"},
/* de */ {IDMS_FILE_DE, "Datei"},
/* fr */ {IDMS_FILE_FR, "Fichier"},
/* it */ {IDMS_FILE_IT, "File"},
/* nl */ {IDMS_FILE_NL, "Bestand"},
/* pl */ {IDMS_FILE_PL, ""},  /* fuzzy */
/* sv */ {IDMS_FILE_SV, "Arkiv"},

/* en */ {IDMS_AUTOSTART_IMAGE,    "Autostart disk/tape image..."},
/* de */ {IDMS_AUTOSTART_IMAGE_DE, "Autostart Disk/Band Image..."},
/* fr */ {IDMS_AUTOSTART_IMAGE_FR, "D�marrage automatique d'une image datassette..."},
/* it */ {IDMS_AUTOSTART_IMAGE_IT, "Avvia automaticamente l'immagine di un disco/cassetta..."},
/* nl */ {IDMS_AUTOSTART_IMAGE_NL, "Autostart disk/tape bestand..."},
/* pl */ {IDMS_AUTOSTART_IMAGE_PL, ""},  /* fuzzy */
/* sv */ {IDMS_AUTOSTART_IMAGE_SV, "Autostarta disk-/bandavbildningsfil..."},

/* en */ {IDMS_ATTACH_DISK_IMAGE,    "Attach disk image"},
/* de */ {IDMS_ATTACH_DISK_IMAGE_DE, "Disk Image einlegen"},
/* fr */ {IDMS_ATTACH_DISK_IMAGE_FR, "Ins�rer une image de disque"},
/* it */ {IDMS_ATTACH_DISK_IMAGE_IT, "Seleziona l'immagine di un disco"},
/* nl */ {IDMS_ATTACH_DISK_IMAGE_NL, "Koppel disk bestand"},
/* pl */ {IDMS_ATTACH_DISK_IMAGE_PL, ""},  /* fuzzy */
/* sv */ {IDMS_ATTACH_DISK_IMAGE_SV, "Anslut diskettavbildningsfil"},

/* en */ {IDMS_DRIVE_8,    "Drive 8"},
/* de */ {IDMS_DRIVE_8_DE, "Laufwerk 8"},
/* fr */ {IDMS_DRIVE_8_FR, "Lecteur #8"},
/* it */ {IDMS_DRIVE_8_IT, "Drive 8"},
/* nl */ {IDMS_DRIVE_8_NL, "Drive 8"},
/* pl */ {IDMS_DRIVE_8_PL, ""},  /* fuzzy */
/* sv */ {IDMS_DRIVE_8_SV, "Enhet 8"},

/* en */ {IDMS_DRIVE_9,    "Drive 9"},
/* de */ {IDMS_DRIVE_9_DE, "Laufwerk 9"},
/* fr */ {IDMS_DRIVE_9_FR, "Lecteur #9"},
/* it */ {IDMS_DRIVE_9_IT, "Drive 9"},
/* nl */ {IDMS_DRIVE_9_NL, "Drive 9"},
/* pl */ {IDMS_DRIVE_9_PL, ""},  /* fuzzy */
/* sv */ {IDMS_DRIVE_9_SV, "Enhet 9"},

/* en */ {IDMS_DRIVE_10,    "Drive 10"},
/* de */ {IDMS_DRIVE_10_DE, "Laufwerk 10"},
/* fr */ {IDMS_DRIVE_10_FR, "Lecteur #10"},
/* it */ {IDMS_DRIVE_10_IT, "Drive 10"},
/* nl */ {IDMS_DRIVE_10_NL, "Drive 10"},
/* pl */ {IDMS_DRIVE_10_PL, ""},  /* fuzzy */
/* sv */ {IDMS_DRIVE_10_SV, "Enhet 10"},

/* en */ {IDMS_DRIVE_11,    "Drive 11"},
/* de */ {IDMS_DRIVE_11_DE, "Laufwerk 11"},
/* fr */ {IDMS_DRIVE_11_FR, "Lecteur #11"},
/* it */ {IDMS_DRIVE_11_IT, "Drive 11"},
/* nl */ {IDMS_DRIVE_11_NL, "Drive 11"},
/* pl */ {IDMS_DRIVE_11_PL, ""},  /* fuzzy */
/* sv */ {IDMS_DRIVE_11_SV, "Enhet 11"},

/* en */ {IDMS_DETACH_DISK_IMAGE,    "Detach disk image"},
/* de */ {IDMS_DETACH_DISK_IMAGE_DE, "Disk Image entfernen"},
/* fr */ {IDMS_DETACH_DISK_IMAGE_FR, "Retirer une image de disque"},
/* it */ {IDMS_DETACH_DISK_IMAGE_IT, "Rimuovi un immagine disco"},
/* nl */ {IDMS_DETACH_DISK_IMAGE_NL, "Ontkoppel disk bestand"},
/* pl */ {IDMS_DETACH_DISK_IMAGE_PL, ""},  /* fuzzy */
/* sv */ {IDMS_DETACH_DISK_IMAGE_SV, "Koppla fr�n diskettavbildningsfil"},

/* en */ {IDMS_ALL,    "All"},
/* de */ {IDMS_ALL_DE, "Alle"},
/* fr */ {IDMS_ALL_FR, "Tout"},
/* it */ {IDMS_ALL_IT, "Tutti"},
/* nl */ {IDMS_ALL_NL, "Alles"},
/* pl */ {IDMS_ALL_PL, ""},  /* fuzzy */
/* sv */ {IDMS_ALL_SV, "Alla"},

/* en */ {IDMS_FLIP_LIST,    "Flip list"},
/* de */ {IDMS_FLIP_LIST_DE, "Flipliste"},
/* fr */ {IDMS_FLIP_LIST_FR, "Groupement de disques"},
/* it */ {IDMS_FLIP_LIST_IT, "Flip list"},
/* nl */ {IDMS_FLIP_LIST_NL, "Flip lijst"},
/* pl */ {IDMS_FLIP_LIST_PL, ""},  /* fuzzy */
/* sv */ {IDMS_FLIP_LIST_SV, "Vallista"},

/* en */ {IDMS_ADD_CURRENT_IMAGE,    "Add current image"},
/* de */ {IDMS_ADD_CURRENT_IMAGE_DE, "Aktuelles Image hinzuf�gen"},
/* fr */ {IDMS_ADD_CURRENT_IMAGE_FR, "Ajouter l'image de disque courante"},
/* it */ {IDMS_ADD_CURRENT_IMAGE_IT, "Aggiungi immagine attuale"},
/* nl */ {IDMS_ADD_CURRENT_IMAGE_NL, "Voeg huidig bestand toe"},
/* pl */ {IDMS_ADD_CURRENT_IMAGE_PL, ""},  /* fuzzy */
/* sv */ {IDMS_ADD_CURRENT_IMAGE_SV, "L�gg till aktuell avbildning"},

/* en */ {IDMS_REMOVE_CURRENT_IMAGE,    "Remove current image"},
/* de */ {IDMS_REMOVE_CURRENT_IMAGE_DE, "Aktuelles Image entfernen"},
/* fr */ {IDMS_REMOVE_CURRENT_IMAGE_FR, "Retirer l'image de disque courante"},
/* it */ {IDMS_REMOVE_CURRENT_IMAGE_IT, "Rimuovi immagine attuale"},
/* nl */ {IDMS_REMOVE_CURRENT_IMAGE_NL, "Verwijder huidig bestand"},
/* pl */ {IDMS_REMOVE_CURRENT_IMAGE_PL, ""},  /* fuzzy */
/* sv */ {IDMS_REMOVE_CURRENT_IMAGE_SV, "Ta bort aktuell avbildning"},

/* en */ {IDMS_ATTACH_NEXT_IMAGE,    "Attach next image"},
/* de */ {IDMS_ATTACH_NEXT_IMAGE_DE, "N�chstes Image"},
/* fr */ {IDMS_ATTACH_NEXT_IMAGE_FR, "Ins�rer la prochaine image"},
/* it */ {IDMS_ATTACH_NEXT_IMAGE_IT, "Seleziona immagine successiva"},
/* nl */ {IDMS_ATTACH_NEXT_IMAGE_NL, "Koppel volgend bestand"},
/* pl */ {IDMS_ATTACH_NEXT_IMAGE_PL, ""},  /* fuzzy */
/* sv */ {IDMS_ATTACH_NEXT_IMAGE_SV, "Anslut n�sta avbildning"},

/* en */ {IDMS_ATTACH_PREVIOUS_IMAGE,    "Attach previous image"},
/* de */ {IDMS_ATTACH_PREVIOUS_IMAGE_DE, "Voriges Image"},
/* fr */ {IDMS_ATTACH_PREVIOUS_IMAGE_FR, "Ins�rer l'image pr�c�dente"},
/* it */ {IDMS_ATTACH_PREVIOUS_IMAGE_IT, "Seleziona immagine precedente"},
/* nl */ {IDMS_ATTACH_PREVIOUS_IMAGE_NL, "Koppel vorig bestand"},
/* pl */ {IDMS_ATTACH_PREVIOUS_IMAGE_PL, ""},  /* fuzzy */
/* sv */ {IDMS_ATTACH_PREVIOUS_IMAGE_SV, "Anslut f�reg�ende avbildning"},

/* en */ {IDMS_LOAD_FLIP_LIST,    "Load flip list"},
/* de */ {IDMS_LOAD_FLIP_LIST_DE, "Fliplist Datei Laden"},
/* fr */ {IDMS_LOAD_FLIP_LIST_FR, "Charger un groupement de disques"},
/* it */ {IDMS_LOAD_FLIP_LIST_IT, "Carica flip list"},
/* nl */ {IDMS_LOAD_FLIP_LIST_NL, "Laad flip lijst"},
/* pl */ {IDMS_LOAD_FLIP_LIST_PL, ""},  /* fuzzy */
/* sv */ {IDMS_LOAD_FLIP_LIST_SV, "L�s in vallistefil"},

/* en */ {IDMS_SAVE_FLIP_LIST,    "Save flip list"},
/* de */ {IDMS_SAVE_FLIP_LIST_DE, "Fliplist Datei Speichern"},
/* fr */ {IDMS_SAVE_FLIP_LIST_FR, "Enregistrer le groupement de disques"},
/* it */ {IDMS_SAVE_FLIP_LIST_IT, "Salva flip list"},
/* nl */ {IDMS_SAVE_FLIP_LIST_NL, "Opslaan flip lijst"},
/* pl */ {IDMS_SAVE_FLIP_LIST_PL, ""},  /* fuzzy */
/* sv */ {IDMS_SAVE_FLIP_LIST_SV, "Spara vallistefil"},

/* en */ {IDMS_ATTACH_TAPE_IMAGE,    "Attach tape image..."},
/* de */ {IDMS_ATTACH_TAPE_IMAGE_DE, "Band Image einlegen..."},
/* fr */ {IDMS_ATTACH_TAPE_IMAGE_FR, "Ins�rer une image datassette..."},
/* it */ {IDMS_ATTACH_TAPE_IMAGE_IT, "Seleziona l' immagine di una cassetta..."},
/* nl */ {IDMS_ATTACH_TAPE_IMAGE_NL, "Koppel tape bestand..."},
/* pl */ {IDMS_ATTACH_TAPE_IMAGE_PL, ""},  /* fuzzy */
/* sv */ {IDMS_ATTACH_TAPE_IMAGE_SV, "Anslut en bandavbildningsfil..."},

/* en */ {IDMS_DETACH_TAPE_IMAGE,    "Detach tape image"},
/* de */ {IDMS_DETACH_TAPE_IMAGE_DE, "Band Image entfernen"},
/* fr */ {IDMS_DETACH_TAPE_IMAGE_FR, "Retirer une image datassette"},
/* it */ {IDMS_DETACH_TAPE_IMAGE_IT, "Rimuovi immagine cassetta"},
/* nl */ {IDMS_DETACH_TAPE_IMAGE_NL, "Ontkoppel tape image"},
/* pl */ {IDMS_DETACH_TAPE_IMAGE_PL, ""},  /* fuzzy */
/* sv */ {IDMS_DETACH_TAPE_IMAGE_SV, "Koppla fr�n bandavbildningsfil"},

/* en */ {IDMS_DATASSETTE_CONTROL,    "Datassette control"},
/* de */ {IDMS_DATASSETTE_CONTROL_DE, "Bandlaufwerk Kontrolle"},
/* fr */ {IDMS_DATASSETTE_CONTROL_FR, "Contr�le datassette"},
/* it */ {IDMS_DATASSETTE_CONTROL_IT, "Controlli del registratore"},
/* nl */ {IDMS_DATASSETTE_CONTROL_NL, "Datassette bediening"},
/* pl */ {IDMS_DATASSETTE_CONTROL_PL, ""},  /* fuzzy */
/* sv */ {IDMS_DATASSETTE_CONTROL_SV, "Datasettestyrning"},

/* en */ {IDMS_STOP,    "Stop"},
/* de */ {IDMS_STOP_DE, "Stop"},
/* fr */ {IDMS_STOP_FR, "Arr�t"},
/* it */ {IDMS_STOP_IT, "Stop"},
/* nl */ {IDMS_STOP_NL, "Stop"},
/* pl */ {IDMS_STOP_PL, ""},  /* fuzzy */
/* sv */ {IDMS_STOP_SV, "Stoppa"},

/* en */ {IDMS_START,    "Start"},
/* de */ {IDMS_START_DE, "Start"},
/* fr */ {IDMS_START_FR, "D�marrer"},
/* it */ {IDMS_START_IT, "Avvia"},
/* nl */ {IDMS_START_NL, "Start"},
/* pl */ {IDMS_START_PL, ""},  /* fuzzy */
/* sv */ {IDMS_START_SV, "Starta"},

/* en */ {IDMS_FORWARD,    "Forward"},
/* de */ {IDMS_FORWARD_DE, "Forward"},
/* fr */ {IDMS_FORWARD_FR, "En avant"},
/* it */ {IDMS_FORWARD_IT, "Avanti"},
/* nl */ {IDMS_FORWARD_NL, "Vooruit"},
/* pl */ {IDMS_FORWARD_PL, ""},  /* fuzzy */
/* sv */ {IDMS_FORWARD_SV, "Spola fram�t"},

/* en */ {IDMS_REWIND,    "Rewind"},
/* de */ {IDMS_REWIND_DE, "Rewind"},
/* fr */ {IDMS_REWIND_FR, "En arri�re"},
/* it */ {IDMS_REWIND_IT, "Indietro"},
/* nl */ {IDMS_REWIND_NL, "Terug"},
/* pl */ {IDMS_REWIND_PL, ""},  /* fuzzy */
/* sv */ {IDMS_REWIND_SV, "Spola bak�t"},

/* en */ {IDMS_RECORD,    "Record"},
/* de */ {IDMS_RECORD_DE, "Record"},
/* fr */ {IDMS_RECORD_FR, "Enregistrer"},
/* it */ {IDMS_RECORD_IT, "Registra"},
/* nl */ {IDMS_RECORD_NL, "Opname"},
/* pl */ {IDMS_RECORD_PL, ""},  /* fuzzy */
/* sv */ {IDMS_RECORD_SV, "Spela in"},

/* en */ {IDMS_RESET,    "Reset"},
/* de */ {IDMS_RESET_DE, "Reset"},
/* fr */ {IDMS_RESET_FR, "R�initialiser"},
/* it */ {IDMS_RESET_IT, "Reset"},
/* nl */ {IDMS_RESET_NL, "Reset"},
/* pl */ {IDMS_RESET_PL, ""},  /* fuzzy */
/* sv */ {IDMS_RESET_SV, "Nollst�ll"},

/* en */ {IDMS_RESET_COUNTER,    "Reset Counter"},
/* de */ {IDMS_RESET_COUNTER_DE, "Z�hler zur�cksetzen"},
/* fr */ {IDMS_RESET_COUNTER_FR, "R�inialiser le compteur"},
/* it */ {IDMS_RESET_COUNTER_IT, "Reset contatore"},
/* nl */ {IDMS_RESET_COUNTER_NL, "Reset teller"},
/* pl */ {IDMS_RESET_COUNTER_PL, ""},  /* fuzzy */
/* sv */ {IDMS_RESET_COUNTER_SV, "Nollst�ll r�knare"},

/* en */ {IDMS_ATTACH_CART_IMAGE,    "Attach cartridge image..."},
/* de */ {IDMS_ATTACH_CART_IMAGE_DE, "Erweiterungsmodule einlegen..."},
/* fr */ {IDMS_ATTACH_CART_IMAGE_FR, "Ins�rer une cartouche..."},
/* it */ {IDMS_ATTACH_CART_IMAGE_IT, "Seleziona l'immagine di una cartuccia..."},
/* nl */ {IDMS_ATTACH_CART_IMAGE_NL, "Koppel cartridge bestand..."},
/* pl */ {IDMS_ATTACH_CART_IMAGE_PL, ""},  /* fuzzy */
/* sv */ {IDMS_ATTACH_CART_IMAGE_SV, "Anslut insticksmodulfil..."},

/* en */ {IDMS_4_8_16KB_AT_2000,    "4/8/16KB image at $2000..."},
/* de */ {IDMS_4_8_16KB_AT_2000_DE, "4/8/16KB Modul Image bei $2000..."},
/* fr */ {IDMS_4_8_16KB_AT_2000_FR, "Insertion d'une image 4/8/16Ko � $2000..."},
/* it */ {IDMS_4_8_16KB_AT_2000_IT, "Immagine da 4/8/16KB caricata a $2000..."},
/* nl */ {IDMS_4_8_16KB_AT_2000_NL, "Koppel 4/8/16KB bestand in $2000..."},
/* pl */ {IDMS_4_8_16KB_AT_2000_PL, ""},  /* fuzzy */
/* sv */ {IDMS_4_8_16KB_AT_2000_SV, "4/8/16KB-fil vid $2000..."},

/* en */ {IDMS_4_8_16KB_AT_4000,    "4/8/16KB image at $4000..."},
/* de */ {IDMS_4_8_16KB_AT_4000_DE, "4/8/16KB Modul Image bei $4000..."},
/* fr */ {IDMS_4_8_16KB_AT_4000_FR, "Insertion d'une image 4/8/16Ko � $4000..."},
/* it */ {IDMS_4_8_16KB_AT_4000_IT, "Immagine da 4/8/16KB caricata a $4000..."},
/* nl */ {IDMS_4_8_16KB_AT_4000_NL, "Koppel 4/8/16KB bestand in $4000..."},
/* pl */ {IDMS_4_8_16KB_AT_4000_PL, ""},  /* fuzzy */
/* sv */ {IDMS_4_8_16KB_AT_4000_SV, "4/8/16KB-fil vid $4000..."},

/* en */ {IDMS_4_8_16KB_AT_6000,    "4/8/16KB image at $6000..."},
/* de */ {IDMS_4_8_16KB_AT_6000_DE, "4/8/16KB Modul Image bei $6000..."},
/* fr */ {IDMS_4_8_16KB_AT_6000_FR, "Insertion d'une image 4/8/16Ko � $6000..."},
/* it */ {IDMS_4_8_16KB_AT_6000_IT, "Immagine da 4/8/16KB caricata a $6000..."},
/* nl */ {IDMS_4_8_16KB_AT_6000_NL, "Koppel 4/8/16KB bestand in $6000..."},
/* pl */ {IDMS_4_8_16KB_AT_6000_PL, ""},  /* fuzzy */
/* sv */ {IDMS_4_8_16KB_AT_6000_SV, "4/8/16KB-fil vid $6000..."},

/* en */ {IDMS_4_8KB_AT_A000,    "4/8KB image at $A000..."},
/* de */ {IDMS_4_8KB_AT_A000_DE, "4/8KB Modul Image bei $A000..."},
/* fr */ {IDMS_4_8KB_AT_A000_FR, "Insertion d'une image 4/8Ko � $A000..."},
/* it */ {IDMS_4_8KB_AT_A000_IT, "Immagine da 4/8KB caricata a $A000..."},
/* nl */ {IDMS_4_8KB_AT_A000_NL, "Koppel 4/8KB bestand in $A000..."},
/* pl */ {IDMS_4_8KB_AT_A000_PL, ""},  /* fuzzy */
/* sv */ {IDMS_4_8KB_AT_A000_SV, "4/8KB-fil vid $A000..."},

/* en */ {IDMS_4KB_AT_B000,    "4KB image at $B000..."},
/* de */ {IDMS_4KB_AT_B000_DE, "4KB Modul Image bei $B000..."},
/* fr */ {IDMS_4KB_AT_B000_FR, "Insertion d'une image 4Ko � $B000..."},
/* it */ {IDMS_4KB_AT_B000_IT, "Immagine da 4KB caricata a $B000..."},
/* nl */ {IDMS_4KB_AT_B000_NL, "Koppel 4KB bestand in $B000..."},
/* pl */ {IDMS_4KB_AT_B000_PL, ""},  /* fuzzy */
/* sv */ {IDMS_4KB_AT_B000_SV, "4KB-fil vid $B000..."},

/* en */ {IDMS_DETACH_CART_IMAGE,    "Detach cartridge image"},
/* de */ {IDMS_DETACH_CART_IMAGE_DE, "Erweiterungsmodul Image entfernen"},
/* fr */ {IDMS_DETACH_CART_IMAGE_FR, "Retirer une cartouche"},
/* it */ {IDMS_DETACH_CART_IMAGE_IT, "Rimuovi immagine cartuccia"},
/* nl */ {IDMS_DETACH_CART_IMAGE_NL, "Ontkoppel cartridge bestand"},
/* pl */ {IDMS_DETACH_CART_IMAGE_PL, ""},  /* fuzzy */
/* sv */ {IDMS_DETACH_CART_IMAGE_SV, "Koppla fr�n insticksmodulfil"},

/* en */ {IDMS_C1_LOW_IMAGE,    "C1 low  image..."},
/* de */ {IDMS_C1_LOW_IMAGE_DE, "C1 low Image Datei..."},
/* fr */ {IDMS_C1_LOW_IMAGE_FR, "Image C1 basse..."},
/* it */ {IDMS_C1_LOW_IMAGE_IT, "Immagine nell'area bassa di C1..."},
/* nl */ {IDMS_C1_LOW_IMAGE_NL, "C1 low  bestand..."},
/* pl */ {IDMS_C1_LOW_IMAGE_PL, ""},  /* fuzzy */
/* sv */ {IDMS_C1_LOW_IMAGE_SV, "C1 l�g-avbildning..."},

/* en */ {IDMS_C1_HIGH_IMAGE,    "C1 high image..."},
/* de */ {IDMS_C1_HIGH_IMAGE_DE, "C1 high Image Datei..."},
/* fr */ {IDMS_C1_HIGH_IMAGE_FR, "Image C1 haute..."},
/* it */ {IDMS_C1_HIGH_IMAGE_IT, "Immagine nell'area alta di C1..."},
/* nl */ {IDMS_C1_HIGH_IMAGE_NL, "C1 high bestand..."},
/* pl */ {IDMS_C1_HIGH_IMAGE_PL, ""},  /* fuzzy */
/* sv */ {IDMS_C1_HIGH_IMAGE_SV, "C1 h�g-avbildning..."},

/* en */ {IDMS_C2_LOW_IMAGE,    "C2 low  image..."},
/* de */ {IDMS_C2_LOW_IMAGE_DE, "C2 low Image Datei..."},
/* fr */ {IDMS_C2_LOW_IMAGE_FR, "Image C2 basse..."},
/* it */ {IDMS_C2_LOW_IMAGE_IT, "Immagine nell'area bassa di C2..."},
/* nl */ {IDMS_C2_LOW_IMAGE_NL, "C2 low  bestand..."},
/* pl */ {IDMS_C2_LOW_IMAGE_PL, ""},  /* fuzzy */
/* sv */ {IDMS_C2_LOW_IMAGE_SV, "C2 l�g-avbildning..."},

/* en */ {IDMS_C2_HIGH_IMAGE,    "C2 high image..."},
/* de */ {IDMS_C2_HIGH_IMAGE_DE, "C2 high Image Datei..."},
/* fr */ {IDMS_C2_HIGH_IMAGE_FR, "Image C2 haute..."},
/* it */ {IDMS_C2_HIGH_IMAGE_IT, "Immagine nell'area alta di C2..."},
/* nl */ {IDMS_C2_HIGH_IMAGE_NL, "C2 high bestand..."},
/* pl */ {IDMS_C2_HIGH_IMAGE_PL, ""},  /* fuzzy */
/* sv */ {IDMS_C2_HIGH_IMAGE_SV, "C2 h�g-avbildning..."},

/* en */ {IDMS_FUNCTION_LOW_3PLUS1,    "Function low  image (3plus1)..."},
/* de */ {IDMS_FUNCTION_LOW_3PLUS1_DE, "Funktions ROM Image Datei low (3plus1)..."},
/* fr */ {IDMS_FUNCTION_LOW_3PLUS1_FR, "Image Fonction basse (3+1)..."},
/* it */ {IDMS_FUNCTION_LOW_3PLUS1_IT, "Function (3plus1) caricata nella memoria bassa..."},
/* nl */ {IDMS_FUNCTION_LOW_3PLUS1_NL, "Function low  bestand (3plus1)..."},
/* pl */ {IDMS_FUNCTION_LOW_3PLUS1_PL, ""},  /* fuzzy */
/* sv */ {IDMS_FUNCTION_LOW_3PLUS1_SV, "Funktion l�g-avbildning (3plus1)..."},

/* en */ {IDMS_FUNCTION_HIGH_3PLUS1,    "Function high image (3plus1)..."},
/* de */ {IDMS_FUNCTION_HIGH_3PLUS1_DE, "Funktions ROM Image Datei high (3plus1)..."},
/* fr */ {IDMS_FUNCTION_HIGH_3PLUS1_FR, "Image Fonction haute (3+1)..."},
/* it */ {IDMS_FUNCTION_HIGH_3PLUS1_IT, "Function (3plus1) caricata nella memoria alta..."},
/* nl */ {IDMS_FUNCTION_HIGH_3PLUS1_NL, "Function high bestand (3plus1)..."},
/* pl */ {IDMS_FUNCTION_HIGH_3PLUS1_PL, ""},  /* fuzzy */
/* sv */ {IDMS_FUNCTION_HIGH_3PLUS1_SV, "Funktion h�g-avbildning (3plus1)..."},

/* en */ {IDMS_CRT_IMAGE,    "CRT image..."},
/* de */ {IDMS_CRT_IMAGE_DE, "CRT Image Datei..."},
/* fr */ {IDMS_CRT_IMAGE_FR, "Nom du fichier image CRT..."},
/* it */ {IDMS_CRT_IMAGE_IT, "Immagine CRT..."},
/* nl */ {IDMS_CRT_IMAGE_NL, "CRT bestand..."},
/* pl */ {IDMS_CRT_IMAGE_PL, ""},  /* fuzzy */
/* sv */ {IDMS_CRT_IMAGE_SV, "CRT-fil..."},

/* en */ {IDMS_GENERIC_8KB_IMAGE,    "Generic 8KB image..."},
/* de */ {IDMS_GENERIC_8KB_IMAGE_DE, "Generische 8Kb Image..."},
/* fr */ {IDMS_GENERIC_8KB_IMAGE_FR, "Ins�rer une image g�n�rique de 8Ko..."},
/* it */ {IDMS_GENERIC_8KB_IMAGE_IT, "Immagine generica da 8KB..."},
/* nl */ {IDMS_GENERIC_8KB_IMAGE_NL, "Algemeen 8KB bestand..."},
/* pl */ {IDMS_GENERIC_8KB_IMAGE_PL, ""},  /* fuzzy */
/* sv */ {IDMS_GENERIC_8KB_IMAGE_SV, "Vanlig 8KB-fil..."},

/* en */ {IDMS_GENERIC_16KB_IMAGE,    "Generic 16KB image..."},
/* de */ {IDMS_GENERIC_16KB_IMAGE_DE, "Generische 16Kb Image..."},
/* fr */ {IDMS_GENERIC_16KB_IMAGE_FR, "Ins�rer une image g�n�rique de 16Ko..."},
/* it */ {IDMS_GENERIC_16KB_IMAGE_IT, "Immagine generica da 16KB..."},
/* nl */ {IDMS_GENERIC_16KB_IMAGE_NL, "Algemeen 16KB bestand..."},
/* pl */ {IDMS_GENERIC_16KB_IMAGE_PL, ""},  /* fuzzy */
/* sv */ {IDMS_GENERIC_16KB_IMAGE_SV, "Vanlig 16KB-fil..."},

/* en */ {IDMS_ACTION_REPLAY_IMAGE,    "Action Replay image..."},
/* de */ {IDMS_ACTION_REPLAY_IMAGE_DE, "Action Replay Image..."},
/* fr */ {IDMS_ACTION_REPLAY_IMAGE_FR, "Ins�rer une cartouche Action Replay..."},
/* it */ {IDMS_ACTION_REPLAY_IMAGE_IT, "Immagine Action Replay..."},
/* nl */ {IDMS_ACTION_REPLAY_IMAGE_NL, "Action Replay bestand"},
/* pl */ {IDMS_ACTION_REPLAY_IMAGE_PL, ""},  /* fuzzy */
/* sv */ {IDMS_ACTION_REPLAY_IMAGE_SV, "Action Replay-fil..."},

/* en */ {IDMS_ATOMIC_POWER_IMAGE,    "Atomic Power image..."},
/* de */ {IDMS_ATOMIC_POWER_IMAGE_DE, "Atomic Power Image..."},
/* fr */ {IDMS_ATOMIC_POWER_IMAGE_FR, "Ins�rer une cartouche Atomic Power..."},
/* it */ {IDMS_ATOMIC_POWER_IMAGE_IT, "Immagine Atomic Power..."},
/* nl */ {IDMS_ATOMIC_POWER_IMAGE_NL, "Atomic Power bestand..."},
/* pl */ {IDMS_ATOMIC_POWER_IMAGE_PL, ""},  /* fuzzy */
/* sv */ {IDMS_ATOMIC_POWER_IMAGE_SV, "Atomic Power-fil..."},

/* en */ {IDMS_EPYX_FASTLOAD_IMAGE,    "Epyx fastload image..."},
/* de */ {IDMS_EPYX_FASTLOAD_IMAGE_DE, "Epyx Fastload Image..."},
/* fr */ {IDMS_EPYX_FASTLOAD_IMAGE_FR, "Ins�rer une cartouche Epyx FastLoad..."},
/* it */ {IDMS_EPYX_FASTLOAD_IMAGE_IT, "Immagine Epyx fastload..."},
/* nl */ {IDMS_EPYX_FASTLOAD_IMAGE_NL, "Epyx fastload bestand..."},
/* pl */ {IDMS_EPYX_FASTLOAD_IMAGE_PL, ""},  /* fuzzy */
/* sv */ {IDMS_EPYX_FASTLOAD_IMAGE_SV, "Epyx fastload-fil..."},

/* en */ {IDMS_IEEE488_INTERFACE_IMAGE,    "IEEE488 interface image..."},
/* de */ {IDMS_IEEE488_INTERFACE_IMAGE_DE, "IEEE 488 Schnittstellenmodul..."},
/* fr */ {IDMS_IEEE488_INTERFACE_IMAGE_FR, "Ins�rer une cartouche interface IEEE488..."},
/* it */ {IDMS_IEEE488_INTERFACE_IMAGE_IT, "Immagine dell'interfaccia IEEE488..."},
/* nl */ {IDMS_IEEE488_INTERFACE_IMAGE_NL, "IEEE488 interface bestand..."},
/* pl */ {IDMS_IEEE488_INTERFACE_IMAGE_PL, ""},  /* fuzzy */
/* sv */ {IDMS_IEEE488_INTERFACE_IMAGE_SV, "IEEE488-gr�nssnittsfil..."},

/* en */ {IDMS_RETRO_REPLAY_IMAGE,    "Retro Replay image..."},
/* de */ {IDMS_RETRO_REPLAY_IMAGE_DE, "Retro Replay Image..."},
/* fr */ {IDMS_RETRO_REPLAY_IMAGE_FR, "Ins�rer une cartouche Retro Replay..."},
/* it */ {IDMS_RETRO_REPLAY_IMAGE_IT, "Immagine Retro Replay..."},
/* nl */ {IDMS_RETRO_REPLAY_IMAGE_NL, "Retro Replay bestand..."},
/* pl */ {IDMS_RETRO_REPLAY_IMAGE_PL, ""},  /* fuzzy */
/* sv */ {IDMS_RETRO_REPLAY_IMAGE_SV, "Retro Replay-fil..."},

/* en */ {IDMS_IDE64_INTERFACE_IMAGE,    "IDE64 interface image..."},
/* de */ {IDMS_IDE64_INTERFACE_IMAGE_DE, "IDE64 Schnittstellenmodul..."},
/* fr */ {IDMS_IDE64_INTERFACE_IMAGE_FR, "Ins�rer une cartouche interface IDE64..."},
/* it */ {IDMS_IDE64_INTERFACE_IMAGE_IT, "Immagine dell'interfaccia IDE64..."},
/* nl */ {IDMS_IDE64_INTERFACE_IMAGE_NL, "IDE64 interface bestand..."},
/* pl */ {IDMS_IDE64_INTERFACE_IMAGE_PL, ""},  /* fuzzy */
/* sv */ {IDMS_IDE64_INTERFACE_IMAGE_SV, "IDE64-gr�nssnittsfil..."},

/* en */ {IDMS_SUPER_SNAPSHOT_4_IMAGE,    "Super Snapshot 4 image..."},
/* de */ {IDMS_SUPER_SNAPSHOT_4_IMAGE_DE, "Super Snapshot 4 Image..."},
/* fr */ {IDMS_SUPER_SNAPSHOT_4_IMAGE_FR, "Ins�rer une cartouche Super Snapshot 4..."},
/* it */ {IDMS_SUPER_SNAPSHOT_4_IMAGE_IT, "Immagine Super Snapshot 4..."},
/* nl */ {IDMS_SUPER_SNAPSHOT_4_IMAGE_NL, "Super Snapshot 4 bestand..."},
/* pl */ {IDMS_SUPER_SNAPSHOT_4_IMAGE_PL, ""},  /* fuzzy */
/* sv */ {IDMS_SUPER_SNAPSHOT_4_IMAGE_SV, "Super Snapshot 4-fil..."},

/* en */ {IDMS_SUPER_SNAPSHOT_5_IMAGE,    "Super Snapshot 5 image..."},
/* de */ {IDMS_SUPER_SNAPSHOT_5_IMAGE_DE, "Super Snapshot 5 Image..."},
/* fr */ {IDMS_SUPER_SNAPSHOT_5_IMAGE_FR, "Ins�rer une cartouche Super Snapshot 5..."},
/* it */ {IDMS_SUPER_SNAPSHOT_5_IMAGE_IT, "Immagine Super Snapshot 5..."},
/* nl */ {IDMS_SUPER_SNAPSHOT_5_IMAGE_NL, "Super Snapshot 5 bestand..."},
/* pl */ {IDMS_SUPER_SNAPSHOT_5_IMAGE_PL, ""},  /* fuzzy */
/* sv */ {IDMS_SUPER_SNAPSHOT_5_IMAGE_SV, "Super Snapshot 5-fil..."},

/* en */ {IDMS_STRUCTURED_BASIC_IMAGE,    "Structured Basic image..."},
/* de */ {IDMS_STRUCTURED_BASIC_IMAGE_DE, "Structured Basic Image..."},
/* fr */ {IDMS_STRUCTURED_BASIC_IMAGE_FR, "Ins�rer une cartouche Structured Basic..."},
/* it */ {IDMS_STRUCTURED_BASIC_IMAGE_IT, "Immagine Structured Basic..."},
/* nl */ {IDMS_STRUCTURED_BASIC_IMAGE_NL, "Structured Basic bestand..."},
/* pl */ {IDMS_STRUCTURED_BASIC_IMAGE_PL, ""},  /* fuzzy */
/* sv */ {IDMS_STRUCTURED_BASIC_IMAGE_SV, "Structured Basic-fil..."},

/* en */ {IDMS_EXPERT_CART,    "Expert Cartridge"},
/* de */ {IDMS_EXPERT_CART_DE, "Expert Cartridge"},
/* fr */ {IDMS_EXPERT_CART_FR, "Activer les param�tres des cartouches experts"},
/* it */ {IDMS_EXPERT_CART_IT, "Expert Cartridge"},
/* nl */ {IDMS_EXPERT_CART_NL, "Expert Cartridge"},
/* pl */ {IDMS_EXPERT_CART_PL, ""},  /* fuzzy */
/* sv */ {IDMS_EXPERT_CART_SV, "Expert Cartridge"},

/* en */ {IDMS_ENABLE,    "Enable"},
/* de */ {IDMS_ENABLE_DE, "Aktivieren"},
/* fr */ {IDMS_ENABLE_FR, "Activer"},
/* it */ {IDMS_ENABLE_IT, "Abilita"},
/* nl */ {IDMS_ENABLE_NL, "Aktiveer"},
/* pl */ {IDMS_ENABLE_PL, ""},  /* fuzzy */
/* sv */ {IDMS_ENABLE_SV, "Aktivera"},

/* en */ {IDMS_OFF,    "Off"},
/* de */ {IDMS_OFF_DE, "Aus"},
/* fr */ {IDMS_OFF_FR, "Off"},
/* it */ {IDMS_OFF_IT, "Off"},
/* nl */ {IDMS_OFF_NL, "Uit"},
/* pl */ {IDMS_OFF_PL, ""},  /* fuzzy */
/* sv */ {IDMS_OFF_SV, "Av"},

/* en */ {IDMS_PRG,    "Prg"},
/* de */ {IDMS_PRG_DE, "Prg"},
/* fr */ {IDMS_PRG_FR, "Prg"},
/* it */ {IDMS_PRG_IT, "Prg"},
/* nl */ {IDMS_PRG_NL, "Prg"},
/* pl */ {IDMS_PRG_PL, ""},  /* fuzzy */
/* sv */ {IDMS_PRG_SV, "Prg"},

/* en */ {IDMS_ON,    "On"},
/* de */ {IDMS_ON_DE, "Ein"},
/* fr */ {IDMS_ON_FR, "On"},
/* it */ {IDMS_ON_IT, "On"},
/* nl */ {IDMS_ON_NL, "Aan"},
/* pl */ {IDMS_ON_PL, ""},  /* fuzzy */
/* sv */ {IDMS_ON_SV, "P�"},

/* en */ {IDMS_SET_CART_AS_DEFAULT,    "Set cartridge as default"},
/* de */ {IDMS_SET_CART_AS_DEFAULT_DE, "Aktuelles Erweiterungsmodul als standard aktivieren"},
/* fr */ {IDMS_SET_CART_AS_DEFAULT_FR, "D�finir cette cartouche par d�faut"},
/* it */ {IDMS_SET_CART_AS_DEFAULT_IT, "Imposta la cartuccia come predefinita"},
/* nl */ {IDMS_SET_CART_AS_DEFAULT_NL, "Zet cartridge als standaard"},
/* pl */ {IDMS_SET_CART_AS_DEFAULT_PL, ""},  /* fuzzy */
/* sv */ {IDMS_SET_CART_AS_DEFAULT_SV, "V�lj insticksmodul som f�rval"},

/* en */ {IDMS_RESET_ON_CART_CHANGE,    "Reset on cart change"},
/* de */ {IDMS_RESET_ON_CART_CHANGE_DE, "Reset bei Wechsel von Erweiterungsmodulen"},
/* fr */ {IDMS_RESET_ON_CART_CHANGE_FR, "R�inialiser sur changement de cartouche"},
/* it */ {IDMS_RESET_ON_CART_CHANGE_IT, "Reset al cambio di cartuccia"},
/* nl */ {IDMS_RESET_ON_CART_CHANGE_NL, "Reset bij cart wisseling"},
/* pl */ {IDMS_RESET_ON_CART_CHANGE_PL, ""},  /* fuzzy */
/* sv */ {IDMS_RESET_ON_CART_CHANGE_SV, "Nollst�ll vid modulbyte"},

/* en */ {IDMS_CART_FREEZE,    "Cartridge freeze"},
/* de */ {IDMS_CART_FREEZE_DE, "Erweiterungsmodul Freeze"},
/* fr */ {IDMS_CART_FREEZE_FR, "Geler la cartouche"},
/* it */ {IDMS_CART_FREEZE_IT, "Freeze della cartuccia"},
/* nl */ {IDMS_CART_FREEZE_NL, "Cartridge freeze"},
/* pl */ {IDMS_CART_FREEZE_PL, ""},  /* fuzzy */
/* sv */ {IDMS_CART_FREEZE_SV, "Frys"},

/* en */ {IDMS_PAUSE,    "Pause"},
/* de */ {IDMS_PAUSE_DE, "Pause"},
/* fr */ {IDMS_PAUSE_FR, "Pause"},
/* it */ {IDMS_PAUSE_IT, "Pausa"},
/* nl */ {IDMS_PAUSE_NL, "Pauze"},
/* pl */ {IDMS_PAUSE_PL, ""},  /* fuzzy */
/* sv */ {IDMS_PAUSE_SV, "Paus"},

/* en */ {IDMS_MONITOR,    "Monitor"},
/* de */ {IDMS_MONITOR_DE, "Monitor"},
/* fr */ {IDMS_MONITOR_FR, "Tracer"},
/* it */ {IDMS_MONITOR_IT, "Monitor"},
/* nl */ {IDMS_MONITOR_NL, "Monitor"},
/* pl */ {IDMS_MONITOR_PL, ""},  /* fuzzy */
/* sv */ {IDMS_MONITOR_SV, "Monitor"},

/* en */ {IDMS_HARD,    "Hard"},
/* de */ {IDMS_HARD_DE, "Hart"},
/* fr */ {IDMS_HARD_FR, "Physiquement"},
/* it */ {IDMS_HARD_IT, "Hard"},
/* nl */ {IDMS_HARD_NL, "Hard"},
/* pl */ {IDMS_HARD_PL, ""},  /* fuzzy */
/* sv */ {IDMS_HARD_SV, "H�rd"},

/* en */ {IDMS_SOFT,    "Soft"},
/* de */ {IDMS_SOFT_DE, "Soft"},
/* fr */ {IDMS_SOFT_FR, "Logiciellement"},
/* it */ {IDMS_SOFT_IT, "Soft"},
/* nl */ {IDMS_SOFT_NL, "Zacht"},
/* pl */ {IDMS_SOFT_PL, ""},  /* fuzzy */
/* sv */ {IDMS_SOFT_SV, "Mjuk"},

/* en */ {IDMS_EXIT,    "Exit"},
/* de */ {IDMS_EXIT_DE, "Exit"},
/* fr */ {IDMS_EXIT_FR, "Quitter"},
/* it */ {IDMS_EXIT_IT, "Esci"},
/* nl */ {IDMS_EXIT_NL, "Afsluiten"},
/* pl */ {IDMS_EXIT_PL, ""},  /* fuzzy */
/* sv */ {IDMS_EXIT_SV, "Avsluta"},

/* en */ {IDMS_SNAPSHOT,    "Snapshot"},
/* de */ {IDMS_SNAPSHOT_DE, "Snapshot"},
/* fr */ {IDMS_SNAPSHOT_FR, "Sauvegarde"},
/* it */ {IDMS_SNAPSHOT_IT, "Snapshot"},
/* nl */ {IDMS_SNAPSHOT_NL, "Momentopname"},
/* pl */ {IDMS_SNAPSHOT_PL, ""},  /* fuzzy */
/* sv */ {IDMS_SNAPSHOT_SV, "�gonblicksbild"},

/* en */ {IDMS_LOAD_SNAPSHOT_IMAGE,    "Load snapshot image..."},
/* de */ {IDMS_LOAD_SNAPSHOT_IMAGE_DE, "Snapshot Image Laden..."},
/* fr */ {IDMS_LOAD_SNAPSHOT_IMAGE_FR, "Charger une sauvegarde...."},
/* it */ {IDMS_LOAD_SNAPSHOT_IMAGE_IT, "Carica immagine snapshot..."},
/* nl */ {IDMS_LOAD_SNAPSHOT_IMAGE_NL, "Laad momentopname bestand..."},
/* pl */ {IDMS_LOAD_SNAPSHOT_IMAGE_PL, ""},  /* fuzzy */
/* sv */ {IDMS_LOAD_SNAPSHOT_IMAGE_SV, "L�s in �gonblicksbild..."},

/* en */ {IDMS_SAVE_SNAPSHOT_IMAGE,    "Save snapshot image..."},
/* de */ {IDMS_SAVE_SNAPSHOT_IMAGE_DE, "Snapshot Image Speichern..."},
/* fr */ {IDMS_SAVE_SNAPSHOT_IMAGE_FR, "Enregistrer une sauvegarde..."},
/* it */ {IDMS_SAVE_SNAPSHOT_IMAGE_IT, "Salva immagine snapshot..."},
/* nl */ {IDMS_SAVE_SNAPSHOT_IMAGE_NL, "Opslaan momentopname bestand..."},
/* pl */ {IDMS_SAVE_SNAPSHOT_IMAGE_PL, ""},  /* fuzzy */
/* sv */ {IDMS_SAVE_SNAPSHOT_IMAGE_SV, "Spara �gonblicksbild..."},

/* en */ {IDMS_LOAD_QUICKSNAPSHOT_IMAGE,    "Load quicksnapshot image"},
/* de */ {IDMS_LOAD_QUICKSNAPSHOT_IMAGE_DE, "Schnellladen von Snapshot Image"},
/* fr */ {IDMS_LOAD_QUICKSNAPSHOT_IMAGE_FR, "Charger la sauvegarde rapide"},
/* it */ {IDMS_LOAD_QUICKSNAPSHOT_IMAGE_IT, "Carica immagine quicksnapshot"},
/* nl */ {IDMS_LOAD_QUICKSNAPSHOT_IMAGE_NL, "Snellaad momentopname bestand"},
/* pl */ {IDMS_LOAD_QUICKSNAPSHOT_IMAGE_PL, ""},  /* fuzzy */
/* sv */ {IDMS_LOAD_QUICKSNAPSHOT_IMAGE_SV, "L�s in snabb�gonblicksbild"},

/* en */ {IDMS_SAVE_QUICKSNAPSHOT_IMAGE,    "Save quicksnapshot image"},
/* de */ {IDMS_SAVE_QUICKSNAPSHOT_IMAGE_DE, "Schnellspeichern von Snapshot"},
/* fr */ {IDMS_SAVE_QUICKSNAPSHOT_IMAGE_FR, "Enregistrer la sauvegarde rapide"},
/* it */ {IDMS_SAVE_QUICKSNAPSHOT_IMAGE_IT, "Salva immagine quicksnapshot"},
/* nl */ {IDMS_SAVE_QUICKSNAPSHOT_IMAGE_NL, "Snelopslaan momentopname bestand"},
/* pl */ {IDMS_SAVE_QUICKSNAPSHOT_IMAGE_PL, ""},  /* fuzzy */
/* sv */ {IDMS_SAVE_QUICKSNAPSHOT_IMAGE_SV, "Spara snabb�gonblicksbildfil"},

/* en */ {IDMS_START_STOP_RECORDING,    "Start/Stop Recording History"},
/* de */ {IDMS_START_STOP_RECORDING_DE, "Start/Stop Aufnahme History"},
/* fr */ {IDMS_START_STOP_RECORDING_FR, "D�marrer/Arr�ter l'historique de l'enregistrement"},
/* it */ {IDMS_START_STOP_RECORDING_IT, "Inizia/termina la registrazione della cronologia"},
/* nl */ {IDMS_START_STOP_RECORDING_NL, "Start/Stop Opname Geschiedenis"},
/* pl */ {IDMS_START_STOP_RECORDING_PL, ""},  /* fuzzy */
/* sv */ {IDMS_START_STOP_RECORDING_SV, "B�rja/sluta spela in h�ndelser"},

/* en */ {IDMS_START_STOP_PLAYBACK,    "Start/Stop Playback History"},
/* de */ {IDMS_START_STOP_PLAYBACK_DE, "Start/Stop Wiedergabe History"},
/* fr */ {IDMS_START_STOP_PLAYBACK_FR, "D�marrer/Arr�ter l'historique de la lecture"},
/* it */ {IDMS_START_STOP_PLAYBACK_IT, "Inizia/termina la riproduzione della cronologia"},
/* nl */ {IDMS_START_STOP_PLAYBACK_NL, "Start/Stop Afspeel Geschiedenis"},
/* pl */ {IDMS_START_STOP_PLAYBACK_PL, ""},  /* fuzzy */
/* sv */ {IDMS_START_STOP_PLAYBACK_SV, "B�rja/sluta spela upp h�ndelser"},

/* en */ {IDMS_SET_MILESTONE,    "Set Recording Milestone"},
/* de */ {IDMS_SET_MILESTONE_DE, "Setze Aufnahme Meilenstein"},
/* fr */ {IDMS_SET_MILESTONE_FR, "Lever le Signet d'enregistrement"},
/* it */ {IDMS_SET_MILESTONE_IT, "Imposta segnalibro"},
/* nl */ {IDMS_SET_MILESTONE_NL, "Zet Opname Mijlpaal"},
/* pl */ {IDMS_SET_MILESTONE_PL, ""},  /* fuzzy */
/* sv */ {IDMS_SET_MILESTONE_SV, "Ange inspelningsbokm�rke"},

/* en */ {IDMS_RETURN_TO_MILESTONE,    "Return to Milestone"},
/* de */ {IDMS_RETURN_TO_MILESTONE_DE, "R�ckkehr zum Meilenstein"},
/* fr */ {IDMS_RETURN_TO_MILESTONE_FR, "Retourner au Signet d'enregistrement"},
/* it */ {IDMS_RETURN_TO_MILESTONE_IT, "Vai a segnalibro"},
/* nl */ {IDMS_RETURN_TO_MILESTONE_NL, "Ga terug naar Mijlpaal"},
/* pl */ {IDMS_RETURN_TO_MILESTONE_PL, ""},  /* fuzzy */
/* sv */ {IDMS_RETURN_TO_MILESTONE_SV, "�terg� till bokm�rke"},

/* en */ {IDMS_RECORDING_START_MODE,    "Recording start mode"},
/* de */ {IDMS_RECORDING_START_MODE_DE, "Aufnahme Start Modus"},
/* fr */ {IDMS_RECORDING_START_MODE_FR, "Mode de d�part d'enregistrement"},
/* it */ {IDMS_RECORDING_START_MODE_IT, "Modalit� di inizio registrazione"},
/* nl */ {IDMS_RECORDING_START_MODE_NL, "Opname start modus"},
/* pl */ {IDMS_RECORDING_START_MODE_PL, ""},  /* fuzzy */
/* sv */ {IDMS_RECORDING_START_MODE_SV, "Spelar in startl�ge"},

/* en */ {IDMS_SAVE_NEW_SNAPSHOT,    "Save new snapshot"},
/* de */ {IDMS_SAVE_NEW_SNAPSHOT_DE, "Speichere neuen Snapshot"},
/* fr */ {IDMS_SAVE_NEW_SNAPSHOT_FR, "Enregistrer une nouvelle sauvegarde"},
/* it */ {IDMS_SAVE_NEW_SNAPSHOT_IT, "Salva nuovo snapshot"},
/* nl */ {IDMS_SAVE_NEW_SNAPSHOT_NL, "Opslaan nieuwe momentopname"},
/* pl */ {IDMS_SAVE_NEW_SNAPSHOT_PL, ""},  /* fuzzy */
/* sv */ {IDMS_SAVE_NEW_SNAPSHOT_SV, "Spara ny �gonblicksbild"},

/* en */ {IDMS_LOAD_EXISTING_SNAPSHOT,    "Load existing snapshot"},
/* de */ {IDMS_LOAD_EXISTING_SNAPSHOT_DE, "Lade existierenden Snapshot"},
/* fr */ {IDMS_LOAD_EXISTING_SNAPSHOT_FR, "Charger une sauvegarde"},
/* it */ {IDMS_LOAD_EXISTING_SNAPSHOT_IT, "Carica snapshot esistente"},
/* nl */ {IDMS_LOAD_EXISTING_SNAPSHOT_NL, "Laad bestaande momentopname"},
/* pl */ {IDMS_LOAD_EXISTING_SNAPSHOT_PL, ""},  /* fuzzy */
/* sv */ {IDMS_LOAD_EXISTING_SNAPSHOT_SV, "L�s in existerande �gonblicksbild"},

/* en */ {IDMS_START_WITH_RESET,    "Start with reset"},
/* de */ {IDMS_START_WITH_RESET_DE, "Mit Reset starten"},
/* fr */ {IDMS_START_WITH_RESET_FR, "R�initialiser au d�marrage"},
/* it */ {IDMS_START_WITH_RESET_IT, "Inizia con reset"},
/* nl */ {IDMS_START_WITH_RESET_NL, "Start bij reset"},
/* pl */ {IDMS_START_WITH_RESET_PL, ""},  /* fuzzy */
/* sv */ {IDMS_START_WITH_RESET_SV, "Starta med nollst�llning"},

/* en */ {IDMS_OVERWRITE_PLAYBACK,    "Overwrite Playback"},
/* de */ {IDMS_OVERWRITE_PLAYBACK_DE, "Laufende Aufnahme �berschreiben"},
/* fr */ {IDMS_OVERWRITE_PLAYBACK_FR, "�craser la lecture actuelle"},
/* it */ {IDMS_OVERWRITE_PLAYBACK_IT, "Sovrascrivi riproduzione"},
/* nl */ {IDMS_OVERWRITE_PLAYBACK_NL, "Overschrijf weergave"},
/* pl */ {IDMS_OVERWRITE_PLAYBACK_PL, ""},  /* fuzzy */
/* sv */ {IDMS_OVERWRITE_PLAYBACK_SV, "Skriv �ver aktiv uppspelning"},

/* en */ {IDMS_SELECT_HISTORY_DIR,    "Select History files/directory"},
/* de */ {IDMS_SELECT_HISTORY_DIR_DE, "Verzeichnis f�r Event Aufnahme"},
/* fr */ {IDMS_SELECT_HISTORY_DIR_FR, "S�lectionnez le r�pertoire des captures"},
/* it */ {IDMS_SELECT_HISTORY_DIR_IT, "Seleziona i file/directory della cronologia"},
/* nl */ {IDMS_SELECT_HISTORY_DIR_NL, "Selecteer Geschiedenis directory"},
/* pl */ {IDMS_SELECT_HISTORY_DIR_PL, ""},  /* fuzzy */
/* sv */ {IDMS_SELECT_HISTORY_DIR_SV, "V�lj historikfiler/-katalog"},

/* en */ {IDMS_SAVE_STOP_MEDIA_FILE,    "Save/stop media file..."},
/* de */ {IDMS_SAVE_STOP_MEDIA_FILE_DE, "Speichern/stop media Datei..."},
/* fr */ {IDMS_SAVE_STOP_MEDIA_FILE_FR, "Enregistrer/Arr�ter fichier m�dia..."},
/* it */ {IDMS_SAVE_STOP_MEDIA_FILE_IT, "Salva/arresta file multimediale..."},
/* nl */ {IDMS_SAVE_STOP_MEDIA_FILE_NL, "Opslaan/stop media bestand..."},
/* pl */ {IDMS_SAVE_STOP_MEDIA_FILE_PL, ""},  /* fuzzy */
/* sv */ {IDMS_SAVE_STOP_MEDIA_FILE_SV, "Starta/stoppa mediafil..."},

/* en */ {IDMS_NETPLAY,    "Netplay (experimental)..."},
/* de */ {IDMS_NETPLAY_DE, ""},  /* fuzzy */
/* fr */ {IDMS_NETPLAY_FR, ""},  /* fuzzy */
/* it */ {IDMS_NETPLAY_IT, "Gioco in rete (sperimentale)..."},
/* nl */ {IDMS_NETPLAY_NL, "Netplay (experimenteel)..."},
/* pl */ {IDMS_NETPLAY_PL, ""},  /* fuzzy */
/* sv */ {IDMS_NETPLAY_SV, "n�tverksspel (experimentell)"},

/* en */ {IDMS_OPTIONS,    "Options"},
/* de */ {IDMS_OPTIONS_DE, "Optionen"},
/* fr */ {IDMS_OPTIONS_FR, "Options"},
/* it */ {IDMS_OPTIONS_IT, "Opzioni"},
/* nl */ {IDMS_OPTIONS_NL, "Opties"},
/* pl */ {IDMS_OPTIONS_PL, ""},  /* fuzzy */
/* sv */ {IDMS_OPTIONS_SV, "Alternativ"},

/* en */ {IDMS_REFRESH_RATE,    "Refresh rate"},
/* de */ {IDMS_REFRESH_RATE_DE, "Wiederholrate"},
/* fr */ {IDMS_REFRESH_RATE_FR, "Taux de rafra�chissement"},
/* it */ {IDMS_REFRESH_RATE_IT, "Frequenza di aggiornamento"},
/* nl */ {IDMS_REFRESH_RATE_NL, "Vernieuwings snelheid"},
/* pl */ {IDMS_REFRESH_RATE_PL, ""},  /* fuzzy */
/* sv */ {IDMS_REFRESH_RATE_SV, "Uppdateringshastighet"},

/* en */ {IDMS_AUTO,    "Auto"},
/* de */ {IDMS_AUTO_DE, "Automatisch"},
/* fr */ {IDMS_AUTO_FR, "Automatique"},
/* it */ {IDMS_AUTO_IT, "Auto"},
/* nl */ {IDMS_AUTO_NL, "Auto"},
/* pl */ {IDMS_AUTO_PL, ""},  /* fuzzy */
/* sv */ {IDMS_AUTO_SV, "Auto"},

/* en */ {IDMS_1_1,    "1/1"},
/* de */ {IDMS_1_1_DE, "1/1"},
/* fr */ {IDMS_1_1_FR, "1/1"},
/* it */ {IDMS_1_1_IT, "1/1"},
/* nl */ {IDMS_1_1_NL, "1/1"},
/* pl */ {IDMS_1_1_PL, "1/1"},
/* sv */ {IDMS_1_1_SV, "1/1"},

/* en */ {IDMS_1_2,    "1/2"},
/* de */ {IDMS_1_2_DE, "1/2"},
/* fr */ {IDMS_1_2_FR, "1/2"},
/* it */ {IDMS_1_2_IT, "1/2"},
/* nl */ {IDMS_1_2_NL, "1/2"},
/* pl */ {IDMS_1_2_PL, "1/2"},
/* sv */ {IDMS_1_2_SV, "1/2"},

/* en */ {IDMS_1_3,    "1/3"},
/* de */ {IDMS_1_3_DE, "1/3"},
/* fr */ {IDMS_1_3_FR, "1/3"},
/* it */ {IDMS_1_3_IT, "1/3"},
/* nl */ {IDMS_1_3_NL, "1/3"},
/* pl */ {IDMS_1_3_PL, "1/3"},
/* sv */ {IDMS_1_3_SV, "1/3"},

/* en */ {IDMS_1_4,    "1/4"},
/* de */ {IDMS_1_4_DE, "1/4"},
/* fr */ {IDMS_1_4_FR, "1/4"},
/* it */ {IDMS_1_4_IT, "1/4"},
/* nl */ {IDMS_1_4_NL, "1/4"},
/* pl */ {IDMS_1_4_PL, "1/4"},
/* sv */ {IDMS_1_4_SV, "1/4"},

/* en */ {IDMS_1_5,    "1/5"},
/* de */ {IDMS_1_5_DE, "1/5"},
/* fr */ {IDMS_1_5_FR, "1/5"},
/* it */ {IDMS_1_5_IT, "1/5"},
/* nl */ {IDMS_1_5_NL, "1/5"},
/* pl */ {IDMS_1_5_PL, "1/5"},
/* sv */ {IDMS_1_5_SV, "1/5"},

/* en */ {IDMS_1_6,    "1/6"},
/* de */ {IDMS_1_6_DE, "1/6"},
/* fr */ {IDMS_1_6_FR, "1/6"},
/* it */ {IDMS_1_6_IT, "1/6"},
/* nl */ {IDMS_1_6_NL, "1/6"},
/* pl */ {IDMS_1_6_PL, "1/6"},
/* sv */ {IDMS_1_6_SV, "1/6"},

/* en */ {IDMS_1_7,    "1/7"},
/* de */ {IDMS_1_7_DE, "1/7"},
/* fr */ {IDMS_1_7_FR, "1/7"},
/* it */ {IDMS_1_7_IT, "1/7"},
/* nl */ {IDMS_1_7_NL, "1/7"},
/* pl */ {IDMS_1_7_PL, "1/7"},
/* sv */ {IDMS_1_7_SV, "1/7"},

/* en */ {IDMS_1_8,    "1/8"},
/* de */ {IDMS_1_8_DE, "1/8"},
/* fr */ {IDMS_1_8_FR, "1/8"},
/* it */ {IDMS_1_8_IT, "1/8"},
/* nl */ {IDMS_1_8_NL, "1/8"},
/* pl */ {IDMS_1_8_PL, "1/8"},
/* sv */ {IDMS_1_8_SV, "1/8"},

/* en */ {IDMS_1_9,    "1/9"},
/* de */ {IDMS_1_9_DE, "1/9"},
/* fr */ {IDMS_1_9_FR, "1/9"},
/* it */ {IDMS_1_9_IT, "1/9"},
/* nl */ {IDMS_1_9_NL, "1/9"},
/* pl */ {IDMS_1_9_PL, "1/9"},
/* sv */ {IDMS_1_9_SV, "1/9"},

/* en */ {IDMS_1_10,    "1/10"},
/* de */ {IDMS_1_10_DE, "1/10"},
/* fr */ {IDMS_1_10_FR, "1/10"},
/* it */ {IDMS_1_10_IT, "1/10"},
/* nl */ {IDMS_1_10_NL, "1/10"},
/* pl */ {IDMS_1_10_PL, "1/10"},
/* sv */ {IDMS_1_10_SV, "1/10"},

/* en */ {IDMS_MAXIMUM_SPEED,    "Maximum Speed"},
/* de */ {IDMS_MAXIMUM_SPEED_DE, "Maximale Geschwindigkeit"},
/* fr */ {IDMS_MAXIMUM_SPEED_FR, "Vitesse Maximale"},
/* it */ {IDMS_MAXIMUM_SPEED_IT, "Velocit� massima"},
/* nl */ {IDMS_MAXIMUM_SPEED_NL, "Maximale Snelheid"},
/* pl */ {IDMS_MAXIMUM_SPEED_PL, ""},  /* fuzzy */
/* sv */ {IDMS_MAXIMUM_SPEED_SV, "Maximal hastighet"},

/* en */ {IDMS_200_PERCENT,    "200%"},
/* de */ {IDMS_200_PERCENT_DE, "200%"},
/* fr */ {IDMS_200_PERCENT_FR, "200%"},
/* it */ {IDMS_200_PERCENT_IT, "200%"},
/* nl */ {IDMS_200_PERCENT_NL, "200%"},
/* pl */ {IDMS_200_PERCENT_PL, "200%"},
/* sv */ {IDMS_200_PERCENT_SV, "200%"},

/* en */ {IDMS_100_PERCENT,    "100%"},
/* de */ {IDMS_100_PERCENT_DE, "100%"},
/* fr */ {IDMS_100_PERCENT_FR, "100%"},
/* it */ {IDMS_100_PERCENT_IT, "100%"},
/* nl */ {IDMS_100_PERCENT_NL, "100%"},
/* pl */ {IDMS_100_PERCENT_PL, "100%"},
/* sv */ {IDMS_100_PERCENT_SV, "100%"},

/* en */ {IDMS_50_PERCENT,    "50%"},
/* de */ {IDMS_50_PERCENT_DE, "50%"},
/* fr */ {IDMS_50_PERCENT_FR, "50%"},
/* it */ {IDMS_50_PERCENT_IT, "50%"},
/* nl */ {IDMS_50_PERCENT_NL, "50%"},
/* pl */ {IDMS_50_PERCENT_PL, "50%"},
/* sv */ {IDMS_50_PERCENT_SV, "50%"},

/* en */ {IDMS_20_PERCENT,    "20%"},
/* de */ {IDMS_20_PERCENT_DE, "20%"},
/* fr */ {IDMS_20_PERCENT_FR, "20%"},
/* it */ {IDMS_20_PERCENT_IT, "20%"},
/* nl */ {IDMS_20_PERCENT_NL, "20%"},
/* pl */ {IDMS_20_PERCENT_PL, "20%"},
/* sv */ {IDMS_20_PERCENT_SV, "20%"},

/* en */ {IDMS_10_PERCENT,    "10%"},
/* de */ {IDMS_10_PERCENT_DE, "10%"},
/* fr */ {IDMS_10_PERCENT_FR, "10%"},
/* it */ {IDMS_10_PERCENT_IT, "10%"},
/* nl */ {IDMS_10_PERCENT_NL, "10%"},
/* pl */ {IDMS_10_PERCENT_PL, "10%"},
/* sv */ {IDMS_10_PERCENT_SV, "10%"},

/* en */ {IDMS_NO_LIMIT,    "No limit"},
/* de */ {IDMS_NO_LIMIT_DE, "Kein Limit"},
/* fr */ {IDMS_NO_LIMIT_FR, "Aucune limite"},
/* it */ {IDMS_NO_LIMIT_IT, "Nessun limite"},
/* nl */ {IDMS_NO_LIMIT_NL, "Geen limiet"},
/* pl */ {IDMS_NO_LIMIT_PL, ""},  /* fuzzy */
/* sv */ {IDMS_NO_LIMIT_SV, "Ingen gr�ns"},

/* en */ {IDMS_CUSTOM,    "Custom"},
/* de */ {IDMS_CUSTOM_DE, "Benutzerdefiniert"},
/* fr */ {IDMS_CUSTOM_FR, "Personnalis�"},
/* it */ {IDMS_CUSTOM_IT, "Personalizzata"},
/* nl */ {IDMS_CUSTOM_NL, "Eigen"},
/* pl */ {IDMS_CUSTOM_PL, ""},  /* fuzzy */
/* sv */ {IDMS_CUSTOM_SV, "Egen"},

/* en */ {IDMS_WARP_MODE,    "Warp mode"},
/* de */ {IDMS_WARP_MODE_DE, "Warp modus"},
/* fr */ {IDMS_WARP_MODE_FR, "Mode turbo"},
/* it */ {IDMS_WARP_MODE_IT, "Modalit� turbo"},
/* nl */ {IDMS_WARP_MODE_NL, "Warp modus"},
/* pl */ {IDMS_WARP_MODE_PL, ""},  /* fuzzy */
/* sv */ {IDMS_WARP_MODE_SV, "Warpl�ge"},

/* en */ {IDMS_FULLSCREEN,    "Fullscreen"},
/* de */ {IDMS_FULLSCREEN_DE, "Vollbild"},
/* fr */ {IDMS_FULLSCREEN_FR, "P�riph�rique plein �cran"},
/* it */ {IDMS_FULLSCREEN_IT, "Schermo intero"},
/* nl */ {IDMS_FULLSCREEN_NL, "Volscherm"},
/* pl */ {IDMS_FULLSCREEN_PL, ""},  /* fuzzy */
/* sv */ {IDMS_FULLSCREEN_SV, "Fullsk�rm"},

/* en */ {IDMS_VIDEO_CACHE,    "Video cache"},
/* de */ {IDMS_VIDEO_CACHE_DE, "Video cache"},
/* fr */ {IDMS_VIDEO_CACHE_FR, "Cache vid�o"},
/* it */ {IDMS_VIDEO_CACHE_IT, "Cache video"},
/* nl */ {IDMS_VIDEO_CACHE_NL, "Video cache"},
/* pl */ {IDMS_VIDEO_CACHE_PL, ""},  /* fuzzy */
/* sv */ {IDMS_VIDEO_CACHE_SV, "Grafikcache"},

/* en */ {IDMS_DOUBLE_SIZE,    "Double size"},
/* de */ {IDMS_DOUBLE_SIZE_DE, "Doppelte Gr��e"},
/* fr */ {IDMS_DOUBLE_SIZE_FR, "Taille double"},
/* it */ {IDMS_DOUBLE_SIZE_IT, "Dimensione doppia"},
/* nl */ {IDMS_DOUBLE_SIZE_NL, "Dubbele grootte"},
/* pl */ {IDMS_DOUBLE_SIZE_PL, ""},  /* fuzzy */
/* sv */ {IDMS_DOUBLE_SIZE_SV, "Dubbel storlek"},

/* en */ {IDMS_DOUBLE_SCAN,    "Double scan"},
/* de */ {IDMS_DOUBLE_SCAN_DE, "Doppelt Scan"},
/* fr */ {IDMS_DOUBLE_SCAN_FR, "Double scan"},
/* it */ {IDMS_DOUBLE_SCAN_IT, "Scansione doppia"},
/* nl */ {IDMS_DOUBLE_SCAN_NL, "Dubbele scan"},
/* pl */ {IDMS_DOUBLE_SCAN_PL, ""},  /* fuzzy */
/* sv */ {IDMS_DOUBLE_SCAN_SV, "Dubbelskanning"},

/* en */ {IDMS_PAL_EMULATION,    "PAL Emulation"},
/* de */ {IDMS_PAL_EMULATION_DE, "PAL Emulation"},
/* fr */ {IDMS_PAL_EMULATION_FR, "�mulation PAL"},
/* it */ {IDMS_PAL_EMULATION_IT, "Emulazione PAL"},
/* nl */ {IDMS_PAL_EMULATION_NL, "PAL Emulatie"},
/* pl */ {IDMS_PAL_EMULATION_PL, ""},  /* fuzzy */
/* sv */ {IDMS_PAL_EMULATION_SV, "PAL-emulering"},

/* en */ {IDMS_SCALE2X,    "Scale2x"},
/* de */ {IDMS_SCALE2X_DE, "Scale2x"},
/* fr */ {IDMS_SCALE2X_FR, "Scale2x"},
/* it */ {IDMS_SCALE2X_IT, "Scale2x"},
/* nl */ {IDMS_SCALE2X_NL, "Schaal2x"},
/* pl */ {IDMS_SCALE2X_PL, ""},  /* fuzzy */
/* sv */ {IDMS_SCALE2X_SV, "Scale2x"},

/* en */ {IDMS_VDC_SETTINGS,    "VDC settings"},
/* de */ {IDMS_VDC_SETTINGS_DE, "VDC Einstellungen"},
/* fr */ {IDMS_VDC_SETTINGS_FR, "Param�tres VDC"},
/* it */ {IDMS_VDC_SETTINGS_IT, "Impostazioni VDC"},
/* nl */ {IDMS_VDC_SETTINGS_NL, "VDC instellingen"},
/* pl */ {IDMS_VDC_SETTINGS_PL, ""},  /* fuzzy */
/* sv */ {IDMS_VDC_SETTINGS_SV, "VDC-inst�llningar"},

/* en */ {IDMS_64KB_VIDEO_MEMORY,    "64KB video memory"},
/* de */ {IDMS_64KB_VIDEO_MEMORY_DE, "64KB Bildschirm Speicher"},
/* fr */ {IDMS_64KB_VIDEO_MEMORY_FR, "M�moire d'affichage de 64Ko"},
/* it */ {IDMS_64KB_VIDEO_MEMORY_IT, "Memoria video da 64KB"},
/* nl */ {IDMS_64KB_VIDEO_MEMORY_NL, "64KB video geheugen"},
/* pl */ {IDMS_64KB_VIDEO_MEMORY_PL, ""},  /* fuzzy */
/* sv */ {IDMS_64KB_VIDEO_MEMORY_SV, "64KB grafikminne"},

/* en */ {IDMS_SWAP_JOYSTICKS,    "Swap Joysticks"},
/* de */ {IDMS_SWAP_JOYSTICKS_DE, "Joystick ports austauschen"},
/* fr */ {IDMS_SWAP_JOYSTICKS_FR, "Interchanger les joysticks"},
/* it */ {IDMS_SWAP_JOYSTICKS_IT, "Scambia joystick"},
/* nl */ {IDMS_SWAP_JOYSTICKS_NL, "Verwissel Joysticks"},
/* pl */ {IDMS_SWAP_JOYSTICKS_PL, ""},  /* fuzzy */
/* sv */ {IDMS_SWAP_JOYSTICKS_SV, "V�xla spelportarna"},

/* en */ {IDMS_SOUND_PLAYBACK,    "Sound playback"},
/* de */ {IDMS_SOUND_PLAYBACK_DE, "Sound Einschalten"},
/* fr */ {IDMS_SOUND_PLAYBACK_FR, "Lecture audio"},
/* it */ {IDMS_SOUND_PLAYBACK_IT, "Riproduzione del suono"},
/* nl */ {IDMS_SOUND_PLAYBACK_NL, "Geluidsuitvoer"},
/* pl */ {IDMS_SOUND_PLAYBACK_PL, ""},  /* fuzzy */
/* sv */ {IDMS_SOUND_PLAYBACK_SV, "Aktivera ljud�tergivning"},

/* en */ {IDMS_TRUE_DRIVE_EMU,    "True drive emulation"},
/* de */ {IDMS_TRUE_DRIVE_EMU_DE, "Pr�zise Floppy Emulation"},
/* fr */ {IDMS_TRUE_DRIVE_EMU_FR, "Activer l'�mulation r�elle des lecteurs"},
/* it */ {IDMS_TRUE_DRIVE_EMU_IT, "Attiva l'emulazione hardware dei drive"},
/* nl */ {IDMS_TRUE_DRIVE_EMU_NL, "Hardware drive emulatie"},
/* pl */ {IDMS_TRUE_DRIVE_EMU_PL, ""},  /* fuzzy */
/* sv */ {IDMS_TRUE_DRIVE_EMU_SV, "�kta diskettenhetsemulering"},

/* en */ {IDMS_VIRTUAL_DEVICE_TRAPS,    "Virtual device traps"},
/* de */ {IDMS_VIRTUAL_DEVICE_TRAPS_DE, "Virtuelle Ger�te Traps"},
/* fr */ {IDMS_VIRTUAL_DEVICE_TRAPS_FR, "Activer les p�riph�riques virtuels"},
/* it */ {IDMS_VIRTUAL_DEVICE_TRAPS_IT, "Rileva periferiche virtuali"},
/* nl */ {IDMS_VIRTUAL_DEVICE_TRAPS_NL, "Virtuele apparaat traps"},
/* pl */ {IDMS_VIRTUAL_DEVICE_TRAPS_PL, ""},  /* fuzzy */
/* sv */ {IDMS_VIRTUAL_DEVICE_TRAPS_SV, "Virtuella enhetstrap:ar"},

/* en */ {IDMS_DRIVE_SYNC_FACTOR,    "Drive sync factor"},
/* de */ {IDMS_DRIVE_SYNC_FACTOR_DE, "Laufwerkt Sync Faktor"},
/* fr */ {IDMS_DRIVE_SYNC_FACTOR_FR, "Facteur de synchro du lecteur"},
/* it */ {IDMS_DRIVE_SYNC_FACTOR_IT, "Fattore di sincronizzazione del drive"},
/* nl */ {IDMS_DRIVE_SYNC_FACTOR_NL, "Drive synchronisatie faktor"},
/* pl */ {IDMS_DRIVE_SYNC_FACTOR_PL, ""},  /* fuzzy */
/* sv */ {IDMS_DRIVE_SYNC_FACTOR_SV, "Synkfaktor f�r diskettstation"},

/* en */ {IDMS_PAL,    "PAL"},
/* de */ {IDMS_PAL_DE, "PAL"},
/* fr */ {IDMS_PAL_FR, "PAL"},
/* it */ {IDMS_PAL_IT, "PAL"},
/* nl */ {IDMS_PAL_NL, "PAL"},
/* pl */ {IDMS_PAL_PL, "PAL"},
/* sv */ {IDMS_PAL_SV, "PAL"},

/* en */ {IDMS_NTSC,    "NTSC"},
/* de */ {IDMS_NTSC_DE, "NTSC"},
/* fr */ {IDMS_NTSC_FR, "NTSC"},
/* it */ {IDMS_NTSC_IT, "NTSC"},
/* nl */ {IDMS_NTSC_NL, "NTSC"},
/* pl */ {IDMS_NTSC_PL, "NTSC"},
/* sv */ {IDMS_NTSC_SV, "NTSC"},

/* en */ {IDMS_VIDEO_STANDARD,    "Video standard"},
/* de */ {IDMS_VIDEO_STANDARD_DE, "Video standard"},
/* fr */ {IDMS_VIDEO_STANDARD_FR, "Standard vid�o"},
/* it */ {IDMS_VIDEO_STANDARD_IT, "Standard video"},
/* nl */ {IDMS_VIDEO_STANDARD_NL, "Video standaard"},
/* pl */ {IDMS_VIDEO_STANDARD_PL, ""},  /* fuzzy */
/* sv */ {IDMS_VIDEO_STANDARD_SV, "TV-standard"},

/* en */ {IDMS_PAL_G,    "PAL-G"},
/* de */ {IDMS_PAL_G_DE, "PAL-G"},
/* fr */ {IDMS_PAL_G_FR, "PAL-G"},
/* it */ {IDMS_PAL_G_IT, "PAL-G"},
/* nl */ {IDMS_PAL_G_NL, "PAL-G"},
/* pl */ {IDMS_PAL_G_PL, "PAL-G"},
/* sv */ {IDMS_PAL_G_SV, "PAL-G"},

/* en */ {IDMS_NTSC_M,    "NTSC-M"},
/* de */ {IDMS_NTSC_M_DE, "NTSC-M"},
/* fr */ {IDMS_NTSC_M_FR, "NTSC-M"},
/* it */ {IDMS_NTSC_M_IT, "NTSC-M"},
/* nl */ {IDMS_NTSC_M_NL, "NTSC-M"},
/* pl */ {IDMS_NTSC_M_PL, "NTSC-M"},
/* sv */ {IDMS_NTSC_M_SV, "NTSC-M"},

/* en */ {IDMS_OLD_NTSC_M,    "Old NTSC-M"},
/* de */ {IDMS_OLD_NTSC_M_DE, "NTSC-M alt"},
/* fr */ {IDMS_OLD_NTSC_M_FR, "Ancien NTSC-M"},
/* it */ {IDMS_OLD_NTSC_M_IT, "Vecchio NTSC-M"},
/* nl */ {IDMS_OLD_NTSC_M_NL, "Oud NTSC-M"},
/* pl */ {IDMS_OLD_NTSC_M_PL, ""},  /* fuzzy */
/* sv */ {IDMS_OLD_NTSC_M_SV, "Gammal NTSC-M"},

/* en */ {IDMS_EMU_ID,    "Emulator Identification"},
/* de */ {IDMS_EMU_ID_DE, "Emulator Identifikation"},
/* fr */ {IDMS_EMU_ID_FR, "Identification de l'�mulateur"},
/* it */ {IDMS_EMU_ID_IT, "Identificazione emulatore"},
/* nl */ {IDMS_EMU_ID_NL, "Emulator Identificatie"},
/* pl */ {IDMS_EMU_ID_PL, ""},  /* fuzzy */
/* sv */ {IDMS_EMU_ID_SV, "Emulatoridentifiering"},

/* en */ {IDMS_VIC_1112_IEEE_488,    "VIC-1112 IEEE 488 module"},
/* de */ {IDMS_VIC_1112_IEEE_488_DE, "VIC-1112 IEEE 488 Modul"},
/* fr */ {IDMS_VIC_1112_IEEE_488_FR, "Module VIC-1112 IEEE 488"},
/* it */ {IDMS_VIC_1112_IEEE_488_IT, "Modulo VIC-1112 IEEE 488"},
/* nl */ {IDMS_VIC_1112_IEEE_488_NL, "VIC-1112 IEEE 488 module"},
/* pl */ {IDMS_VIC_1112_IEEE_488_PL, ""},  /* fuzzy */
/* sv */ {IDMS_VIC_1112_IEEE_488_SV, "VIC-1112 IEEE 488-modul"},

/* en */ {IDMS_IEEE_488_INTERFACE,    "IEEE 488 Interface emulation"},
/* de */ {IDMS_IEEE_488_INTERFACE_DE, "IEEE488 Schnittstellen-Emulation"},
/* fr */ {IDMS_IEEE_488_INTERFACE_FR, "Interface d'�mulation IEEE488"},
/* it */ {IDMS_IEEE_488_INTERFACE_IT, "Emulazione interfaccia IEEE488"},
/* nl */ {IDMS_IEEE_488_INTERFACE_NL, "IEEE488 Interface emulatie"},
/* pl */ {IDMS_IEEE_488_INTERFACE_PL, ""},  /* fuzzy */
/* sv */ {IDMS_IEEE_488_INTERFACE_SV, "Emulering av IEEE488-gr�nssnitt"},

/* en */ {IDMS_1351_MOUSE,    "1351 mouse emulation"},
/* de */ {IDMS_1351_MOUSE_DE, "1351 Mausemulation"},
/* fr */ {IDMS_1351_MOUSE_FR, "�mulation de la souris 1351"},
/* it */ {IDMS_1351_MOUSE_IT, "Emulazione mouse 1351"},
/* nl */ {IDMS_1351_MOUSE_NL, "1351 muis emulatie"},
/* pl */ {IDMS_1351_MOUSE_PL, ""},  /* fuzzy */
/* sv */ {IDMS_1351_MOUSE_SV, "1351-musemulering"},

/* en */ {IDMS_SETTINGS,    "Settings"},
/* de */ {IDMS_SETTINGS_DE, "Einstellungen"},
/* fr */ {IDMS_SETTINGS_FR, "Param�tres"},
/* it */ {IDMS_SETTINGS_IT, "Impostazioni"},
/* nl */ {IDMS_SETTINGS_NL, "Instellingen"},
/* pl */ {IDMS_SETTINGS_PL, ""},  /* fuzzy */
/* sv */ {IDMS_SETTINGS_SV, "Inst�llningar"},

/* en */ {IDMS_VIDEO_SETTINGS,    "Video settings..."},
/* de */ {IDMS_VIDEO_SETTINGS_DE, "Video Einstellungen..."},
/* fr */ {IDMS_VIDEO_SETTINGS_FR, "Param�tres vid�o..."},
/* it */ {IDMS_VIDEO_SETTINGS_IT, "Impostazioni video..."},
/* nl */ {IDMS_VIDEO_SETTINGS_NL, "Video instellingen..."},
/* pl */ {IDMS_VIDEO_SETTINGS_PL, ""},  /* fuzzy */
/* sv */ {IDMS_VIDEO_SETTINGS_SV, "Grafikinst�llningar..."},

/* en */ {IDMS_VIC_SETTINGS,    "VIC settings..."},
/* de */ {IDMS_VIC_SETTINGS_DE, "VIC Einstellungen..."},
/* fr */ {IDMS_VIC_SETTINGS_FR, "Param�tres VIC..."},
/* it */ {IDMS_VIC_SETTINGS_IT, "Impostazioni VIC..."},
/* nl */ {IDMS_VIC_SETTINGS_NL, "VIC instellingen..."},
/* pl */ {IDMS_VIC_SETTINGS_PL, ""},  /* fuzzy */
/* sv */ {IDMS_VIC_SETTINGS_SV, "VIC-inst�llningar..."},

/* en */ {IDMS_CBM2_SETTINGS,    "CBM2 settings..."},
/* de */ {IDMS_CBM2_SETTINGS_DE, "CBM2 Einstellungen..."},
/* fr */ {IDMS_CBM2_SETTINGS_FR, "Param�tres CBM2..."},
/* it */ {IDMS_CBM2_SETTINGS_IT, "Impostazioni CBM2..."},
/* nl */ {IDMS_CBM2_SETTINGS_NL, "CBM2 instellingen..."},
/* pl */ {IDMS_CBM2_SETTINGS_PL, ""},  /* fuzzy */
/* sv */ {IDMS_CBM2_SETTINGS_SV, "CBM2-inst�llningar..."},

/* en */ {IDMS_PET_SETTINGS,    "PET settings..."},
/* de */ {IDMS_PET_SETTINGS_DE, "PET Einstellungen..."},
/* fr */ {IDMS_PET_SETTINGS_FR, "Param�tres PET..."},
/* it */ {IDMS_PET_SETTINGS_IT, "Impostazioni PET..."},
/* nl */ {IDMS_PET_SETTINGS_NL, "PET instellingen..."},
/* pl */ {IDMS_PET_SETTINGS_PL, ""},  /* fuzzy */
/* sv */ {IDMS_PET_SETTINGS_SV, "PET-inst�llningar..."},

/* en */ {IDMS_PERIPHERAL_SETTINGS,    "Peripheral settings..."},
/* de */ {IDMS_PERIPHERAL_SETTINGS_DE, "Peripherie Einstellungen..."},
/* fr */ {IDMS_PERIPHERAL_SETTINGS_FR, "Param�tres des p�riph�riques..."},
/* it */ {IDMS_PERIPHERAL_SETTINGS_IT, "Impostazioni periferiche..."},
/* nl */ {IDMS_PERIPHERAL_SETTINGS_NL, "Rand Apparaten instellingen..."},
/* pl */ {IDMS_PERIPHERAL_SETTINGS_PL, ""},  /* fuzzy */
/* sv */ {IDMS_PERIPHERAL_SETTINGS_SV, "Inst�llningar f�r kringutrustning..."},

/* en */ {IDMS_DRIVE_SETTINGS,    "Drive settings..."},
/* de */ {IDMS_DRIVE_SETTINGS_DE, "Floppy Einstellungen..."},
/* fr */ {IDMS_DRIVE_SETTINGS_FR, "Param�tres des lecteurs..."},
/* it */ {IDMS_DRIVE_SETTINGS_IT, "Impostazioni drive..."},
/* nl */ {IDMS_DRIVE_SETTINGS_NL, "Drive instellingen..."},
/* pl */ {IDMS_DRIVE_SETTINGS_PL, ""},  /* fuzzy */
/* sv */ {IDMS_DRIVE_SETTINGS_SV, "Diskettenhetsinst�llningar..."},

/* en */ {IDMS_DATASETTE_SETTINGS,    "Datasette settings..."},
/* de */ {IDMS_DATASETTE_SETTINGS_DE, "Bandlaufwerk Einstellungen..."},
/* fr */ {IDMS_DATASETTE_SETTINGS_FR, "Param�tres du datassette..."},
/* it */ {IDMS_DATASETTE_SETTINGS_IT, "Impostazioni registratore..."},
/* nl */ {IDMS_DATASETTE_SETTINGS_NL, "Datasette instellingen..."},
/* pl */ {IDMS_DATASETTE_SETTINGS_PL, ""},  /* fuzzy */
/* sv */ {IDMS_DATASETTE_SETTINGS_SV, "Datasetteinst�llningar..."},

/* en */ {IDMS_PLUS4_SETTINGS,    "Plus4 settings..."},
/* de */ {IDMS_PLUS4_SETTINGS_DE, "Plus4 Einstellungen..."},
/* fr */ {IDMS_PLUS4_SETTINGS_FR, "Param�tres Plus4..."},
/* it */ {IDMS_PLUS4_SETTINGS_IT, "Impostazioni Plus4..."},
/* nl */ {IDMS_PLUS4_SETTINGS_NL, "Plus4 instellingen"},
/* pl */ {IDMS_PLUS4_SETTINGS_PL, ""},  /* fuzzy */
/* sv */ {IDMS_PLUS4_SETTINGS_SV, "Plus4-inst�llningar..."},

/* en */ {IDMS_VICII_SETTINGS,    "VIC-II settings..."},
/* de */ {IDMS_VICII_SETTINGS_DE, "VIC-II Einstellungen..."},
/* fr */ {IDMS_VICII_SETTINGS_FR, "Param�tres VIC-II..."},
/* it */ {IDMS_VICII_SETTINGS_IT, "Impostazioni VIC-II..."},
/* nl */ {IDMS_VICII_SETTINGS_NL, "VIC-II instellingen..."},
/* pl */ {IDMS_VICII_SETTINGS_PL, ""},  /* fuzzy */
/* sv */ {IDMS_VICII_SETTINGS_SV, "VIC-II-inst�llningar..."},

/* en */ {IDMS_JOYSTICK_SETTINGS,    "Joystick settings..."},
/* de */ {IDMS_JOYSTICK_SETTINGS_DE, "Joystick Einstellungen..."},
/* fr */ {IDMS_JOYSTICK_SETTINGS_FR, "Param�tres des joysticks..."},
/* it */ {IDMS_JOYSTICK_SETTINGS_IT, "Impostazioni joystick..."},
/* nl */ {IDMS_JOYSTICK_SETTINGS_NL, "Joystick instellingen..."},
/* pl */ {IDMS_JOYSTICK_SETTINGS_PL, ""},  /* fuzzy */
/* sv */ {IDMS_JOYSTICK_SETTINGS_SV, "Inst�llningar f�r joystick..."},

/* en */ {IDMS_JOYSTICK_DEVICE_SELECT,    "Joystick device selection"},
/* de */ {IDMS_JOYSTICK_DEVICE_SELECT_DE, ""},  /* fuzzy */
/* fr */ {IDMS_JOYSTICK_DEVICE_SELECT_FR, ""},  /* fuzzy */
/* it */ {IDMS_JOYSTICK_DEVICE_SELECT_IT, "Seleziona il joystick"},
/* nl */ {IDMS_JOYSTICK_DEVICE_SELECT_NL, "Joystick apparaat selectie"},
/* pl */ {IDMS_JOYSTICK_DEVICE_SELECT_PL, ""},  /* fuzzy */
/* sv */ {IDMS_JOYSTICK_DEVICE_SELECT_SV, "enhetsinst�llningar f�r joystick"},

/* en */ {IDMS_JOYSTICK_FIRE_SELECT,    "Joystick fire button selection"},
/* de */ {IDMS_JOYSTICK_FIRE_SELECT_DE, ""},  /* fuzzy */
/* fr */ {IDMS_JOYSTICK_FIRE_SELECT_FR, ""},  /* fuzzy */
/* it */ {IDMS_JOYSTICK_FIRE_SELECT_IT, "Seleziona il pulsante di fuoco"},
/* nl */ {IDMS_JOYSTICK_FIRE_SELECT_NL, "Joystick vuur knop selectie"},
/* pl */ {IDMS_JOYSTICK_FIRE_SELECT_PL, ""},  /* fuzzy */
/* sv */ {IDMS_JOYSTICK_FIRE_SELECT_SV, "val f�r joystickens fire-knapp"},

/* en */ {IDMS_KEYBOARD_SETTINGS,    "Keyboard settings..."},
/* de */ {IDMS_KEYBOARD_SETTINGS_DE, "Tastatur Einstellungen..."},
/* fr */ {IDMS_KEYBOARD_SETTINGS_FR, "Param�tres du clavier..."},
/* it */ {IDMS_KEYBOARD_SETTINGS_IT, "Impostazioni tastiera..."},
/* nl */ {IDMS_KEYBOARD_SETTINGS_NL, "Toetsenbord instellingen..."},
/* pl */ {IDMS_KEYBOARD_SETTINGS_PL, ""},  /* fuzzy */
/* sv */ {IDMS_KEYBOARD_SETTINGS_SV, "Tangentbordsinst�llningar..."},

/* en */ {IDMS_SOUND_SETTINGS,    "Sound settings..."},
/* de */ {IDMS_SOUND_SETTINGS_DE, "Sound Einstellungen..."},
/* fr */ {IDMS_SOUND_SETTINGS_FR, "Param�tres son..."},
/* it */ {IDMS_SOUND_SETTINGS_IT, "Impostazioni suono..."},
/* nl */ {IDMS_SOUND_SETTINGS_NL, "Geluid instellingen..."},
/* pl */ {IDMS_SOUND_SETTINGS_PL, ""},  /* fuzzy */
/* sv */ {IDMS_SOUND_SETTINGS_SV, "Ljudinst�llningar..."},

/* en */ {IDMS_SID_SETTINGS,    "SID settings..."},
/* de */ {IDMS_SID_SETTINGS_DE, "SID Einstellungen..."},
/* fr */ {IDMS_SID_SETTINGS_FR, "Param�tres SID..."},
/* it */ {IDMS_SID_SETTINGS_IT, "Impostazioni SID..."},
/* nl */ {IDMS_SID_SETTINGS_NL, "SID instellingen..."},
/* pl */ {IDMS_SID_SETTINGS_PL, ""},  /* fuzzy */
/* sv */ {IDMS_SID_SETTINGS_SV, "SID-inst�llningar..."},

/* en */ {IDMS_ROM_SETTINGS,    "ROM settings..."},
/* de */ {IDMS_ROM_SETTINGS_DE, "ROM Einstellungen..."},
/* fr */ {IDMS_ROM_SETTINGS_FR, "Param�tres ROM..."},
/* it */ {IDMS_ROM_SETTINGS_IT, "Impostazioni ROM..."},
/* nl */ {IDMS_ROM_SETTINGS_NL, "ROM instellingen..."},
/* pl */ {IDMS_ROM_SETTINGS_PL, ""},  /* fuzzy */
/* sv */ {IDMS_ROM_SETTINGS_SV, "ROM-inst�llningar..."},

/* en */ {IDMS_RAM_SETTINGS,    "RAM settings..."},
/* de */ {IDMS_RAM_SETTINGS_DE, "RAM Einstellungen..."},
/* fr */ {IDMS_RAM_SETTINGS_FR, "Param�tres de la RAM..."},
/* it */ {IDMS_RAM_SETTINGS_IT, "Impostazioni RAM..."},
/* nl */ {IDMS_RAM_SETTINGS_NL, "RAM instellingen..."},
/* pl */ {IDMS_RAM_SETTINGS_PL, ""},  /* fuzzy */
/* sv */ {IDMS_RAM_SETTINGS_SV, "RAM-inst�llningar..."},

/* en */ {IDMS_RS232_SETTINGS,    "RS232 settings..."},
/* de */ {IDMS_RS232_SETTINGS_DE, "RS232 Einstellungen..."},
/* fr */ {IDMS_RS232_SETTINGS_FR, "Param�tres RS232..."},
/* it */ {IDMS_RS232_SETTINGS_IT, "Impostazioni RS232..."},
/* nl */ {IDMS_RS232_SETTINGS_NL, "RS232 instellingen..."},
/* pl */ {IDMS_RS232_SETTINGS_PL, ""},  /* fuzzy */
/* sv */ {IDMS_RS232_SETTINGS_SV, "RS232-inst�llningar..."},

/* en */ {IDMS_C128_SETTINGS,    "C128 settings..."},
/* de */ {IDMS_C128_SETTINGS_DE, "C128 Einstellungen..."},
/* fr */ {IDMS_C128_SETTINGS_FR, "Param�tres C128..."},
/* it */ {IDMS_C128_SETTINGS_IT, "Impostazioni C128..."},
/* nl */ {IDMS_C128_SETTINGS_NL, "C128 instellingen..."},
/* pl */ {IDMS_C128_SETTINGS_PL, ""},  /* fuzzy */
/* sv */ {IDMS_C128_SETTINGS_SV, "C128-inst�llningar..."},

/* en */ {IDMS_CART_IO_SETTINGS,    "Cartridge/IO settings"},
/* de */ {IDMS_CART_IO_SETTINGS_DE, "Erweiterungsmodul Einstellungen"},
/* fr */ {IDMS_CART_IO_SETTINGS_FR, "Param�tres E/S cartouche"},
/* it */ {IDMS_CART_IO_SETTINGS_IT, "Impostazioni I/O della cartuccia"},
/* nl */ {IDMS_CART_IO_SETTINGS_NL, "Cartridge Instellingen"},
/* pl */ {IDMS_CART_IO_SETTINGS_PL, ""},  /* fuzzy */
/* sv */ {IDMS_CART_IO_SETTINGS_SV, "Inst�llningar f�r insticksmoduler..."},

/* en */ {IDMS_REU_SETTINGS,    "REU settings..."},
/* de */ {IDMS_REU_SETTINGS_DE, "REU Einstellungen..."},
/* fr */ {IDMS_REU_SETTINGS_FR, "Param�tres REU..."},
/* it */ {IDMS_REU_SETTINGS_IT, "Impostazioni REU..."},
/* nl */ {IDMS_REU_SETTINGS_NL, "REU instellingen..."},
/* pl */ {IDMS_REU_SETTINGS_PL, ""},  /* fuzzy */
/* sv */ {IDMS_REU_SETTINGS_SV, "REU-inst�llningar..."},

/* en */ {IDMS_GEORAM_SETTINGS,    "GEORAM settings..."},
/* de */ {IDMS_GEORAM_SETTINGS_DE, "GEORAM Einstellungen..."},
/* fr */ {IDMS_GEORAM_SETTINGS_FR, "Param�tres GEORAM..."},
/* it */ {IDMS_GEORAM_SETTINGS_IT, "Impostazioni GEORAM..."},
/* nl */ {IDMS_GEORAM_SETTINGS_NL, "GEORAM instellingen..."},
/* pl */ {IDMS_GEORAM_SETTINGS_PL, ""},  /* fuzzy */
/* sv */ {IDMS_GEORAM_SETTINGS_SV, "GEORAM-inst�llningar..."},

/* en */ {IDMS_RAMCART_SETTINGS,    "RAMCART settings..."},
/* de */ {IDMS_RAMCART_SETTINGS_DE, "RAMCART Einstellungen..."},
/* fr */ {IDMS_RAMCART_SETTINGS_FR, "Param�tres RAMCART..."},
/* it */ {IDMS_RAMCART_SETTINGS_IT, "Impostazioni RAMCART..."},
/* nl */ {IDMS_RAMCART_SETTINGS_NL, "RAMCART instellingen..."},
/* pl */ {IDMS_RAMCART_SETTINGS_PL, ""},  /* fuzzy */
/* sv */ {IDMS_RAMCART_SETTINGS_SV, "RAMCART-inst�llningar..."},

/* en */ {IDMS_PLUS60K_SETTINGS,    "PLUS60K settings..."},
/* de */ {IDMS_PLUS60K_SETTINGS_DE, "PLUS60K Einstellungen..."},
/* fr */ {IDMS_PLUS60K_SETTINGS_FR, "Param�tres PLUS60K..."},
/* it */ {IDMS_PLUS60K_SETTINGS_IT, "Impostazioni PLUS60K..."},
/* nl */ {IDMS_PLUS60K_SETTINGS_NL, "PLUS60K instellingen..."},
/* pl */ {IDMS_PLUS60K_SETTINGS_PL, ""},  /* fuzzy */
/* sv */ {IDMS_PLUS60K_SETTINGS_SV, "PLUS60K-inst�llningar..."},

/* en */ {IDMS_PLUS256K_SETTINGS,    "PLUS256K settings..."},
/* de */ {IDMS_PLUS256K_SETTINGS_DE, "PLUS256K Einstellungen..."},
/* fr */ {IDMS_PLUS256K_SETTINGS_FR, "Param�tres PLUS256K..."},
/* it */ {IDMS_PLUS256K_SETTINGS_IT, "Impostazioni PLUS256K..."},
/* nl */ {IDMS_PLUS256K_SETTINGS_NL, "PLUS256K instellingen..."},
/* pl */ {IDMS_PLUS256K_SETTINGS_PL, ""},  /* fuzzy */
/* sv */ {IDMS_PLUS256K_SETTINGS_SV, "PLUS256K-inst�llningar..."},

/* en */ {IDMS_256K_SETTINGS,    "256K settings..."},
/* de */ {IDMS_256K_SETTINGS_DE, "256K Einstellungen..."},
/* fr */ {IDMS_256K_SETTINGS_FR, "Param�tres 256K..."},
/* it */ {IDMS_256K_SETTINGS_IT, "Impostazioni 256K..."},
/* nl */ {IDMS_256K_SETTINGS_NL, "256K instellingen..."},
/* pl */ {IDMS_256K_SETTINGS_PL, ""},  /* fuzzy */
/* sv */ {IDMS_256K_SETTINGS_SV, "256K-inst�llningar..."},

/* en */ {IDMS_IDE64_SETTINGS,    "IDE64 settings..."},
/* de */ {IDMS_IDE64_SETTINGS_DE, "IDE64 Einstellungen..."},
/* fr */ {IDMS_IDE64_SETTINGS_FR, "Param�tres IDE64..."},
/* it */ {IDMS_IDE64_SETTINGS_IT, "Impostazioni IDE64..."},
/* nl */ {IDMS_IDE64_SETTINGS_NL, "IDE64 instellingen..."},
/* pl */ {IDMS_IDE64_SETTINGS_PL, ""},  /* fuzzy */
/* sv */ {IDMS_IDE64_SETTINGS_SV, "IDE64-inst�llningar..."},

/* en */ {IDMS_ETHERNET_SETTINGS,    "Ethernet settings..."},
/* de */ {IDMS_ETHERNET_SETTINGS_DE, "Ethernet Einstellungen..."},
/* fr */ {IDMS_ETHERNET_SETTINGS_FR, "�mulation Ethernet..."},
/* it */ {IDMS_ETHERNET_SETTINGS_IT, "Impostazioni Ethernet..."},
/* nl */ {IDMS_ETHERNET_SETTINGS_NL, "Ethernet instellingen..."},
/* pl */ {IDMS_ETHERNET_SETTINGS_PL, ""},  /* fuzzy */
/* sv */ {IDMS_ETHERNET_SETTINGS_SV, "Ethernetinst�llningar..."},

/* en */ {IDMS_ACIA_SETTINGS,    "ACIA settings..."},
/* de */ {IDMS_ACIA_SETTINGS_DE, "ACIA Einstellungen..."},
/* fr */ {IDMS_ACIA_SETTINGS_FR, "Param�tres ACIA..."},
/* it */ {IDMS_ACIA_SETTINGS_IT, "Impostazioni ACIA..."},
/* nl */ {IDMS_ACIA_SETTINGS_NL, "ACIA instellingen..."},
/* pl */ {IDMS_ACIA_SETTINGS_PL, ""},  /* fuzzy */
/* sv */ {IDMS_ACIA_SETTINGS_SV, "ACIA-inst�llningar..."},

/* en */ {IDMS_PETREU_SETTINGS,    "PET REU settings..."},
/* de */ {IDMS_PETREU_SETTINGS_DE, "PET REU Einstellungen..."},
/* fr */ {IDMS_PETREU_SETTINGS_FR, "Param�tres PET REU..."},
/* it */ {IDMS_PETREU_SETTINGS_IT, "Impostazioni PET REU..."},
/* nl */ {IDMS_PETREU_SETTINGS_NL, "PET REU instellingen"},
/* pl */ {IDMS_PETREU_SETTINGS_PL, ""},  /* fuzzy */
/* sv */ {IDMS_PETREU_SETTINGS_SV, "PET REU-inst�llningar..."},

/* en */ {IDMS_RS232_USERPORT_SETTINGS,    "RS232 userport settings..."},
/* de */ {IDMS_RS232_USERPORT_SETTINGS_DE, "RS232 Userport Einstellungen..."},
/* fr */ {IDMS_RS232_USERPORT_SETTINGS_FR, "Param�tres RS232 userport..."},
/* it */ {IDMS_RS232_USERPORT_SETTINGS_IT, "Impostazioni RS232 su userport..."},
/* nl */ {IDMS_RS232_USERPORT_SETTINGS_NL, "RS232 userport instellingen..."},
/* pl */ {IDMS_RS232_USERPORT_SETTINGS_PL, ""},  /* fuzzy */
/* sv */ {IDMS_RS232_USERPORT_SETTINGS_SV, "RS232-anv�ndarportinst�llningar..."},

/* en */ {IDMS_SAVE_CURRENT_SETTINGS,    "Save current settings"},
/* de */ {IDMS_SAVE_CURRENT_SETTINGS_DE, "Einstellungen speichern"},
/* fr */ {IDMS_SAVE_CURRENT_SETTINGS_FR, "Enregistrer les param�tres courants"},
/* it */ {IDMS_SAVE_CURRENT_SETTINGS_IT, "Salva le impostazioni attuali"},
/* nl */ {IDMS_SAVE_CURRENT_SETTINGS_NL, "Huidige instellingen opslaan"},
/* pl */ {IDMS_SAVE_CURRENT_SETTINGS_PL, ""},  /* fuzzy */
/* sv */ {IDMS_SAVE_CURRENT_SETTINGS_SV, "Spara nuvarande inst�llningar"},

/* en */ {IDMS_LOAD_SAVED_SETTINGS,    "Load saved settings"},
/* de */ {IDMS_LOAD_SAVED_SETTINGS_DE, "Einstellungen Laden"},
/* fr */ {IDMS_LOAD_SAVED_SETTINGS_FR, "Charger les param�tres"},
/* it */ {IDMS_LOAD_SAVED_SETTINGS_IT, "Carica impostazioni salvate"},
/* nl */ {IDMS_LOAD_SAVED_SETTINGS_NL, "Opgeslagen instelling laden"},
/* pl */ {IDMS_LOAD_SAVED_SETTINGS_PL, ""},  /* fuzzy */
/* sv */ {IDMS_LOAD_SAVED_SETTINGS_SV, "L�s sparade inst�llningar"},

/* en */ {IDMS_SET_DEFAULT_SETTINGS,    "Set default settings"},
/* de */ {IDMS_SET_DEFAULT_SETTINGS_DE, "Wiederherstellen Standard Einstellungen"},
/* fr */ {IDMS_SET_DEFAULT_SETTINGS_FR, "R�tablir les param�tres par d�faut"},
/* it */ {IDMS_SET_DEFAULT_SETTINGS_IT, "Ripristina impostazioni predefinite"},
/* nl */ {IDMS_SET_DEFAULT_SETTINGS_NL, "Herstel standaard instellingen"},
/* pl */ {IDMS_SET_DEFAULT_SETTINGS_PL, ""},  /* fuzzy */
/* sv */ {IDMS_SET_DEFAULT_SETTINGS_SV, "�terst�ll f�rvalda inst�llningar"},

/* en */ {IDMS_SAVE_SETTING_ON_EXIT,    "Save settings on exit"},
/* de */ {IDMS_SAVE_SETTING_ON_EXIT_DE, "Einstellungen beim Beenden speichern"},
/* fr */ {IDMS_SAVE_SETTING_ON_EXIT_FR, "Enregistrer les param�tres � la sortie"},
/* it */ {IDMS_SAVE_SETTING_ON_EXIT_IT, "Salva impostazioni all'uscita"},
/* nl */ {IDMS_SAVE_SETTING_ON_EXIT_NL, "Sla instellingen op bij afsluiten"},
/* pl */ {IDMS_SAVE_SETTING_ON_EXIT_PL, ""},  /* fuzzy */
/* sv */ {IDMS_SAVE_SETTING_ON_EXIT_SV, "Spara inst�llningar vid avslut"},

/* en */ {IDMS_CONFIRM_ON_EXIT,    "Confirm on exit"},
/* de */ {IDMS_CONFIRM_ON_EXIT_DE, "Best�tigung beim Beenden"},
/* fr */ {IDMS_CONFIRM_ON_EXIT_FR, "Confirmation � la sortie"},
/* it */ {IDMS_CONFIRM_ON_EXIT_IT, "Conferma all'uscita"},
/* nl */ {IDMS_CONFIRM_ON_EXIT_NL, "Bevestigen bij afsluiten"},
/* pl */ {IDMS_CONFIRM_ON_EXIT_PL, ""},  /* fuzzy */
/* sv */ {IDMS_CONFIRM_ON_EXIT_SV, "Bekr�fta avslutning"},

/* en */ {IDMS_HELP,    "Help"},
/* de */ {IDMS_HELP_DE, "Hilfe"},
/* fr */ {IDMS_HELP_FR, "Aide"},
/* it */ {IDMS_HELP_IT, "Aiuto"},
/* nl */ {IDMS_HELP_NL, "Help"},
/* pl */ {IDMS_HELP_PL, ""},  /* fuzzy */
/* sv */ {IDMS_HELP_SV, "Hj�lp"},

/* en */ {IDMS_ABOUT,    "About..."},
/* de */ {IDMS_ABOUT_DE, "�ber VICE..."},
/* fr */ {IDMS_ABOUT_FR, "� Propos..."},
/* it */ {IDMS_ABOUT_IT, "Informazioni su VICE..."},
/* nl */ {IDMS_ABOUT_NL, "Over VICE..."},
/* pl */ {IDMS_ABOUT_PL, ""},  /* fuzzy */
/* sv */ {IDMS_ABOUT_SV, "Om VICE..."},

/* en */ {IDMS_COMMAND_LINE_OPTIONS,    "Command line options"},
/* de */ {IDMS_COMMAND_LINE_OPTIONS_DE, "Kommandozeilen Optionen"},
/* fr */ {IDMS_COMMAND_LINE_OPTIONS_FR, "Options de ligne de commande"},
/* it */ {IDMS_COMMAND_LINE_OPTIONS_IT, "Opzioni da riga di comando"},
/* nl */ {IDMS_COMMAND_LINE_OPTIONS_NL, "Commando invoer opties"},
/* pl */ {IDMS_COMMAND_LINE_OPTIONS_PL, ""},  /* fuzzy */
/* sv */ {IDMS_COMMAND_LINE_OPTIONS_SV, "Kommandoradsflaggor"},

/* en */ {IDMS_CONTRIBUTORS,    "Contributors"},
/* de */ {IDMS_CONTRIBUTORS_DE, "VICE Hackers"},
/* fr */ {IDMS_CONTRIBUTORS_FR, "Contributeurs"},
/* it */ {IDMS_CONTRIBUTORS_IT, "Collaboratori"},
/* nl */ {IDMS_CONTRIBUTORS_NL, "Medewerkers aan het VICE project"},
/* pl */ {IDMS_CONTRIBUTORS_PL, ""},  /* fuzzy */
/* sv */ {IDMS_CONTRIBUTORS_SV, "Bidragsl�mnare"},

/* en */ {IDMS_LICENSE,    "License"},
/* de */ {IDMS_LICENSE_DE, "Lizenz"},
/* fr */ {IDMS_LICENSE_FR, "License"},
/* it */ {IDMS_LICENSE_IT, "Licenza"},
/* nl */ {IDMS_LICENSE_NL, "Licensie"},
/* pl */ {IDMS_LICENSE_PL, ""},  /* fuzzy */
/* sv */ {IDMS_LICENSE_SV, "Licens"},

/* en */ {IDMS_NO_WARRANTY,    "No warranty"},
/* de */ {IDMS_NO_WARRANTY_DE, "Keine Garantie!"},
/* fr */ {IDMS_NO_WARRANTY_FR, "Aucune garantie!"},
/* it */ {IDMS_NO_WARRANTY_IT, "Nessuna garanzia"},
/* nl */ {IDMS_NO_WARRANTY_NL, "Geen garantie"},
/* pl */ {IDMS_NO_WARRANTY_PL, ""},  /* fuzzy */
/* sv */ {IDMS_NO_WARRANTY_SV, "Ingen garanti"},

/* en */ {IDMS_LANGUAGE_ENGLISH,    "English"},
/* de */ {IDMS_LANGUAGE_ENGLISH_DE, "Englisch"},
/* fr */ {IDMS_LANGUAGE_ENGLISH_FR, "Anglais"},
/* it */ {IDMS_LANGUAGE_ENGLISH_IT, "Inglese"},
/* nl */ {IDMS_LANGUAGE_ENGLISH_NL, "Engels"},
/* pl */ {IDMS_LANGUAGE_ENGLISH_PL, ""},  /* fuzzy */
/* sv */ {IDMS_LANGUAGE_ENGLISH_SV, "Engelska"},

/* en */ {IDMS_LANGUAGE_GERMAN,    "German"},
/* de */ {IDMS_LANGUAGE_GERMAN_DE, "Deutsch"},
/* fr */ {IDMS_LANGUAGE_GERMAN_FR, "Allemand"},
/* it */ {IDMS_LANGUAGE_GERMAN_IT, "Tedesco"},
/* nl */ {IDMS_LANGUAGE_GERMAN_NL, "Duits"},
/* pl */ {IDMS_LANGUAGE_GERMAN_PL, ""},  /* fuzzy */
/* sv */ {IDMS_LANGUAGE_GERMAN_SV, "Tyska"},

/* en */ {IDMS_LANGUAGE_FRENCH,    "French"},
/* de */ {IDMS_LANGUAGE_FRENCH_DE, "Franz�sisch"},
/* fr */ {IDMS_LANGUAGE_FRENCH_FR, "Fran�ais"},
/* it */ {IDMS_LANGUAGE_FRENCH_IT, "Francese"},
/* nl */ {IDMS_LANGUAGE_FRENCH_NL, "Frans"},
/* pl */ {IDMS_LANGUAGE_FRENCH_PL, ""},  /* fuzzy */
/* sv */ {IDMS_LANGUAGE_FRENCH_SV, "Franska"},

/* en */ {IDMS_LANGUAGE_ITALIAN,    "Italian"},
/* de */ {IDMS_LANGUAGE_ITALIAN_DE, "Italienisch"},
/* fr */ {IDMS_LANGUAGE_ITALIAN_FR, "Italien"},
/* it */ {IDMS_LANGUAGE_ITALIAN_IT, "Italiano"},
/* nl */ {IDMS_LANGUAGE_ITALIAN_NL, "Italiaans"},
/* pl */ {IDMS_LANGUAGE_ITALIAN_PL, ""},  /* fuzzy */
/* sv */ {IDMS_LANGUAGE_ITALIAN_SV, "Italiense"},

/* en */ {IDMS_LANGUAGE_DUTCH,    "Dutch"},
/* de */ {IDMS_LANGUAGE_DUTCH_DE, "Holl�ndisch"},
/* fr */ {IDMS_LANGUAGE_DUTCH_FR, "Hollandais"},
/* it */ {IDMS_LANGUAGE_DUTCH_IT, "Olandese"},
/* nl */ {IDMS_LANGUAGE_DUTCH_NL, "Nederlands"},
/* pl */ {IDMS_LANGUAGE_DUTCH_PL, ""},  /* fuzzy */
/* sv */ {IDMS_LANGUAGE_DUTCH_SV, "Nederl�ndska"},

/* en */ {IDMS_LANGUAGE_POLISH,    "Polish"},
/* de */ {IDMS_LANGUAGE_POLISH_DE, "Polnisch"},
/* fr */ {IDMS_LANGUAGE_POLISH_FR, "Polonais"},
/* it */ {IDMS_LANGUAGE_POLISH_IT, "Polacco"},
/* nl */ {IDMS_LANGUAGE_POLISH_NL, "Pools"},
/* pl */ {IDMS_LANGUAGE_POLISH_PL, ""},  /* fuzzy */
/* sv */ {IDMS_LANGUAGE_POLISH_SV, "Polska"},

/* en */ {IDMS_LANGUAGE_SWEDISH,    "Swedish"},
/* de */ {IDMS_LANGUAGE_SWEDISH_DE, "Schwedisch"},
/* fr */ {IDMS_LANGUAGE_SWEDISH_FR, "Su�dois"},
/* it */ {IDMS_LANGUAGE_SWEDISH_IT, "Svedese"},
/* nl */ {IDMS_LANGUAGE_SWEDISH_NL, "Zweeds"},
/* pl */ {IDMS_LANGUAGE_SWEDISH_PL, ""},  /* fuzzy */
/* sv */ {IDMS_LANGUAGE_SWEDISH_SV, "Svenska"},

/* en */ {IDMS_LANGUAGE,    "Language"},
/* de */ {IDMS_LANGUAGE_DE, "Sprache"},
/* fr */ {IDMS_LANGUAGE_FR, "Langage"},
/* it */ {IDMS_LANGUAGE_IT, "Lingua"},
/* nl */ {IDMS_LANGUAGE_NL, "Taal"},
/* pl */ {IDMS_LANGUAGE_PL, ""},  /* fuzzy */
/* sv */ {IDMS_LANGUAGE_SV, "Spr�k"},

/* en */ {IDMS_FULLSCREEN_STATUSBAR,    "Fullscreen Statusbar"},
/* de */ {IDMS_FULLSCREEN_STATUSBAR_DE, ""},  /* fuzzy */
/* fr */ {IDMS_FULLSCREEN_STATUSBAR_FR, ""},  /* fuzzy */
/* it */ {IDMS_FULLSCREEN_STATUSBAR_IT, "Barra di stato a schermo intero"},
/* nl */ {IDMS_FULLSCREEN_STATUSBAR_NL, "Volscherm Statusbalk"},
/* pl */ {IDMS_FULLSCREEN_STATUSBAR_PL, ""},  /* fuzzy */
/* sv */ {IDMS_FULLSCREEN_STATUSBAR_SV, "Status rad vid fullsk�rm"},

/* en */ {IDMS_VIDEO_OVERLAY,    "Video overlay"},
/* de */ {IDMS_VIDEO_OVERLAY_DE, ""},  /* fuzzy */
/* fr */ {IDMS_VIDEO_OVERLAY_FR, ""},  /* fuzzy */
/* it */ {IDMS_VIDEO_OVERLAY_IT, "Video overlay"},
/* nl */ {IDMS_VIDEO_OVERLAY_NL, "Video overlay"},
/* pl */ {IDMS_VIDEO_OVERLAY_PL, ""},  /* fuzzy */
/* sv */ {IDMS_VIDEO_OVERLAY_SV, "Video overlay"},

/* ----------------------- AmigaOS Message/Error Strings ----------------------- */

/* en */ {IDMES_SETTINGS_SAVED_SUCCESS,    "Settings saved successfully."},
/* de */ {IDMES_SETTINGS_SAVED_SUCCESS_DE, "Einstellungen erfolgreich gespeichert."},
/* fr */ {IDMES_SETTINGS_SAVED_SUCCESS_FR, "Sauvegarde des param�tres effectu�e correctement."},
/* it */ {IDMES_SETTINGS_SAVED_SUCCESS_IT, "Impostazioni salvate con successo."},
/* nl */ {IDMES_SETTINGS_SAVED_SUCCESS_NL, "Instellingen met succes opgeslagen."},
/* pl */ {IDMES_SETTINGS_SAVED_SUCCESS_PL, ""},  /* fuzzy */
/* sv */ {IDMES_SETTINGS_SAVED_SUCCESS_SV, "Inst�llningarna har sparats."},

/* en */ {IDMES_SETTINGS_LOAD_SUCCESS,    "Settings loaded successfully."},
/* de */ {IDMES_SETTINGS_LOAD_SUCCESS_DE, "Einstellungen erfolgreich geladen."},
/* fr */ {IDMES_SETTINGS_LOAD_SUCCESS_FR, "Chargement des param�tres r�ussi."},
/* it */ {IDMES_SETTINGS_LOAD_SUCCESS_IT, "Impostazioni caricate con successo."},
/* nl */ {IDMES_SETTINGS_LOAD_SUCCESS_NL, "Instellingen met succes geladen."},
/* pl */ {IDMES_SETTINGS_LOAD_SUCCESS_PL, ""},  /* fuzzy */
/* sv */ {IDMES_SETTINGS_LOAD_SUCCESS_SV, "Inst�llningarna har l�sts in."},

/* en */ {IDMES_DFLT_SETTINGS_RESTORED,    "Default settings restored."},
/* de */ {IDMES_DFLT_SETTINGS_RESTORED_DE, "Standard wiederhergestellt."},
/* fr */ {IDMES_DFLT_SETTINGS_RESTORED_FR, "Param�tres par d�faut restaur�s."},
/* it */ {IDMES_DFLT_SETTINGS_RESTORED_IT, "Impostazioni predefinite ripristinate."},
/* nl */ {IDMES_DFLT_SETTINGS_RESTORED_NL, "Standaard instellingen hersteld."},
/* pl */ {IDMES_DFLT_SETTINGS_RESTORED_PL, ""},  /* fuzzy */
/* sv */ {IDMES_DFLT_SETTINGS_RESTORED_SV, "F�rvalda inst�llningar �terst�llda."},

/* en */ {IDMES_VICE_MESSAGE,    "VICE Message"},
/* de */ {IDMES_VICE_MESSAGE_DE, "VICE Nachricht"},
/* fr */ {IDMES_VICE_MESSAGE_FR, "Message VICE"},
/* it */ {IDMES_VICE_MESSAGE_IT, "Messaggio di VICE"},
/* nl */ {IDMES_VICE_MESSAGE_NL, "VICE Bericht"},
/* pl */ {IDMES_VICE_MESSAGE_PL, ""},  /* fuzzy */
/* sv */ {IDMES_VICE_MESSAGE_SV, "VICE-meddelande"},

/* en */ {IDMES_OK,    "OK"},
/* de */ {IDMES_OK_DE, "OK"},
/* fr */ {IDMES_OK_FR, "OK"},
/* it */ {IDMES_OK_IT, "OK"},
/* nl */ {IDMES_OK_NL, "OK"},
/* pl */ {IDMES_OK_PL, "OK"},
/* sv */ {IDMES_OK_SV, "OK"},

/* en */ {IDMES_VICE_CONTRIBUTORS,    "VICE contributors"},
/* de */ {IDMES_VICE_CONTRIBUTORS_DE, "VICE Hackers"},
/* fr */ {IDMES_VICE_CONTRIBUTORS_FR, "VICE Contributeurs"},
/* it */ {IDMES_VICE_CONTRIBUTORS_IT, "Collaboratori al progetto VICE"},
/* nl */ {IDMES_VICE_CONTRIBUTORS_NL, "Medewerkers aan het VICE project"},
/* pl */ {IDMES_VICE_CONTRIBUTORS_PL, ""},  /* fuzzy */
/* sv */ {IDMES_VICE_CONTRIBUTORS_SV, "VICE Bidragsl�mnare"},

/* en */ {IDMES_WHO_MADE_WHAT,    "Who made what?"},
/* de */ {IDMES_WHO_MADE_WHAT_DE, "Wer hat was gemacht?"},
/* fr */ {IDMES_WHO_MADE_WHAT_FR, "Qui fait quoi?"},
/* it */ {IDMES_WHO_MADE_WHAT_IT, "Chi ha fatto cosa?"},
/* nl */ {IDMES_WHO_MADE_WHAT_NL, "Wie heeft wat gemaakt?"},
/* pl */ {IDMES_WHO_MADE_WHAT_PL, ""},  /* fuzzy */
/* sv */ {IDMES_WHO_MADE_WHAT_SV, "Vem gjorde vad?"},

/* en */ {IDMES_VICE_DIST_NO_WARRANTY,    "VICE is distributed WITHOUT ANY WARRANTY!"},
/* de */ {IDMES_VICE_DIST_NO_WARRANTY_DE, "VICE ist ohne Garantie"},
/* fr */ {IDMES_VICE_DIST_NO_WARRANTY_FR, "VICE est distribu� SANS AUCUN GARANTIE!"},
/* it */ {IDMES_VICE_DIST_NO_WARRANTY_IT, "VICE � distribuito SENZA NESSUNA GARANZIA!"},
/* nl */ {IDMES_VICE_DIST_NO_WARRANTY_NL, "VICE heeft ABSOLUUT GEEN GARANTIE!"},
/* pl */ {IDMES_VICE_DIST_NO_WARRANTY_PL, ""},  /* fuzzy */
/* sv */ {IDMES_VICE_DIST_NO_WARRANTY_SV, "VICE distribueras UTAN N�GON SOM HELST GARANTI!"},

/* en */ {IDMES_WHICH_COMMANDS_AVAILABLE,    "Which command line options are available?"},
/* de */ {IDMES_WHICH_COMMANDS_AVAILABLE_DE, "Welche Kommandozeilen Parameter sind verf�gber?"},
/* fr */ {IDMES_WHICH_COMMANDS_AVAILABLE_FR, "Quelles sont les lignes de commandes disponibles?"},
/* it */ {IDMES_WHICH_COMMANDS_AVAILABLE_IT, "Quali parametri da riga di comando sono disponibili?"},
/* nl */ {IDMES_WHICH_COMMANDS_AVAILABLE_NL, "Welke commando opties zijn beschikbaar?"},
/* pl */ {IDMES_WHICH_COMMANDS_AVAILABLE_PL, ""},  /* fuzzy */
/* sv */ {IDMES_WHICH_COMMANDS_AVAILABLE_SV, "Vilka kommandoradsflaggor �r tillg�ngliga?"},

/* en */ {IDMES_CANNOT_SAVE_SETTINGS,    "Cannot save settings."},
/* de */ {IDMES_CANNOT_SAVE_SETTINGS_DE, "Fehler beim Speichern der Einstellungen."},
/* fr */ {IDMES_CANNOT_SAVE_SETTINGS_FR, "Impossible d'enregistrer les param�tres."},
/* it */ {IDMES_CANNOT_SAVE_SETTINGS_IT, "Impossibile salvare le impostazioni."},
/* nl */ {IDMES_CANNOT_SAVE_SETTINGS_NL, "Kan de instellingen niet opslaan."},
/* pl */ {IDMES_CANNOT_SAVE_SETTINGS_PL, ""},  /* fuzzy */
/* sv */ {IDMES_CANNOT_SAVE_SETTINGS_SV, "Kan inte spara inst�llningar."},

/* en */ {IDMES_CANNOT_LOAD_SETTINGS,    "Cannot load settings."},
/* de */ {IDMES_CANNOT_LOAD_SETTINGS_DE, "Fehler beim Laden der Einstellungen."},
/* fr */ {IDMES_CANNOT_LOAD_SETTINGS_FR, "Impossible de charger les param�tres."},
/* it */ {IDMES_CANNOT_LOAD_SETTINGS_IT, "Impossibile caricare le impostazioni."},
/* nl */ {IDMES_CANNOT_LOAD_SETTINGS_NL, "Kan de instellingen niet laden."},
/* pl */ {IDMES_CANNOT_LOAD_SETTINGS_PL, ""},  /* fuzzy */
/* sv */ {IDMES_CANNOT_LOAD_SETTINGS_SV, "Kan inte l�sa inst�llningar."},

/* en */ {IDMES_VICE_ERROR,    "VICE Error"},
/* de */ {IDMES_VICE_ERROR_DE, "VICE Fehler"},
/* fr */ {IDMES_VICE_ERROR_FR, "Erreur VICE"},
/* it */ {IDMES_VICE_ERROR_IT, "Errore di VICE"},
/* nl */ {IDMES_VICE_ERROR_NL, "Vice Fout"},
/* pl */ {IDMES_VICE_ERROR_PL, ""},  /* fuzzy */
/* sv */ {IDMES_VICE_ERROR_SV, "VICE-fel"},

/* en */ {IDMES_NO_JOY_ON_PORT_D,    "No joystick or joypad found on port %d"},
/* de */ {IDMES_NO_JOY_ON_PORT_D_DE, ""},  /* fuzzy */
/* fr */ {IDMES_NO_JOY_ON_PORT_D_FR, ""},  /* fuzzy */
/* it */ {IDMES_NO_JOY_ON_PORT_D_IT, "Nessun  joystick o joypad connesso alla porta %d"},
/* nl */ {IDMES_NO_JOY_ON_PORT_D_NL, "Geen joystick of joypad gevonden op poort %d"},
/* pl */ {IDMES_NO_JOY_ON_PORT_D_PL, ""},  /* fuzzy */
/* sv */ {IDMES_NO_JOY_ON_PORT_D_SV, "Ingen joystick eller joypad hittades p� port %d"},

/* en */ {IDMES_MOUSE_ON_PORT_D,    "A mouse was found on port %d"},
/* de */ {IDMES_MOUSE_ON_PORT_D_DE, ""},  /* fuzzy */
/* fr */ {IDMES_MOUSE_ON_PORT_D_FR, ""},  /* fuzzy */
/* it */ {IDMES_MOUSE_ON_PORT_D_IT, "Mouse rilevato sulla porta %d"},
/* nl */ {IDMES_MOUSE_ON_PORT_D_NL, "Een muis was gevonden op poort %d"},
/* pl */ {IDMES_MOUSE_ON_PORT_D_PL, ""},  /* fuzzy */
/* sv */ {IDMES_MOUSE_ON_PORT_D_SV, "En mus hittades p� port %d"},

/* en */ {IDMES_UNKNOWN_DEVICE_ON_PORT_D,    "Unknown device found on port %d"},
/* de */ {IDMES_UNKNOWN_DEVICE_ON_PORT_D_DE, ""},  /* fuzzy */
/* fr */ {IDMES_UNKNOWN_DEVICE_ON_PORT_D_FR, ""},  /* fuzzy */
/* it */ {IDMES_UNKNOWN_DEVICE_ON_PORT_D_IT, "Periferica sconosciuta rilevata sulla porta %d"},
/* nl */ {IDMES_UNKNOWN_DEVICE_ON_PORT_D_NL, "Een onbekent apparaat was gevonden op poort %d"},
/* pl */ {IDMES_UNKNOWN_DEVICE_ON_PORT_D_PL, ""},  /* fuzzy */
/* sv */ {IDMES_UNKNOWN_DEVICE_ON_PORT_D_SV, "En ok�nd enhet hittades p� port %d"},

/* en */ {IDMES_DEVICE_NOT_GAMEPAD,    "The device on this port is not a gamepad"},
/* de */ {IDMES_DEVICE_NOT_GAMEPAD_DE, ""},  /* fuzzy */
/* fr */ {IDMES_DEVICE_NOT_GAMEPAD_FR, ""},  /* fuzzy */
/* it */ {IDMES_DEVICE_NOT_GAMEPAD_IT, "La periferia connessa a questa porta non � un gamepad"},
/* nl */ {IDMES_DEVICE_NOT_GAMEPAD_NL, "Het apparaat op deze poort is niet een gamepad"},
/* pl */ {IDMES_DEVICE_NOT_GAMEPAD_PL, ""},  /* fuzzy */
/* sv */ {IDMES_DEVICE_NOT_GAMEPAD_SV, "Enheten p� denna port �r ingen gamepad"},

/* en */ {IDMES_NOT_MAPPED_TO_AMIGA_PORT,    "The port is not mapped to an Amiga port"},
/* de */ {IDMES_NOT_MAPPED_TO_AMIGA_PORT_DE, ""},  /* fuzzy */
/* fr */ {IDMES_NOT_MAPPED_TO_AMIGA_PORT_FR, ""},  /* fuzzy */
/* it */ {IDMES_NOT_MAPPED_TO_AMIGA_PORT_IT, "Questa porta non � mappata su una porta Amiga"},
/* nl */ {IDMES_NOT_MAPPED_TO_AMIGA_PORT_NL, "De poort is niet verbonden met een Amiga poort"},
/* pl */ {IDMES_NOT_MAPPED_TO_AMIGA_PORT_PL, ""},  /* fuzzy */
/* sv */ {IDMES_NOT_MAPPED_TO_AMIGA_PORT_SV, "Porten �r inte bunden till en Amiga port"},

/* en */ {IDMES_CANNOT_AUTOSTART_FILE,    "Cannot autostart specified file."},
/* de */ {IDMES_CANNOT_AUTOSTART_FILE_DE, "Autostart von angeforderter Datei fehlgeschlagen."},
/* fr */ {IDMES_CANNOT_AUTOSTART_FILE_FR, "Impossible de d�marrer automatiquement le fichier sp�cifi�"},
/* it */ {IDMES_CANNOT_AUTOSTART_FILE_IT, "Impossibile avviare automaticamente il file specificato."},
/* nl */ {IDMES_CANNOT_AUTOSTART_FILE_NL, "Kan opgegeven bestand niet autostarten."},
/* pl */ {IDMES_CANNOT_AUTOSTART_FILE_PL, ""},  /* fuzzy */
/* sv */ {IDMES_CANNOT_AUTOSTART_FILE_SV, "Kan inte autostarta angiven fil."},

/* en */ {IDMES_CANNOT_ATTACH_FILE,    "Cannot attach specified file"},
/* de */ {IDMES_CANNOT_ATTACH_FILE_DE, "Kann Image Datei nicht einlegen"},
/* fr */ {IDMES_CANNOT_ATTACH_FILE_FR, "Impossible d'attacher le fichier sp�cifi�"},
/* it */ {IDMES_CANNOT_ATTACH_FILE_IT, "Impossibile selezionare il file specificato"},
/* nl */ {IDMES_CANNOT_ATTACH_FILE_NL, "Kan het opgegeven bestand niet koppelen"},
/* pl */ {IDMES_CANNOT_ATTACH_FILE_PL, ""},  /* fuzzy */
/* sv */ {IDMES_CANNOT_ATTACH_FILE_SV, "Kan inte ansluta filen"},

/* en */ {IDMES_INVALID_CART,    "Invalid cartridge"},
/* de */ {IDMES_INVALID_CART_DE, "Ung�ltiges Erweiterungsmodul"},
/* fr */ {IDMES_INVALID_CART_FR, "Cartouche invalide"},
/* it */ {IDMES_INVALID_CART_IT, "Cartuccia non valida"},
/* nl */ {IDMES_INVALID_CART_NL, "Ongeldige cartridge"},
/* pl */ {IDMES_INVALID_CART_PL, ""},  /* fuzzy */
/* sv */ {IDMES_INVALID_CART_SV, "Ogiltig insticksmodul"},

/* en */ {IDMES_BAD_CART_CONFIG_IN_UI,    "Bad cartridge config in UI!"},
/* de */ {IDMES_BAD_CART_CONFIG_IN_UI_DE, "Fehlerhafte Modul Konfiguration im UI!"},
/* fr */ {IDMES_BAD_CART_CONFIG_IN_UI_FR, "Mauvaise configuration de la cartouche dans l'interface utilisateur!"},
/* it */ {IDMES_BAD_CART_CONFIG_IN_UI_IT, "Configurazione della cartuccia errata nell'IU!"},
/* nl */ {IDMES_BAD_CART_CONFIG_IN_UI_NL, "Slechte cartridge configuratie in UI!"},
/* pl */ {IDMES_BAD_CART_CONFIG_IN_UI_PL, ""},  /* fuzzy */
/* sv */ {IDMES_BAD_CART_CONFIG_IN_UI_SV, "Felaktiga insticksmodulsinst�llningar i UI!"},

/* en */ {IDMES_INVALID_CART_IMAGE,    "Invalid cartridge image"},
/* de */ {IDMES_INVALID_CART_IMAGE_DE, "Ung�ltiges Erweiterungsmodul Image"},
/* fr */ {IDMES_INVALID_CART_IMAGE_FR, "Image de cartouche invalide"},
/* it */ {IDMES_INVALID_CART_IMAGE_IT, "Immagine della cartuccia non valida"},
/* nl */ {IDMES_INVALID_CART_IMAGE_NL, "Ongeldig cartridge bestand"},
/* pl */ {IDMES_INVALID_CART_IMAGE_PL, ""},  /* fuzzy */
/* sv */ {IDMES_INVALID_CART_IMAGE_SV, "Ogiltig insticksmodulfil"},

/* en */ {IDMES_CANNOT_CREATE_IMAGE,    "Cannot create image"},
/* de */ {IDMES_CANNOT_CREATE_IMAGE_DE, "Kann Image Datei nicht erzeugen"},
/* fr */ {IDMES_CANNOT_CREATE_IMAGE_FR, "Impossible de cr�er l'image"},
/* it */ {IDMES_CANNOT_CREATE_IMAGE_IT, "Impossibile creare l'immagine"},
/* nl */ {IDMES_CANNOT_CREATE_IMAGE_NL, "Kan bestand niet maken"},
/* pl */ {IDMES_CANNOT_CREATE_IMAGE_PL, ""},  /* fuzzy */
/* sv */ {IDMES_CANNOT_CREATE_IMAGE_SV, "Kan inte skapa avbildningsfil"},

/* en */ {IDMES_ERROR_STARTING_SERVER,    "An error occured starting the server."},
/* de */ {IDMES_ERROR_STARTING_SERVER_DE, ""},  /* fuzzy */
/* fr */ {IDMES_ERROR_STARTING_SERVER_FR, ""},  /* fuzzy */
/* it */ {IDMES_ERROR_STARTING_SERVER_IT, "Errore durante l'avvio del server."},
/* nl */ {IDMES_ERROR_STARTING_SERVER_NL, "Er is een fout opgetreden bij het opstarten van de server"},
/* pl */ {IDMES_ERROR_STARTING_SERVER_PL, ""},  /* fuzzy */
/* sv */ {IDMES_ERROR_STARTING_SERVER_SV, "Ett fel intr�ffade n�r servern skulle startas"},

/* en */ {IDMES_ERROR_CONNECTING_CLIENT,    "An error occured connecting the client."},
/* de */ {IDMES_ERROR_CONNECTING_CLIENT_DE, ""},  /* fuzzy */
/* fr */ {IDMES_ERROR_CONNECTING_CLIENT_FR, ""},  /* fuzzy */
/* it */ {IDMES_ERROR_CONNECTING_CLIENT_IT, "Errore durante la connessione al client."},
/* nl */ {IDMES_ERROR_CONNECTING_CLIENT_NL, ""},  /* fuzzy */
/* pl */ {IDMES_ERROR_CONNECTING_CLIENT_PL, ""},  /* fuzzy */
/* sv */ {IDMES_ERROR_CONNECTING_CLIENT_SV, "Ett fel intr�ffade vid f�rs�k att kontakta klienten"},

/* en */ {IDMES_INVALID_PORT_NUMBER,    "Invalid port number"},
/* de */ {IDMES_INVALID_PORT_NUMBER_DE, "Ung�ltige Port Nummer"},
/* fr */ {IDMES_INVALID_PORT_NUMBER_FR, ""},  /* fuzzy */
/* it */ {IDMES_INVALID_PORT_NUMBER_IT, "Numero di porta non valido"},
/* nl */ {IDMES_INVALID_PORT_NUMBER_NL, "Ongeldig poort nummer"},
/* pl */ {IDMES_INVALID_PORT_NUMBER_PL, ""},  /* fuzzy */
/* sv */ {IDMES_INVALID_PORT_NUMBER_SV, "Ogiltigt portnummer"},

/* en */ {IDMES_THIS_MACHINE_NO_SID,    "This machine may not have a SID"},
/* de */ {IDMES_THIS_MACHINE_NO_SID_DE, "Diese Maschine hat m�glicherweise keinen SID"},
/* fr */ {IDMES_THIS_MACHINE_NO_SID_FR, "Cette machine n'a peut-�tre pas de SID"},
/* it */ {IDMES_THIS_MACHINE_NO_SID_IT, "Questa macchina potrebbe non avere un SID"},
/* nl */ {IDMES_THIS_MACHINE_NO_SID_NL, "Deze machine heeft mogelijk geen SID"},
/* pl */ {IDMES_THIS_MACHINE_NO_SID_PL, ""},  /* fuzzy */
/* sv */ {IDMES_THIS_MACHINE_NO_SID_SV, "Denna maskin kan inte ha en SID"},

/* en */ {IDMES_SCREENSHOT_SAVE_S_FAILED,    "Screenshot save of %s failed"},
/* de */ {IDMES_SCREENSHOT_SAVE_S_FAILED_DE, ""},  /* fuzzy */
/* fr */ {IDMES_SCREENSHOT_SAVE_S_FAILED_FR, ""},  /* fuzzy */
/* it */ {IDMES_SCREENSHOT_SAVE_S_FAILED_IT, "Salvataggio della schermata %s fallito"},
/* nl */ {IDMES_SCREENSHOT_SAVE_S_FAILED_NL, "Opslaan van schermafdruk %s niet gelukt."},
/* pl */ {IDMES_SCREENSHOT_SAVE_S_FAILED_PL, ""},  /* fuzzy */
/* sv */ {IDMES_SCREENSHOT_SAVE_S_FAILED_SV, "Fel vid skrivning av sk�rmdump \"%s\""},

/* en */ {IDMES_SCREENSHOT_S_SAVED,    "Screenshot %s saved."},
/* de */ {IDMES_SCREENSHOT_S_SAVED_DE, ""},  /* fuzzy */
/* fr */ {IDMES_SCREENSHOT_S_SAVED_FR, ""},  /* fuzzy */
/* it */ {IDMES_SCREENSHOT_S_SAVED_IT, "schermata %s salvata."},
/* nl */ {IDMES_SCREENSHOT_S_SAVED_NL, "Schermafdruk %s opgeslagen."},
/* pl */ {IDMES_SCREENSHOT_S_SAVED_PL, ""},  /* fuzzy */
/* sv */ {IDMES_SCREENSHOT_S_SAVED_SV, "Sk�rmdump \"%s\" sparad."},

/* ----------------------- AmigaOS Strings ----------------------- */

/* en */ {IDS_PRESS_KEY_BUTTON,    "Press desired key/button, move stick or press ESC for no key."},
/* de */ {IDS_PRESS_KEY_BUTTON_DE, ""},  /* fuzzy */
/* fr */ {IDS_PRESS_KEY_BUTTON_FR, ""},  /* fuzzy */
/* it */ {IDS_PRESS_KEY_BUTTON_IT, "Premi il tasto/pulsante desiderato, muovi il joy o premi ESC per proseguire."},
/* nl */ {IDS_PRESS_KEY_BUTTON_NL, "Druk de toets/knop, beweeg de joystick of druk op ESC voor geen toets."},
/* pl */ {IDS_PRESS_KEY_BUTTON_PL, ""},  /* fuzzy */
/* sv */ {IDS_PRESS_KEY_BUTTON_SV, "Tryck �nskad tangent/knapp, r�r joysticken eller tryck ESC f�r att inte v�lja n�gon knapp."},

/* en */ {IDS_SAVE,    "Save"},
/* de */ {IDS_SAVE_DE, "Speichern"},
/* fr */ {IDS_SAVE_FR, "Enregistrer"},
/* it */ {IDS_SAVE_IT, "Salva"},
/* nl */ {IDS_SAVE_NL, "Opslaan"},
/* pl */ {IDS_SAVE_PL, ""},  /* fuzzy */
/* sv */ {IDS_SAVE_SV, "Spara"},

/* en */ {IDS_SAVE_SCREENSHOT,    "Save Screenshot"},
/* de */ {IDS_SAVE_SCREENSHOT_DE, "Screenshot Speichern"},
/* fr */ {IDS_SAVE_SCREENSHOT_FR, "Enregistrer une capture d'�cran"},
/* it */ {IDS_SAVE_SCREENSHOT_IT, "Salva schermata"},
/* nl */ {IDS_SAVE_SCREENSHOT_NL, "Scherm afdruk opslaan"},
/* pl */ {IDS_SAVE_SCREENSHOT_PL, ""},  /* fuzzy */
/* sv */ {IDS_SAVE_SCREENSHOT_SV, "Spara sk�rmdump"},

/* en */ {IDS_CANCEL,    "Cancel"},
/* de */ {IDS_CANCEL_DE, "Abbruch"},
/* fr */ {IDS_CANCEL_FR, "Annuler"},
/* it */ {IDS_CANCEL_IT, "Annulla"},
/* nl */ {IDS_CANCEL_NL, "Annuleren"},
/* pl */ {IDS_CANCEL_PL, ""},  /* fuzzy */
/* sv */ {IDS_CANCEL_SV, "Avbryt"},

/* en */ {IDS_CHOOSE_SCREENSHOT_FORMAT,    "Choose screenshot format"},
/* de */ {IDS_CHOOSE_SCREENSHOT_FORMAT_DE, ""},  /* fuzzy */
/* fr */ {IDS_CHOOSE_SCREENSHOT_FORMAT_FR, ""},  /* fuzzy */
/* it */ {IDS_CHOOSE_SCREENSHOT_FORMAT_IT, "Scegli il formato per la schermata"},
/* nl */ {IDS_CHOOSE_SCREENSHOT_FORMAT_NL, "Kies schermafdruk formaat"},
/* pl */ {IDS_CHOOSE_SCREENSHOT_FORMAT_PL, ""},  /* fuzzy */
/* sv */ {IDS_CHOOSE_SCREENSHOT_FORMAT_SV, "V�lj �nskat format f�r sk�rmdumpen"},

/* en */ {IDS_SAVE_S_SCREENSHOT,    "Save %s screenshot"},
/* de */ {IDS_SAVE_S_SCREENSHOT_DE, ""},  /* fuzzy */
/* fr */ {IDS_SAVE_S_SCREENSHOT_FR, ""},  /* fuzzy */
/* it */ {IDS_SAVE_S_SCREENSHOT_IT, "Salva schermata in formato %s"},
/* nl */ {IDS_SAVE_S_SCREENSHOT_NL, "Sla schermafdruk %s op"},
/* pl */ {IDS_SAVE_S_SCREENSHOT_PL, ""},  /* fuzzy */
/* sv */ {IDS_SAVE_S_SCREENSHOT_SV, "Spara sk�rmdumpen i %s format"},

/* en */ {IDS_DETACHED_DEVICE_D,    "Detached device %d"},
/* de */ {IDS_DETACHED_DEVICE_D_DE, "Ger�t %d entfernt"},
/* fr */ {IDS_DETACHED_DEVICE_D_FR, "P�riph�rique d�tach� %d"},
/* it */ {IDS_DETACHED_DEVICE_D_IT, "Periferica %d rimossa"},
/* nl */ {IDS_DETACHED_DEVICE_D_NL, "Apparaat %d ontkoppelt"},
/* pl */ {IDS_DETACHED_DEVICE_D_PL, ""},  /* fuzzy */
/* sv */ {IDS_DETACHED_DEVICE_D_SV, "Kopplade fran enhet %d"},

/* en */ {IDS_ATTACHED_S_TO_DEVICE_D,    "Attached %s to device#%d"},
/* de */ {IDS_ATTACHED_S_TO_DEVICE_D_DE, "%s mit Ger�t #%d verbunden"},
/* fr */ {IDS_ATTACHED_S_TO_DEVICE_D_FR, "%s attach� au p�riph�rique #%s"},
/* it */ {IDS_ATTACHED_S_TO_DEVICE_D_IT, "Selezionato %s per la periferica #%d"},
/* nl */ {IDS_ATTACHED_S_TO_DEVICE_D_NL, "%s aan apparaat#%d gekoppelt"},
/* pl */ {IDS_ATTACHED_S_TO_DEVICE_D_PL, ""},  /* fuzzy */
/* sv */ {IDS_ATTACHED_S_TO_DEVICE_D_SV, "Ansl�t %s till enhet %d"},

/* en */ {IDS_VICE_QUESTION,    "VICE Question"},
/* de */ {IDS_VICE_QUESTION_DE, "VICE Frage"},
/* fr */ {IDS_VICE_QUESTION_FR, "Question VICE"},
/* it */ {IDS_VICE_QUESTION_IT, "Domanda di VICE"},
/* nl */ {IDS_VICE_QUESTION_NL, "VICE vraag"},
/* pl */ {IDS_VICE_QUESTION_PL, ""},  /* fuzzy */
/* sv */ {IDS_VICE_QUESTION_SV, "VICE-fraga"},

/* en */ {IDS_EXTEND_TO_40_TRACK,    "Extend image to 40-track format?"},
/* de */ {IDS_EXTEND_TO_40_TRACK_DE, "Image Dateu auf 40-Spur Format erweitern?"},
/* fr */ {IDS_EXTEND_TO_40_TRACK_FR, "�tendre l'image au format 40 pistes?"},
/* it */ {IDS_EXTEND_TO_40_TRACK_IT, "Astendere l'immagine al formato 40 tracce?"},
/* nl */ {IDS_EXTEND_TO_40_TRACK_NL, "Bestand uitbreiden naar 40-sporen formaat?"},
/* pl */ {IDS_EXTEND_TO_40_TRACK_PL, ""},  /* fuzzy */
/* sv */ {IDS_EXTEND_TO_40_TRACK_SV, "Ut�ka avbildning till 40-sparsformat?"},

/* en */ {IDS_YES_NO,    "Yes|No"},
/* de */ {IDS_YES_NO_DE, ""},  /* fuzzy */
/* fr */ {IDS_YES_NO_FR, ""},  /* fuzzy */
/* it */ {IDS_YES_NO_IT, "Si|No"},
/* nl */ {IDS_YES_NO_NL, "Ja|Nee"},
/* pl */ {IDS_YES_NO_PL, ""},  /* fuzzy */
/* sv */ {IDS_YES_NO_SV, "Ja|Nej"},

/* en */ {IDS_DETACHED_TAPE,    "Detached tape"},
/* de */ {IDS_DETACHED_TAPE_DE, "Band Image entfernt"},
/* fr */ {IDS_DETACHED_TAPE_FR, "Datassette d�tach�"},
/* it */ {IDS_DETACHED_TAPE_IT, "Cassetta rimossa"},
/* nl */ {IDS_DETACHED_TAPE_NL, "Tape ontkoppelt"},
/* pl */ {IDS_DETACHED_TAPE_PL, ""},  /* fuzzy */
/* sv */ {IDS_DETACHED_TAPE_SV, "Kopplade fran band"},

/* en */ {IDS_ATTACHED_TAPE_S,    "Attached tape %s"},
/* de */ {IDS_ATTACHED_TAPE_S_DE, "Band Image %s eingelegt"},
/* fr */ {IDS_ATTACHED_TAPE_S_FR, "Datassette %s attach�"},
/* it */ {IDS_ATTACHED_TAPE_S_IT, "Cassetta %s selezionata"},
/* nl */ {IDS_ATTACHED_TAPE_S_NL, "Tape %s gekoppelt"},
/* pl */ {IDS_ATTACHED_TAPE_S_PL, ""},  /* fuzzy */
/* sv */ {IDS_ATTACHED_TAPE_S_SV, "Ansl�t band %s"},

/* en */ {IDS_SELECT_START_SNAPSHOT,    "Select start snapshot for event history"},
/* de */ {IDS_SELECT_START_SNAPSHOT_DE, "Startsnapshot f�r Ereignishistory festlegen"},
/* fr */ {IDS_SELECT_START_SNAPSHOT_FR, "S�lectionner la sauvegarde de d�part pour l'historique des �v�nements"},
/* it */ {IDS_SELECT_START_SNAPSHOT_IT, "Seleziona lo snapshot iniziale per la cronologia degli eventi"},
/* nl */ {IDS_SELECT_START_SNAPSHOT_NL, "Selecteer start momentopname voor gebeurtenis geschiedenis"},
/* pl */ {IDS_SELECT_START_SNAPSHOT_PL, ""},  /* fuzzy */
/* sv */ {IDS_SELECT_START_SNAPSHOT_SV, "V�lj start�gonblicksmodul f�r h�ndelsehistorik"},

/* en */ {IDS_SELECT_END_SNAPSHOT,    "Select end snapshot for event history"},
/* de */ {IDS_SELECT_END_SNAPSHOT_DE, ""},  /* fuzzy */
/* fr */ {IDS_SELECT_END_SNAPSHOT_FR, ""},  /* fuzzy */
/* it */ {IDS_SELECT_END_SNAPSHOT_IT, "Seleziona lo snapshot finale per la cronologia degli eventi"},
/* nl */ {IDS_SELECT_END_SNAPSHOT_NL, "Selecteer eind momentopname voor gebeurtenis geschiedenis"},
/* pl */ {IDS_SELECT_END_SNAPSHOT_PL, ""},  /* fuzzy */
/* sv */ {IDS_SELECT_END_SNAPSHOT_SV, "V�lj slut�gonblicksbild f�r h�ndelsehistorik"},

/* en */ {IDS_REALLY_EXIT,    "Do you really want to exit?\n\nAll the data present in the emulated RAM will be lost."},
/* de */ {IDS_REALLY_EXIT_DE, "Willst Du wirklich beenden?\n\nDaten im emulierten Speicher RAM gehen verloren."},
/* fr */ {IDS_REALLY_EXIT_FR, "D�sirez-vous vraiment quitter?\n\nToutes les donn�es pr�sentes dans la m�moire de l'�mulateur seront perdues."},
/* it */ {IDS_REALLY_EXIT_IT, "Uscire davvero?\n\nTutti i dati presenti nella RAM emulata saranno persi."},
/* nl */ {IDS_REALLY_EXIT_NL, "Wilt U echt afsluiten?\n\nAlle data in ge�muleerd geheugen zal verloren gaan."},
/* pl */ {IDS_REALLY_EXIT_PL, ""},  /* fuzzy */
/* sv */ {IDS_REALLY_EXIT_SV, "Vill du verkligen avsluta?\n\nAlla data i emulerat RAM kommer f�rloras."},

/* en */ {IDS_ATTACH_CRT_IMAGE,    "Attach CRT cartridge image"},
/* de */ {IDS_ATTACH_CRT_IMAGE_DE, "CRT Erweiterungsmodul Image einlegen"},
/* fr */ {IDS_ATTACH_CRT_IMAGE_FR, "Ins�rer une cartouche CRT"},
/* it */ {IDS_ATTACH_CRT_IMAGE_IT, "Seleziona l'immagine di una cartuccia CRT"},
/* nl */ {IDS_ATTACH_CRT_IMAGE_NL, "Koppel CRT cartridge bestand"},
/* pl */ {IDS_ATTACH_CRT_IMAGE_PL, ""},  /* fuzzy */
/* sv */ {IDS_ATTACH_CRT_IMAGE_SV, "Anslut CRT-insticksmodulfil"},

/* en */ {IDS_ATTACH_RAW_8KB_IMAGE,    "Attach raw 8KB cartridge image"},
/* de */ {IDS_ATTACH_RAW_8KB_IMAGE_DE, "8KB Erweiterungsmodul (raw) Image einlegen"},
/* fr */ {IDS_ATTACH_RAW_8KB_IMAGE_FR, "Ins�rer une cartouche 8KO"},
/* it */ {IDS_ATTACH_RAW_8KB_IMAGE_IT, "Seleziona l'immagine di una cartuccia da 8KB"},
/* nl */ {IDS_ATTACH_RAW_8KB_IMAGE_NL, "Koppel binair 8KB cartridge bestand"},
/* pl */ {IDS_ATTACH_RAW_8KB_IMAGE_PL, ""},  /* fuzzy */
/* sv */ {IDS_ATTACH_RAW_8KB_IMAGE_SV, "Anslut ra 8KB-insticksmodulavbilding"},

/* en */ {IDS_ATTACH_RAW_16KB_IMAGE,    "Attach raw 16KB cartridge image"},
/* de */ {IDS_ATTACH_RAW_16KB_IMAGE_DE, "16KB Erweiterungsmodul (raw) Image einlegen"},
/* fr */ {IDS_ATTACH_RAW_16KB_IMAGE_FR, "Ins�rer une cartouche 16KO"},
/* it */ {IDS_ATTACH_RAW_16KB_IMAGE_IT, "Seleziona l'immagine di una cartuccia da 16KB"},
/* nl */ {IDS_ATTACH_RAW_16KB_IMAGE_NL, "Koppel binair 16KB cartridge bestand"},
/* pl */ {IDS_ATTACH_RAW_16KB_IMAGE_PL, ""},  /* fuzzy */
/* sv */ {IDS_ATTACH_RAW_16KB_IMAGE_SV, "Anslut ra 16KB-insticksmodulavbilding"},

/* en */ {IDS_ATTACH_ACTION_REPLAY_IMAGE,    "Attach Action Replay cartridge image"},
/* de */ {IDS_ATTACH_ACTION_REPLAY_IMAGE_DE, "Action Replay Erweiterungsmodul Image einlegen"},
/* fr */ {IDS_ATTACH_ACTION_REPLAY_IMAGE_FR, "Ins�rer une cartouche Action Replay"},
/* it */ {IDS_ATTACH_ACTION_REPLAY_IMAGE_IT, "Seleziona l'immagine di una cartuccia Action Replay"},
/* nl */ {IDS_ATTACH_ACTION_REPLAY_IMAGE_NL, "Koppel Action Replay cartridge bestand"},
/* pl */ {IDS_ATTACH_ACTION_REPLAY_IMAGE_PL, ""},  /* fuzzy */
/* sv */ {IDS_ATTACH_ACTION_REPLAY_IMAGE_SV, "Anslut Action Replay-avbildningsfil"},

/* en */ {IDS_ATTACH_ATOMIC_POWER_IMAGE,    "Attach Atomic Power cartridge image"},
/* de */ {IDS_ATTACH_ATOMIC_POWER_IMAGE_DE, "Atomic Power Erweiterungsmodul Image einlegen"},
/* fr */ {IDS_ATTACH_ATOMIC_POWER_IMAGE_FR, "Ins�rer une cartouche Atomic Power"},
/* it */ {IDS_ATTACH_ATOMIC_POWER_IMAGE_IT, "Seleziona l'immagine di una cartuccia Atomic Power"},
/* nl */ {IDS_ATTACH_ATOMIC_POWER_IMAGE_NL, "Koppel Atomic Power cartridge bestand"},
/* pl */ {IDS_ATTACH_ATOMIC_POWER_IMAGE_PL, ""},  /* fuzzy */
/* sv */ {IDS_ATTACH_ATOMIC_POWER_IMAGE_SV, "Anslut Atomic Power-avbildningsfil"},

/* en */ {IDS_ATTACH_EPYX_FASTLOAD_IMAGE,    "Attach Epyx fastload cartridge image"},
/* de */ {IDS_ATTACH_EPYX_FASTLOAD_IMAGE_DE, "Epyx Fastload Erweiterungsmodul Image einlegen"},
/* fr */ {IDS_ATTACH_EPYX_FASTLOAD_IMAGE_FR, "Ins�rer une cartouche Epyx FastLoad"},
/* it */ {IDS_ATTACH_EPYX_FASTLOAD_IMAGE_IT, "Seleziona l'immagine di una cartuccia Epyx fastload"},
/* nl */ {IDS_ATTACH_EPYX_FASTLOAD_IMAGE_NL, "Koppel Epyx fastload cartridge bestand"},
/* pl */ {IDS_ATTACH_EPYX_FASTLOAD_IMAGE_PL, ""},  /* fuzzy */
/* sv */ {IDS_ATTACH_EPYX_FASTLOAD_IMAGE_SV, "Anslut Epyx fastload-avbildningsfil"},

/* en */ {IDS_ATTACH_IEEE488_IMAGE,    "Attach IEEE interface cartridge image"},
/* de */ {IDS_ATTACH_IEEE488_IMAGE_DE, "IEEE Schnittstellen Erweiterungsmodul Image einlegen"},
/* fr */ {IDS_ATTACH_IEEE488_IMAGE_FR, "Ins�rer une cartouche interface IEEE"},
/* it */ {IDS_ATTACH_IEEE488_IMAGE_IT, "Seleziona l'immagine di una cartuccia di interfaccia IEEE"},
/* nl */ {IDS_ATTACH_IEEE488_IMAGE_NL, "Koppel IEEE interface cartridge bestand"},
/* pl */ {IDS_ATTACH_IEEE488_IMAGE_PL, ""},  /* fuzzy */
/* sv */ {IDS_ATTACH_IEEE488_IMAGE_SV, "Anslut IEE-gr�nssnittsavbildningsfil"},

/* en */ {IDS_ATTACH_RETRO_REPLAY_IMAGE,    "Attach Retro Replay cartridge image"},
/* de */ {IDS_ATTACH_RETRO_REPLAY_IMAGE_DE, "Retro Replay Erweiterungsmodul Image einlegen"},
/* fr */ {IDS_ATTACH_RETRO_REPLAY_IMAGE_FR, "Ins�rer une cartouche Retro Replay"},
/* it */ {IDS_ATTACH_RETRO_REPLAY_IMAGE_IT, "Seleziona l'immagine di una cartuccia Retro Replay"},
/* nl */ {IDS_ATTACH_RETRO_REPLAY_IMAGE_NL, "Koppel Retro Replay cartridge bestand"},
/* pl */ {IDS_ATTACH_RETRO_REPLAY_IMAGE_PL, ""},  /* fuzzy */
/* sv */ {IDS_ATTACH_RETRO_REPLAY_IMAGE_SV, "Anslut Retro Replay-avbildningsfil"},

/* en */ {IDS_ATTACH_IDE64_IMAGE,    "Attach IDE64 interface cartridge image"},
/* de */ {IDS_ATTACH_IDE64_IMAGE_DE, "IDE64 Schnittstellen Erweiterungsmodul Image einlegen"},
/* fr */ {IDS_ATTACH_IDE64_IMAGE_FR, "Ins�rer une cartouche interface IDE64"},
/* it */ {IDS_ATTACH_IDE64_IMAGE_IT, "Seleziona l'immagine di una cartuccia di interfaccia IDE64"},
/* nl */ {IDS_ATTACH_IDE64_IMAGE_NL, "Koppel IDE64 interface cartridge bestand"},
/* pl */ {IDS_ATTACH_IDE64_IMAGE_PL, ""},  /* fuzzy */
/* sv */ {IDS_ATTACH_IDE64_IMAGE_SV, "Anslut IDE64-gr�nssnittsavbildningsfil"},

/* en */ {IDS_ATTACH_SS4_IMAGE,    "Attach Super Snapshot 4 cartridge image"},
/* de */ {IDS_ATTACH_SS4_IMAGE_DE, "Super Snapshot 4 Erweiterungsmodul Image einlegen"},
/* fr */ {IDS_ATTACH_SS4_IMAGE_FR, "Ins�rer une cartouche Super Snapshot 4"},
/* it */ {IDS_ATTACH_SS4_IMAGE_IT, "Seleziona l'immagine di una cartuccia Super Snapshot 4"},
/* nl */ {IDS_ATTACH_SS4_IMAGE_NL, "Koppel Super Snapshot 4 cartridge bestand"},
/* pl */ {IDS_ATTACH_SS4_IMAGE_PL, ""},  /* fuzzy */
/* sv */ {IDS_ATTACH_SS4_IMAGE_SV, "Anslut Super Snapshot 4-avbildningsfil"},

/* en */ {IDS_ATTACH_SS5_IMAGE,    "Attach Super Snapshot 5 cartridge image"},
/* de */ {IDS_ATTACH_SS5_IMAGE_DE, "Super Snapshot 5 Erweiterungsmodul Image einlegen"},
/* fr */ {IDS_ATTACH_SS5_IMAGE_FR, "Ins�rer une cartouche Super Snapshot 5"},
/* it */ {IDS_ATTACH_SS5_IMAGE_IT, "Seleziona l'immagine di una cartuccia Super Snapshot 5"},
/* nl */ {IDS_ATTACH_SS5_IMAGE_NL, "Koppel Super Snapshot 5 cartridge bestand"},
/* pl */ {IDS_ATTACH_SS5_IMAGE_PL, ""},  /* fuzzy */
/* sv */ {IDS_ATTACH_SS5_IMAGE_SV, "Anslut Super Snapshot 5-avbildningsfil"},

/* en */ {IDS_ATTACH_SB_IMAGE,    "Attach Structured Basic cartridge image"},
/* de */ {IDS_ATTACH_SB_IMAGE_DE, "Erweiterungsmodul Structured Basic einlegen"},
/* fr */ {IDS_ATTACH_SB_IMAGE_FR, ""},  /* fuzzy */
/* it */ {IDS_ATTACH_SB_IMAGE_IT, "Seleziona l'immagine di una cartuccia Structured Basic"},
/* nl */ {IDS_ATTACH_SB_IMAGE_NL, "Koppel Structured Basic cartridge bestand"},
/* pl */ {IDS_ATTACH_SB_IMAGE_PL, ""},  /* fuzzy */
/* sv */ {IDS_ATTACH_SB_IMAGE_SV, "Anslut Structured Basic insticksavbildningsfil"},

/* en */ {IDS_AVAILABLE_CMDLINE_OPTIONS,    "\nAvailable command-line options:\n\n"},
/* de */ {IDS_AVAILABLE_CMDLINE_OPTIONS_DE, "\nZul�ssige Kommandozeilen Optionen:\n\n"},
/* fr */ {IDS_AVAILABLE_CMDLINE_OPTIONS_FR, "\nOptions de ligne de commande disponibles:\n\n"},
/* it */ {IDS_AVAILABLE_CMDLINE_OPTIONS_IT, "\nOpzioni disponibili da riga di comando:\n\n"},
/* nl */ {IDS_AVAILABLE_CMDLINE_OPTIONS_NL, "\nBeschikbare command-line opties:\n\n"},
/* pl */ {IDS_AVAILABLE_CMDLINE_OPTIONS_PL, "\nTillg�ngliga kommandoradsflaggor:\n\n"},
/* sv */ {IDS_AVAILABLE_CMDLINE_OPTIONS_SV, "\nTillg�ngliga kommandoradsargument:\n\n"},

/* en */ {IDS_ATTACH_4_8_16KB_AT_2000,    "Attach 4/8/16KB cartridge image at $2000"},
/* de */ {IDS_ATTACH_4_8_16KB_AT_2000_DE, "4/8/16KB Modul Image bei $2000 einlegen"},
/* fr */ {IDS_ATTACH_4_8_16KB_AT_2000_FR, "Insertion d'une image 4/8/16Ko   $2000"},
/* it */ {IDS_ATTACH_4_8_16KB_AT_2000_IT, "Seleziona l'immagine di una cartuccia da 4/8/16KB a $2000"},
/* nl */ {IDS_ATTACH_4_8_16KB_AT_2000_NL, "Koppel 4/8/16KB bestand op $2000"},
/* pl */ {IDS_ATTACH_4_8_16KB_AT_2000_PL, ""},  /* fuzzy */
/* sv */ {IDS_ATTACH_4_8_16KB_AT_2000_SV, "Anslut 4/8/16KB-modulfil vid $2000"},

/* en */ {IDS_ATTACH_4_8_16KB_AT_4000,    "Attach 4/8/16KB cartridge image at $4000"},
/* de */ {IDS_ATTACH_4_8_16KB_AT_4000_DE, "4/8/16KB Modul Image bei $4000 einlegen"},
/* fr */ {IDS_ATTACH_4_8_16KB_AT_4000_FR, "Insertion d'une image 4/8/16Ko   $4000"},
/* it */ {IDS_ATTACH_4_8_16KB_AT_4000_IT, "Seleziona l'immagine di una cartuccia da 4/8/16KB a $4000"},
/* nl */ {IDS_ATTACH_4_8_16KB_AT_4000_NL, "Koppel 4/8/16KB bestand op $4000"},
/* pl */ {IDS_ATTACH_4_8_16KB_AT_4000_PL, ""},  /* fuzzy */
/* sv */ {IDS_ATTACH_4_8_16KB_AT_4000_SV, "Anslut 4/8/16KB-modulfil vid $4000"},

/* en */ {IDS_ATTACH_4_8_16KB_AT_6000,    "Attach 4/8/16KB cartridge image at $6000"},
/* de */ {IDS_ATTACH_4_8_16KB_AT_6000_DE, "4/8/16KB Modul Image bei $6000 einlegen"},
/* fr */ {IDS_ATTACH_4_8_16KB_AT_6000_FR, "Insertion d'une image 4/8/16Ko   $6000"},
/* it */ {IDS_ATTACH_4_8_16KB_AT_6000_IT, "Seleziona l'immagine di una cartuccia da 4/8/16KB a $6000"},
/* nl */ {IDS_ATTACH_4_8_16KB_AT_6000_NL, "Koppel 4/8/16KB bestand op $6000"},
/* pl */ {IDS_ATTACH_4_8_16KB_AT_6000_PL, ""},  /* fuzzy */
/* sv */ {IDS_ATTACH_4_8_16KB_AT_6000_SV, "Anslut 4/8/16KB-modulfil vid $6000"},

/* en */ {IDS_ATTACH_4_8KB_AT_A000,    "Attach 4/8KB cartridge image at $A000"},
/* de */ {IDS_ATTACH_4_8KB_AT_A000_DE, "4/8KB Modul Image bei $A000 einlegen"},
/* fr */ {IDS_ATTACH_4_8KB_AT_A000_FR, "Insertion d'une image 4/8Ko   $A000"},
/* it */ {IDS_ATTACH_4_8KB_AT_A000_IT, "Seleziona l'immagine di una cartuccia da 4/8KB a $A000"},
/* nl */ {IDS_ATTACH_4_8KB_AT_A000_NL, "Koppel 4/8KB bestand op $A000"},
/* pl */ {IDS_ATTACH_4_8KB_AT_A000_PL, ""},  /* fuzzy */
/* sv */ {IDS_ATTACH_4_8KB_AT_A000_SV, "Anslut 4/8KB-modulfil vid $A000"},

/* en */ {IDS_ATTACH_4KB_AT_B000,    "Attach 4KB cartridge image at $B000"},
/* de */ {IDS_ATTACH_4KB_AT_B000_DE, "4KB Modul Image bei $B000 einlegen"},
/* fr */ {IDS_ATTACH_4KB_AT_B000_FR, "Insertion d'une image 4Ko   $B000"},
/* it */ {IDS_ATTACH_4KB_AT_B000_IT, "Seleziona l'immagine di una cartuccia da 4KB a $B000"},
/* nl */ {IDS_ATTACH_4KB_AT_B000_NL, "Koppel 4KB bestand op $B000"},
/* pl */ {IDS_ATTACH_4KB_AT_B000_PL, ""},  /* fuzzy */
/* sv */ {IDS_ATTACH_4KB_AT_B000_SV, "Anslut 4KB-modulfil vid $B000"},

/* en */ {IDS_S_AT_D_SPEED,    "%s at %d%% speed, %d fps%s"},
/* de */ {IDS_S_AT_D_SPEED_DE, "%s bei %d%% Geschwindigkeit, %d fps%s"},
/* fr */ {IDS_S_AT_D_SPEED_FR, "%s   une vitesse de %d%%,   %d img/s%s"},
/* it */ {IDS_S_AT_D_SPEED_IT, "%s velocit� %d%%, %d fps%s"},
/* nl */ {IDS_S_AT_D_SPEED_NL, "%s met %d%% snelheid, %d fps%s"},
/* pl */ {IDS_S_AT_D_SPEED_PL, ""},  /* fuzzy */
/* sv */ {IDS_S_AT_D_SPEED_SV, "%s med %d%% hastighet, %d b/s%s"},

/* en */ {IDS_ATTACH,    "Attach"},
/* de */ {IDS_ATTACH_DE, ""},  /* fuzzy */
/* fr */ {IDS_ATTACH_FR, ""},  /* fuzzy */
/* it */ {IDS_ATTACH_IT, "Seleziona"},
/* nl */ {IDS_ATTACH_NL, "Koppel"},
/* pl */ {IDS_ATTACH_PL, ""},  /* fuzzy */
/* sv */ {IDS_ATTACH_SV, "Anslut"},

/* en */ {IDS_PARENT,    "Parent"},
/* de */ {IDS_PARENT_DE, ""},  /* fuzzy */
/* fr */ {IDS_PARENT_FR, ""},  /* fuzzy */
/* it */ {IDS_PARENT_IT, "Livello superiore"},
/* nl */ {IDS_PARENT_NL, "Bovenliggende directory"},
/* pl */ {IDS_PARENT_PL, ""},  /* fuzzy */
/* sv */ {IDS_PARENT_SV, "Moderl�da"},

/* en */ {IDS_ATTACH_READ_ONLY,    "Attach read only"},
/* de */ {IDS_ATTACH_READ_ONLY_DE, "Schreibgesch�tzt einlegen"},
/* fr */ {IDS_ATTACH_READ_ONLY_FR, "Ins�rer en lecture seule"},
/* it */ {IDS_ATTACH_READ_ONLY_IT, "Seleziona in sola lettura"},
/* nl */ {IDS_ATTACH_READ_ONLY_NL, "Alleen lezen"},
/* pl */ {IDS_ATTACH_READ_ONLY_PL, ""},  /* fuzzy */
/* sv */ {IDS_ATTACH_READ_ONLY_SV, "Anslut skrivskyddat"},

/* en */ {IDS_NAME,    "Name"},
/* de */ {IDS_NAME_DE, "Name"},
/* fr */ {IDS_NAME_FR, "Nom"},
/* it */ {IDS_NAME_IT, "Nome"},
/* nl */ {IDS_NAME_NL, "Naam"},
/* pl */ {IDS_NAME_PL, ""},  /* fuzzy */
/* sv */ {IDS_NAME_SV, "Namn"},

/* en */ {IDS_CREATE_IMAGE,    "Create Image"},
/* de */ {IDS_CREATE_IMAGE_DE, "Image Datei erzeugen"},
/* fr */ {IDS_CREATE_IMAGE_FR, "Cr�ation d'image"},
/* it */ {IDS_CREATE_IMAGE_IT, "Crea immagine"},
/* nl */ {IDS_CREATE_IMAGE_NL, "Maak Bestand"},
/* pl */ {IDS_CREATE_IMAGE_PL, ""},  /* fuzzy */
/* sv */ {IDS_CREATE_IMAGE_SV, "Skapa avbildning"},

/* en */ {IDS_NEW_IMAGE,    "New Image"},
/* de */ {IDS_NEW_IMAGE_DE, "Neue Image Datei"},
/* fr */ {IDS_NEW_IMAGE_FR, "Nouvelle image"},
/* it */ {IDS_NEW_IMAGE_IT, "Nuova immagine"},
/* nl */ {IDS_NEW_IMAGE_NL, "Nieuw Bestand"},
/* pl */ {IDS_NEW_IMAGE_PL, ""},  /* fuzzy */
/* sv */ {IDS_NEW_IMAGE_SV, "Ny avbildning"},

/* en */ {IDS_NEW_TAP_IMAGE,    "New TAP Image"},
/* de */ {IDS_NEW_TAP_IMAGE_DE, "Neue TAP Image Datei"},
/* fr */ {IDS_NEW_TAP_IMAGE_FR, "Nouvelle image TAP"},
/* it */ {IDS_NEW_TAP_IMAGE_IT, "Nuova immagine TAP"},
/* nl */ {IDS_NEW_TAP_IMAGE_NL, "Nieuw TAP Bestand"},
/* pl */ {IDS_NEW_TAP_IMAGE_PL, ""},  /* fuzzy */
/* sv */ {IDS_NEW_TAP_IMAGE_SV, "Ny TAP-avbildning"},

/* en */ {IDS_OVERWRITE_EXISTING_IMAGE,    "Overwrite existing image?"},
/* de */ {IDS_OVERWRITE_EXISTING_IMAGE_DE, "Existierende Datei �berschreiben?"},
/* fr */ {IDS_OVERWRITE_EXISTING_IMAGE_FR, "�craser les images existantes?"},
/* it */ {IDS_OVERWRITE_EXISTING_IMAGE_IT, "Sovrascrivo immagine esistente?"},
/* nl */ {IDS_OVERWRITE_EXISTING_IMAGE_NL, "Bestaand bestand overschrijven?"},
/* pl */ {IDS_OVERWRITE_EXISTING_IMAGE_PL, ""},  /* fuzzy */
/* sv */ {IDS_OVERWRITE_EXISTING_IMAGE_SV, "Skriva �ver existerande avbildning?"},

/* en */ {IDS_ABOUT,    "About"},
/* de */ {IDS_ABOUT_DE, "�ber VICE"},
/* fr */ {IDS_ABOUT_FR, "� Propos"},
/* it */ {IDS_ABOUT_IT, "Informazioni su VICE"},
/* nl */ {IDS_ABOUT_NL, "Over VICE"},
/* pl */ {IDS_ABOUT_PL, ""},  /* fuzzy */
/* sv */ {IDS_ABOUT_SV, "Om VICE"},

/* en */ {IDS_IMAGE_CONTENTS,    "Image Contents"},
/* de */ {IDS_IMAGE_CONTENTS_DE, "Image Datei Inhalt"},
/* fr */ {IDS_IMAGE_CONTENTS_FR, "Contenu de l'image"},
/* it */ {IDS_IMAGE_CONTENTS_IT, "Contenuti"},
/* nl */ {IDS_IMAGE_CONTENTS_NL, "Bestand Inhoud"},
/* pl */ {IDS_IMAGE_CONTENTS_PL, ""},  /* fuzzy */
/* sv */ {IDS_IMAGE_CONTENTS_SV, "Innehall i avbildning"},

/* en */ {IDMS_DISABLED,    "Disabled"},
/* de */ {IDMS_DISABLED_DE, "Deaktiviert"},
/* fr */ {IDMS_DISABLED_FR, "D�sactiv�"},
/* it */ {IDMS_DISABLED_IT, "Disabilitato"},
/* nl */ {IDMS_DISABLED_NL, "Uit"},
/* pl */ {IDMS_DISABLED_PL, ""},  /* fuzzy */
/* sv */ {IDMS_DISABLED_SV, "Av"},

/* en */ {IDS_ENABLED,    "Enabled"},
/* de */ {IDS_ENABLED_DE, ""},  /* fuzzy */
/* fr */ {IDS_ENABLED_FR, ""},  /* fuzzy */
/* it */ {IDS_ENABLED_IT, "Abilitato"},
/* nl */ {IDS_ENABLED_NL, "Aan"},
/* pl */ {IDS_ENABLED_PL, ""},  /* fuzzy */
/* sv */ {IDS_ENABLED_SV, "P�"},

/* en */ {IDS_RS232_DEVICE_1,    "RS232 Device 1"},
/* de */ {IDS_RS232_DEVICE_1_DE, "RS232 Ger�t 1"},
/* fr */ {IDS_RS232_DEVICE_1_FR, "1er P�riph�rique RS232"},
/* it */ {IDS_RS232_DEVICE_1_IT, "Dispositivo RS232 #1"},
/* nl */ {IDS_RS232_DEVICE_1_NL, "RS232 apparaat 1"},
/* pl */ {IDS_RS232_DEVICE_1_PL, ""},  /* fuzzy */
/* sv */ {IDS_RS232_DEVICE_1_SV, "RS232-enhet 1"},

/* en */ {IDS_RS232_DEVICE_2,    "RS232 Device 2"},
/* de */ {IDS_RS232_DEVICE_2_DE, "RS232 Ger�t 2"},
/* fr */ {IDS_RS232_DEVICE_2_FR, "2e P�riph�rique RS232"},
/* it */ {IDS_RS232_DEVICE_2_IT, "Dispositivo RS232 #2"},
/* nl */ {IDS_RS232_DEVICE_2_NL, "RS232 apparaat 2"},
/* pl */ {IDS_RS232_DEVICE_2_PL, ""},  /* fuzzy */
/* sv */ {IDS_RS232_DEVICE_2_SV, "RS232-enhet 2"},

/* en */ {IDS_RS232_DEVICE_3,    "RS232 Device 3"},
/* de */ {IDS_RS232_DEVICE_3_DE, "RS232 Ger�t 3"},
/* fr */ {IDS_RS232_DEVICE_3_FR, "3e P�riph�rique RS232"},
/* it */ {IDS_RS232_DEVICE_3_IT, "Dispositivo RS232 #3"},
/* nl */ {IDS_RS232_DEVICE_3_NL, "RS232 apparaat 3"},
/* pl */ {IDS_RS232_DEVICE_3_PL, ""},  /* fuzzy */
/* sv */ {IDS_RS232_DEVICE_3_SV, "RS232-enhet 3"},

/* en */ {IDS_RS232_DEVICE_4,    "RS232 Device 4"},
/* de */ {IDS_RS232_DEVICE_4_DE, "RS232 Ger�t 4"},
/* fr */ {IDS_RS232_DEVICE_4_FR, "4e P�riph�rique RS232"},
/* it */ {IDS_RS232_DEVICE_4_IT, "Dispositivo RS232 #4"},
/* nl */ {IDS_RS232_DEVICE_4_NL, "RS232 apparaat 4"},
/* pl */ {IDS_RS232_DEVICE_4_PL, ""},  /* fuzzy */
/* sv */ {IDS_RS232_DEVICE_4_SV, "RS232-enhet 4"},

/* en */ {IDS_NONE,    "None"},
/* de */ {IDS_NONE_DE, "Kein"},
/* fr */ {IDS_NONE_FR, "Aucun"},
/* it */ {IDS_NONE_IT, "Nessuno"},
/* nl */ {IDS_NONE_NL, "Geen"},
/* pl */ {IDS_NONE_PL, ""},  /* fuzzy */
/* sv */ {IDS_NONE_SV, "Ingen"},

/* en */ {IDS_IRQ,    "IRQ"},
/* de */ {IDS_IRQ_DE, "IRQ"},
/* fr */ {IDS_IRQ_FR, "IRQ"},
/* it */ {IDS_IRQ_IT, "IRQ"},
/* nl */ {IDS_IRQ_NL, "IRQ"},
/* pl */ {IDS_IRQ_PL, "IRQ"},
/* sv */ {IDS_IRQ_SV, "IRQ"},

/* en */ {IDS_NMI,    "NMI"},
/* de */ {IDS_NMI_DE, "NMI"},
/* fr */ {IDS_NMI_FR, "NMI"},
/* it */ {IDS_NMI_IT, "NMI"},
/* nl */ {IDS_NMI_NL, "NMI"},
/* pl */ {IDS_NMI_PL, "NMI"},
/* sv */ {IDS_NMI_SV, "NMI"},

/* en */ {IDS_ACIA_DEVICE,    "ACIA Device"},
/* de */ {IDS_ACIA_DEVICE_DE, "ACIA Ger�t"},
/* fr */ {IDS_ACIA_DEVICE_FR, "P�riph�rique ACIA"},
/* it */ {IDS_ACIA_DEVICE_IT, "Dispositivo ACIA"},
/* nl */ {IDS_ACIA_DEVICE_NL, "ACIA apparaat"},
/* pl */ {IDS_ACIA_DEVICE_PL, ""},  /* fuzzy */
/* sv */ {IDS_ACIA_DEVICE_SV, "ACIA-enhet"},

/* en */ {IDS_ACIA_INTERRUPT,    "ACIA Interrupt"},
/* de */ {IDS_ACIA_INTERRUPT_DE, "ACIA Interrupt"},
/* fr */ {IDS_ACIA_INTERRUPT_FR, "Interruption ACIA"},
/* it */ {IDS_ACIA_INTERRUPT_IT, "Interrupt di ACIA"},
/* nl */ {IDS_ACIA_INTERRUPT_NL, "ACIA Interrupt"},
/* pl */ {IDS_ACIA_INTERRUPT_PL, ""},  /* fuzzy */
/* sv */ {IDS_ACIA_INTERRUPT_SV, "ACIA-avbrott"},

/* en */ {IDS_ACIA_MODE,    "ACIA Mode"},
/* de */ {IDS_ACIA_MODE_DE, ""},  /* fuzzy */
/* fr */ {IDS_ACIA_MODE_FR, ""},  /* fuzzy */
/* it */ {IDS_ACIA_MODE_IT, "Modo ACIA"},
/* nl */ {IDS_ACIA_MODE_NL, "ACIA Modus"},
/* pl */ {IDS_ACIA_MODE_PL, ""},  /* fuzzy */
/* sv */ {IDS_ACIA_MODE_SV, "ACIA l�ge"},

/* en */ {IDS_ACIA_SETTINGS,    "ACIA Settings"},
/* de */ {IDS_ACIA_SETTINGS_DE, "ACIA Einstellungen"},
/* fr */ {IDS_ACIA_SETTINGS_FR, "Parametres ACIA"},
/* it */ {IDS_ACIA_SETTINGS_IT, "Impostazioni ACIA"},
/* nl */ {IDS_ACIA_SETTINGS_NL, "ACIA Instellingen"},
/* pl */ {IDS_ACIA_SETTINGS_PL, ""},  /* fuzzy */
/* sv */ {IDS_ACIA_SETTINGS_SV, "ACIA-inst�llningar"},

/* en */ {IDS_256K_ENABLED,    "256K Enabled"},
/* de */ {IDS_256K_ENABLED_DE, ""},  /* fuzzy */
/* fr */ {IDS_256K_ENABLED_FR, ""},  /* fuzzy */
/* it */ {IDS_256K_ENABLED_IT, "256K Abilitati"},
/* nl */ {IDS_256K_ENABLED_NL, "256K Aan/Uit"},
/* pl */ {IDS_256K_ENABLED_PL, ""},  /* fuzzy */
/* sv */ {IDS_256K_ENABLED_SV, ""},  /* fuzzy */

/* en */ {IDS_256K_BASE,    "256K Base"},
/* de */ {IDS_256K_BASE_DE, "256K Basis"},
/* fr */ {IDS_256K_BASE_FR, ""},  /* fuzzy */
/* it */ {IDS_256K_BASE_IT, "Indirizzo base 256K"},
/* nl */ {IDS_256K_BASE_NL, "256K basis adres"},
/* pl */ {IDS_256K_BASE_PL, ""},  /* fuzzy */
/* sv */ {IDS_256K_BASE_SV, "256K bas-adress"},

/* en */ {IDS_256K_SETTINGS,    "256K Settings"},
/* de */ {IDS_256K_SETTINGS_DE, ""},  /* fuzzy */
/* fr */ {IDS_256K_SETTINGS_FR, ""},  /* fuzzy */
/* it */ {IDS_256K_SETTINGS_IT, "Impostazioni 256K"},
/* nl */ {IDS_256K_SETTINGS_NL, "256K Instellingen"},
/* pl */ {IDS_256K_SETTINGS_PL, ""},  /* fuzzy */
/* sv */ {IDS_256K_SETTINGS_SV, "256K inst�llningar"},

/* en */ {IDS_MODEL_LINE,    "model line"},
/* de */ {IDS_MODEL_LINE_DE, "modell linie"},
/* fr */ {IDS_MODEL_LINE_FR, "ligne du modele"},
/* it */ {IDS_MODEL_LINE_IT, "Modelli"},
/* nl */ {IDS_MODEL_LINE_NL, "model lijn"},
/* pl */ {IDS_MODEL_LINE_PL, ""},  /* fuzzy */
/* sv */ {IDS_MODEL_LINE_SV, "modelltyp"},

/* en */ {IDS_RAM_SIZE,    "RAM Size"},
/* de */ {IDS_RAM_SIZE_DE, "RAM Gr��e"},
/* fr */ {IDS_RAM_SIZE_FR, "Taille du RAM"},
/* it */ {IDS_RAM_SIZE_IT, "Dimensione RAM"},
/* nl */ {IDS_RAM_SIZE_NL, "RAM grootte"},
/* pl */ {IDS_RAM_SIZE_PL, ""},  /* fuzzy */
/* sv */ {IDS_RAM_SIZE_SV, "RAM-storlek"},

/* en */ {IDS_RAM_BLOCK_0400_0FFF,    "RAM Block $0400-$0FFF"},
/* de */ {IDS_RAM_BLOCK_0400_0FFF_DE, "RAM in $0400-$0FFF"},
/* fr */ {IDS_RAM_BLOCK_0400_0FFF_FR, "RAM dans $0400-$0FFF"},
/* it */ {IDS_RAM_BLOCK_0400_0FFF_IT, "RAM a $0400-$0FFF"},
/* nl */ {IDS_RAM_BLOCK_0400_0FFF_NL, "RAM Blok $0400-$0FFF"},
/* pl */ {IDS_RAM_BLOCK_0400_0FFF_PL, ""},  /* fuzzy */
/* sv */ {IDS_RAM_BLOCK_0400_0FFF_SV, "RAM pa $0400-$0FFF"},

/* en */ {IDS_RAM_BLOCK_0800_0FFF,    "RAM Block $0800-$0FFF"},
/* de */ {IDS_RAM_BLOCK_0800_0FFF_DE, "RAM in $0800-$0FFF"},
/* fr */ {IDS_RAM_BLOCK_0800_0FFF_FR, "RAM dans $0800-$0FFF"},
/* it */ {IDS_RAM_BLOCK_0800_0FFF_IT, "RAM a $0800-$0FFF"},
/* nl */ {IDS_RAM_BLOCK_0800_0FFF_NL, "RAM Blok $0800-$0FFF"},
/* pl */ {IDS_RAM_BLOCK_0800_0FFF_PL, ""},  /* fuzzy */
/* sv */ {IDS_RAM_BLOCK_0800_0FFF_SV, "RAM pa $0800-$0FFF"},

/* en */ {IDS_RAM_BLOCK_1000_1FFF,    "RAM Block $1000-$1FFF"},
/* de */ {IDS_RAM_BLOCK_1000_1FFF_DE, "RAM in $1000-$1FFF"},
/* fr */ {IDS_RAM_BLOCK_1000_1FFF_FR, "RAM dans $1000-$1FFF"},
/* it */ {IDS_RAM_BLOCK_1000_1FFF_IT, "RAM a $1000-$1FFF"},
/* nl */ {IDS_RAM_BLOCK_1000_1FFF_NL, "RAM Blok $1000-$1FFF"},
/* pl */ {IDS_RAM_BLOCK_1000_1FFF_PL, ""},  /* fuzzy */
/* sv */ {IDS_RAM_BLOCK_1000_1FFF_SV, "RAM pa $1000-$1FFF"},

/* en */ {IDS_RAM_BLOCK_2000_3FFF,    "RAM Block $2000-$3FFF"},
/* de */ {IDS_RAM_BLOCK_2000_3FFF_DE, "RAM in $2000-$3FFF"},
/* fr */ {IDS_RAM_BLOCK_2000_3FFF_FR, "RAM dans $2000-$3FFF"},
/* it */ {IDS_RAM_BLOCK_2000_3FFF_IT, "RAM a $2000-$3FFF"},
/* nl */ {IDS_RAM_BLOCK_2000_3FFF_NL, "RAM Blok $2000-$3FFF"},
/* pl */ {IDS_RAM_BLOCK_2000_3FFF_PL, ""},  /* fuzzy */
/* sv */ {IDS_RAM_BLOCK_2000_3FFF_SV, "RAM pa $2000-$3FFF"},

/* en */ {IDS_RAM_BLOCK_4000_5FFF,    "RAM Block $4000-$5FFF"},
/* de */ {IDS_RAM_BLOCK_4000_5FFF_DE, "RAM in $4000-$5FFF"},
/* fr */ {IDS_RAM_BLOCK_4000_5FFF_FR, "RAM dans $4000-$5FFF"},
/* it */ {IDS_RAM_BLOCK_4000_5FFF_IT, "RAM a $4000-$5FFF"},
/* nl */ {IDS_RAM_BLOCK_4000_5FFF_NL, "RAM Blok $4000-$5FFF"},
/* pl */ {IDS_RAM_BLOCK_4000_5FFF_PL, ""},  /* fuzzy */
/* sv */ {IDS_RAM_BLOCK_4000_5FFF_SV, "RAM pa $4000-$5FFF"},

/* en */ {IDS_RAM_BLOCK_6000_7FFF,    "RAM Block $6000-$7FFF"},
/* de */ {IDS_RAM_BLOCK_6000_7FFF_DE, "RAM in $6000-$7FFF"},
/* fr */ {IDS_RAM_BLOCK_6000_7FFF_FR, "RAM dans $6000-$7FFF"},
/* it */ {IDS_RAM_BLOCK_6000_7FFF_IT, "RAM a $6000-$7FFF"},
/* nl */ {IDS_RAM_BLOCK_6000_7FFF_NL, "RAM Blok $6000-$7FFF"},
/* pl */ {IDS_RAM_BLOCK_6000_7FFF_PL, ""},  /* fuzzy */
/* sv */ {IDS_RAM_BLOCK_6000_7FFF_SV, "RAM pa $6000-$7FFF"},

/* en */ {IDS_RAM_BLOCK_A000_BFFF,    "RAM Block $A000-$BFFF"},
/* de */ {IDS_RAM_BLOCK_A000_BFFF_DE, "RAM in $A000-$BFFF"},
/* fr */ {IDS_RAM_BLOCK_A000_BFFF_FR, "RAM dans $A000-$BFFF"},
/* it */ {IDS_RAM_BLOCK_A000_BFFF_IT, "RAM a $A000-$BFFF"},
/* nl */ {IDS_RAM_BLOCK_A000_BFFF_NL, "RAM Blok $A000-$BFFF"},
/* pl */ {IDS_RAM_BLOCK_A000_BFFF_PL, ""},  /* fuzzy */
/* sv */ {IDS_RAM_BLOCK_A000_BFFF_SV, "RAM pa $A000-$BFFF"},

/* en */ {IDS_RAM_BLOCK_C000_CFFF,    "RAM Block $C000-$CFFF"},
/* de */ {IDS_RAM_BLOCK_C000_CFFF_DE, "RAM in $C000-$CFFF"},
/* fr */ {IDS_RAM_BLOCK_C000_CFFF_FR, "RAM dans $C000-$CFFF"},
/* it */ {IDS_RAM_BLOCK_C000_CFFF_IT, "RAM a $C000-$CFFF"},
/* nl */ {IDS_RAM_BLOCK_C000_CFFF_NL, "RAM Blok $C000-$CFFF"},
/* pl */ {IDS_RAM_BLOCK_C000_CFFF_PL, ""},  /* fuzzy */
/* sv */ {IDS_RAM_BLOCK_C000_CFFF_SV, "RAM pa $C000-$CFFF"},

/* en */ {IDS_CBM2_SETTINGS,    "CBM2 Settings"},
/* de */ {IDS_CBM2_SETTINGS_DE, "CBM2 Einstellungen"},
/* fr */ {IDS_CBM2_SETTINGS_FR, "Parametres CBM2"},
/* it */ {IDS_CBM2_SETTINGS_IT, "Impostazioni di CBM2"},
/* nl */ {IDS_CBM2_SETTINGS_NL, "CBM2 Instellingen"},
/* pl */ {IDS_CBM2_SETTINGS_PL, ""},  /* fuzzy */
/* sv */ {IDS_CBM2_SETTINGS_SV, "CBM2-inst�llningar"},

/* en */ {IDS_RESET_DATASETTE_WITH_CPU,    "Reset Datasette with CPU"},
/* de */ {IDS_RESET_DATASETTE_WITH_CPU_DE, "Zur�cksetzen des Bandlaufwerks bei CPU Reset"},
/* fr */ {IDS_RESET_DATASETTE_WITH_CPU_FR, "R�initialiser la datassette avec le CPU"},
/* it */ {IDS_RESET_DATASETTE_WITH_CPU_IT, "Reset del registratore al reset della CPU"},
/* nl */ {IDS_RESET_DATASETTE_WITH_CPU_NL, "Reset Datasette met CPU"},
/* pl */ {IDS_RESET_DATASETTE_WITH_CPU_PL, ""},  /* fuzzy */
/* sv */ {IDS_RESET_DATASETTE_WITH_CPU_SV, "Nollst�ll Datasette med CPU"},

/* en */ {IDS_ADDITIONAL_DELAY,    "Additional Delay"},
/* de */ {IDS_ADDITIONAL_DELAY_DE, ""},  /* fuzzy */
/* fr */ {IDS_ADDITIONAL_DELAY_FR, ""},  /* fuzzy */
/* it */ {IDS_ADDITIONAL_DELAY_IT, "Ritardo aggiuntivo"},
/* nl */ {IDS_ADDITIONAL_DELAY_NL, "Extra vertraging"},
/* pl */ {IDS_ADDITIONAL_DELAY_PL, ""},  /* fuzzy */
/* sv */ {IDS_ADDITIONAL_DELAY_SV, "Ytterligare f�rdr�jning"},

/* en */ {IDS_DELAY_AT_ZERO_VALUES,    "Delay at Zero Values"},
/* de */ {IDS_DELAY_AT_ZERO_VALUES_DE, "Verz�gerung bei Zero-Werte"},
/* fr */ {IDS_DELAY_AT_ZERO_VALUES_FR, "D�lai aux valeurs-z�ro"},
/* it */ {IDS_DELAY_AT_ZERO_VALUES_IT, "Ritardo pari a zero"},
/* nl */ {IDS_DELAY_AT_ZERO_VALUES_NL, "Nul-waarden vertraging"},
/* pl */ {IDS_DELAY_AT_ZERO_VALUES_PL, ""},  /* fuzzy */
/* sv */ {IDS_DELAY_AT_ZERO_VALUES_SV, "F�rdr�jning vid &nollv�rden"},

/* en */ {IDS_DATASETTE_SETTINGS,    "Datasette Settings"},
/* de */ {IDS_DATASETTE_SETTINGS_DE, "Datasette Einstellungen"},
/* fr */ {IDS_DATASETTE_SETTINGS_FR, "R�glages datassette"},
/* it */ {IDS_DATASETTE_SETTINGS_IT, "Impostazioni registratore"},
/* nl */ {IDS_DATASETTE_SETTINGS_NL, "Datasette Instellingen"},
/* pl */ {IDS_DATASETTE_SETTINGS_PL, ""},  /* fuzzy */
/* sv */ {IDS_DATASETTE_SETTINGS_SV, "Datasetteinst�llningar"},

/* en */ {IDS_NEVER_EXTEND,    "Never extend"},
/* de */ {IDS_NEVER_EXTEND_DE, "Nie Erweitern"},
/* fr */ {IDS_NEVER_EXTEND_FR, "Ne jamais �&tendre"},
/* it */ {IDS_NEVER_EXTEND_IT, "Non estendere mai"},
/* nl */ {IDS_NEVER_EXTEND_NL, "Nooit uitbreiden"},
/* pl */ {IDS_NEVER_EXTEND_PL, ""},  /* fuzzy */
/* sv */ {IDS_NEVER_EXTEND_SV, "Ut�ka aldrig"},

/* en */ {IDS_ASK_ON_EXTEND,    "Ask on extend"},
/* de */ {IDS_ASK_ON_EXTEND_DE, "Bei Erweiterung R�ckfrage"},
/* fr */ {IDS_ASK_ON_EXTEND_FR, "Demander avant d'�tendre"},
/* it */ {IDS_ASK_ON_EXTEND_IT, "Estendi su richiesta"},
/* nl */ {IDS_ASK_ON_EXTEND_NL, "Vragen bij uitbreiden"},
/* pl */ {IDS_ASK_ON_EXTEND_PL, ""},  /* fuzzy */
/* sv */ {IDS_ASK_ON_EXTEND_SV, "Fraga vid ut�kning"},

/* en */ {IDS_EXTEND_ON_ACCESS,    "Extend on access"},
/* de */ {IDS_EXTEND_ON_ACCESS_DE, "Erweitern wenn n�tig"},
/* fr */ {IDS_EXTEND_ON_ACCESS_FR, "�tendre   l'acces"},
/* it */ {IDS_EXTEND_ON_ACCESS_IT, "Estendi all'accesso"},
/* nl */ {IDS_EXTEND_ON_ACCESS_NL, "Uitbreiden bij toegang"},
/* pl */ {IDS_EXTEND_ON_ACCESS_PL, ""},  /* fuzzy */
/* sv */ {IDS_EXTEND_ON_ACCESS_SV, "Ut�ka vid atkomst"},

/* en */ {IDS_NO_TRAPS,    "No traps"},
/* de */ {IDS_NO_TRAPS_DE, "Kein Traps"},
/* fr */ {IDS_NO_TRAPS_FR, "No traps"},
/* it */ {IDS_NO_TRAPS_IT, "Nessun rilevamento"},
/* nl */ {IDS_NO_TRAPS_NL, "Geen traps"},
/* pl */ {IDS_NO_TRAPS_PL, ""},  /* fuzzy */
/* sv */ {IDS_NO_TRAPS_SV, "Inga Traps"},

/* en */ {IDS_SKIP_CYCLES,    "Skip cycles"},
/* de */ {IDS_SKIP_CYCLES_DE, "Zyklen verwerfen"},
/* fr */ {IDS_SKIP_CYCLES_FR, "Sauter des cycles"},
/* it */ {IDS_SKIP_CYCLES_IT, "Salta cicli"},
/* nl */ {IDS_SKIP_CYCLES_NL, "Sla cycli over"},
/* pl */ {IDS_SKIP_CYCLES_PL, ""},  /* fuzzy */
/* sv */ {IDS_SKIP_CYCLES_SV, "Hoppa cykler"},

/* en */ {IDS_TRAP_IDLE,    "Trap idle"},
/* de */ {IDS_TRAP_IDLE_DE, "Trap idle"},
/* fr */ {IDS_TRAP_IDLE_FR, "Trap idle"},
/* it */ {IDS_TRAP_IDLE_IT, "Rileva inattivit�"},
/* nl */ {IDS_TRAP_IDLE_NL, "Trap idle"},
/* pl */ {IDS_TRAP_IDLE_PL, ""},  /* fuzzy */
/* sv */ {IDS_TRAP_IDLE_SV, "F�nga inaktiva"},

/* en */ {IDS_DRIVE_TYPE,    "Drive type"},
/* de */ {IDS_DRIVE_TYPE_DE, "Ger�te Typ"},
/* fr */ {IDS_DRIVE_TYPE_FR, "Type de lecteur"},
/* it */ {IDS_DRIVE_TYPE_IT, "Tipo di drive"},
/* nl */ {IDS_DRIVE_TYPE_NL, "Drive soort"},
/* pl */ {IDS_DRIVE_TYPE_PL, ""},  /* fuzzy */
/* sv */ {IDS_DRIVE_TYPE_SV, "Enhetstyp"},

/* en */ {IDS_40_TRACK_HANDLING,    "40 track handling"},
/* de */ {IDS_40_TRACK_HANDLING_DE, "40 Spur Umgang"},
/* fr */ {IDS_40_TRACK_HANDLING_FR, "Prise en charge du 40 pistes"},
/* it */ {IDS_40_TRACK_HANDLING_IT, "Gestisci 40 tracce"},
/* nl */ {IDS_40_TRACK_HANDLING_NL, "40 sporen ondersteuning"},
/* pl */ {IDS_40_TRACK_HANDLING_PL, ""},  /* fuzzy */
/* sv */ {IDS_40_TRACK_HANDLING_SV, "Hantering av 40 spar"},

/* en */ {IDS_DRIVE_EXPANSION,    "Drive expansion"},
/* de */ {IDS_DRIVE_EXPANSION_DE, "Laufwerk Erweiterung"},
/* fr */ {IDS_DRIVE_EXPANSION_FR, "Expansion de lecteur"},
/* it */ {IDS_DRIVE_EXPANSION_IT, "Espasione del drive"},
/* nl */ {IDS_DRIVE_EXPANSION_NL, "Drive uitbreiding"},
/* pl */ {IDS_DRIVE_EXPANSION_PL, ""},  /* fuzzy */
/* sv */ {IDS_DRIVE_EXPANSION_SV, "Enhetsut�kning"},

/* en */ {IDS_IDLE_METHOD,    "Idle method"},
/* de */ {IDS_IDLE_METHOD_DE, "Idle Methode"},
/* fr */ {IDS_IDLE_METHOD_FR, "M�thode d'idle"},
/* it */ {IDS_IDLE_METHOD_IT, "Modo Idle"},
/* nl */ {IDS_IDLE_METHOD_NL, "Idle methode"},
/* pl */ {IDS_IDLE_METHOD_PL, ""},  /* fuzzy */
/* sv */ {IDS_IDLE_METHOD_SV, "Pausmetod"},

/* en */ {IDS_PARALLEL_CABLE,    "Parallel cable"},
/* de */ {IDS_PARALLEL_CABLE_DE, "Parallel Kabel"},
/* fr */ {IDS_PARALLEL_CABLE_FR, "C�ble parallele"},
/* it */ {IDS_PARALLEL_CABLE_IT, "Cavo parallelo"},
/* nl */ {IDS_PARALLEL_CABLE_NL, "Parallel kabel"},
/* pl */ {IDS_PARALLEL_CABLE_PL, ""},  /* fuzzy */
/* sv */ {IDS_PARALLEL_CABLE_SV, "Parallellkabel"},

/* en */ {IDS_DRIVE_SETTINGS,    "Drive Settings"},
/* de */ {IDS_DRIVE_SETTINGS_DE, "Floppy Einstellungen"},
/* fr */ {IDS_DRIVE_SETTINGS_FR, "Parametres des lecteurs"},
/* it */ {IDS_DRIVE_SETTINGS_IT, "Impostazioni drive"},
/* nl */ {IDS_DRIVE_SETTINGS_NL, "Drive instellingen"},
/* pl */ {IDS_DRIVE_SETTINGS_PL, ""},  /* fuzzy */
/* sv */ {IDS_DRIVE_SETTINGS_SV, "Diskettenhetsinst�llningar"},

/* en */ {IDS_GEORAM_SIZE,    "GEORAM Size"},
/* de */ {IDS_GEORAM_SIZE_DE, "GEORAM Gr��e"},
/* fr */ {IDS_GEORAM_SIZE_FR, "Taille du GEORAM"},
/* it */ {IDS_GEORAM_SIZE_IT, "Dimensione GEORAM"},
/* nl */ {IDS_GEORAM_SIZE_NL, "GEORAM grootte"},
/* pl */ {IDS_GEORAM_SIZE_PL, ""},  /* fuzzy */
/* sv */ {IDS_GEORAM_SIZE_SV, "GEORAM-storlek"},

/* en */ {IDS_GEORAM_SETTINGS,    "GEORAM Settings"},
/* de */ {IDS_GEORAM_SETTINGS_DE, "GEORAM Einstellungen"},
/* fr */ {IDS_GEORAM_SETTINGS_FR, "Parametres GEORAM"},
/* it */ {IDS_GEORAM_SETTINGS_IT, "Impostazioni GEORAM"},
/* nl */ {IDS_GEORAM_SETTINGS_NL, "GEORAM Instellingen"},
/* pl */ {IDS_GEORAM_SETTINGS_PL, ""},  /* fuzzy */
/* sv */ {IDS_GEORAM_SETTINGS_SV, "GEORAM-inst�llningar"},

/* en */ {IDS_JOYSTICK_IN_PORT_1,    "Joystick in port #1"},
/* de */ {IDS_JOYSTICK_IN_PORT_1_DE, "Joystick in Port #1"},
/* fr */ {IDS_JOYSTICK_IN_PORT_1_FR, "Joystick dans le port #1"},
/* it */ {IDS_JOYSTICK_IN_PORT_1_IT, "Joystick nella porta #1"},
/* nl */ {IDS_JOYSTICK_IN_PORT_1_NL, "Joystick in poort #1"},
/* pl */ {IDS_JOYSTICK_IN_PORT_1_PL, ""},  /* fuzzy */
/* sv */ {IDS_JOYSTICK_IN_PORT_1_SV, "Joystick i spelport 1"},

/* en */ {IDS_JOYSTICK_IN_PORT_2,    "Joystick in port #2"},
/* de */ {IDS_JOYSTICK_IN_PORT_2_DE, "Joystick in Port #2"},
/* fr */ {IDS_JOYSTICK_IN_PORT_2_FR, "Joystick dans le port #2"},
/* it */ {IDS_JOYSTICK_IN_PORT_2_IT, "Joystick nella porta #2"},
/* nl */ {IDS_JOYSTICK_IN_PORT_2_NL, "Joystick in poort #2"},
/* pl */ {IDS_JOYSTICK_IN_PORT_2_PL, ""},  /* fuzzy */
/* sv */ {IDS_JOYSTICK_IN_PORT_2_SV, "Joystick i spelport 2"},

/* en */ {IDS_JOYSTICK_SETTINGS,    "Joystick Settings"},
/* de */ {IDS_JOYSTICK_SETTINGS_DE, "Joystick Einstellungen"},
/* fr */ {IDS_JOYSTICK_SETTINGS_FR, "Parametres des joysticks"},
/* it */ {IDS_JOYSTICK_SETTINGS_IT, "Impostazioni joystick"},
/* nl */ {IDS_JOYSTICK_SETTINGS_NL, "Joystick instellingen"},
/* pl */ {IDS_JOYSTICK_SETTINGS_PL, ""},  /* fuzzy */
/* sv */ {IDS_JOYSTICK_SETTINGS_SV, "Inst�llningar f�r joystick"},

/* en */ {IDS_KEYPAD,    "Numpad"},
/* de */ {IDS_KEYPAD_DE, ""},  /* fuzzy */
/* fr */ {IDS_KEYPAD_FR, ""},  /* fuzzy */
/* it */ {IDS_KEYPAD_IT, "Numpad"},
/* nl */ {IDS_KEYPAD_NL, "Numpad"},
/* pl */ {IDS_KEYPAD_PL, ""},  /* fuzzy */
/* sv */ {IDS_KEYPAD_SV, "Numreriskt tangentbord"},

/* en */ {IDS_JOY_PORT_0,    "Joy Port 0"},
/* de */ {IDS_JOY_PORT_0_DE, ""},  /* fuzzy */
/* fr */ {IDS_JOY_PORT_0_FR, ""},  /* fuzzy */
/* it */ {IDS_JOY_PORT_0_IT, "Joy Porta 0"},
/* nl */ {IDS_JOY_PORT_0_NL, "Joy Poort 0"},
/* pl */ {IDS_JOY_PORT_0_PL, ""},  /* fuzzy */
/* sv */ {IDS_JOY_PORT_0_SV, "Joy port 0"},

/* en */ {IDS_JOY_PORT_1,    "Joy Port 1"},
/* de */ {IDS_JOY_PORT_1_DE, ""},  /* fuzzy */
/* fr */ {IDS_JOY_PORT_1_FR, ""},  /* fuzzy */
/* it */ {IDS_JOY_PORT_1_IT, "Joy Porta 1"},
/* nl */ {IDS_JOY_PORT_1_NL, "Joy Poort 1"},
/* pl */ {IDS_JOY_PORT_1_PL, ""},  /* fuzzy */
/* sv */ {IDS_JOY_PORT_1_SV, "Joy Port 1"},

/* en */ {IDS_JOY_PORT_2,    "Joy Port 2"},
/* de */ {IDS_JOY_PORT_2_DE, ""},  /* fuzzy */
/* fr */ {IDS_JOY_PORT_2_FR, ""},  /* fuzzy */
/* it */ {IDS_JOY_PORT_2_IT, "Joy Porta 2"},
/* nl */ {IDS_JOY_PORT_2_NL, "Joy Poort 2"},
/* pl */ {IDS_JOY_PORT_2_PL, ""},  /* fuzzy */
/* sv */ {IDS_JOY_PORT_2_SV, "Joy Port 2"},

/* en */ {IDS_JOY_PORT_3,    "Joy Port 3"},
/* de */ {IDS_JOY_PORT_3_DE, ""},  /* fuzzy */
/* fr */ {IDS_JOY_PORT_3_FR, ""},  /* fuzzy */
/* it */ {IDS_JOY_PORT_3_IT, "Joy Porta 3"},
/* nl */ {IDS_JOY_PORT_3_NL, "Joy Poort 3"},
/* pl */ {IDS_JOY_PORT_3_PL, ""},  /* fuzzy */
/* sv */ {IDS_JOY_PORT_3_SV, "Joy Port 3"},

/* en */ {IDS_STOP_BLUE,    "Stop/Blue"},
/* de */ {IDS_STOP_BLUE_DE, ""},  /* fuzzy */
/* fr */ {IDS_STOP_BLUE_FR, ""},  /* fuzzy */
/* it */ {IDS_STOP_BLUE_IT, "Stop/Blu"},
/* nl */ {IDS_STOP_BLUE_NL, "Stop/Blauw"},
/* pl */ {IDS_STOP_BLUE_PL, ""},  /* fuzzy */
/* sv */ {IDS_STOP_BLUE_SV, "Stop/Bl�"},

/* en */ {IDS_SELECT_RED,    "Select/Red"},
/* de */ {IDS_SELECT_RED_DE, ""},  /* fuzzy */
/* fr */ {IDS_SELECT_RED_FR, ""},  /* fuzzy */
/* it */ {IDS_SELECT_RED_IT, "Seleziona/Rosso"},
/* nl */ {IDS_SELECT_RED_NL, "Select/Rood"},
/* pl */ {IDS_SELECT_RED_PL, ""},  /* fuzzy */
/* sv */ {IDS_SELECT_RED_SV, "Select/R�d"},

/* en */ {IDS_REPEAT_YELLOW,    "Repeat/Yellow"},
/* de */ {IDS_REPEAT_YELLOW_DE, ""},  /* fuzzy */
/* fr */ {IDS_REPEAT_YELLOW_FR, ""},  /* fuzzy */
/* it */ {IDS_REPEAT_YELLOW_IT, "Ripeti/Giallo"},
/* nl */ {IDS_REPEAT_YELLOW_NL, "Repeat/Geel"},
/* pl */ {IDS_REPEAT_YELLOW_PL, ""},  /* fuzzy */
/* sv */ {IDS_REPEAT_YELLOW_SV, "Repeat/Gul"},

/* en */ {IDS_SHUFFLE_GREEN,    "Shuffle/Green"},
/* de */ {IDS_SHUFFLE_GREEN_DE, ""},  /* fuzzy */
/* fr */ {IDS_SHUFFLE_GREEN_FR, ""},  /* fuzzy */
/* it */ {IDS_SHUFFLE_GREEN_IT, ""},  /* fuzzy */
/* nl */ {IDS_SHUFFLE_GREEN_NL, "Shuffle/Groen"},
/* pl */ {IDS_SHUFFLE_GREEN_PL, ""},  /* fuzzy */
/* sv */ {IDS_SHUFFLE_GREEN_SV, "Shuffle/Gr�n"},

/* en */ {IDS_FORWARD_CHARCOAL,    "Forward/Charcoal"},
/* de */ {IDS_FORWARD_CHARCOAL_DE, ""},  /* fuzzy */
/* fr */ {IDS_FORWARD_CHARCOAL_FR, ""},  /* fuzzy */
/* it */ {IDS_FORWARD_CHARCOAL_IT, "Avanti/Grigio scuro"},
/* nl */ {IDS_FORWARD_CHARCOAL_NL, "Forward/Charcoal"},
/* pl */ {IDS_FORWARD_CHARCOAL_PL, ""},  /* fuzzy */
/* sv */ {IDS_FORWARD_CHARCOAL_SV, "Forward/Tr�kol"},

/* en */ {IDS_REVERSE_CHARCOAL,    "Reverse/Charcoal"},
/* de */ {IDS_REVERSE_CHARCOAL_DE, ""},  /* fuzzy */
/* fr */ {IDS_REVERSE_CHARCOAL_FR, ""},  /* fuzzy */
/* it */ {IDS_REVERSE_CHARCOAL_IT, "Indietro/Grigio scuro"},
/* nl */ {IDS_REVERSE_CHARCOAL_NL, "Reverse/Charcoal"},
/* pl */ {IDS_REVERSE_CHARCOAL_PL, ""},  /* fuzzy */
/* sv */ {IDS_REVERSE_CHARCOAL_SV, "Reverse/Tr�kol"},

/* en */ {IDS_PLAY_PAUSE_GREY,    "Play-Pause/Grey"},
/* de */ {IDS_PLAY_PAUSE_GREY_DE, ""},  /* fuzzy */
/* fr */ {IDS_PLAY_PAUSE_GREY_FR, ""},  /* fuzzy */
/* it */ {IDS_PLAY_PAUSE_GREY_IT, "Play-Pausa/Grigio"},
/* nl */ {IDS_PLAY_PAUSE_GREY_NL, "Play-Pause/Grijs"},
/* pl */ {IDS_PLAY_PAUSE_GREY_PL, ""},  /* fuzzy */
/* sv */ {IDS_PLAY_PAUSE_GREY_SV, "[Play/Pause]/Gr�"},

/* en */ {IDS_JOY_1_DEVICE,    "Joy 1 Device"},
/* de */ {IDS_JOY_1_DEVICE_DE, ""},  /* fuzzy */
/* fr */ {IDS_JOY_1_DEVICE_FR, ""},  /* fuzzy */
/* it */ {IDS_JOY_1_DEVICE_IT, "Periferica Joy 1"},
/* nl */ {IDS_JOY_1_DEVICE_NL, "Joy 1 Apparaat"},
/* pl */ {IDS_JOY_1_DEVICE_PL, ""},  /* fuzzy */
/* sv */ {IDS_JOY_1_DEVICE_SV, "Joy 1-enhet"},

/* en */ {IDS_JOY_2_DEVICE,    "Joy 2 Device"},
/* de */ {IDS_JOY_2_DEVICE_DE, ""},  /* fuzzy */
/* fr */ {IDS_JOY_2_DEVICE_FR, ""},  /* fuzzy */
/* it */ {IDS_JOY_2_DEVICE_IT, "Periferica Joy 2"},
/* nl */ {IDS_JOY_2_DEVICE_NL, "Joy 2 Apparaat"},
/* pl */ {IDS_JOY_2_DEVICE_PL, ""},  /* fuzzy */
/* sv */ {IDS_JOY_2_DEVICE_SV, "Joy 2-enhet"},

/* en */ {IDS_JOY_1_FIRE,    "Joy 1 Fire"},
/* de */ {IDS_JOY_1_FIRE_DE, ""},  /* fuzzy */
/* fr */ {IDS_JOY_1_FIRE_FR, ""},  /* fuzzy */
/* it */ {IDS_JOY_1_FIRE_IT, "Fuoco Joy 1"},
/* nl */ {IDS_JOY_1_FIRE_NL, "Joy 1 Vuur"},
/* pl */ {IDS_JOY_1_FIRE_PL, ""},  /* fuzzy */
/* sv */ {IDS_JOY_1_FIRE_SV, "Joy 1 Fire"},

/* en */ {IDS_JOY_2_FIRE,    "Joy 2 Fire"},
/* de */ {IDS_JOY_2_FIRE_DE, ""},  /* fuzzy */
/* fr */ {IDS_JOY_2_FIRE_FR, ""},  /* fuzzy */
/* it */ {IDS_JOY_2_FIRE_IT, "Fuoco Joy 2"},
/* nl */ {IDS_JOY_2_FIRE_NL, "Joy 1 Vuur"},
/* pl */ {IDS_JOY_2_FIRE_PL, ""},  /* fuzzy */
/* sv */ {IDS_JOY_2_FIRE_SV, "Joy 2 Fire"},

/* en */ {IDS_SET_INPUT_JOYSTICK_1,    "Joystick #1 emulation: (0:None, 1: Numpad, 2: Keyset A/AI, 3: Keyset B/AI)"},
/* de */ {IDS_SET_INPUT_JOYSTICK_1_DE, ""},  /* fuzzy */
/* fr */ {IDS_SET_INPUT_JOYSTICK_1_FR, ""},  /* fuzzy */
/* it */ {IDS_SET_INPUT_JOYSTICK_1_IT, "Emulazione Joystick #1: (0:Nessuna, 1: Numpad, 2: Gruppo tasti A/AI, 3: Gruppo tasti B/AI)"},
/* nl */ {IDS_SET_INPUT_JOYSTICK_1_NL, "Joystick #1 emulatie: (0: Geen, 1: Numpad, 2: Keyset A/AI, 3: Keyset B/AI)"},
/* pl */ {IDS_SET_INPUT_JOYSTICK_1_PL, ""},  /* fuzzy */
/* sv */ {IDS_SET_INPUT_JOYSTICK_1_SV, "Joystick #1 emulation: (0:Ingen, 1: Numeriskt tangentbord, 2: Teckenupps�ttning A/AI, 3: Teckenupps�ttning B/AI)" },

/* en */ {IDS_SET_INPUT_JOYSTICK_2,    "Joystick #2 emulation: (0: None, 1: Numpad, 2: Keyset A/AI, 3: Keyset B/AI)"},
/* de */ {IDS_SET_INPUT_JOYSTICK_2_DE, ""},  /* fuzzy */
/* fr */ {IDS_SET_INPUT_JOYSTICK_2_FR, ""},  /* fuzzy */
/* it */ {IDS_SET_INPUT_JOYSTICK_2_IT, "Emulazione Joystick #2: (0:Nessuna, 1: Numpad, 2: Gruppo tasti A/AI, 3: Gruppo tasti B/AI)"},
/* nl */ {IDS_SET_INPUT_JOYSTICK_2_NL, "Joystick #2 emulatie: (0: Geen, 1: Numpad, 2: Keyset A/AI, 3: Keyset B/AI)"},
/* pl */ {IDS_SET_INPUT_JOYSTICK_2_PL, ""},  /* fuzzy */
/* sv */ {IDS_SET_INPUT_JOYSTICK_2_SV, "Joystick #2 emulation: (0:Ingen, 1: Numeriskt tangentbord, 2: Teckenupps�ttning A/AI, 3: Teckenupps�ttning B/AI)" },

/* en */ {IDS_SET_INPUT_JOYLL_1,    "Joystick #1 emulation: (0: None, 1: Numpad, 2-5: Amiga Joy 0-3"},
/* de */ {IDS_SET_INPUT_JOYLL_1_DE, ""},  /* fuzzy */
/* fr */ {IDS_SET_INPUT_JOYLL_1_FR, ""},  /* fuzzy */
/* it */ {IDS_SET_INPUT_JOYLL_1_IT, "Emulazione Joystick #1: (0: Nessuna, 1: Numpad, 2-5: Joy Amiga 0-3"},
/* nl */ {IDS_SET_INPUT_JOYLL_1_NL, "Joystick #1 emulatie: (0: Geen, 1: Numpad, 2-5: Amiga Joy 0-3"},
/* pl */ {IDS_SET_INPUT_JOYLL_1_PL, ""},  /* fuzzy */
/* sv */ {IDS_SET_INPUT_JOYLL_1_SV, "Joystick #1 emulation: (0:Ingen, 1: Numeriskt tangentbord, 2-5: Amiga Joy 0-3"},

/* en */ {IDS_SET_INPUT_JOYLL_2,    "Joystick #2 emulation: (0: None, 1: Numpad, 2-5: Amiga Joy 0-3"},
/* de */ {IDS_SET_INPUT_JOYLL_2_DE, ""},  /* fuzzy */
/* fr */ {IDS_SET_INPUT_JOYLL_2_FR, ""},  /* fuzzy */
/* it */ {IDS_SET_INPUT_JOYLL_2_IT, "Emulazione Joystick #2: (0: Nessuna, 1: Numpad, 2-5: Joy Amiga 0-3"},
/* nl */ {IDS_SET_INPUT_JOYLL_2_NL, "Joystick #2 emulatie (0: Geen, 1: Numpad, 2-5: Amiga Joy 0-3"},
/* pl */ {IDS_SET_INPUT_JOYLL_2_PL, ""},  /* fuzzy */
/* sv */ {IDS_SET_INPUT_JOYLL_2_SV, "Joystick #2 emulation: (0:Ingen, 1: Numeriskt tangentbord, 2-5: Amiga Joy 0-3"},

/* en */ {IDS_SAVE_SETTINGS_ON_EXIT,    "Save settings (resources) on exit"},
/* de */ {IDS_SAVE_SETTINGS_ON_EXIT_DE, "Einstellungen (Ressourcen) beim Beenden speichern"},
/* fr */ {IDS_SAVE_SETTINGS_ON_EXIT_FR, "Sauvegarder les parametres   la sortie"},
/* it */ {IDS_SAVE_SETTINGS_ON_EXIT_IT, "Salva le impostazioni in uscita"},
/* nl */ {IDS_SAVE_SETTINGS_ON_EXIT_NL, "Instellingen (resources) opslaan bij afsluiten"},
/* pl */ {IDS_SAVE_SETTINGS_ON_EXIT_PL, ""},  /* fuzzy */
/* sv */ {IDS_SAVE_SETTINGS_ON_EXIT_SV, "Spara inst�llningar (resurser) vid avslut"},

/* en */ {IDS_NEVER_SAVE_SETTINGS_EXIT,    "Never save settings (resources) on exit"},
/* de */ {IDS_NEVER_SAVE_SETTINGS_EXIT_DE, "Einstellungen beim Beenden nie speichern"},
/* fr */ {IDS_NEVER_SAVE_SETTINGS_EXIT_FR, "Ne jamais enregistrer les parametres   la sortie"},
/* it */ {IDS_NEVER_SAVE_SETTINGS_EXIT_IT, "Non salvare mai le impostazioni in uscita"},
/* nl */ {IDS_NEVER_SAVE_SETTINGS_EXIT_NL, "Instellingen (resources) niet opslaan bij afsluiten"},
/* pl */ {IDS_NEVER_SAVE_SETTINGS_EXIT_PL, ""},  /* fuzzy */
/* sv */ {IDS_NEVER_SAVE_SETTINGS_EXIT_SV, "Spara aldrig inst�llningar (resurser) vid avslut"},

/* en */ {IDS_CONFIRM_QUITING_VICE,    "Confirm quiting VICE"},
/* de */ {IDS_CONFIRM_QUITING_VICE_DE, "Best�tige beenden von VICE"},
/* fr */ {IDS_CONFIRM_QUITING_VICE_FR, "Demander une confirmation avant de quitter VICE"},
/* it */ {IDS_CONFIRM_QUITING_VICE_IT, "Conferma l'uscita da VICE"},
/* nl */ {IDS_CONFIRM_QUITING_VICE_NL, "Bevestigen van het aflsuiten van VICE"},
/* pl */ {IDS_CONFIRM_QUITING_VICE_PL, ""},  /* fuzzy */
/* sv */ {IDS_CONFIRM_QUITING_VICE_SV, "Bekr�fta f�r att avsluta VICE"},

/* en */ {IDS_NEVER_CONFIRM_QUITING_VICE,    "Never confirm quiting VICE"},
/* de */ {IDS_NEVER_CONFIRM_QUITING_VICE_DE, "Keine Best�tigung beim Beenden von VICE"},
/* fr */ {IDS_NEVER_CONFIRM_QUITING_VICE_FR, "Ne pas demander de confirmation pour quitter VICE"},
/* it */ {IDS_NEVER_CONFIRM_QUITING_VICE_IT, "Non confermare mai l'uscita da VICE"},
/* nl */ {IDS_NEVER_CONFIRM_QUITING_VICE_NL, "Niet bevestigen van het afsluiten van VICE"},
/* pl */ {IDS_NEVER_CONFIRM_QUITING_VICE_PL, ""},  /* fuzzy */
/* sv */ {IDS_NEVER_CONFIRM_QUITING_VICE_SV, "Bekr�fta inte f�r att avsluta VICE"},

/* en */ {IDS_PET_REU_SIZE,    "PET REU Size"},
/* de */ {IDS_PET_REU_SIZE_DE, "PET REU Gr��e"},
/* fr */ {IDS_PET_REU_SIZE_FR, "Taille du PET REU"},
/* it */ {IDS_PET_REU_SIZE_IT, "Dimensione PET REU"},
/* nl */ {IDS_PET_REU_SIZE_NL, "PET REU grootte"},
/* pl */ {IDS_PET_REU_SIZE_PL, ""},  /* fuzzy */
/* sv */ {IDS_PET_REU_SIZE_SV, "PET REU-storlek"},

/* en */ {IDS_PET_REU_SETTINGS,    "PET REU Settings"},
/* de */ {IDS_PET_REU_SETTINGS_DE, ""},  /* fuzzy */
/* fr */ {IDS_PET_REU_SETTINGS_FR, ""},  /* fuzzy */
/* it */ {IDS_PET_REU_SETTINGS_IT, "Impostazioni PET REU"},
/* nl */ {IDS_PET_REU_SETTINGS_NL, "PET REU Instellingen"},
/* pl */ {IDS_PET_REU_SETTINGS_PL, ""},  /* fuzzy */
/* sv */ {IDS_PET_REU_SETTINGS_SV, "PET REU-inst�llningar"},

/* en */ {IDS_PLUS256K_SETTINGS,    "PLUS256K Settings"},
/* de */ {IDS_PLUS256K_SETTINGS_DE, ""},  /* fuzzy */
/* fr */ {IDS_PLUS256K_SETTINGS_FR, ""},  /* fuzzy */
/* it */ {IDS_PLUS256K_SETTINGS_IT, "Impostazioni PLUS256K"},
/* nl */ {IDS_PLUS256K_SETTINGS_NL, "PLUS256K Instellingen"},
/* pl */ {IDS_PLUS256K_SETTINGS_PL, ""},  /* fuzzy */
/* sv */ {IDS_PLUS256K_SETTINGS_SV, "PLUS256K-inst�llningar"},

/* en */ {IDS_PLUS60K_BASE,    "PLUS60K Base"},
/* de */ {IDS_PLUS60K_BASE_DE, "PLUS60K Basis Adresse"},
/* fr */ {IDS_PLUS60K_BASE_FR, ""},  /* fuzzy */
/* it */ {IDS_PLUS60K_BASE_IT, "Indirizzo base di PLUS60K"},
/* nl */ {IDS_PLUS60K_BASE_NL, "PLUS60K basis adres"},
/* pl */ {IDS_PLUS60K_BASE_PL, ""},  /* fuzzy */
/* sv */ {IDS_PLUS60K_BASE_SV, "PLUS60K bas-adress"},

/* en */ {IDS_PLUS60K_SETTINGS,    "PLUS60K Settings"},
/* de */ {IDS_PLUS60K_SETTINGS_DE, ""},  /* fuzzy */
/* fr */ {IDS_PLUS60K_SETTINGS_FR, ""},  /* fuzzy */
/* it */ {IDS_PLUS60K_SETTINGS_IT, "Impostazioni PLUS60K"},
/* nl */ {IDS_PLUS60K_SETTINGS_NL, "PLUS60K Instellingen"},
/* pl */ {IDS_PLUS60K_SETTINGS_PL, ""},  /* fuzzy */
/* sv */ {IDS_PLUS60K_SETTINGS_SV, "PLUS60K-inst�llningar"},

/* en */ {IDS_VALUE_FIRST_BYTE,    "Value of first byte"},
/* de */ {IDS_VALUE_FIRST_BYTE_DE, "Wert erstes Byte"},
/* fr */ {IDS_VALUE_FIRST_BYTE_FR, "Valeur du premier octet"},
/* it */ {IDS_VALUE_FIRST_BYTE_IT, "Valore del primo byte"},
/* nl */ {IDS_VALUE_FIRST_BYTE_NL, "Waarde van eerste byte"},
/* pl */ {IDS_VALUE_FIRST_BYTE_PL, ""},  /* fuzzy */
/* sv */ {IDS_VALUE_FIRST_BYTE_SV, "V�rde pa f�rsta byte"},

/* en */ {IDS_LENGTH_CONSTANT_VALUES,    "Length of constant values"},
/* de */ {IDS_LENGTH_CONSTANT_VALUES_DE, "L�nge der konstaten Werte"},
/* fr */ {IDS_LENGTH_CONSTANT_VALUES_FR, "Longeur des valeurs constantes"},
/* it */ {IDS_LENGTH_CONSTANT_VALUES_IT, "Numero di valori costanti"},
/* nl */ {IDS_LENGTH_CONSTANT_VALUES_NL, "Lengte van constante waarden"},
/* pl */ {IDS_LENGTH_CONSTANT_VALUES_PL, ""},  /* fuzzy */
/* sv */ {IDS_LENGTH_CONSTANT_VALUES_SV, "L�ngd pa konstanta v�rden"},

/* en */ {IDS_LENGTH_CONSTANT_PATTERN,    "Length of constant pattern"},
/* de */ {IDS_LENGTH_CONSTANT_PATTERN_DE, "L�nge des konstanten Musters"},
/* fr */ {IDS_LENGTH_CONSTANT_PATTERN_FR, "Longeur des contantes pattern"},
/* it */ {IDS_LENGTH_CONSTANT_PATTERN_IT, "Dimensione del pattern costante"},
/* nl */ {IDS_LENGTH_CONSTANT_PATTERN_NL, "Lengte van constant patroon"},
/* pl */ {IDS_LENGTH_CONSTANT_PATTERN_PL, ""},  /* fuzzy */
/* sv */ {IDS_LENGTH_CONSTANT_PATTERN_SV, "L�ngd pa konstant m�nster"},

/* en */ {IDS_RAM_SETTINGS,    "RAM Settings"},
/* de */ {IDS_RAM_SETTINGS_DE, "RAM Einstellungen"},
/* fr */ {IDS_RAM_SETTINGS_FR, "Parametres de la RAM"},
/* it */ {IDS_RAM_SETTINGS_IT, "Impostazioni della RAM"},
/* nl */ {IDS_RAM_SETTINGS_NL, "RAM instellingen"},
/* pl */ {IDS_RAM_SETTINGS_PL, ""},  /* fuzzy */
/* sv */ {IDS_RAM_SETTINGS_SV, "RAM-inst�llningar"},

/* en */ {IDS_READ_WRITE,    "read/write"},
/* de */ {IDS_READ_WRITE_DE, ""},  /* fuzzy */
/* fr */ {IDS_READ_WRITE_FR, ""},  /* fuzzy */
/* it */ {IDS_READ_WRITE_IT, "lettura/scrittura"},
/* nl */ {IDS_READ_WRITE_NL, "lezen/schrijven"},
/* pl */ {IDS_READ_WRITE_PL, ""},  /* fuzzy */
/* sv */ {IDS_READ_WRITE_SV, "l�s/skriv"},

/* en */ {IDS_READ_ONLY,    "read-only"},
/* de */ {IDS_READ_ONLY_DE, "Schreibgschutz"},
/* fr */ {IDS_READ_ONLY_FR, "Lecture seule"},
/* it */ {IDS_READ_ONLY_IT, "Accesso in sola lettura"},
/* nl */ {IDS_READ_ONLY_NL, "alleen lezen"},
/* pl */ {IDS_READ_ONLY_PL, ""},  /* fuzzy */
/* sv */ {IDS_READ_ONLY_SV, "Endast l�satkomst"},

/* en */ {IDS_RAMCART_READ_WRITE,    "RAMCART Read/Write"},
/* de */ {IDS_RAMCART_READ_WRITE_DE, ""},  /* fuzzy */
/* fr */ {IDS_RAMCART_READ_WRITE_FR, ""},  /* fuzzy */
/* it */ {IDS_RAMCART_READ_WRITE_IT, "lettura/scrittura di RAMCART"},
/* nl */ {IDS_RAMCART_READ_WRITE_NL, "RAMCART Lezen/Schrijven"},
/* pl */ {IDS_RAMCART_READ_WRITE_PL, ""},  /* fuzzy */
/* sv */ {IDS_RAMCART_READ_WRITE_SV, "RAMCART l�s/skriv"},

/* en */ {IDS_RAMCART_SIZE,    "RAMCART Size"},
/* de */ {IDS_RAMCART_SIZE_DE, "RAMCART Gr��e"},
/* fr */ {IDS_RAMCART_SIZE_FR, "Taille du RAMCART"},
/* it */ {IDS_RAMCART_SIZE_IT, "Dimensione RAMCART"},
/* nl */ {IDS_RAMCART_SIZE_NL, "RAMCART grootte"},
/* pl */ {IDS_RAMCART_SIZE_PL, ""},  /* fuzzy */
/* sv */ {IDS_RAMCART_SIZE_SV, "Storlek pa RAMCART"},

/* en */ {IDS_RAMCART_SETTINGS,    "RAMCART Settings"},
/* de */ {IDS_RAMCART_SETTINGS_DE, "RAMCART Einstellungen"},
/* fr */ {IDS_RAMCART_SETTINGS_FR, "Parametres RAMCART"},
/* it */ {IDS_RAMCART_SETTINGS_IT, "Impostazioni RAMCART"},
/* nl */ {IDS_RAMCART_SETTINGS_NL, "RAMCART instellingen"},
/* pl */ {IDS_RAMCART_SETTINGS_PL, ""},  /* fuzzy */
/* sv */ {IDS_RAMCART_SETTINGS_SV, "RAMCART-inst�llningar"},

/* en */ {IDS_REU_SIZE,    "REU Size"},
/* de */ {IDS_REU_SIZE_DE, "REU Gr��e"},
/* fr */ {IDS_REU_SIZE_FR, "Taille du REU"},
/* it */ {IDS_REU_SIZE_IT, "Dimensione REU"},
/* nl */ {IDS_REU_SIZE_NL, "REU grootte"},
/* pl */ {IDS_REU_SIZE_PL, ""},  /* fuzzy */
/* sv */ {IDS_REU_SIZE_SV, "Storlek pa minnesexpansion"},

/* en */ {IDS_REU_SETTINGS,    "REU Settings"},
/* de */ {IDS_REU_SETTINGS_DE, "REU Einstellungen"},
/* fr */ {IDS_REU_SETTINGS_FR, "Parametres REU"},
/* it */ {IDS_REU_SETTINGS_IT, "Impostazioni REU"},
/* nl */ {IDS_REU_SETTINGS_NL, "REU Instellingen"},
/* pl */ {IDS_REU_SETTINGS_PL, ""},  /* fuzzy */
/* sv */ {IDS_REU_SETTINGS_SV, "REU-Inst�llningar"},

/* en */ {IDS_USERPORT_RS232,    "Userport RS232"},
/* de */ {IDS_USERPORT_RS232_DE, "Userport RS232"},
/* fr */ {IDS_USERPORT_RS232_FR, "Port utilisateur RS232"},
/* it */ {IDS_USERPORT_RS232_IT, "Userport RS232"},
/* nl */ {IDS_USERPORT_RS232_NL, "Userport RS232"},
/* pl */ {IDS_USERPORT_RS232_PL, ""},  /* fuzzy */
/* sv */ {IDS_USERPORT_RS232_SV, "Anv�ndarportens RS232"},

/* en */ {IDS_USERPORT_DEVICE,    "Userport RS232 Device"},
/* de */ {IDS_USERPORT_DEVICE_DE, "Userport RS232 Ger�t"},
/* fr */ {IDS_USERPORT_DEVICE_FR, "P�riph�rique de port utilisateur RS232"},
/* it */ {IDS_USERPORT_DEVICE_IT, "Periferica RS232 su Userport"},
/* nl */ {IDS_USERPORT_DEVICE_NL, "Userport RS232 apparaat"},
/* pl */ {IDS_USERPORT_DEVICE_PL, ""},  /* fuzzy */
/* sv */ {IDS_USERPORT_DEVICE_SV, "Anv�ndarportens RS232-enhet"},

/* en */ {IDS_USERPORT_BAUD_RATE,    "Userport RS232 baud rate"},
/* de */ {IDS_USERPORT_BAUD_RATE_DE, "Userport RS232 Baud Rate"},
/* fr */ {IDS_USERPORT_BAUD_RATE_FR, "P�riph�rique de port utilisateur RS232"},
/* it */ {IDS_USERPORT_BAUD_RATE_IT, "Velocit� della RS232 su Userport"},
/* nl */ {IDS_USERPORT_BAUD_RATE_NL, "Userport RS232 baudrate"},
/* pl */ {IDS_USERPORT_BAUD_RATE_PL, ""},  /* fuzzy */
/* sv */ {IDS_USERPORT_BAUD_RATE_SV, "Anv�ndarportens RS232-baudhastighet"},

/* en */ {IDS_RS232_USERPORT_SETTINGS,    "RS232 Userport Settings"},
/* de */ {IDS_RS232_USERPORT_SETTINGS_DE, "RS232 Userport Einstellungen"},
/* fr */ {IDS_RS232_USERPORT_SETTINGS_FR, "Parametres RS232 Userport"},
/* it */ {IDS_RS232_USERPORT_SETTINGS_IT, "Impostazioni RS232 su userport"},
/* nl */ {IDS_RS232_USERPORT_SETTINGS_NL, "RS232 Userport Instellingen"},
/* pl */ {IDS_RS232_USERPORT_SETTINGS_PL, ""},  /* fuzzy */
/* sv */ {IDS_RS232_USERPORT_SETTINGS_SV, "RS232-anv�ndarportinst�llningar"},

/* en */ {IDS_FAST,    "fast"},
/* de */ {IDS_FAST_DE, "Schnell"},
/* fr */ {IDS_FAST_FR, "rapide"},
/* it */ {IDS_FAST_IT, "veloce"},
/* nl */ {IDS_FAST_NL, "snel"},
/* pl */ {IDS_FAST_PL, ""},  /* fuzzy */
/* sv */ {IDS_FAST_SV, "snabbt"},

/* en */ {IDS_INTERPOLATING,    "interpolating"},
/* de */ {IDS_INTERPOLATING_DE, "Interpolierend"},
/* fr */ {IDS_INTERPOLATING_FR, "par interpolation"},
/* it */ {IDS_INTERPOLATING_IT, "Interpolazione"},
/* nl */ {IDS_INTERPOLATING_NL, "interpolating"},
/* pl */ {IDS_INTERPOLATING_PL, ""},  /* fuzzy */
/* sv */ {IDS_INTERPOLATING_SV, "interpolerande"},

/* en */ {IDS_RESAMPLING,    "resampling"},
/* de */ {IDS_RESAMPLING_DE, "Resampling"},
/* fr */ {IDS_RESAMPLING_FR, "r��chantillonage"},
/* it */ {IDS_RESAMPLING_IT, "ricampionamento"},
/* nl */ {IDS_RESAMPLING_NL, "resampling"},
/* pl */ {IDS_RESAMPLING_PL, ""},  /* fuzzy */
/* sv */ {IDS_RESAMPLING_SV, "omsamplande"},

/* en */ {IDS_FAST_RESAMPLING,    "fast resampling"},
/* de */ {IDS_FAST_RESAMPLING_DE, "Schnelles Resampling"},
/* fr */ {IDS_FAST_RESAMPLING_FR, "r��chantillonage rapide"},
/* it */ {IDS_FAST_RESAMPLING_IT, "ricampionamento veloce"},
/* nl */ {IDS_FAST_RESAMPLING_NL, "snelle resampling"},
/* pl */ {IDS_FAST_RESAMPLING_PL, ""},  /* fuzzy */
/* sv */ {IDS_FAST_RESAMPLING_SV, "snabb omsampling"},

/* en */ {IDS_6581_OLD,    "6581 (old)"},
/* de */ {IDS_6581_OLD_DE, "6581 (alt)"},
/* fr */ {IDS_6581_OLD_FR, "6581 (ancien)"},
/* it */ {IDS_6581_OLD_IT, "6581 (vecchio)"},
/* nl */ {IDS_6581_OLD_NL, "6581 (oud)"},
/* pl */ {IDS_6581_OLD_PL, ""},  /* fuzzy */
/* sv */ {IDS_6581_OLD_SV, "6581 (gammal)"},

/* en */ {IDS_8580_NEW,    "8580 (new)"},
/* de */ {IDS_8580_NEW_DE, "8580 (neu)"},
/* fr */ {IDS_8580_NEW_FR, "8580 (nouveau)"},
/* it */ {IDS_8580_NEW_IT, "8580 (nuovo)"},
/* nl */ {IDS_8580_NEW_NL, "8580 (nieuw)"},
/* pl */ {IDS_8580_NEW_PL, ""},  /* fuzzy */
/* sv */ {IDS_8580_NEW_SV, "8580 (ny)"},

/* en */ {IDS_SID_MODEL,    "SID Model"},
/* de */ {IDS_SID_MODEL_DE, "SID Modell"},
/* fr */ {IDS_SID_MODEL_FR, "Modele SID"},
/* it */ {IDS_SID_MODEL_IT, "Modello SID"},
/* nl */ {IDS_SID_MODEL_NL, "SID Model"},
/* pl */ {IDS_SID_MODEL_PL, ""},  /* fuzzy */
/* sv */ {IDS_SID_MODEL_SV, "SID-modell"},

/* en */ {IDS_SID_STEREO,    "SID Stereo"},
/* de */ {IDS_SID_STEREO_DE, "SID Stereo"},
/* fr */ {IDS_SID_STEREO_FR, "SID St�r�o"},
/* it */ {IDS_SID_STEREO_IT, "SID Stereo"},
/* nl */ {IDS_SID_STEREO_NL, "Stereo SID"},
/* pl */ {IDS_SID_STEREO_PL, ""},  /* fuzzy */
/* sv */ {IDS_SID_STEREO_SV, "SID-stereo"},

/* en */ {IDS_STEREO_SID_AT,    "Stereo SID at"},
/* de */ {IDS_STEREO_SID_AT_DE, "Stereo SID bei"},
/* fr */ {IDS_STEREO_SID_AT_FR, "SID St�r�o  "},
/* it */ {IDS_STEREO_SID_AT_IT, "SID Stereo a"},
/* nl */ {IDS_STEREO_SID_AT_NL, "Stereo SID op"},
/* pl */ {IDS_STEREO_SID_AT_PL, ""},  /* fuzzy */
/* sv */ {IDS_STEREO_SID_AT_SV, "Stereo-SID pa"},

/* en */ {IDS_SID_FILTERS,    "SID Filters"},
/* de */ {IDS_SID_FILTERS_DE, "SID Filter"},
/* fr */ {IDS_SID_FILTERS_FR, "Filtres SID"},
/* it */ {IDS_SID_FILTERS_IT, "Filtri SID"},
/* nl */ {IDS_SID_FILTERS_NL, "SID Filters"},
/* pl */ {IDS_SID_FILTERS_PL, ""},  /* fuzzy */
/* sv */ {IDS_SID_FILTERS_SV, "SID-filter"},

/* en */ {IDS_SAMPLE_METHOD,    "Sample method"},
/* de */ {IDS_SAMPLE_METHOD_DE, "Sample Methode"},
/* fr */ {IDS_SAMPLE_METHOD_FR, "M�thode d'�chantillonnage"},
/* it */ {IDS_SAMPLE_METHOD_IT, "Metodo di campionamento"},
/* nl */ {IDS_SAMPLE_METHOD_NL, "Sample methode"},
/* pl */ {IDS_SAMPLE_METHOD_PL, ""},  /* fuzzy */
/* sv */ {IDS_SAMPLE_METHOD_SV, "Samplingsmetod"},

/* en */ {IDS_PASSBAND_0_90,    "Passband (0-90%)"},
/* de */ {IDS_PASSBAND_0_90_DE, "Passband (0-90%)"},
/* fr */ {IDS_PASSBAND_0_90_FR, "Bande passante (0-90%)"},
/* it */ {IDS_PASSBAND_0_90_IT, "Passabanda (0-90%)"},
/* nl */ {IDS_PASSBAND_0_90_NL, "Passband (0-90%)"},
/* pl */ {IDS_PASSBAND_0_90_PL, ""},  /* fuzzy */
/* sv */ {IDS_PASSBAND_0_90_SV, "Passband (0-90%)"},

/* en */ {IDS_NOT_IMPLEMENTED_YET,    "Not implemented yet!"},
/* de */ {IDS_NOT_IMPLEMENTED_YET_DE, ""},  /* fuzzy */
/* fr */ {IDS_NOT_IMPLEMENTED_YET_FR, ""},  /* fuzzy */
/* it */ {IDS_NOT_IMPLEMENTED_YET_IT, "Non ancora implementato!"},
/* nl */ {IDS_NOT_IMPLEMENTED_YET_NL, "Nog niet ondersteund!"},
/* pl */ {IDS_NOT_IMPLEMENTED_YET_PL, ""},  /* fuzzy */
/* sv */ {IDS_NOT_IMPLEMENTED_YET_SV, "Ej implementerat!"},

/* en */ {IDS_SID_SETTINGS,    "SID Settings"},
/* de */ {IDS_SID_SETTINGS_DE, "SID Einstellungen"},
/* fr */ {IDS_SID_SETTINGS_FR, "Parametres SID"},
/* it */ {IDS_SID_SETTINGS_IT, "Impostazioni SID"},
/* nl */ {IDS_SID_SETTINGS_NL, "SID instellingen"},
/* pl */ {IDS_SID_SETTINGS_PL, ""},  /* fuzzy */
/* sv */ {IDS_SID_SETTINGS_SV, "SID-inst�llningar"},

/* en */ {IDS_FLEXIBLE,    "Flexible"},
/* de */ {IDS_FLEXIBLE_DE, "Flexibel"},
/* fr */ {IDS_FLEXIBLE_FR, "Flexible"},
/* it */ {IDS_FLEXIBLE_IT, "Flessibile"},
/* nl */ {IDS_FLEXIBLE_NL, "Flexibel"},
/* pl */ {IDS_FLEXIBLE_PL, ""},  /* fuzzy */
/* sv */ {IDS_FLEXIBLE_SV, "Flexibel"},

/* en */ {IDS_ADJUSTING,    "Adjusting"},
/* de */ {IDS_ADJUSTING_DE, "Anpassend"},
/* fr */ {IDS_ADJUSTING_FR, "Ajust�"},
/* it */ {IDS_ADJUSTING_IT, "Adattamento in corso"},
/* nl */ {IDS_ADJUSTING_NL, "Aanpassend"},
/* pl */ {IDS_ADJUSTING_PL, ""},  /* fuzzy */
/* sv */ {IDS_ADJUSTING_SV, "Anpassande"},

/* en */ {IDS_EXACT,    "Exact"},
/* de */ {IDS_EXACT_DE, "Exakt"},
/* fr */ {IDS_EXACT_FR, "Exact"},
/* it */ {IDS_EXACT_IT, "Esatta"},
/* nl */ {IDS_EXACT_NL, "Exact"},
/* pl */ {IDS_EXACT_PL, ""},  /* fuzzy */
/* sv */ {IDS_EXACT_SV, "Exakt"},

/* en */ {IDS_SAMPLE_RATE,    "Sample Rate"},
/* de */ {IDS_SAMPLE_RATE_DE, "Sample Rate"},
/* fr */ {IDS_SAMPLE_RATE_FR, "Taux d'�chantillonage"},
/* it */ {IDS_SAMPLE_RATE_IT, "Frequenza di campionamento"},
/* nl */ {IDS_SAMPLE_RATE_NL, "Sample snelheid"},
/* pl */ {IDS_SAMPLE_RATE_PL, ""},  /* fuzzy */
/* sv */ {IDS_SAMPLE_RATE_SV, "Samplingshastighet"},

/* en */ {IDS_BUFFER_SIZE,    "Buffer Size"},
/* de */ {IDS_BUFFER_SIZE_DE, "Puffergr��e"},
/* fr */ {IDS_BUFFER_SIZE_FR, "Taille du tampon"},
/* it */ {IDS_BUFFER_SIZE_IT, "Dimensione buffer"},
/* nl */ {IDS_BUFFER_SIZE_NL, "Buffer grootte"},
/* pl */ {IDS_BUFFER_SIZE_PL, ""},  /* fuzzy */
/* sv */ {IDS_BUFFER_SIZE_SV, "Buffertstorlek"},

/* en */ {IDS_OVERSAMPLE,    "Oversample"},
/* de */ {IDS_OVERSAMPLE_DE, "Oversample"},
/* fr */ {IDS_OVERSAMPLE_FR, "Sur�chantillonage"},
/* it */ {IDS_OVERSAMPLE_IT, "Sovracampionamento"},
/* nl */ {IDS_OVERSAMPLE_NL, "Oversample"},
/* pl */ {IDS_OVERSAMPLE_PL, ""},  /* fuzzy */
/* sv */ {IDS_OVERSAMPLE_SV, "-versampling"},

/* en */ {IDS_SPEED_ADJUSTMENT,    "Speed Adjustment"},
/* de */ {IDS_SPEED_ADJUSTMENT_DE, ""},  /* fuzzy */
/* fr */ {IDS_SPEED_ADJUSTMENT_FR, ""},  /* fuzzy */
/* it */ {IDS_SPEED_ADJUSTMENT_IT, "Adattamento della Velocit�"},
/* nl */ {IDS_SPEED_ADJUSTMENT_NL, "Snelheid aanpassing"},
/* pl */ {IDS_SPEED_ADJUSTMENT_PL, ""},  /* fuzzy */
/* sv */ {IDS_SPEED_ADJUSTMENT_SV, "Hastighetsjustering"},

/* en */ {IDS_SOUND_SETTINGS,    "Sound Settings"},
/* de */ {IDS_SOUND_SETTINGS_DE, "Sound Einstellungen"},
/* fr */ {IDS_SOUND_SETTINGS_FR, "Parametres son"},
/* it */ {IDS_SOUND_SETTINGS_IT, "Impostazioni suono"},
/* nl */ {IDS_SOUND_SETTINGS_NL, "Geluid instellingen"},
/* pl */ {IDS_SOUND_SETTINGS_PL, ""},  /* fuzzy */
/* sv */ {IDS_SOUND_SETTINGS_SV, "Ljudinst�llningar"},

/* en */ {IDS_VIC_SETTINGS,    "VIC Settings"},
/* de */ {IDS_VIC_SETTINGS_DE, "VIC Einstellungen"},
/* fr */ {IDS_VIC_SETTINGS_FR, "Parametres VIC"},
/* it */ {IDS_VIC_SETTINGS_IT, "Impostazioni VIC"},
/* nl */ {IDS_VIC_SETTINGS_NL, "VIC instellingen"},
/* pl */ {IDS_VIC_SETTINGS_PL, ""},  /* fuzzy */
/* sv */ {IDS_VIC_SETTINGS_SV, "VIC-inst�llningar"},

/* en */ {IDS_SPRITE_SPRITE_COL,    "Sprite Sprite Collisions"},
/* de */ {IDS_SPRITE_SPRITE_COL_DE, "Sprite-sprite Kollisionen"},
/* fr */ {IDS_SPRITE_SPRITE_COL_FR, "Collisions sprite-sprite"},
/* it */ {IDS_SPRITE_SPRITE_COL_IT, "Collisioni sprite-sprite"},
/* nl */ {IDS_SPRITE_SPRITE_COL_NL, "Sprite-sprite botsingen"},
/* pl */ {IDS_SPRITE_SPRITE_COL_PL, ""},  /* fuzzy */
/* sv */ {IDS_SPRITE_SPRITE_COL_SV, "Sprite-sprite-kollisioner"},

/* en */ {IDS_SPRITE_BACKGROUND_COL,    "Sprite Background Collisions"},
/* de */ {IDS_SPRITE_BACKGROUND_COL_DE, "Sprite-Hintergrund Kollisionen"},
/* fr */ {IDS_SPRITE_BACKGROUND_COL_FR, "Collisions sprite-arriere-plan"},
/* it */ {IDS_SPRITE_BACKGROUND_COL_IT, "Collisioni sprite-sfondo"},
/* nl */ {IDS_SPRITE_BACKGROUND_COL_NL, "Sprite-achtergrond botsingen"},
/* pl */ {IDS_SPRITE_BACKGROUND_COL_PL, ""},  /* fuzzy */
/* sv */ {IDS_SPRITE_BACKGROUND_COL_SV, "Sprite-bakgrundkollisioner"},

/* en */ {IDS_NEW_LUMINANCES,    "New Luminances"},
/* de */ {IDS_NEW_LUMINANCES_DE, "Neue Helligkeitsemulation"},
/* fr */ {IDS_NEW_LUMINANCES_FR, "Nouvelles luminescences"},
/* it */ {IDS_NEW_LUMINANCES_IT, "Nuove luminanze"},
/* nl */ {IDS_NEW_LUMINANCES_NL, "Nieuwe kleuren"},
/* pl */ {IDS_NEW_LUMINANCES_PL, ""},  /* fuzzy */
/* sv */ {IDS_NEW_LUMINANCES_SV, "Nya ljusstyrkor"},

/* en */ {IDS_VICII_SETTINGS,    "VIC-II Settings"},
/* de */ {IDS_VICII_SETTINGS_DE, "VIC-II Einstellungen"},
/* fr */ {IDS_VICII_SETTINGS_FR, "Parametres VIC-II"},
/* it */ {IDS_VICII_SETTINGS_IT, "Impostazioni VIC-II"},
/* nl */ {IDS_VICII_SETTINGS_NL, "VIC-II Settings"},
/* pl */ {IDS_VICII_SETTINGS_PL, ""},  /* fuzzy */
/* sv */ {IDS_VICII_SETTINGS_SV, "VIC-II-inst�llningar"},

/* en */ {IDS_ATTACH_TAPE_IMAGE,    "Attach tape image"},
/* de */ {IDS_ATTACH_TAPE_IMAGE_DE, "Band Image einlegen"},
/* fr */ {IDS_ATTACH_TAPE_IMAGE_FR, "Ins�rer une image datassette"},
/* it */ {IDS_ATTACH_TAPE_IMAGE_IT, "Seleziona l'immagine di una cassetta"},
/* nl */ {IDS_ATTACH_TAPE_IMAGE_NL, "Koppel tape bestand"},
/* pl */ {IDS_ATTACH_TAPE_IMAGE_PL, ""},  /* fuzzy */
/* sv */ {IDS_ATTACH_TAPE_IMAGE_SV, "Anslut en bandavbildningsfil"},

/* en */ {IDS_AUTOSTART_IMAGE,    "Autostart disk/tape image"},
/* de */ {IDS_AUTOSTART_IMAGE_DE, "Autostart Disk/Band Image"},
/* fr */ {IDS_AUTOSTART_IMAGE_FR, "D�marrage automatique d'une image datassette"},
/* it */ {IDS_AUTOSTART_IMAGE_IT, "Avvia automaticamente l'immagine di un disco/cassetta"},
/* nl */ {IDS_AUTOSTART_IMAGE_NL, "Autostart disk/tape bestand"},
/* pl */ {IDS_AUTOSTART_IMAGE_PL, ""},  /* fuzzy */
/* sv */ {IDS_AUTOSTART_IMAGE_SV, "Autostarta disk-/bandavbildningsfil"},

};


static int intl_translate_text_table[][countof(language_table)] = {

/* ----------------------- AmigaOS Menu Strings ----------------------- */

/* en */ {IDMS_FILE,
/* de */  IDMS_FILE_DE,
/* fr */  IDMS_FILE_FR,
/* it */  IDMS_FILE_IT,
/* nl */  IDMS_FILE_NL,
/* pl */  IDMS_FILE_PL,
/* sv */  IDMS_FILE_SV},

/* en */ {IDMS_AUTOSTART_IMAGE,
/* de */  IDMS_AUTOSTART_IMAGE_DE,
/* fr */  IDMS_AUTOSTART_IMAGE_FR,
/* it */  IDMS_AUTOSTART_IMAGE_IT,
/* nl */  IDMS_AUTOSTART_IMAGE_NL,
/* pl */  IDMS_AUTOSTART_IMAGE_PL,
/* sv */  IDMS_AUTOSTART_IMAGE_SV},

/* en */ {IDMS_ATTACH_DISK_IMAGE,
/* de */  IDMS_ATTACH_DISK_IMAGE_DE,
/* fr */  IDMS_ATTACH_DISK_IMAGE_FR,
/* it */  IDMS_ATTACH_DISK_IMAGE_IT,
/* nl */  IDMS_ATTACH_DISK_IMAGE_NL,
/* pl */  IDMS_ATTACH_DISK_IMAGE_PL,
/* sv */  IDMS_ATTACH_DISK_IMAGE_SV},

/* en */ {IDMS_DRIVE_8,
/* de */  IDMS_DRIVE_8_DE,
/* fr */  IDMS_DRIVE_8_FR,
/* it */  IDMS_DRIVE_8_IT,
/* nl */  IDMS_DRIVE_8_NL,
/* pl */  IDMS_DRIVE_8_PL,
/* sv */  IDMS_DRIVE_8_SV},

/* en */ {IDMS_DRIVE_9,
/* de */  IDMS_DRIVE_9_DE,
/* fr */  IDMS_DRIVE_9_FR,
/* it */  IDMS_DRIVE_9_IT,
/* nl */  IDMS_DRIVE_9_NL,
/* pl */  IDMS_DRIVE_9_PL,
/* sv */  IDMS_DRIVE_9_SV},

/* en */ {IDMS_DRIVE_10,
/* de */  IDMS_DRIVE_10_DE,
/* fr */  IDMS_DRIVE_10_FR,
/* it */  IDMS_DRIVE_10_IT,
/* nl */  IDMS_DRIVE_10_NL,
/* pl */  IDMS_DRIVE_10_PL,
/* sv */  IDMS_DRIVE_10_SV},

/* en */ {IDMS_DRIVE_11,
/* de */  IDMS_DRIVE_11_DE,
/* fr */  IDMS_DRIVE_11_FR,
/* it */  IDMS_DRIVE_11_IT,
/* nl */  IDMS_DRIVE_11_NL,
/* pl */  IDMS_DRIVE_11_PL,
/* sv */  IDMS_DRIVE_11_SV},

/* en */ {IDMS_DETACH_DISK_IMAGE,
/* de */  IDMS_DETACH_DISK_IMAGE_DE,
/* fr */  IDMS_DETACH_DISK_IMAGE_FR,
/* it */  IDMS_DETACH_DISK_IMAGE_IT,
/* nl */  IDMS_DETACH_DISK_IMAGE_NL,
/* pl */  IDMS_DETACH_DISK_IMAGE_PL,
/* sv */  IDMS_DETACH_DISK_IMAGE_SV},

/* en */ {IDMS_ALL,
/* de */  IDMS_ALL_DE,
/* fr */  IDMS_ALL_FR,
/* it */  IDMS_ALL_IT,
/* nl */  IDMS_ALL_NL,
/* pl */  IDMS_ALL_PL,
/* sv */  IDMS_ALL_SV},

/* en */ {IDMS_FLIP_LIST,
/* de */  IDMS_FLIP_LIST_DE,
/* fr */  IDMS_FLIP_LIST_FR,
/* it */  IDMS_FLIP_LIST_IT,
/* nl */  IDMS_FLIP_LIST_NL,
/* pl */  IDMS_FLIP_LIST_PL,
/* sv */  IDMS_FLIP_LIST_SV},

/* en */ {IDMS_ADD_CURRENT_IMAGE,
/* de */  IDMS_ADD_CURRENT_IMAGE_DE,
/* fr */  IDMS_ADD_CURRENT_IMAGE_FR,
/* it */  IDMS_ADD_CURRENT_IMAGE_IT,
/* nl */  IDMS_ADD_CURRENT_IMAGE_NL,
/* pl */  IDMS_ADD_CURRENT_IMAGE_PL,
/* sv */  IDMS_ADD_CURRENT_IMAGE_SV},

/* en */ {IDMS_REMOVE_CURRENT_IMAGE,
/* de */  IDMS_REMOVE_CURRENT_IMAGE_DE,
/* fr */  IDMS_REMOVE_CURRENT_IMAGE_FR,
/* it */  IDMS_REMOVE_CURRENT_IMAGE_IT,
/* nl */  IDMS_REMOVE_CURRENT_IMAGE_NL,
/* pl */  IDMS_REMOVE_CURRENT_IMAGE_PL,
/* sv */  IDMS_REMOVE_CURRENT_IMAGE_SV},

/* en */ {IDMS_ATTACH_NEXT_IMAGE,
/* de */  IDMS_ATTACH_NEXT_IMAGE_DE,
/* fr */  IDMS_ATTACH_NEXT_IMAGE_FR,
/* it */  IDMS_ATTACH_NEXT_IMAGE_IT,
/* nl */  IDMS_ATTACH_NEXT_IMAGE_NL,
/* pl */  IDMS_ATTACH_NEXT_IMAGE_PL,
/* sv */  IDMS_ATTACH_NEXT_IMAGE_SV},

/* en */ {IDMS_ATTACH_PREVIOUS_IMAGE,
/* de */  IDMS_ATTACH_PREVIOUS_IMAGE_DE,
/* fr */  IDMS_ATTACH_PREVIOUS_IMAGE_FR,
/* it */  IDMS_ATTACH_PREVIOUS_IMAGE_IT,
/* nl */  IDMS_ATTACH_PREVIOUS_IMAGE_NL,
/* pl */  IDMS_ATTACH_PREVIOUS_IMAGE_PL,
/* sv */  IDMS_ATTACH_PREVIOUS_IMAGE_SV},

/* en */ {IDMS_LOAD_FLIP_LIST,
/* de */  IDMS_LOAD_FLIP_LIST_DE,
/* fr */  IDMS_LOAD_FLIP_LIST_FR,
/* it */  IDMS_LOAD_FLIP_LIST_IT,
/* nl */  IDMS_LOAD_FLIP_LIST_NL,
/* pl */  IDMS_LOAD_FLIP_LIST_PL,
/* sv */  IDMS_LOAD_FLIP_LIST_SV},

/* en */ {IDMS_SAVE_FLIP_LIST,
/* de */  IDMS_SAVE_FLIP_LIST_DE,
/* fr */  IDMS_SAVE_FLIP_LIST_FR,
/* it */  IDMS_SAVE_FLIP_LIST_IT,
/* nl */  IDMS_SAVE_FLIP_LIST_NL,
/* pl */  IDMS_SAVE_FLIP_LIST_PL,
/* sv */  IDMS_SAVE_FLIP_LIST_SV},

/* en */ {IDMS_ATTACH_TAPE_IMAGE,
/* de */  IDMS_ATTACH_TAPE_IMAGE_DE,
/* fr */  IDMS_ATTACH_TAPE_IMAGE_FR,
/* it */  IDMS_ATTACH_TAPE_IMAGE_IT,
/* nl */  IDMS_ATTACH_TAPE_IMAGE_NL,
/* pl */  IDMS_ATTACH_TAPE_IMAGE_PL,
/* sv */  IDMS_ATTACH_TAPE_IMAGE_SV},

/* en */ {IDMS_DETACH_TAPE_IMAGE,
/* de */  IDMS_DETACH_TAPE_IMAGE_DE,
/* fr */  IDMS_DETACH_TAPE_IMAGE_FR,
/* it */  IDMS_DETACH_TAPE_IMAGE_IT,
/* nl */  IDMS_DETACH_TAPE_IMAGE_NL,
/* pl */  IDMS_DETACH_TAPE_IMAGE_PL,
/* sv */  IDMS_DETACH_TAPE_IMAGE_SV},

/* en */ {IDMS_DATASSETTE_CONTROL,
/* de */  IDMS_DATASSETTE_CONTROL_DE,
/* fr */  IDMS_DATASSETTE_CONTROL_FR,
/* it */  IDMS_DATASSETTE_CONTROL_IT,
/* nl */  IDMS_DATASSETTE_CONTROL_NL,
/* pl */  IDMS_DATASSETTE_CONTROL_PL,
/* sv */  IDMS_DATASSETTE_CONTROL_SV},

/* en */ {IDMS_STOP,
/* de */  IDMS_STOP_DE,
/* fr */  IDMS_STOP_FR,
/* it */  IDMS_STOP_IT,
/* nl */  IDMS_STOP_NL,
/* pl */  IDMS_STOP_PL,
/* sv */  IDMS_STOP_SV},

/* en */ {IDMS_START,
/* de */  IDMS_START_DE,
/* fr */  IDMS_START_FR,
/* it */  IDMS_START_IT,
/* nl */  IDMS_START_NL,
/* pl */  IDMS_START_PL,
/* sv */  IDMS_START_SV},

/* en */ {IDMS_FORWARD,
/* de */  IDMS_FORWARD_DE,
/* fr */  IDMS_FORWARD_FR,
/* it */  IDMS_FORWARD_IT,
/* nl */  IDMS_FORWARD_NL,
/* pl */  IDMS_FORWARD_PL,
/* sv */  IDMS_FORWARD_SV},

/* en */ {IDMS_REWIND,
/* de */  IDMS_REWIND_DE,
/* fr */  IDMS_REWIND_FR,
/* it */  IDMS_REWIND_IT,
/* nl */  IDMS_REWIND_NL,
/* pl */  IDMS_REWIND_PL,
/* sv */  IDMS_REWIND_SV},

/* en */ {IDMS_RECORD,
/* de */  IDMS_RECORD_DE,
/* fr */  IDMS_RECORD_FR,
/* it */  IDMS_RECORD_IT,
/* nl */  IDMS_RECORD_NL,
/* pl */  IDMS_RECORD_PL,
/* sv */  IDMS_RECORD_SV},

/* en */ {IDMS_RESET,
/* de */  IDMS_RESET_DE,
/* fr */  IDMS_RESET_FR,
/* it */  IDMS_RESET_IT,
/* nl */  IDMS_RESET_NL,
/* pl */  IDMS_RESET_PL,
/* sv */  IDMS_RESET_SV},

/* en */ {IDMS_RESET_COUNTER,
/* de */  IDMS_RESET_COUNTER_DE,
/* fr */  IDMS_RESET_COUNTER_FR,
/* it */  IDMS_RESET_COUNTER_IT,
/* nl */  IDMS_RESET_COUNTER_NL,
/* pl */  IDMS_RESET_COUNTER_PL,
/* sv */  IDMS_RESET_COUNTER_SV},

/* en */ {IDMS_ATTACH_CART_IMAGE,
/* de */  IDMS_ATTACH_CART_IMAGE_DE,
/* fr */  IDMS_ATTACH_CART_IMAGE_FR,
/* it */  IDMS_ATTACH_CART_IMAGE_IT,
/* nl */  IDMS_ATTACH_CART_IMAGE_NL,
/* pl */  IDMS_ATTACH_CART_IMAGE_PL,
/* sv */  IDMS_ATTACH_CART_IMAGE_SV},

/* en */ {IDMS_4_8_16KB_AT_2000,
/* de */  IDMS_4_8_16KB_AT_2000_DE,
/* fr */  IDMS_4_8_16KB_AT_2000_FR,
/* it */  IDMS_4_8_16KB_AT_2000_IT,
/* nl */  IDMS_4_8_16KB_AT_2000_NL,
/* pl */  IDMS_4_8_16KB_AT_2000_PL,
/* sv */  IDMS_4_8_16KB_AT_2000_SV},

/* en */ {IDMS_4_8_16KB_AT_4000,
/* de */  IDMS_4_8_16KB_AT_4000_DE,
/* fr */  IDMS_4_8_16KB_AT_4000_FR,
/* it */  IDMS_4_8_16KB_AT_4000_IT,
/* nl */  IDMS_4_8_16KB_AT_4000_NL,
/* pl */  IDMS_4_8_16KB_AT_4000_PL,
/* sv */  IDMS_4_8_16KB_AT_4000_SV},

/* en */ {IDMS_4_8_16KB_AT_6000,
/* de */  IDMS_4_8_16KB_AT_6000_DE,
/* fr */  IDMS_4_8_16KB_AT_6000_FR,
/* it */  IDMS_4_8_16KB_AT_6000_IT,
/* nl */  IDMS_4_8_16KB_AT_6000_NL,
/* pl */  IDMS_4_8_16KB_AT_6000_PL,
/* sv */  IDMS_4_8_16KB_AT_6000_SV},

/* en */ {IDMS_4_8KB_AT_A000,
/* de */  IDMS_4_8KB_AT_A000_DE,
/* fr */  IDMS_4_8KB_AT_A000_FR,
/* it */  IDMS_4_8KB_AT_A000_IT,
/* nl */  IDMS_4_8KB_AT_A000_NL,
/* pl */  IDMS_4_8KB_AT_A000_PL,
/* sv */  IDMS_4_8KB_AT_A000_SV},

/* en */ {IDMS_4KB_AT_B000,
/* de */  IDMS_4KB_AT_B000_DE,
/* fr */  IDMS_4KB_AT_B000_FR,
/* it */  IDMS_4KB_AT_B000_IT,
/* nl */  IDMS_4KB_AT_B000_NL,
/* pl */  IDMS_4KB_AT_B000_PL,
/* sv */  IDMS_4KB_AT_B000_SV},

/* en */ {IDMS_DETACH_CART_IMAGE,
/* de */  IDMS_DETACH_CART_IMAGE_DE,
/* fr */  IDMS_DETACH_CART_IMAGE_FR,
/* it */  IDMS_DETACH_CART_IMAGE_IT,
/* nl */  IDMS_DETACH_CART_IMAGE_NL,
/* pl */  IDMS_DETACH_CART_IMAGE_PL,
/* sv */  IDMS_DETACH_CART_IMAGE_SV},

/* en */ {IDMS_C1_LOW_IMAGE,
/* de */  IDMS_C1_LOW_IMAGE_DE,
/* fr */  IDMS_C1_LOW_IMAGE_FR,
/* it */  IDMS_C1_LOW_IMAGE_IT,
/* nl */  IDMS_C1_LOW_IMAGE_NL,
/* pl */  IDMS_C1_LOW_IMAGE_PL,
/* sv */  IDMS_C1_LOW_IMAGE_SV},

/* en */ {IDMS_C1_HIGH_IMAGE,
/* de */  IDMS_C1_HIGH_IMAGE_DE,
/* fr */  IDMS_C1_HIGH_IMAGE_FR,
/* it */  IDMS_C1_HIGH_IMAGE_IT,
/* nl */  IDMS_C1_HIGH_IMAGE_NL,
/* pl */  IDMS_C1_HIGH_IMAGE_PL,
/* sv */  IDMS_C1_HIGH_IMAGE_SV},

/* en */ {IDMS_C2_LOW_IMAGE,
/* de */  IDMS_C2_LOW_IMAGE_DE,
/* fr */  IDMS_C2_LOW_IMAGE_FR,
/* it */  IDMS_C2_LOW_IMAGE_IT,
/* nl */  IDMS_C2_LOW_IMAGE_NL,
/* pl */  IDMS_C2_LOW_IMAGE_PL,
/* sv */  IDMS_C2_LOW_IMAGE_SV},

/* en */ {IDMS_C2_HIGH_IMAGE,
/* de */  IDMS_C2_HIGH_IMAGE_DE,
/* fr */  IDMS_C2_HIGH_IMAGE_FR,
/* it */  IDMS_C2_HIGH_IMAGE_IT,
/* nl */  IDMS_C2_HIGH_IMAGE_NL,
/* pl */  IDMS_C2_HIGH_IMAGE_PL,
/* sv */  IDMS_C2_HIGH_IMAGE_SV},

/* en */ {IDMS_FUNCTION_LOW_3PLUS1,
/* de */  IDMS_FUNCTION_LOW_3PLUS1_DE,
/* fr */  IDMS_FUNCTION_LOW_3PLUS1_FR,
/* it */  IDMS_FUNCTION_LOW_3PLUS1_IT,
/* nl */  IDMS_FUNCTION_LOW_3PLUS1_NL,
/* pl */  IDMS_FUNCTION_LOW_3PLUS1_PL,
/* sv */  IDMS_FUNCTION_LOW_3PLUS1_SV},

/* en */ {IDMS_FUNCTION_HIGH_3PLUS1,
/* de */  IDMS_FUNCTION_HIGH_3PLUS1_DE,
/* fr */  IDMS_FUNCTION_HIGH_3PLUS1_FR,
/* it */  IDMS_FUNCTION_HIGH_3PLUS1_IT,
/* nl */  IDMS_FUNCTION_HIGH_3PLUS1_NL,
/* pl */  IDMS_FUNCTION_HIGH_3PLUS1_PL,
/* sv */  IDMS_FUNCTION_HIGH_3PLUS1_SV},

/* en */ {IDMS_CRT_IMAGE,
/* de */  IDMS_CRT_IMAGE_DE,
/* fr */  IDMS_CRT_IMAGE_FR,
/* it */  IDMS_CRT_IMAGE_IT,
/* nl */  IDMS_CRT_IMAGE_NL,
/* pl */  IDMS_CRT_IMAGE_PL,
/* sv */  IDMS_CRT_IMAGE_SV},

/* en */ {IDMS_GENERIC_8KB_IMAGE,
/* de */  IDMS_GENERIC_8KB_IMAGE_DE,
/* fr */  IDMS_GENERIC_8KB_IMAGE_FR,
/* it */  IDMS_GENERIC_8KB_IMAGE_IT,
/* nl */  IDMS_GENERIC_8KB_IMAGE_NL,
/* pl */  IDMS_GENERIC_8KB_IMAGE_PL,
/* sv */  IDMS_GENERIC_8KB_IMAGE_SV},

/* en */ {IDMS_GENERIC_16KB_IMAGE,
/* de */  IDMS_GENERIC_16KB_IMAGE_DE,
/* fr */  IDMS_GENERIC_16KB_IMAGE_FR,
/* it */  IDMS_GENERIC_16KB_IMAGE_IT,
/* nl */  IDMS_GENERIC_16KB_IMAGE_NL,
/* pl */  IDMS_GENERIC_16KB_IMAGE_PL,
/* sv */  IDMS_GENERIC_16KB_IMAGE_SV},

/* en */ {IDMS_ACTION_REPLAY_IMAGE,
/* de */  IDMS_ACTION_REPLAY_IMAGE_DE,
/* fr */  IDMS_ACTION_REPLAY_IMAGE_FR,
/* it */  IDMS_ACTION_REPLAY_IMAGE_IT,
/* nl */  IDMS_ACTION_REPLAY_IMAGE_NL,
/* pl */  IDMS_ACTION_REPLAY_IMAGE_PL,
/* sv */  IDMS_ACTION_REPLAY_IMAGE_SV},

/* en */ {IDMS_ATOMIC_POWER_IMAGE,
/* de */  IDMS_ATOMIC_POWER_IMAGE_DE,
/* fr */  IDMS_ATOMIC_POWER_IMAGE_FR,
/* it */  IDMS_ATOMIC_POWER_IMAGE_IT,
/* nl */  IDMS_ATOMIC_POWER_IMAGE_NL,
/* pl */  IDMS_ATOMIC_POWER_IMAGE_PL,
/* sv */  IDMS_ATOMIC_POWER_IMAGE_SV},

/* en */ {IDMS_EPYX_FASTLOAD_IMAGE,
/* de */  IDMS_EPYX_FASTLOAD_IMAGE_DE,
/* fr */  IDMS_EPYX_FASTLOAD_IMAGE_FR,
/* it */  IDMS_EPYX_FASTLOAD_IMAGE_IT,
/* nl */  IDMS_EPYX_FASTLOAD_IMAGE_NL,
/* pl */  IDMS_EPYX_FASTLOAD_IMAGE_PL,
/* sv */  IDMS_EPYX_FASTLOAD_IMAGE_SV},

/* en */ {IDMS_IEEE488_INTERFACE_IMAGE,
/* de */  IDMS_IEEE488_INTERFACE_IMAGE_DE,
/* fr */  IDMS_IEEE488_INTERFACE_IMAGE_FR,
/* it */  IDMS_IEEE488_INTERFACE_IMAGE_IT,
/* nl */  IDMS_IEEE488_INTERFACE_IMAGE_NL,
/* pl */  IDMS_IEEE488_INTERFACE_IMAGE_PL,
/* sv */  IDMS_IEEE488_INTERFACE_IMAGE_SV},

/* en */ {IDMS_RETRO_REPLAY_IMAGE,
/* de */  IDMS_RETRO_REPLAY_IMAGE_DE,
/* fr */  IDMS_RETRO_REPLAY_IMAGE_FR,
/* it */  IDMS_RETRO_REPLAY_IMAGE_IT,
/* nl */  IDMS_RETRO_REPLAY_IMAGE_NL,
/* pl */  IDMS_RETRO_REPLAY_IMAGE_PL,
/* sv */  IDMS_RETRO_REPLAY_IMAGE_SV},

/* en */ {IDMS_IDE64_INTERFACE_IMAGE,
/* de */  IDMS_IDE64_INTERFACE_IMAGE_DE,
/* fr */  IDMS_IDE64_INTERFACE_IMAGE_FR,
/* it */  IDMS_IDE64_INTERFACE_IMAGE_IT,
/* nl */  IDMS_IDE64_INTERFACE_IMAGE_NL,
/* pl */  IDMS_IDE64_INTERFACE_IMAGE_PL,
/* sv */  IDMS_IDE64_INTERFACE_IMAGE_SV},

/* en */ {IDMS_SUPER_SNAPSHOT_4_IMAGE,
/* de */  IDMS_SUPER_SNAPSHOT_4_IMAGE_DE,
/* fr */  IDMS_SUPER_SNAPSHOT_4_IMAGE_FR,
/* it */  IDMS_SUPER_SNAPSHOT_4_IMAGE_IT,
/* nl */  IDMS_SUPER_SNAPSHOT_4_IMAGE_NL,
/* pl */  IDMS_SUPER_SNAPSHOT_4_IMAGE_PL,
/* sv */  IDMS_SUPER_SNAPSHOT_4_IMAGE_SV},

/* en */ {IDMS_SUPER_SNAPSHOT_5_IMAGE,
/* de */  IDMS_SUPER_SNAPSHOT_5_IMAGE_DE,
/* fr */  IDMS_SUPER_SNAPSHOT_5_IMAGE_FR,
/* it */  IDMS_SUPER_SNAPSHOT_5_IMAGE_IT,
/* nl */  IDMS_SUPER_SNAPSHOT_5_IMAGE_NL,
/* pl */  IDMS_SUPER_SNAPSHOT_5_IMAGE_PL,
/* sv */  IDMS_SUPER_SNAPSHOT_5_IMAGE_SV},

/* en */ {IDMS_STRUCTURED_BASIC_IMAGE,
/* de */  IDMS_STRUCTURED_BASIC_IMAGE_DE,
/* fr */  IDMS_STRUCTURED_BASIC_IMAGE_FR,
/* it */  IDMS_STRUCTURED_BASIC_IMAGE_IT,
/* nl */  IDMS_STRUCTURED_BASIC_IMAGE_NL,
/* pl */  IDMS_STRUCTURED_BASIC_IMAGE_PL,
/* sv */  IDMS_STRUCTURED_BASIC_IMAGE_SV},

/* en */ {IDMS_EXPERT_CART,
/* de */  IDMS_EXPERT_CART_DE,
/* fr */  IDMS_EXPERT_CART_FR,
/* it */  IDMS_EXPERT_CART_IT,
/* nl */  IDMS_EXPERT_CART_NL,
/* pl */  IDMS_EXPERT_CART_PL,
/* sv */  IDMS_EXPERT_CART_SV},

/* en */ {IDMS_ENABLE,
/* de */  IDMS_ENABLE_DE,
/* fr */  IDMS_ENABLE_FR,
/* it */  IDMS_ENABLE_IT,
/* nl */  IDMS_ENABLE_NL,
/* pl */  IDMS_ENABLE_PL,
/* sv */  IDMS_ENABLE_SV},

/* en */ {IDMS_OFF,
/* de */  IDMS_OFF_DE,
/* fr */  IDMS_OFF_FR,
/* it */  IDMS_OFF_IT,
/* nl */  IDMS_OFF_NL,
/* pl */  IDMS_OFF_PL,
/* sv */  IDMS_OFF_SV},

/* en */ {IDMS_PRG,
/* de */  IDMS_PRG_DE,
/* fr */  IDMS_PRG_FR,
/* it */  IDMS_PRG_IT,
/* nl */  IDMS_PRG_NL,
/* pl */  IDMS_PRG_PL,
/* sv */  IDMS_PRG_SV},

/* en */ {IDMS_ON,
/* de */  IDMS_ON_DE,
/* fr */  IDMS_ON_FR,
/* it */  IDMS_ON_IT,
/* nl */  IDMS_ON_NL,
/* pl */  IDMS_ON_PL,
/* sv */  IDMS_ON_SV},

/* en */ {IDMS_SET_CART_AS_DEFAULT,
/* de */  IDMS_SET_CART_AS_DEFAULT_DE,
/* fr */  IDMS_SET_CART_AS_DEFAULT_FR,
/* it */  IDMS_SET_CART_AS_DEFAULT_IT,
/* nl */  IDMS_SET_CART_AS_DEFAULT_NL,
/* pl */  IDMS_SET_CART_AS_DEFAULT_PL,
/* sv */  IDMS_SET_CART_AS_DEFAULT_SV},

/* en */ {IDMS_RESET_ON_CART_CHANGE,
/* de */  IDMS_RESET_ON_CART_CHANGE_DE,
/* fr */  IDMS_RESET_ON_CART_CHANGE_FR,
/* it */  IDMS_RESET_ON_CART_CHANGE_IT,
/* nl */  IDMS_RESET_ON_CART_CHANGE_NL,
/* pl */  IDMS_RESET_ON_CART_CHANGE_PL,
/* sv */  IDMS_RESET_ON_CART_CHANGE_SV},

/* en */ {IDMS_CART_FREEZE,
/* de */  IDMS_CART_FREEZE_DE,
/* fr */  IDMS_CART_FREEZE_FR,
/* it */  IDMS_CART_FREEZE_IT,
/* nl */  IDMS_CART_FREEZE_NL,
/* pl */  IDMS_CART_FREEZE_PL,
/* sv */  IDMS_CART_FREEZE_SV},

/* en */ {IDMS_PAUSE,
/* de */  IDMS_PAUSE_DE,
/* fr */  IDMS_PAUSE_FR,
/* it */  IDMS_PAUSE_IT,
/* nl */  IDMS_PAUSE_NL,
/* pl */  IDMS_PAUSE_PL,
/* sv */  IDMS_PAUSE_SV},

/* en */ {IDMS_MONITOR,
/* de */  IDMS_MONITOR_DE,
/* fr */  IDMS_MONITOR_FR,
/* it */  IDMS_MONITOR_IT,
/* nl */  IDMS_MONITOR_NL,
/* pl */  IDMS_MONITOR_PL,
/* sv */  IDMS_MONITOR_SV},

/* en */ {IDMS_HARD,
/* de */  IDMS_HARD_DE,
/* fr */  IDMS_HARD_FR,
/* it */  IDMS_HARD_IT,
/* nl */  IDMS_HARD_NL,
/* pl */  IDMS_HARD_PL,
/* sv */  IDMS_HARD_SV},

/* en */ {IDMS_SOFT,
/* de */  IDMS_SOFT_DE,
/* fr */  IDMS_SOFT_FR,
/* it */  IDMS_SOFT_IT,
/* nl */  IDMS_SOFT_NL,
/* pl */  IDMS_SOFT_PL,
/* sv */  IDMS_SOFT_SV},

/* en */ {IDMS_EXIT,
/* de */  IDMS_EXIT_DE,
/* fr */  IDMS_EXIT_FR,
/* it */  IDMS_EXIT_IT,
/* nl */  IDMS_EXIT_NL,
/* pl */  IDMS_EXIT_PL,
/* sv */  IDMS_EXIT_SV},

/* en */ {IDMS_SNAPSHOT,
/* de */  IDMS_SNAPSHOT_DE,
/* fr */  IDMS_SNAPSHOT_FR,
/* it */  IDMS_SNAPSHOT_IT,
/* nl */  IDMS_SNAPSHOT_NL,
/* pl */  IDMS_SNAPSHOT_PL,
/* sv */  IDMS_SNAPSHOT_SV},

/* en */ {IDMS_LOAD_SNAPSHOT_IMAGE,
/* de */  IDMS_LOAD_SNAPSHOT_IMAGE_DE,
/* fr */  IDMS_LOAD_SNAPSHOT_IMAGE_FR,
/* it */  IDMS_LOAD_SNAPSHOT_IMAGE_IT,
/* nl */  IDMS_LOAD_SNAPSHOT_IMAGE_NL,
/* pl */  IDMS_LOAD_SNAPSHOT_IMAGE_PL,
/* sv */  IDMS_LOAD_SNAPSHOT_IMAGE_SV},

/* en */ {IDMS_SAVE_SNAPSHOT_IMAGE,
/* de */  IDMS_SAVE_SNAPSHOT_IMAGE_DE,
/* fr */  IDMS_SAVE_SNAPSHOT_IMAGE_FR,
/* it */  IDMS_SAVE_SNAPSHOT_IMAGE_IT,
/* nl */  IDMS_SAVE_SNAPSHOT_IMAGE_NL,
/* pl */  IDMS_SAVE_SNAPSHOT_IMAGE_PL,
/* sv */  IDMS_SAVE_SNAPSHOT_IMAGE_SV},

/* en */ {IDMS_LOAD_QUICKSNAPSHOT_IMAGE,
/* de */  IDMS_LOAD_QUICKSNAPSHOT_IMAGE_DE,
/* fr */  IDMS_LOAD_QUICKSNAPSHOT_IMAGE_FR,
/* it */  IDMS_LOAD_QUICKSNAPSHOT_IMAGE_IT,
/* nl */  IDMS_LOAD_QUICKSNAPSHOT_IMAGE_NL,
/* pl */  IDMS_LOAD_QUICKSNAPSHOT_IMAGE_PL,
/* sv */  IDMS_LOAD_QUICKSNAPSHOT_IMAGE_SV},

/* en */ {IDMS_SAVE_QUICKSNAPSHOT_IMAGE,
/* de */  IDMS_SAVE_QUICKSNAPSHOT_IMAGE_DE,
/* fr */  IDMS_SAVE_QUICKSNAPSHOT_IMAGE_FR,
/* it */  IDMS_SAVE_QUICKSNAPSHOT_IMAGE_IT,
/* nl */  IDMS_SAVE_QUICKSNAPSHOT_IMAGE_NL,
/* pl */  IDMS_SAVE_QUICKSNAPSHOT_IMAGE_PL,
/* sv */  IDMS_SAVE_QUICKSNAPSHOT_IMAGE_SV},

/* en */ {IDMS_START_STOP_RECORDING,
/* de */  IDMS_START_STOP_RECORDING_DE,
/* fr */  IDMS_START_STOP_RECORDING_FR,
/* it */  IDMS_START_STOP_RECORDING_IT,
/* nl */  IDMS_START_STOP_RECORDING_NL,
/* pl */  IDMS_START_STOP_RECORDING_PL,
/* sv */  IDMS_START_STOP_RECORDING_SV},

/* en */ {IDMS_START_STOP_PLAYBACK,
/* de */  IDMS_START_STOP_PLAYBACK_DE,
/* fr */  IDMS_START_STOP_PLAYBACK_FR,
/* it */  IDMS_START_STOP_PLAYBACK_IT,
/* nl */  IDMS_START_STOP_PLAYBACK_NL,
/* pl */  IDMS_START_STOP_PLAYBACK_PL,
/* sv */  IDMS_START_STOP_PLAYBACK_SV},

/* en */ {IDMS_SET_MILESTONE,
/* de */  IDMS_SET_MILESTONE_DE,
/* fr */  IDMS_SET_MILESTONE_FR,
/* it */  IDMS_SET_MILESTONE_IT,
/* nl */  IDMS_SET_MILESTONE_NL,
/* pl */  IDMS_SET_MILESTONE_PL,
/* sv */  IDMS_SET_MILESTONE_SV},

/* en */ {IDMS_RETURN_TO_MILESTONE,
/* de */  IDMS_RETURN_TO_MILESTONE_DE,
/* fr */  IDMS_RETURN_TO_MILESTONE_FR,
/* it */  IDMS_RETURN_TO_MILESTONE_IT,
/* nl */  IDMS_RETURN_TO_MILESTONE_NL,
/* pl */  IDMS_RETURN_TO_MILESTONE_PL,
/* sv */  IDMS_RETURN_TO_MILESTONE_SV},

/* en */ {IDMS_RECORDING_START_MODE,
/* de */  IDMS_RECORDING_START_MODE_DE,
/* fr */  IDMS_RECORDING_START_MODE_FR,
/* it */  IDMS_RECORDING_START_MODE_IT,
/* nl */  IDMS_RECORDING_START_MODE_NL,
/* pl */  IDMS_RECORDING_START_MODE_PL,
/* sv */  IDMS_RECORDING_START_MODE_SV},

/* en */ {IDMS_SAVE_NEW_SNAPSHOT,
/* de */  IDMS_SAVE_NEW_SNAPSHOT_DE,
/* fr */  IDMS_SAVE_NEW_SNAPSHOT_FR,
/* it */  IDMS_SAVE_NEW_SNAPSHOT_IT,
/* nl */  IDMS_SAVE_NEW_SNAPSHOT_NL,
/* pl */  IDMS_SAVE_NEW_SNAPSHOT_PL,
/* sv */  IDMS_SAVE_NEW_SNAPSHOT_SV},

/* en */ {IDMS_LOAD_EXISTING_SNAPSHOT,
/* de */  IDMS_LOAD_EXISTING_SNAPSHOT_DE,
/* fr */  IDMS_LOAD_EXISTING_SNAPSHOT_FR,
/* it */  IDMS_LOAD_EXISTING_SNAPSHOT_IT,
/* nl */  IDMS_LOAD_EXISTING_SNAPSHOT_NL,
/* pl */  IDMS_LOAD_EXISTING_SNAPSHOT_PL,
/* sv */  IDMS_LOAD_EXISTING_SNAPSHOT_SV},

/* en */ {IDMS_START_WITH_RESET,
/* de */  IDMS_START_WITH_RESET_DE,
/* fr */  IDMS_START_WITH_RESET_FR,
/* it */  IDMS_START_WITH_RESET_IT,
/* nl */  IDMS_START_WITH_RESET_NL,
/* pl */  IDMS_START_WITH_RESET_PL,
/* sv */  IDMS_START_WITH_RESET_SV},

/* en */ {IDMS_OVERWRITE_PLAYBACK,
/* de */  IDMS_OVERWRITE_PLAYBACK_DE,
/* fr */  IDMS_OVERWRITE_PLAYBACK_FR,
/* it */  IDMS_OVERWRITE_PLAYBACK_IT,
/* nl */  IDMS_OVERWRITE_PLAYBACK_NL,
/* pl */  IDMS_OVERWRITE_PLAYBACK_PL,
/* sv */  IDMS_OVERWRITE_PLAYBACK_SV},

/* en */ {IDMS_SELECT_HISTORY_DIR,
/* de */  IDMS_SELECT_HISTORY_DIR_DE,
/* fr */  IDMS_SELECT_HISTORY_DIR_FR,
/* it */  IDMS_SELECT_HISTORY_DIR_IT,
/* nl */  IDMS_SELECT_HISTORY_DIR_NL,
/* pl */  IDMS_SELECT_HISTORY_DIR_PL,
/* sv */  IDMS_SELECT_HISTORY_DIR_SV},

/* en */ {IDMS_SAVE_STOP_MEDIA_FILE,
/* de */  IDMS_SAVE_STOP_MEDIA_FILE_DE,
/* fr */  IDMS_SAVE_STOP_MEDIA_FILE_FR,
/* it */  IDMS_SAVE_STOP_MEDIA_FILE_IT,
/* nl */  IDMS_SAVE_STOP_MEDIA_FILE_NL,
/* pl */  IDMS_SAVE_STOP_MEDIA_FILE_PL,
/* sv */  IDMS_SAVE_STOP_MEDIA_FILE_SV},

/* en */ {IDMS_NETPLAY,
/* de */  IDMS_NETPLAY_DE,
/* fr */  IDMS_NETPLAY_FR,
/* it */  IDMS_NETPLAY_IT,
/* nl */  IDMS_NETPLAY_NL,
/* pl */  IDMS_NETPLAY_PL,
/* sv */  IDMS_NETPLAY_SV},

/* en */ {IDMS_OPTIONS,
/* de */  IDMS_OPTIONS_DE,
/* fr */  IDMS_OPTIONS_FR,
/* it */  IDMS_OPTIONS_IT,
/* nl */  IDMS_OPTIONS_NL,
/* pl */  IDMS_OPTIONS_PL,
/* sv */  IDMS_OPTIONS_SV},

/* en */ {IDMS_REFRESH_RATE,
/* de */  IDMS_REFRESH_RATE_DE,
/* fr */  IDMS_REFRESH_RATE_FR,
/* it */  IDMS_REFRESH_RATE_IT,
/* nl */  IDMS_REFRESH_RATE_NL,
/* pl */  IDMS_REFRESH_RATE_PL,
/* sv */  IDMS_REFRESH_RATE_SV},

/* en */ {IDMS_AUTO,
/* de */  IDMS_AUTO_DE,
/* fr */  IDMS_AUTO_FR,
/* it */  IDMS_AUTO_IT,
/* nl */  IDMS_AUTO_NL,
/* pl */  IDMS_AUTO_PL,
/* sv */  IDMS_AUTO_SV},

/* en */ {IDMS_1_1,
/* de */  IDMS_1_1_DE,
/* fr */  IDMS_1_1_FR,
/* it */  IDMS_1_1_IT,
/* nl */  IDMS_1_1_NL,
/* pl */  IDMS_1_1_PL,
/* sv */  IDMS_1_1_SV},

/* en */ {IDMS_1_2,
/* de */  IDMS_1_2_DE,
/* fr */  IDMS_1_2_FR,
/* it */  IDMS_1_2_IT,
/* nl */  IDMS_1_2_NL,
/* pl */  IDMS_1_2_PL,
/* sv */  IDMS_1_2_SV},

/* en */ {IDMS_1_3,
/* de */  IDMS_1_3_DE,
/* fr */  IDMS_1_3_FR,
/* it */  IDMS_1_3_IT,
/* nl */  IDMS_1_3_NL,
/* pl */  IDMS_1_3_PL,
/* sv */  IDMS_1_3_SV},

/* en */ {IDMS_1_4,
/* de */  IDMS_1_4_DE,
/* fr */  IDMS_1_4_FR,
/* it */  IDMS_1_4_IT,
/* nl */  IDMS_1_4_NL,
/* pl */  IDMS_1_4_PL,
/* sv */  IDMS_1_4_SV},

/* en */ {IDMS_1_5,
/* de */  IDMS_1_5_DE,
/* fr */  IDMS_1_5_FR,
/* it */  IDMS_1_5_IT,
/* nl */  IDMS_1_5_NL,
/* pl */  IDMS_1_5_PL,
/* sv */  IDMS_1_5_SV},

/* en */ {IDMS_1_6,
/* de */  IDMS_1_6_DE,
/* fr */  IDMS_1_6_FR,
/* it */  IDMS_1_6_IT,
/* nl */  IDMS_1_6_NL,
/* pl */  IDMS_1_6_PL,
/* sv */  IDMS_1_6_SV},

/* en */ {IDMS_1_7,
/* de */  IDMS_1_7_DE,
/* fr */  IDMS_1_7_FR,
/* it */  IDMS_1_7_IT,
/* nl */  IDMS_1_7_NL,
/* pl */  IDMS_1_7_PL,
/* sv */  IDMS_1_7_SV},

/* en */ {IDMS_1_8,
/* de */  IDMS_1_8_DE,
/* fr */  IDMS_1_8_FR,
/* it */  IDMS_1_8_IT,
/* nl */  IDMS_1_8_NL,
/* pl */  IDMS_1_8_PL,
/* sv */  IDMS_1_8_SV},

/* en */ {IDMS_1_9,
/* de */  IDMS_1_9_DE,
/* fr */  IDMS_1_9_FR,
/* it */  IDMS_1_9_IT,
/* nl */  IDMS_1_9_NL,
/* pl */  IDMS_1_9_PL,
/* sv */  IDMS_1_9_SV},

/* en */ {IDMS_1_10,
/* de */  IDMS_1_10_DE,
/* fr */  IDMS_1_10_FR,
/* it */  IDMS_1_10_IT,
/* nl */  IDMS_1_10_NL,
/* pl */  IDMS_1_10_PL,
/* sv */  IDMS_1_10_SV},

/* en */ {IDMS_MAXIMUM_SPEED,
/* de */  IDMS_MAXIMUM_SPEED_DE,
/* fr */  IDMS_MAXIMUM_SPEED_FR,
/* it */  IDMS_MAXIMUM_SPEED_IT,
/* nl */  IDMS_MAXIMUM_SPEED_NL,
/* pl */  IDMS_MAXIMUM_SPEED_PL,
/* sv */  IDMS_MAXIMUM_SPEED_SV},

/* en */ {IDMS_200_PERCENT,
/* de */  IDMS_200_PERCENT_DE,
/* fr */  IDMS_200_PERCENT_FR,
/* it */  IDMS_200_PERCENT_IT,
/* nl */  IDMS_200_PERCENT_NL,
/* pl */  IDMS_200_PERCENT_PL,
/* sv */  IDMS_200_PERCENT_SV},

/* en */ {IDMS_100_PERCENT,
/* de */  IDMS_100_PERCENT_DE,
/* fr */  IDMS_100_PERCENT_FR,
/* it */  IDMS_100_PERCENT_IT,
/* nl */  IDMS_100_PERCENT_NL,
/* pl */  IDMS_100_PERCENT_PL,
/* sv */  IDMS_100_PERCENT_SV},

/* en */ {IDMS_50_PERCENT,
/* de */  IDMS_50_PERCENT_DE,
/* fr */  IDMS_50_PERCENT_FR,
/* it */  IDMS_50_PERCENT_IT,
/* nl */  IDMS_50_PERCENT_NL,
/* pl */  IDMS_50_PERCENT_PL,
/* sv */  IDMS_50_PERCENT_SV},

/* en */ {IDMS_20_PERCENT,
/* de */  IDMS_20_PERCENT_DE,
/* fr */  IDMS_20_PERCENT_FR,
/* it */  IDMS_20_PERCENT_IT,
/* nl */  IDMS_20_PERCENT_NL,
/* pl */  IDMS_20_PERCENT_PL,
/* sv */  IDMS_20_PERCENT_SV},

/* en */ {IDMS_10_PERCENT,
/* de */  IDMS_10_PERCENT_DE,
/* fr */  IDMS_10_PERCENT_FR,
/* it */  IDMS_10_PERCENT_IT,
/* nl */  IDMS_10_PERCENT_NL,
/* pl */  IDMS_10_PERCENT_PL,
/* sv */  IDMS_10_PERCENT_SV},

/* en */ {IDMS_NO_LIMIT,
/* de */  IDMS_NO_LIMIT_DE,
/* fr */  IDMS_NO_LIMIT_FR,
/* it */  IDMS_NO_LIMIT_IT,
/* nl */  IDMS_NO_LIMIT_NL,
/* pl */  IDMS_NO_LIMIT_PL,
/* sv */  IDMS_NO_LIMIT_SV},

/* en */ {IDMS_CUSTOM,
/* de */  IDMS_CUSTOM_DE,
/* fr */  IDMS_CUSTOM_FR,
/* it */  IDMS_CUSTOM_IT,
/* nl */  IDMS_CUSTOM_NL,
/* pl */  IDMS_CUSTOM_PL,
/* sv */  IDMS_CUSTOM_SV},

/* en */ {IDMS_WARP_MODE,
/* de */  IDMS_WARP_MODE_DE,
/* fr */  IDMS_WARP_MODE_FR,
/* it */  IDMS_WARP_MODE_IT,
/* nl */  IDMS_WARP_MODE_NL,
/* pl */  IDMS_WARP_MODE_PL,
/* sv */  IDMS_WARP_MODE_SV},

/* en */ {IDMS_FULLSCREEN,
/* de */  IDMS_FULLSCREEN_DE,
/* fr */  IDMS_FULLSCREEN_FR,
/* it */  IDMS_FULLSCREEN_IT,
/* nl */  IDMS_FULLSCREEN_NL,
/* pl */  IDMS_FULLSCREEN_PL,
/* sv */  IDMS_FULLSCREEN_SV},

/* en */ {IDMS_VIDEO_CACHE,
/* de */  IDMS_VIDEO_CACHE_DE,
/* fr */  IDMS_VIDEO_CACHE_FR,
/* it */  IDMS_VIDEO_CACHE_IT,
/* nl */  IDMS_VIDEO_CACHE_NL,
/* pl */  IDMS_VIDEO_CACHE_PL,
/* sv */  IDMS_VIDEO_CACHE_SV},

/* en */ {IDMS_DOUBLE_SIZE,
/* de */  IDMS_DOUBLE_SIZE_DE,
/* fr */  IDMS_DOUBLE_SIZE_FR,
/* it */  IDMS_DOUBLE_SIZE_IT,
/* nl */  IDMS_DOUBLE_SIZE_NL,
/* pl */  IDMS_DOUBLE_SIZE_PL,
/* sv */  IDMS_DOUBLE_SIZE_SV},

/* en */ {IDMS_DOUBLE_SCAN,
/* de */  IDMS_DOUBLE_SCAN_DE,
/* fr */  IDMS_DOUBLE_SCAN_FR,
/* it */  IDMS_DOUBLE_SCAN_IT,
/* nl */  IDMS_DOUBLE_SCAN_NL,
/* pl */  IDMS_DOUBLE_SCAN_PL,
/* sv */  IDMS_DOUBLE_SCAN_SV},

/* en */ {IDMS_PAL_EMULATION,
/* de */  IDMS_PAL_EMULATION_DE,
/* fr */  IDMS_PAL_EMULATION_FR,
/* it */  IDMS_PAL_EMULATION_IT,
/* nl */  IDMS_PAL_EMULATION_NL,
/* pl */  IDMS_PAL_EMULATION_PL,
/* sv */  IDMS_PAL_EMULATION_SV},

/* en */ {IDMS_SCALE2X,
/* de */  IDMS_SCALE2X_DE,
/* fr */  IDMS_SCALE2X_FR,
/* it */  IDMS_SCALE2X_IT,
/* nl */  IDMS_SCALE2X_NL,
/* pl */  IDMS_SCALE2X_PL,
/* sv */  IDMS_SCALE2X_SV},

/* en */ {IDMS_VDC_SETTINGS,
/* de */  IDMS_VDC_SETTINGS_DE,
/* fr */  IDMS_VDC_SETTINGS_FR,
/* it */  IDMS_VDC_SETTINGS_IT,
/* nl */  IDMS_VDC_SETTINGS_NL,
/* pl */  IDMS_VDC_SETTINGS_PL,
/* sv */  IDMS_VDC_SETTINGS_SV},

/* en */ {IDMS_64KB_VIDEO_MEMORY,
/* de */  IDMS_64KB_VIDEO_MEMORY_DE,
/* fr */  IDMS_64KB_VIDEO_MEMORY_FR,
/* it */  IDMS_64KB_VIDEO_MEMORY_IT,
/* nl */  IDMS_64KB_VIDEO_MEMORY_NL,
/* pl */  IDMS_64KB_VIDEO_MEMORY_PL,
/* sv */  IDMS_64KB_VIDEO_MEMORY_SV},

/* en */ {IDMS_SWAP_JOYSTICKS,
/* de */  IDMS_SWAP_JOYSTICKS_DE,
/* fr */  IDMS_SWAP_JOYSTICKS_FR,
/* it */  IDMS_SWAP_JOYSTICKS_IT,
/* nl */  IDMS_SWAP_JOYSTICKS_NL,
/* pl */  IDMS_SWAP_JOYSTICKS_PL,
/* sv */  IDMS_SWAP_JOYSTICKS_SV},

/* en */ {IDMS_SOUND_PLAYBACK,
/* de */  IDMS_SOUND_PLAYBACK_DE,
/* fr */  IDMS_SOUND_PLAYBACK_FR,
/* it */  IDMS_SOUND_PLAYBACK_IT,
/* nl */  IDMS_SOUND_PLAYBACK_NL,
/* pl */  IDMS_SOUND_PLAYBACK_PL,
/* sv */  IDMS_SOUND_PLAYBACK_SV},

/* en */ {IDMS_TRUE_DRIVE_EMU,
/* de */  IDMS_TRUE_DRIVE_EMU_DE,
/* fr */  IDMS_TRUE_DRIVE_EMU_FR,
/* it */  IDMS_TRUE_DRIVE_EMU_IT,
/* nl */  IDMS_TRUE_DRIVE_EMU_NL,
/* pl */  IDMS_TRUE_DRIVE_EMU_PL,
/* sv */  IDMS_TRUE_DRIVE_EMU_SV},

/* en */ {IDMS_VIRTUAL_DEVICE_TRAPS,
/* de */  IDMS_VIRTUAL_DEVICE_TRAPS_DE,
/* fr */  IDMS_VIRTUAL_DEVICE_TRAPS_FR,
/* it */  IDMS_VIRTUAL_DEVICE_TRAPS_IT,
/* nl */  IDMS_VIRTUAL_DEVICE_TRAPS_NL,
/* pl */  IDMS_VIRTUAL_DEVICE_TRAPS_PL,
/* sv */  IDMS_VIRTUAL_DEVICE_TRAPS_SV},

/* en */ {IDMS_DRIVE_SYNC_FACTOR,
/* de */  IDMS_DRIVE_SYNC_FACTOR_DE,
/* fr */  IDMS_DRIVE_SYNC_FACTOR_FR,
/* it */  IDMS_DRIVE_SYNC_FACTOR_IT,
/* nl */  IDMS_DRIVE_SYNC_FACTOR_NL,
/* pl */  IDMS_DRIVE_SYNC_FACTOR_PL,
/* sv */  IDMS_DRIVE_SYNC_FACTOR_SV},

/* en */ {IDMS_PAL,
/* de */  IDMS_PAL_DE,
/* fr */  IDMS_PAL_FR,
/* it */  IDMS_PAL_IT,
/* nl */  IDMS_PAL_NL,
/* pl */  IDMS_PAL_PL,
/* sv */  IDMS_PAL_SV},

/* en */ {IDMS_NTSC,
/* de */  IDMS_NTSC_DE,
/* fr */  IDMS_NTSC_FR,
/* it */  IDMS_NTSC_IT,
/* nl */  IDMS_NTSC_NL,
/* pl */  IDMS_NTSC_PL,
/* sv */  IDMS_NTSC_SV},

/* en */ {IDMS_VIDEO_STANDARD,
/* de */  IDMS_VIDEO_STANDARD_DE,
/* fr */  IDMS_VIDEO_STANDARD_FR,
/* it */  IDMS_VIDEO_STANDARD_IT,
/* nl */  IDMS_VIDEO_STANDARD_NL,
/* pl */  IDMS_VIDEO_STANDARD_PL,
/* sv */  IDMS_VIDEO_STANDARD_SV},

/* en */ {IDMS_PAL_G,
/* de */  IDMS_PAL_G_DE,
/* fr */  IDMS_PAL_G_FR,
/* it */  IDMS_PAL_G_IT,
/* nl */  IDMS_PAL_G_NL,
/* pl */  IDMS_PAL_G_PL,
/* sv */  IDMS_PAL_G_SV},

/* en */ {IDMS_NTSC_M,
/* de */  IDMS_NTSC_M_DE,
/* fr */  IDMS_NTSC_M_FR,
/* it */  IDMS_NTSC_M_IT,
/* nl */  IDMS_NTSC_M_NL,
/* pl */  IDMS_NTSC_M_PL,
/* sv */  IDMS_NTSC_M_SV},

/* en */ {IDMS_OLD_NTSC_M,
/* de */  IDMS_OLD_NTSC_M_DE,
/* fr */  IDMS_OLD_NTSC_M_FR,
/* it */  IDMS_OLD_NTSC_M_IT,
/* nl */  IDMS_OLD_NTSC_M_NL,
/* pl */  IDMS_OLD_NTSC_M_PL,
/* sv */  IDMS_OLD_NTSC_M_SV},

/* en */ {IDMS_EMU_ID,
/* de */  IDMS_EMU_ID_DE,
/* fr */  IDMS_EMU_ID_FR,
/* it */  IDMS_EMU_ID_IT,
/* nl */  IDMS_EMU_ID_NL,
/* pl */  IDMS_EMU_ID_PL,
/* sv */  IDMS_EMU_ID_SV},

/* en */ {IDMS_VIC_1112_IEEE_488,
/* de */  IDMS_VIC_1112_IEEE_488_DE,
/* fr */  IDMS_VIC_1112_IEEE_488_FR,
/* it */  IDMS_VIC_1112_IEEE_488_IT,
/* nl */  IDMS_VIC_1112_IEEE_488_NL,
/* pl */  IDMS_VIC_1112_IEEE_488_PL,
/* sv */  IDMS_VIC_1112_IEEE_488_SV},

/* en */ {IDMS_IEEE_488_INTERFACE,
/* de */  IDMS_IEEE_488_INTERFACE_DE,
/* fr */  IDMS_IEEE_488_INTERFACE_FR,
/* it */  IDMS_IEEE_488_INTERFACE_IT,
/* nl */  IDMS_IEEE_488_INTERFACE_NL,
/* pl */  IDMS_IEEE_488_INTERFACE_PL,
/* sv */  IDMS_IEEE_488_INTERFACE_SV},

/* en */ {IDMS_1351_MOUSE,
/* de */  IDMS_1351_MOUSE_DE,
/* fr */  IDMS_1351_MOUSE_FR,
/* it */  IDMS_1351_MOUSE_IT,
/* nl */  IDMS_1351_MOUSE_NL,
/* pl */  IDMS_1351_MOUSE_PL,
/* sv */  IDMS_1351_MOUSE_SV},

/* en */ {IDMS_SETTINGS,
/* de */  IDMS_SETTINGS_DE,
/* fr */  IDMS_SETTINGS_FR,
/* it */  IDMS_SETTINGS_IT,
/* nl */  IDMS_SETTINGS_NL,
/* pl */  IDMS_SETTINGS_PL,
/* sv */  IDMS_SETTINGS_SV},

/* en */ {IDMS_VIDEO_SETTINGS,
/* de */  IDMS_VIDEO_SETTINGS_DE,
/* fr */  IDMS_VIDEO_SETTINGS_FR,
/* it */  IDMS_VIDEO_SETTINGS_IT,
/* nl */  IDMS_VIDEO_SETTINGS_NL,
/* pl */  IDMS_VIDEO_SETTINGS_PL,
/* sv */  IDMS_VIDEO_SETTINGS_SV},

/* en */ {IDMS_VIC_SETTINGS,
/* de */  IDMS_VIC_SETTINGS_DE,
/* fr */  IDMS_VIC_SETTINGS_FR,
/* it */  IDMS_VIC_SETTINGS_IT,
/* nl */  IDMS_VIC_SETTINGS_NL,
/* pl */  IDMS_VIC_SETTINGS_PL,
/* sv */  IDMS_VIC_SETTINGS_SV},

/* en */ {IDMS_CBM2_SETTINGS,
/* de */  IDMS_CBM2_SETTINGS_DE,
/* fr */  IDMS_CBM2_SETTINGS_FR,
/* it */  IDMS_CBM2_SETTINGS_IT,
/* nl */  IDMS_CBM2_SETTINGS_NL,
/* pl */  IDMS_CBM2_SETTINGS_PL,
/* sv */  IDMS_CBM2_SETTINGS_SV},

/* en */ {IDMS_PET_SETTINGS,
/* de */  IDMS_PET_SETTINGS_DE,
/* fr */  IDMS_PET_SETTINGS_FR,
/* it */  IDMS_PET_SETTINGS_IT,
/* nl */  IDMS_PET_SETTINGS_NL,
/* pl */  IDMS_PET_SETTINGS_PL,
/* sv */  IDMS_PET_SETTINGS_SV},

/* en */ {IDMS_PERIPHERAL_SETTINGS,
/* de */  IDMS_PERIPHERAL_SETTINGS_DE,
/* fr */  IDMS_PERIPHERAL_SETTINGS_FR,
/* it */  IDMS_PERIPHERAL_SETTINGS_IT,
/* nl */  IDMS_PERIPHERAL_SETTINGS_NL,
/* pl */  IDMS_PERIPHERAL_SETTINGS_PL,
/* sv */  IDMS_PERIPHERAL_SETTINGS_SV},

/* en */ {IDMS_DRIVE_SETTINGS,
/* de */  IDMS_DRIVE_SETTINGS_DE,
/* fr */  IDMS_DRIVE_SETTINGS_FR,
/* it */  IDMS_DRIVE_SETTINGS_IT,
/* nl */  IDMS_DRIVE_SETTINGS_NL,
/* pl */  IDMS_DRIVE_SETTINGS_PL,
/* sv */  IDMS_DRIVE_SETTINGS_SV},

/* en */ {IDMS_DATASETTE_SETTINGS,
/* de */  IDMS_DATASETTE_SETTINGS_DE,
/* fr */  IDMS_DATASETTE_SETTINGS_FR,
/* it */  IDMS_DATASETTE_SETTINGS_IT,
/* nl */  IDMS_DATASETTE_SETTINGS_NL,
/* pl */  IDMS_DATASETTE_SETTINGS_PL,
/* sv */  IDMS_DATASETTE_SETTINGS_SV},

/* en */ {IDMS_PLUS4_SETTINGS,
/* de */  IDMS_PLUS4_SETTINGS_DE,
/* fr */  IDMS_PLUS4_SETTINGS_FR,
/* it */  IDMS_PLUS4_SETTINGS_IT,
/* nl */  IDMS_PLUS4_SETTINGS_NL,
/* pl */  IDMS_PLUS4_SETTINGS_PL,
/* sv */  IDMS_PLUS4_SETTINGS_SV},

/* en */ {IDMS_VICII_SETTINGS,
/* de */  IDMS_VICII_SETTINGS_DE,
/* fr */  IDMS_VICII_SETTINGS_FR,
/* it */  IDMS_VICII_SETTINGS_IT,
/* nl */  IDMS_VICII_SETTINGS_NL,
/* pl */  IDMS_VICII_SETTINGS_PL,
/* sv */  IDMS_VICII_SETTINGS_SV},

/* en */ {IDMS_JOYSTICK_SETTINGS,
/* de */  IDMS_JOYSTICK_SETTINGS_DE,
/* fr */  IDMS_JOYSTICK_SETTINGS_FR,
/* it */  IDMS_JOYSTICK_SETTINGS_IT,
/* nl */  IDMS_JOYSTICK_SETTINGS_NL,
/* pl */  IDMS_JOYSTICK_SETTINGS_PL,
/* sv */  IDMS_JOYSTICK_SETTINGS_SV},

/* en */ {IDMS_JOYSTICK_DEVICE_SELECT,
/* de */  IDMS_JOYSTICK_DEVICE_SELECT_DE,
/* fr */  IDMS_JOYSTICK_DEVICE_SELECT_FR,
/* it */  IDMS_JOYSTICK_DEVICE_SELECT_IT,
/* nl */  IDMS_JOYSTICK_DEVICE_SELECT_NL,
/* pl */  IDMS_JOYSTICK_DEVICE_SELECT_PL,
/* sv */  IDMS_JOYSTICK_DEVICE_SELECT_SV},

/* en */ {IDMS_JOYSTICK_FIRE_SELECT,
/* de */  IDMS_JOYSTICK_FIRE_SELECT_DE,
/* fr */  IDMS_JOYSTICK_FIRE_SELECT_FR,
/* it */  IDMS_JOYSTICK_FIRE_SELECT_IT,
/* nl */  IDMS_JOYSTICK_FIRE_SELECT_NL,
/* pl */  IDMS_JOYSTICK_FIRE_SELECT_PL,
/* sv */  IDMS_JOYSTICK_FIRE_SELECT_SV},

/* en */ {IDMS_KEYBOARD_SETTINGS,
/* de */  IDMS_KEYBOARD_SETTINGS_DE,
/* fr */  IDMS_KEYBOARD_SETTINGS_FR,
/* it */  IDMS_KEYBOARD_SETTINGS_IT,
/* nl */  IDMS_KEYBOARD_SETTINGS_NL,
/* pl */  IDMS_KEYBOARD_SETTINGS_PL,
/* sv */  IDMS_KEYBOARD_SETTINGS_SV},

/* en */ {IDMS_SOUND_SETTINGS,
/* de */  IDMS_SOUND_SETTINGS_DE,
/* fr */  IDMS_SOUND_SETTINGS_FR,
/* it */  IDMS_SOUND_SETTINGS_IT,
/* nl */  IDMS_SOUND_SETTINGS_NL,
/* pl */  IDMS_SOUND_SETTINGS_PL,
/* sv */  IDMS_SOUND_SETTINGS_SV},

/* en */ {IDMS_SID_SETTINGS,
/* de */  IDMS_SID_SETTINGS_DE,
/* fr */  IDMS_SID_SETTINGS_FR,
/* it */  IDMS_SID_SETTINGS_IT,
/* nl */  IDMS_SID_SETTINGS_NL,
/* pl */  IDMS_SID_SETTINGS_PL,
/* sv */  IDMS_SID_SETTINGS_SV},

/* en */ {IDMS_ROM_SETTINGS,
/* de */  IDMS_ROM_SETTINGS_DE,
/* fr */  IDMS_ROM_SETTINGS_FR,
/* it */  IDMS_ROM_SETTINGS_IT,
/* nl */  IDMS_ROM_SETTINGS_NL,
/* pl */  IDMS_ROM_SETTINGS_PL,
/* sv */  IDMS_ROM_SETTINGS_SV},

/* en */ {IDMS_RAM_SETTINGS,
/* de */  IDMS_RAM_SETTINGS_DE,
/* fr */  IDMS_RAM_SETTINGS_FR,
/* it */  IDMS_RAM_SETTINGS_IT,
/* nl */  IDMS_RAM_SETTINGS_NL,
/* pl */  IDMS_RAM_SETTINGS_PL,
/* sv */  IDMS_RAM_SETTINGS_SV},

/* en */ {IDMS_RS232_SETTINGS,
/* de */  IDMS_RS232_SETTINGS_DE,
/* fr */  IDMS_RS232_SETTINGS_FR,
/* it */  IDMS_RS232_SETTINGS_IT,
/* nl */  IDMS_RS232_SETTINGS_NL,
/* pl */  IDMS_RS232_SETTINGS_PL,
/* sv */  IDMS_RS232_SETTINGS_SV},

/* en */ {IDMS_C128_SETTINGS,
/* de */  IDMS_C128_SETTINGS_DE,
/* fr */  IDMS_C128_SETTINGS_FR,
/* it */  IDMS_C128_SETTINGS_IT,
/* nl */  IDMS_C128_SETTINGS_NL,
/* pl */  IDMS_C128_SETTINGS_PL,
/* sv */  IDMS_C128_SETTINGS_SV},

/* en */ {IDMS_CART_IO_SETTINGS,
/* de */  IDMS_CART_IO_SETTINGS_DE,
/* fr */  IDMS_CART_IO_SETTINGS_FR,
/* it */  IDMS_CART_IO_SETTINGS_IT,
/* nl */  IDMS_CART_IO_SETTINGS_NL,
/* pl */  IDMS_CART_IO_SETTINGS_PL,
/* sv */  IDMS_CART_IO_SETTINGS_SV},

/* en */ {IDMS_REU_SETTINGS,
/* de */  IDMS_REU_SETTINGS_DE,
/* fr */  IDMS_REU_SETTINGS_FR,
/* it */  IDMS_REU_SETTINGS_IT,
/* nl */  IDMS_REU_SETTINGS_NL,
/* pl */  IDMS_REU_SETTINGS_PL,
/* sv */  IDMS_REU_SETTINGS_SV},

/* en */ {IDMS_GEORAM_SETTINGS,
/* de */  IDMS_GEORAM_SETTINGS_DE,
/* fr */  IDMS_GEORAM_SETTINGS_FR,
/* it */  IDMS_GEORAM_SETTINGS_IT,
/* nl */  IDMS_GEORAM_SETTINGS_NL,
/* pl */  IDMS_GEORAM_SETTINGS_PL,
/* sv */  IDMS_GEORAM_SETTINGS_SV},

/* en */ {IDMS_RAMCART_SETTINGS,
/* de */  IDMS_RAMCART_SETTINGS_DE,
/* fr */  IDMS_RAMCART_SETTINGS_FR,
/* it */  IDMS_RAMCART_SETTINGS_IT,
/* nl */  IDMS_RAMCART_SETTINGS_NL,
/* pl */  IDMS_RAMCART_SETTINGS_PL,
/* sv */  IDMS_RAMCART_SETTINGS_SV},

/* en */ {IDMS_PLUS60K_SETTINGS,
/* de */  IDMS_PLUS60K_SETTINGS_DE,
/* fr */  IDMS_PLUS60K_SETTINGS_FR,
/* it */  IDMS_PLUS60K_SETTINGS_IT,
/* nl */  IDMS_PLUS60K_SETTINGS_NL,
/* pl */  IDMS_PLUS60K_SETTINGS_PL,
/* sv */  IDMS_PLUS60K_SETTINGS_SV},

/* en */ {IDMS_PLUS256K_SETTINGS,
/* de */  IDMS_PLUS256K_SETTINGS_DE,
/* fr */  IDMS_PLUS256K_SETTINGS_FR,
/* it */  IDMS_PLUS256K_SETTINGS_IT,
/* nl */  IDMS_PLUS256K_SETTINGS_NL,
/* pl */  IDMS_PLUS256K_SETTINGS_PL,
/* sv */  IDMS_PLUS256K_SETTINGS_SV},

/* en */ {IDMS_256K_SETTINGS,
/* de */  IDMS_256K_SETTINGS_DE,
/* fr */  IDMS_256K_SETTINGS_FR,
/* it */  IDMS_256K_SETTINGS_IT,
/* nl */  IDMS_256K_SETTINGS_NL,
/* pl */  IDMS_256K_SETTINGS_PL,
/* sv */  IDMS_256K_SETTINGS_SV},

/* en */ {IDMS_IDE64_SETTINGS,
/* de */  IDMS_IDE64_SETTINGS_DE,
/* fr */  IDMS_IDE64_SETTINGS_FR,
/* it */  IDMS_IDE64_SETTINGS_IT,
/* nl */  IDMS_IDE64_SETTINGS_NL,
/* pl */  IDMS_IDE64_SETTINGS_PL,
/* sv */  IDMS_IDE64_SETTINGS_SV},

/* en */ {IDMS_ETHERNET_SETTINGS,
/* de */  IDMS_ETHERNET_SETTINGS_DE,
/* fr */  IDMS_ETHERNET_SETTINGS_FR,
/* it */  IDMS_ETHERNET_SETTINGS_IT,
/* nl */  IDMS_ETHERNET_SETTINGS_NL,
/* pl */  IDMS_ETHERNET_SETTINGS_PL,
/* sv */  IDMS_ETHERNET_SETTINGS_SV},

/* en */ {IDMS_ACIA_SETTINGS,
/* de */  IDMS_ACIA_SETTINGS_DE,
/* fr */  IDMS_ACIA_SETTINGS_FR,
/* it */  IDMS_ACIA_SETTINGS_IT,
/* nl */  IDMS_ACIA_SETTINGS_NL,
/* pl */  IDMS_ACIA_SETTINGS_PL,
/* sv */  IDMS_ACIA_SETTINGS_SV},

/* en */ {IDMS_PETREU_SETTINGS,
/* de */  IDMS_PETREU_SETTINGS_DE,
/* fr */  IDMS_PETREU_SETTINGS_FR,
/* it */  IDMS_PETREU_SETTINGS_IT,
/* nl */  IDMS_PETREU_SETTINGS_NL,
/* pl */  IDMS_PETREU_SETTINGS_PL,
/* sv */  IDMS_PETREU_SETTINGS_SV},

/* en */ {IDMS_RS232_USERPORT_SETTINGS,
/* de */  IDMS_RS232_USERPORT_SETTINGS_DE,
/* fr */  IDMS_RS232_USERPORT_SETTINGS_FR,
/* it */  IDMS_RS232_USERPORT_SETTINGS_IT,
/* nl */  IDMS_RS232_USERPORT_SETTINGS_NL,
/* pl */  IDMS_RS232_USERPORT_SETTINGS_PL,
/* sv */  IDMS_RS232_USERPORT_SETTINGS_SV},

/* en */ {IDMS_SAVE_CURRENT_SETTINGS,
/* de */  IDMS_SAVE_CURRENT_SETTINGS_DE,
/* fr */  IDMS_SAVE_CURRENT_SETTINGS_FR,
/* it */  IDMS_SAVE_CURRENT_SETTINGS_IT,
/* nl */  IDMS_SAVE_CURRENT_SETTINGS_NL,
/* pl */  IDMS_SAVE_CURRENT_SETTINGS_PL,
/* sv */  IDMS_SAVE_CURRENT_SETTINGS_SV},

/* en */ {IDMS_LOAD_SAVED_SETTINGS,
/* de */  IDMS_LOAD_SAVED_SETTINGS_DE,
/* fr */  IDMS_LOAD_SAVED_SETTINGS_FR,
/* it */  IDMS_LOAD_SAVED_SETTINGS_IT,
/* nl */  IDMS_LOAD_SAVED_SETTINGS_NL,
/* pl */  IDMS_LOAD_SAVED_SETTINGS_PL,
/* sv */  IDMS_LOAD_SAVED_SETTINGS_SV},

/* en */ {IDMS_SET_DEFAULT_SETTINGS,
/* de */  IDMS_SET_DEFAULT_SETTINGS_DE,
/* fr */  IDMS_SET_DEFAULT_SETTINGS_FR,
/* it */  IDMS_SET_DEFAULT_SETTINGS_IT,
/* nl */  IDMS_SET_DEFAULT_SETTINGS_NL,
/* pl */  IDMS_SET_DEFAULT_SETTINGS_PL,
/* sv */  IDMS_SET_DEFAULT_SETTINGS_SV},

/* en */ {IDMS_SAVE_SETTING_ON_EXIT,
/* de */  IDMS_SAVE_SETTING_ON_EXIT_DE,
/* fr */  IDMS_SAVE_SETTING_ON_EXIT_FR,
/* it */  IDMS_SAVE_SETTING_ON_EXIT_IT,
/* nl */  IDMS_SAVE_SETTING_ON_EXIT_NL,
/* pl */  IDMS_SAVE_SETTING_ON_EXIT_PL,
/* sv */  IDMS_SAVE_SETTING_ON_EXIT_SV},

/* en */ {IDMS_CONFIRM_ON_EXIT,
/* de */  IDMS_CONFIRM_ON_EXIT_DE,
/* fr */  IDMS_CONFIRM_ON_EXIT_FR,
/* it */  IDMS_CONFIRM_ON_EXIT_IT,
/* nl */  IDMS_CONFIRM_ON_EXIT_NL,
/* pl */  IDMS_CONFIRM_ON_EXIT_PL,
/* sv */  IDMS_CONFIRM_ON_EXIT_SV},

/* en */ {IDMS_HELP,
/* de */  IDMS_HELP_DE,
/* fr */  IDMS_HELP_FR,
/* it */  IDMS_HELP_IT,
/* nl */  IDMS_HELP_NL,
/* pl */  IDMS_HELP_PL,
/* sv */  IDMS_HELP_SV},

/* en */ {IDMS_ABOUT,
/* de */  IDMS_ABOUT_DE,
/* fr */  IDMS_ABOUT_FR,
/* it */  IDMS_ABOUT_IT,
/* nl */  IDMS_ABOUT_NL,
/* pl */  IDMS_ABOUT_PL,
/* sv */  IDMS_ABOUT_SV},

/* en */ {IDMS_COMMAND_LINE_OPTIONS,
/* de */  IDMS_COMMAND_LINE_OPTIONS_DE,
/* fr */  IDMS_COMMAND_LINE_OPTIONS_FR,
/* it */  IDMS_COMMAND_LINE_OPTIONS_IT,
/* nl */  IDMS_COMMAND_LINE_OPTIONS_NL,
/* pl */  IDMS_COMMAND_LINE_OPTIONS_PL,
/* sv */  IDMS_COMMAND_LINE_OPTIONS_SV},

/* en */ {IDMS_CONTRIBUTORS,
/* de */  IDMS_CONTRIBUTORS_DE,
/* fr */  IDMS_CONTRIBUTORS_FR,
/* it */  IDMS_CONTRIBUTORS_IT,
/* nl */  IDMS_CONTRIBUTORS_NL,
/* pl */  IDMS_CONTRIBUTORS_PL,
/* sv */  IDMS_CONTRIBUTORS_SV},

/* en */ {IDMS_LICENSE,
/* de */  IDMS_LICENSE_DE,
/* fr */  IDMS_LICENSE_FR,
/* it */  IDMS_LICENSE_IT,
/* nl */  IDMS_LICENSE_NL,
/* pl */  IDMS_LICENSE_PL,
/* sv */  IDMS_LICENSE_SV},

/* en */ {IDMS_NO_WARRANTY,
/* de */  IDMS_NO_WARRANTY_DE,
/* fr */  IDMS_NO_WARRANTY_FR,
/* it */  IDMS_NO_WARRANTY_IT,
/* nl */  IDMS_NO_WARRANTY_NL,
/* pl */  IDMS_NO_WARRANTY_PL,
/* sv */  IDMS_NO_WARRANTY_SV},

/* en */ {IDMS_LANGUAGE_ENGLISH,
/* de */  IDMS_LANGUAGE_ENGLISH_DE,
/* fr */  IDMS_LANGUAGE_ENGLISH_FR,
/* it */  IDMS_LANGUAGE_ENGLISH_IT,
/* nl */  IDMS_LANGUAGE_ENGLISH_NL,
/* pl */  IDMS_LANGUAGE_ENGLISH_PL,
/* sv */  IDMS_LANGUAGE_ENGLISH_SV},

/* en */ {IDMS_LANGUAGE_GERMAN,
/* de */  IDMS_LANGUAGE_GERMAN_DE,
/* fr */  IDMS_LANGUAGE_GERMAN_FR,
/* it */  IDMS_LANGUAGE_GERMAN_IT,
/* nl */  IDMS_LANGUAGE_GERMAN_NL,
/* pl */  IDMS_LANGUAGE_GERMAN_PL,
/* sv */  IDMS_LANGUAGE_GERMAN_SV},

/* en */ {IDMS_LANGUAGE_FRENCH,
/* de */  IDMS_LANGUAGE_FRENCH_DE,
/* fr */  IDMS_LANGUAGE_FRENCH_FR,
/* it */  IDMS_LANGUAGE_FRENCH_IT,
/* nl */  IDMS_LANGUAGE_FRENCH_NL,
/* pl */  IDMS_LANGUAGE_FRENCH_PL,
/* sv */  IDMS_LANGUAGE_FRENCH_SV},

/* en */ {IDMS_LANGUAGE_ITALIAN,
/* de */  IDMS_LANGUAGE_ITALIAN_DE,
/* fr */  IDMS_LANGUAGE_ITALIAN_FR,
/* it */  IDMS_LANGUAGE_ITALIAN_IT,
/* nl */  IDMS_LANGUAGE_ITALIAN_NL,
/* pl */  IDMS_LANGUAGE_ITALIAN_PL,
/* sv */  IDMS_LANGUAGE_ITALIAN_SV},

/* en */ {IDMS_LANGUAGE_DUTCH,
/* de */  IDMS_LANGUAGE_DUTCH_DE,
/* fr */  IDMS_LANGUAGE_DUTCH_FR,
/* it */  IDMS_LANGUAGE_DUTCH_IT,
/* nl */  IDMS_LANGUAGE_DUTCH_NL,
/* pl */  IDMS_LANGUAGE_DUTCH_PL,
/* sv */  IDMS_LANGUAGE_DUTCH_SV},

/* en */ {IDMS_LANGUAGE_POLISH,
/* de */  IDMS_LANGUAGE_POLISH_DE,
/* fr */  IDMS_LANGUAGE_POLISH_FR,
/* it */  IDMS_LANGUAGE_POLISH_IT,
/* nl */  IDMS_LANGUAGE_POLISH_NL,
/* pl */  IDMS_LANGUAGE_POLISH_PL,
/* sv */  IDMS_LANGUAGE_POLISH_SV},

/* en */ {IDMS_LANGUAGE_SWEDISH,
/* de */  IDMS_LANGUAGE_SWEDISH_DE,
/* fr */  IDMS_LANGUAGE_SWEDISH_FR,
/* it */  IDMS_LANGUAGE_SWEDISH_IT,
/* nl */  IDMS_LANGUAGE_SWEDISH_NL,
/* pl */  IDMS_LANGUAGE_SWEDISH_PL,
/* sv */  IDMS_LANGUAGE_SWEDISH_SV},

/* en */ {IDMS_LANGUAGE,
/* de */  IDMS_LANGUAGE_DE,
/* fr */  IDMS_LANGUAGE_FR,
/* it */  IDMS_LANGUAGE_IT,
/* nl */  IDMS_LANGUAGE_NL,
/* pl */  IDMS_LANGUAGE_PL,
/* sv */  IDMS_LANGUAGE_SV},

/* en */ {IDMS_FULLSCREEN_STATUSBAR,
/* de */  IDMS_FULLSCREEN_STATUSBAR_DE,
/* fr */  IDMS_FULLSCREEN_STATUSBAR_FR,
/* it */  IDMS_FULLSCREEN_STATUSBAR_IT,
/* nl */  IDMS_FULLSCREEN_STATUSBAR_NL,
/* pl */  IDMS_FULLSCREEN_STATUSBAR_PL,
/* sv */  IDMS_FULLSCREEN_STATUSBAR_SV},

/* en */ {IDMS_VIDEO_OVERLAY,
/* de */  IDMS_VIDEO_OVERLAY_DE,
/* fr */  IDMS_VIDEO_OVERLAY_FR,
/* it */  IDMS_VIDEO_OVERLAY_IT,
/* nl */  IDMS_VIDEO_OVERLAY_NL,
/* pl */  IDMS_VIDEO_OVERLAY_PL,
/* sv */  IDMS_VIDEO_OVERLAY_SV},

/* ----------------------- AmigaOS Message/Error Strings ----------------------- */

/* en */ {IDMES_SETTINGS_SAVED_SUCCESS,
/* de */  IDMES_SETTINGS_SAVED_SUCCESS_DE,
/* fr */  IDMES_SETTINGS_SAVED_SUCCESS_FR,
/* it */  IDMES_SETTINGS_SAVED_SUCCESS_IT,
/* nl */  IDMES_SETTINGS_SAVED_SUCCESS_NL,
/* pl */  IDMES_SETTINGS_SAVED_SUCCESS_PL,
/* sv */  IDMES_SETTINGS_SAVED_SUCCESS_SV},

/* en */ {IDMES_SETTINGS_LOAD_SUCCESS,
/* de */  IDMES_SETTINGS_LOAD_SUCCESS_DE,
/* fr */  IDMES_SETTINGS_LOAD_SUCCESS_FR,
/* it */  IDMES_SETTINGS_LOAD_SUCCESS_IT,
/* nl */  IDMES_SETTINGS_LOAD_SUCCESS_NL,
/* pl */  IDMES_SETTINGS_LOAD_SUCCESS_PL,
/* sv */  IDMES_SETTINGS_LOAD_SUCCESS_SV},

/* en */ {IDMES_DFLT_SETTINGS_RESTORED,
/* de */  IDMES_DFLT_SETTINGS_RESTORED_DE,
/* fr */  IDMES_DFLT_SETTINGS_RESTORED_FR,
/* it */  IDMES_DFLT_SETTINGS_RESTORED_IT,
/* nl */  IDMES_DFLT_SETTINGS_RESTORED_NL,
/* pl */  IDMES_DFLT_SETTINGS_RESTORED_PL,
/* sv */  IDMES_DFLT_SETTINGS_RESTORED_SV},

/* en */ {IDMES_VICE_MESSAGE,
/* de */  IDMES_VICE_MESSAGE_DE,
/* fr */  IDMES_VICE_MESSAGE_FR,
/* it */  IDMES_VICE_MESSAGE_IT,
/* nl */  IDMES_VICE_MESSAGE_NL,
/* pl */  IDMES_VICE_MESSAGE_PL,
/* sv */  IDMES_VICE_MESSAGE_SV},

/* en */ {IDMES_OK,
/* de */  IDMES_OK_DE,
/* fr */  IDMES_OK_FR,
/* it */  IDMES_OK_IT,
/* nl */  IDMES_OK_NL,
/* pl */  IDMES_OK_PL,
/* sv */  IDMES_OK_SV},

/* en */ {IDMES_VICE_CONTRIBUTORS,
/* de */  IDMES_VICE_CONTRIBUTORS_DE,
/* fr */  IDMES_VICE_CONTRIBUTORS_FR,
/* it */  IDMES_VICE_CONTRIBUTORS_IT,
/* nl */  IDMES_VICE_CONTRIBUTORS_NL,
/* pl */  IDMES_VICE_CONTRIBUTORS_PL,
/* sv */  IDMES_VICE_CONTRIBUTORS_SV},

/* en */ {IDMES_WHO_MADE_WHAT,
/* de */  IDMES_WHO_MADE_WHAT_DE,
/* fr */  IDMES_WHO_MADE_WHAT_FR,
/* it */  IDMES_WHO_MADE_WHAT_IT,
/* nl */  IDMES_WHO_MADE_WHAT_NL,
/* pl */  IDMES_WHO_MADE_WHAT_PL,
/* sv */  IDMES_WHO_MADE_WHAT_SV},

/* en */ {IDMES_VICE_DIST_NO_WARRANTY,
/* de */  IDMES_VICE_DIST_NO_WARRANTY_DE,
/* fr */  IDMES_VICE_DIST_NO_WARRANTY_FR,
/* it */  IDMES_VICE_DIST_NO_WARRANTY_IT,
/* nl */  IDMES_VICE_DIST_NO_WARRANTY_NL,
/* pl */  IDMES_VICE_DIST_NO_WARRANTY_PL,
/* sv */  IDMES_VICE_DIST_NO_WARRANTY_SV},

/* en */ {IDMES_WHICH_COMMANDS_AVAILABLE,
/* de */  IDMES_WHICH_COMMANDS_AVAILABLE_DE,
/* fr */  IDMES_WHICH_COMMANDS_AVAILABLE_FR,
/* it */  IDMES_WHICH_COMMANDS_AVAILABLE_IT,
/* nl */  IDMES_WHICH_COMMANDS_AVAILABLE_NL,
/* pl */  IDMES_WHICH_COMMANDS_AVAILABLE_PL,
/* sv */  IDMES_WHICH_COMMANDS_AVAILABLE_SV},

/* en */ {IDMES_CANNOT_SAVE_SETTINGS,
/* de */  IDMES_CANNOT_SAVE_SETTINGS_DE,
/* fr */  IDMES_CANNOT_SAVE_SETTINGS_FR,
/* it */  IDMES_CANNOT_SAVE_SETTINGS_IT,
/* nl */  IDMES_CANNOT_SAVE_SETTINGS_NL,
/* pl */  IDMES_CANNOT_SAVE_SETTINGS_PL,
/* sv */  IDMES_CANNOT_SAVE_SETTINGS_SV},

/* en */ {IDMES_CANNOT_LOAD_SETTINGS,
/* de */  IDMES_CANNOT_LOAD_SETTINGS_DE,
/* fr */  IDMES_CANNOT_LOAD_SETTINGS_FR,
/* it */  IDMES_CANNOT_LOAD_SETTINGS_IT,
/* nl */  IDMES_CANNOT_LOAD_SETTINGS_NL,
/* pl */  IDMES_CANNOT_LOAD_SETTINGS_PL,
/* sv */  IDMES_CANNOT_LOAD_SETTINGS_SV},

/* en */ {IDMES_VICE_ERROR,
/* de */  IDMES_VICE_ERROR_DE,
/* fr */  IDMES_VICE_ERROR_FR,
/* it */  IDMES_VICE_ERROR_IT,
/* nl */  IDMES_VICE_ERROR_NL,
/* pl */  IDMES_VICE_ERROR_PL,
/* sv */  IDMES_VICE_ERROR_SV},

/* en */ {IDMES_NO_JOY_ON_PORT_D,
/* de */  IDMES_NO_JOY_ON_PORT_D_DE,
/* fr */  IDMES_NO_JOY_ON_PORT_D_FR,
/* it */  IDMES_NO_JOY_ON_PORT_D_IT,
/* nl */  IDMES_NO_JOY_ON_PORT_D_NL,
/* pl */  IDMES_NO_JOY_ON_PORT_D_PL,
/* sv */  IDMES_NO_JOY_ON_PORT_D_SV},

/* en */ {IDMES_MOUSE_ON_PORT_D,
/* de */  IDMES_MOUSE_ON_PORT_D_DE,
/* fr */  IDMES_MOUSE_ON_PORT_D_FR,
/* it */  IDMES_MOUSE_ON_PORT_D_IT,
/* nl */  IDMES_MOUSE_ON_PORT_D_NL,
/* pl */  IDMES_MOUSE_ON_PORT_D_PL,
/* sv */  IDMES_MOUSE_ON_PORT_D_SV},

/* en */ {IDMES_UNKNOWN_DEVICE_ON_PORT_D,
/* de */  IDMES_UNKNOWN_DEVICE_ON_PORT_D_DE,
/* fr */  IDMES_UNKNOWN_DEVICE_ON_PORT_D_FR,
/* it */  IDMES_UNKNOWN_DEVICE_ON_PORT_D_IT,
/* nl */  IDMES_UNKNOWN_DEVICE_ON_PORT_D_NL,
/* pl */  IDMES_UNKNOWN_DEVICE_ON_PORT_D_PL,
/* sv */  IDMES_UNKNOWN_DEVICE_ON_PORT_D_SV},

/* en */ {IDMES_DEVICE_NOT_GAMEPAD,
/* de */  IDMES_DEVICE_NOT_GAMEPAD_DE,
/* fr */  IDMES_DEVICE_NOT_GAMEPAD_FR,
/* it */  IDMES_DEVICE_NOT_GAMEPAD_IT,
/* nl */  IDMES_DEVICE_NOT_GAMEPAD_NL,
/* pl */  IDMES_DEVICE_NOT_GAMEPAD_PL,
/* sv */  IDMES_DEVICE_NOT_GAMEPAD_SV},

/* en */ {IDMES_NOT_MAPPED_TO_AMIGA_PORT,
/* de */  IDMES_NOT_MAPPED_TO_AMIGA_PORT_DE,
/* fr */  IDMES_NOT_MAPPED_TO_AMIGA_PORT_FR,
/* it */  IDMES_NOT_MAPPED_TO_AMIGA_PORT_IT,
/* nl */  IDMES_NOT_MAPPED_TO_AMIGA_PORT_NL,
/* pl */  IDMES_NOT_MAPPED_TO_AMIGA_PORT_PL,
/* sv */  IDMES_NOT_MAPPED_TO_AMIGA_PORT_SV},

/* en */ {IDMES_CANNOT_AUTOSTART_FILE,
/* de */  IDMES_CANNOT_AUTOSTART_FILE_DE,
/* fr */  IDMES_CANNOT_AUTOSTART_FILE_FR,
/* it */  IDMES_CANNOT_AUTOSTART_FILE_IT,
/* nl */  IDMES_CANNOT_AUTOSTART_FILE_NL,
/* pl */  IDMES_CANNOT_AUTOSTART_FILE_PL,
/* sv */  IDMES_CANNOT_AUTOSTART_FILE_SV},

/* en */ {IDMES_CANNOT_ATTACH_FILE,
/* de */  IDMES_CANNOT_ATTACH_FILE_DE,
/* fr */  IDMES_CANNOT_ATTACH_FILE_FR,
/* it */  IDMES_CANNOT_ATTACH_FILE_IT,
/* nl */  IDMES_CANNOT_ATTACH_FILE_NL,
/* pl */  IDMES_CANNOT_ATTACH_FILE_PL,
/* sv */  IDMES_CANNOT_ATTACH_FILE_SV},

/* en */ {IDMES_INVALID_CART,
/* de */  IDMES_INVALID_CART_DE,
/* fr */  IDMES_INVALID_CART_FR,
/* it */  IDMES_INVALID_CART_IT,
/* nl */  IDMES_INVALID_CART_NL,
/* pl */  IDMES_INVALID_CART_PL,
/* sv */  IDMES_INVALID_CART_SV},

/* en */ {IDMES_BAD_CART_CONFIG_IN_UI,
/* de */  IDMES_BAD_CART_CONFIG_IN_UI_DE,
/* fr */  IDMES_BAD_CART_CONFIG_IN_UI_FR,
/* it */  IDMES_BAD_CART_CONFIG_IN_UI_IT,
/* nl */  IDMES_BAD_CART_CONFIG_IN_UI_NL,
/* pl */  IDMES_BAD_CART_CONFIG_IN_UI_PL,
/* sv */  IDMES_BAD_CART_CONFIG_IN_UI_SV},

/* en */ {IDMES_INVALID_CART_IMAGE,
/* de */  IDMES_INVALID_CART_IMAGE_DE,
/* fr */  IDMES_INVALID_CART_IMAGE_FR,
/* it */  IDMES_INVALID_CART_IMAGE_IT,
/* nl */  IDMES_INVALID_CART_IMAGE_NL,
/* pl */  IDMES_INVALID_CART_IMAGE_PL,
/* sv */  IDMES_INVALID_CART_IMAGE_SV},

/* en */ {IDMES_CANNOT_CREATE_IMAGE,
/* de */  IDMES_CANNOT_CREATE_IMAGE_DE,
/* fr */  IDMES_CANNOT_CREATE_IMAGE_FR,
/* it */  IDMES_CANNOT_CREATE_IMAGE_IT,
/* nl */  IDMES_CANNOT_CREATE_IMAGE_NL,
/* pl */  IDMES_CANNOT_CREATE_IMAGE_PL,
/* sv */  IDMES_CANNOT_CREATE_IMAGE_SV},

/* en */ {IDMES_ERROR_STARTING_SERVER,
/* de */  IDMES_ERROR_STARTING_SERVER_DE,
/* fr */  IDMES_ERROR_STARTING_SERVER_FR,
/* it */  IDMES_ERROR_STARTING_SERVER_IT,
/* nl */  IDMES_ERROR_STARTING_SERVER_NL,
/* pl */  IDMES_ERROR_STARTING_SERVER_PL,
/* sv */  IDMES_ERROR_STARTING_SERVER_SV},

/* en */ {IDMES_ERROR_CONNECTING_CLIENT,
/* de */  IDMES_ERROR_CONNECTING_CLIENT_DE,
/* fr */  IDMES_ERROR_CONNECTING_CLIENT_FR,
/* it */  IDMES_ERROR_CONNECTING_CLIENT_IT,
/* nl */  IDMES_ERROR_CONNECTING_CLIENT_NL,
/* pl */  IDMES_ERROR_CONNECTING_CLIENT_PL,
/* sv */  IDMES_ERROR_CONNECTING_CLIENT_SV},

/* en */ {IDMES_INVALID_PORT_NUMBER,
/* de */  IDMES_INVALID_PORT_NUMBER_DE,
/* fr */  IDMES_INVALID_PORT_NUMBER_FR,
/* it */  IDMES_INVALID_PORT_NUMBER_IT,
/* nl */  IDMES_INVALID_PORT_NUMBER_NL,
/* pl */  IDMES_INVALID_PORT_NUMBER_PL,
/* sv */  IDMES_INVALID_PORT_NUMBER_SV},

/* en */ {IDMES_THIS_MACHINE_NO_SID,
/* de */  IDMES_THIS_MACHINE_NO_SID_DE,
/* fr */  IDMES_THIS_MACHINE_NO_SID_FR,
/* it */  IDMES_THIS_MACHINE_NO_SID_IT,
/* nl */  IDMES_THIS_MACHINE_NO_SID_NL,
/* pl */  IDMES_THIS_MACHINE_NO_SID_PL,
/* sv */  IDMES_THIS_MACHINE_NO_SID_SV},

/* en */ {IDMES_SCREENSHOT_SAVE_S_FAILED,
/* de */  IDMES_SCREENSHOT_SAVE_S_FAILED_DE,
/* fr */  IDMES_SCREENSHOT_SAVE_S_FAILED_FR,
/* it */  IDMES_SCREENSHOT_SAVE_S_FAILED_IT,
/* nl */  IDMES_SCREENSHOT_SAVE_S_FAILED_NL,
/* pl */  IDMES_SCREENSHOT_SAVE_S_FAILED_PL,
/* sv */  IDMES_SCREENSHOT_SAVE_S_FAILED_SV},

/* en */ {IDMES_SCREENSHOT_S_SAVED,
/* de */  IDMES_SCREENSHOT_S_SAVED_DE,
/* fr */  IDMES_SCREENSHOT_S_SAVED_FR,
/* it */  IDMES_SCREENSHOT_S_SAVED_IT,
/* nl */  IDMES_SCREENSHOT_S_SAVED_NL,
/* pl */  IDMES_SCREENSHOT_S_SAVED_PL,
/* sv */  IDMES_SCREENSHOT_S_SAVED_SV},

/* ----------------------- AmigaOS Strings ----------------------- */

/* en */ {IDS_PRESS_KEY_BUTTON,
/* de */  IDS_PRESS_KEY_BUTTON_DE,
/* fr */  IDS_PRESS_KEY_BUTTON_FR,
/* it */  IDS_PRESS_KEY_BUTTON_IT,
/* nl */  IDS_PRESS_KEY_BUTTON_NL,
/* pl */  IDS_PRESS_KEY_BUTTON_PL,
/* sv */  IDS_PRESS_KEY_BUTTON_SV},

/* en */ {IDS_SAVE,
/* de */  IDS_SAVE_DE,
/* fr */  IDS_SAVE_FR,
/* it */  IDS_SAVE_IT,
/* nl */  IDS_SAVE_NL,
/* pl */  IDS_SAVE_PL,
/* sv */  IDS_SAVE_SV},

/* en */ {IDS_SAVE_SCREENSHOT,
/* de */  IDS_SAVE_SCREENSHOT_DE,
/* fr */  IDS_SAVE_SCREENSHOT_FR,
/* it */  IDS_SAVE_SCREENSHOT_IT,
/* nl */  IDS_SAVE_SCREENSHOT_NL,
/* pl */  IDS_SAVE_SCREENSHOT_PL,
/* sv */  IDS_SAVE_SCREENSHOT_SV},

/* en */ {IDS_CANCEL,
/* de */  IDS_CANCEL_DE,
/* fr */  IDS_CANCEL_FR,
/* it */  IDS_CANCEL_IT,
/* nl */  IDS_CANCEL_NL,
/* pl */  IDS_CANCEL_PL,
/* sv */  IDS_CANCEL_SV},

/* en */ {IDS_CHOOSE_SCREENSHOT_FORMAT,
/* de */  IDS_CHOOSE_SCREENSHOT_FORMAT_DE,
/* fr */  IDS_CHOOSE_SCREENSHOT_FORMAT_FR,
/* it */  IDS_CHOOSE_SCREENSHOT_FORMAT_IT,
/* nl */  IDS_CHOOSE_SCREENSHOT_FORMAT_NL,
/* pl */  IDS_CHOOSE_SCREENSHOT_FORMAT_PL,
/* sv */  IDS_CHOOSE_SCREENSHOT_FORMAT_SV},

/* en */ {IDS_SAVE_S_SCREENSHOT,
/* de */  IDS_SAVE_S_SCREENSHOT_DE,
/* fr */  IDS_SAVE_S_SCREENSHOT_FR,
/* it */  IDS_SAVE_S_SCREENSHOT_IT,
/* nl */  IDS_SAVE_S_SCREENSHOT_NL,
/* pl */  IDS_SAVE_S_SCREENSHOT_PL,
/* sv */  IDS_SAVE_S_SCREENSHOT_SV},

/* en */ {IDS_DETACHED_DEVICE_D,
/* de */  IDS_DETACHED_DEVICE_D_DE,
/* fr */  IDS_DETACHED_DEVICE_D_FR,
/* it */  IDS_DETACHED_DEVICE_D_IT,
/* nl */  IDS_DETACHED_DEVICE_D_NL,
/* pl */  IDS_DETACHED_DEVICE_D_PL,
/* sv */  IDS_DETACHED_DEVICE_D_SV},

/* en */ {IDS_ATTACHED_S_TO_DEVICE_D,
/* de */  IDS_ATTACHED_S_TO_DEVICE_D_DE,
/* fr */  IDS_ATTACHED_S_TO_DEVICE_D_FR,
/* it */  IDS_ATTACHED_S_TO_DEVICE_D_IT,
/* nl */  IDS_ATTACHED_S_TO_DEVICE_D_NL,
/* pl */  IDS_ATTACHED_S_TO_DEVICE_D_PL,
/* sv */  IDS_ATTACHED_S_TO_DEVICE_D_SV},

/* en */ {IDS_VICE_QUESTION,
/* de */  IDS_VICE_QUESTION_DE,
/* fr */  IDS_VICE_QUESTION_FR,
/* it */  IDS_VICE_QUESTION_IT,
/* nl */  IDS_VICE_QUESTION_NL,
/* pl */  IDS_VICE_QUESTION_PL,
/* sv */  IDS_VICE_QUESTION_SV},

/* en */ {IDS_EXTEND_TO_40_TRACK,
/* de */  IDS_EXTEND_TO_40_TRACK_DE,
/* fr */  IDS_EXTEND_TO_40_TRACK_FR,
/* it */  IDS_EXTEND_TO_40_TRACK_IT,
/* nl */  IDS_EXTEND_TO_40_TRACK_NL,
/* pl */  IDS_EXTEND_TO_40_TRACK_PL,
/* sv */  IDS_EXTEND_TO_40_TRACK_SV},

/* en */ {IDS_YES_NO,
/* de */  IDS_YES_NO_DE,
/* fr */  IDS_YES_NO_FR,
/* it */  IDS_YES_NO_IT,
/* nl */  IDS_YES_NO_NL,
/* pl */  IDS_YES_NO_PL,
/* sv */  IDS_YES_NO_SV},

/* en */ {IDS_DETACHED_TAPE,
/* de */  IDS_DETACHED_TAPE_DE,
/* fr */  IDS_DETACHED_TAPE_FR,
/* it */  IDS_DETACHED_TAPE_IT,
/* nl */  IDS_DETACHED_TAPE_NL,
/* pl */  IDS_DETACHED_TAPE_PL,
/* sv */  IDS_DETACHED_TAPE_SV},

/* en */ {IDS_ATTACHED_TAPE_S,
/* de */  IDS_ATTACHED_TAPE_S_DE,
/* fr */  IDS_ATTACHED_TAPE_S_FR,
/* it */  IDS_ATTACHED_TAPE_S_IT,
/* nl */  IDS_ATTACHED_TAPE_S_NL,
/* pl */  IDS_ATTACHED_TAPE_S_PL,
/* sv */  IDS_ATTACHED_TAPE_S_SV},

/* en */ {IDS_SELECT_START_SNAPSHOT,
/* de */  IDS_SELECT_START_SNAPSHOT_DE,
/* fr */  IDS_SELECT_START_SNAPSHOT_FR,
/* it */  IDS_SELECT_START_SNAPSHOT_IT,
/* nl */  IDS_SELECT_START_SNAPSHOT_NL,
/* pl */  IDS_SELECT_START_SNAPSHOT_PL,
/* sv */  IDS_SELECT_START_SNAPSHOT_SV},

/* en */ {IDS_SELECT_END_SNAPSHOT,
/* de */  IDS_SELECT_END_SNAPSHOT_DE,
/* fr */  IDS_SELECT_END_SNAPSHOT_FR,
/* it */  IDS_SELECT_END_SNAPSHOT_IT,
/* nl */  IDS_SELECT_END_SNAPSHOT_NL,
/* pl */  IDS_SELECT_END_SNAPSHOT_PL,
/* sv */  IDS_SELECT_END_SNAPSHOT_SV},

/* en */ {IDS_REALLY_EXIT,
/* de */  IDS_REALLY_EXIT_DE,
/* fr */  IDS_REALLY_EXIT_FR,
/* it */  IDS_REALLY_EXIT_IT,
/* nl */  IDS_REALLY_EXIT_NL,
/* pl */  IDS_REALLY_EXIT_PL,
/* sv */  IDS_REALLY_EXIT_SV},

/* en */ {IDS_ATTACH_CRT_IMAGE,
/* de */  IDS_ATTACH_CRT_IMAGE_DE,
/* fr */  IDS_ATTACH_CRT_IMAGE_FR,
/* it */  IDS_ATTACH_CRT_IMAGE_IT,
/* nl */  IDS_ATTACH_CRT_IMAGE_NL,
/* pl */  IDS_ATTACH_CRT_IMAGE_PL,
/* sv */  IDS_ATTACH_CRT_IMAGE_SV},

/* en */ {IDS_ATTACH_RAW_8KB_IMAGE,
/* de */  IDS_ATTACH_RAW_8KB_IMAGE_DE,
/* fr */  IDS_ATTACH_RAW_8KB_IMAGE_FR,
/* it */  IDS_ATTACH_RAW_8KB_IMAGE_IT,
/* nl */  IDS_ATTACH_RAW_8KB_IMAGE_NL,
/* pl */  IDS_ATTACH_RAW_8KB_IMAGE_PL,
/* sv */  IDS_ATTACH_RAW_8KB_IMAGE_SV},

/* en */ {IDS_ATTACH_RAW_16KB_IMAGE,
/* de */  IDS_ATTACH_RAW_16KB_IMAGE_DE,
/* fr */  IDS_ATTACH_RAW_16KB_IMAGE_FR,
/* it */  IDS_ATTACH_RAW_16KB_IMAGE_IT,
/* nl */  IDS_ATTACH_RAW_16KB_IMAGE_NL,
/* pl */  IDS_ATTACH_RAW_16KB_IMAGE_PL,
/* sv */  IDS_ATTACH_RAW_16KB_IMAGE_SV},

/* en */ {IDS_ATTACH_ACTION_REPLAY_IMAGE,
/* de */  IDS_ATTACH_ACTION_REPLAY_IMAGE_DE,
/* fr */  IDS_ATTACH_ACTION_REPLAY_IMAGE_FR,
/* it */  IDS_ATTACH_ACTION_REPLAY_IMAGE_IT,
/* nl */  IDS_ATTACH_ACTION_REPLAY_IMAGE_NL,
/* pl */  IDS_ATTACH_ACTION_REPLAY_IMAGE_PL,
/* sv */  IDS_ATTACH_ACTION_REPLAY_IMAGE_SV},

/* en */ {IDS_ATTACH_ATOMIC_POWER_IMAGE,
/* de */  IDS_ATTACH_ATOMIC_POWER_IMAGE_DE,
/* fr */  IDS_ATTACH_ATOMIC_POWER_IMAGE_FR,
/* it */  IDS_ATTACH_ATOMIC_POWER_IMAGE_IT,
/* nl */  IDS_ATTACH_ATOMIC_POWER_IMAGE_NL,
/* pl */  IDS_ATTACH_ATOMIC_POWER_IMAGE_PL,
/* sv */  IDS_ATTACH_ATOMIC_POWER_IMAGE_SV},

/* en */ {IDS_ATTACH_EPYX_FASTLOAD_IMAGE,
/* de */  IDS_ATTACH_EPYX_FASTLOAD_IMAGE_DE,
/* fr */  IDS_ATTACH_EPYX_FASTLOAD_IMAGE_FR,
/* it */  IDS_ATTACH_EPYX_FASTLOAD_IMAGE_IT,
/* nl */  IDS_ATTACH_EPYX_FASTLOAD_IMAGE_NL,
/* pl */  IDS_ATTACH_EPYX_FASTLOAD_IMAGE_PL,
/* sv */  IDS_ATTACH_EPYX_FASTLOAD_IMAGE_SV},

/* en */ {IDS_ATTACH_IEEE488_IMAGE,
/* de */  IDS_ATTACH_IEEE488_IMAGE_DE,
/* fr */  IDS_ATTACH_IEEE488_IMAGE_FR,
/* it */  IDS_ATTACH_IEEE488_IMAGE_IT,
/* nl */  IDS_ATTACH_IEEE488_IMAGE_NL,
/* pl */  IDS_ATTACH_IEEE488_IMAGE_PL,
/* sv */  IDS_ATTACH_IEEE488_IMAGE_SV},

/* en */ {IDS_ATTACH_RETRO_REPLAY_IMAGE,
/* de */  IDS_ATTACH_RETRO_REPLAY_IMAGE_DE,
/* fr */  IDS_ATTACH_RETRO_REPLAY_IMAGE_FR,
/* it */  IDS_ATTACH_RETRO_REPLAY_IMAGE_IT,
/* nl */  IDS_ATTACH_RETRO_REPLAY_IMAGE_NL,
/* pl */  IDS_ATTACH_RETRO_REPLAY_IMAGE_PL,
/* sv */  IDS_ATTACH_RETRO_REPLAY_IMAGE_SV},

/* en */ {IDS_ATTACH_IDE64_IMAGE,
/* de */  IDS_ATTACH_IDE64_IMAGE_DE,
/* fr */  IDS_ATTACH_IDE64_IMAGE_FR,
/* it */  IDS_ATTACH_IDE64_IMAGE_IT,
/* nl */  IDS_ATTACH_IDE64_IMAGE_NL,
/* pl */  IDS_ATTACH_IDE64_IMAGE_PL,
/* sv */  IDS_ATTACH_IDE64_IMAGE_SV},

/* en */ {IDS_ATTACH_SS4_IMAGE,
/* de */  IDS_ATTACH_SS4_IMAGE_DE,
/* fr */  IDS_ATTACH_SS4_IMAGE_FR,
/* it */  IDS_ATTACH_SS4_IMAGE_IT,
/* nl */  IDS_ATTACH_SS4_IMAGE_NL,
/* pl */  IDS_ATTACH_SS4_IMAGE_PL,
/* sv */  IDS_ATTACH_SS4_IMAGE_SV},

/* en */ {IDS_ATTACH_SS5_IMAGE,
/* de */  IDS_ATTACH_SS5_IMAGE_DE,
/* fr */  IDS_ATTACH_SS5_IMAGE_FR,
/* it */  IDS_ATTACH_SS5_IMAGE_IT,
/* nl */  IDS_ATTACH_SS5_IMAGE_NL,
/* pl */  IDS_ATTACH_SS5_IMAGE_PL,
/* sv */  IDS_ATTACH_SS5_IMAGE_SV},

/* en */ {IDS_ATTACH_SB_IMAGE,
/* de */  IDS_ATTACH_SB_IMAGE_DE,
/* fr */  IDS_ATTACH_SB_IMAGE_FR,
/* it */  IDS_ATTACH_SB_IMAGE_IT,
/* nl */  IDS_ATTACH_SB_IMAGE_NL,
/* pl */  IDS_ATTACH_SB_IMAGE_PL,
/* sv */  IDS_ATTACH_SB_IMAGE_SV},

/* en */ {IDS_AVAILABLE_CMDLINE_OPTIONS,
/* de */  IDS_AVAILABLE_CMDLINE_OPTIONS_DE,
/* fr */  IDS_AVAILABLE_CMDLINE_OPTIONS_FR,
/* it */  IDS_AVAILABLE_CMDLINE_OPTIONS_IT,
/* nl */  IDS_AVAILABLE_CMDLINE_OPTIONS_NL,
/* pl */  IDS_AVAILABLE_CMDLINE_OPTIONS_PL,
/* sv */  IDS_AVAILABLE_CMDLINE_OPTIONS_SV},

/* en */ {IDS_ATTACH_4_8_16KB_AT_2000,
/* de */  IDS_ATTACH_4_8_16KB_AT_2000_DE,
/* fr */  IDS_ATTACH_4_8_16KB_AT_2000_FR,
/* it */  IDS_ATTACH_4_8_16KB_AT_2000_IT,
/* nl */  IDS_ATTACH_4_8_16KB_AT_2000_NL,
/* pl */  IDS_ATTACH_4_8_16KB_AT_2000_PL,
/* sv */  IDS_ATTACH_4_8_16KB_AT_2000_SV},

/* en */ {IDS_ATTACH_4_8_16KB_AT_4000,
/* de */  IDS_ATTACH_4_8_16KB_AT_4000_DE,
/* fr */  IDS_ATTACH_4_8_16KB_AT_4000_FR,
/* it */  IDS_ATTACH_4_8_16KB_AT_4000_IT,
/* nl */  IDS_ATTACH_4_8_16KB_AT_4000_NL,
/* pl */  IDS_ATTACH_4_8_16KB_AT_4000_PL,
/* sv */  IDS_ATTACH_4_8_16KB_AT_4000_SV},

/* en */ {IDS_ATTACH_4_8_16KB_AT_6000,
/* de */  IDS_ATTACH_4_8_16KB_AT_6000_DE,
/* fr */  IDS_ATTACH_4_8_16KB_AT_6000_FR,
/* it */  IDS_ATTACH_4_8_16KB_AT_6000_IT,
/* nl */  IDS_ATTACH_4_8_16KB_AT_6000_NL,
/* pl */  IDS_ATTACH_4_8_16KB_AT_6000_PL,
/* sv */  IDS_ATTACH_4_8_16KB_AT_6000_SV},

/* en */ {IDS_ATTACH_4_8KB_AT_A000,
/* de */  IDS_ATTACH_4_8KB_AT_A000_DE,
/* fr */  IDS_ATTACH_4_8KB_AT_A000_FR,
/* it */  IDS_ATTACH_4_8KB_AT_A000_IT,
/* nl */  IDS_ATTACH_4_8KB_AT_A000_NL,
/* pl */  IDS_ATTACH_4_8KB_AT_A000_PL,
/* sv */  IDS_ATTACH_4_8KB_AT_A000_SV},

/* en */ {IDS_ATTACH_4KB_AT_B000,
/* de */  IDS_ATTACH_4KB_AT_B000_DE,
/* fr */  IDS_ATTACH_4KB_AT_B000_FR,
/* it */  IDS_ATTACH_4KB_AT_B000_IT,
/* nl */  IDS_ATTACH_4KB_AT_B000_NL,
/* pl */  IDS_ATTACH_4KB_AT_B000_PL,
/* sv */  IDS_ATTACH_4KB_AT_B000_SV},

/* en */ {IDS_S_AT_D_SPEED,
/* de */  IDS_S_AT_D_SPEED_DE,
/* fr */  IDS_S_AT_D_SPEED_FR,
/* it */  IDS_S_AT_D_SPEED_IT,
/* nl */  IDS_S_AT_D_SPEED_NL,
/* pl */  IDS_S_AT_D_SPEED_PL,
/* sv */  IDS_S_AT_D_SPEED_SV},

/* en */ {IDS_ATTACH,
/* de */  IDS_ATTACH_DE,
/* fr */  IDS_ATTACH_FR,
/* it */  IDS_ATTACH_IT,
/* nl */  IDS_ATTACH_NL,
/* pl */  IDS_ATTACH_PL,
/* sv */  IDS_ATTACH_SV},

/* en */ {IDS_PARENT,
/* de */  IDS_PARENT_DE,
/* fr */  IDS_PARENT_FR,
/* it */  IDS_PARENT_IT,
/* nl */  IDS_PARENT_NL,
/* pl */  IDS_PARENT_PL,
/* sv */  IDS_PARENT_SV},

/* en */ {IDS_ATTACH_READ_ONLY,
/* de */  IDS_ATTACH_READ_ONLY_DE,
/* fr */  IDS_ATTACH_READ_ONLY_FR,
/* it */  IDS_ATTACH_READ_ONLY_IT,
/* nl */  IDS_ATTACH_READ_ONLY_NL,
/* pl */  IDS_ATTACH_READ_ONLY_PL,
/* sv */  IDS_ATTACH_READ_ONLY_SV},

/* en */ {IDS_NAME,
/* de */  IDS_NAME_DE,
/* fr */  IDS_NAME_FR,
/* it */  IDS_NAME_IT,
/* nl */  IDS_NAME_NL,
/* pl */  IDS_NAME_PL,
/* sv */  IDS_NAME_SV},

/* en */ {IDS_CREATE_IMAGE,
/* de */  IDS_CREATE_IMAGE_DE,
/* fr */  IDS_CREATE_IMAGE_FR,
/* it */  IDS_CREATE_IMAGE_IT,
/* nl */  IDS_CREATE_IMAGE_NL,
/* pl */  IDS_CREATE_IMAGE_PL,
/* sv */  IDS_CREATE_IMAGE_SV},

/* en */ {IDS_NEW_IMAGE,
/* de */  IDS_NEW_IMAGE_DE,
/* fr */  IDS_NEW_IMAGE_FR,
/* it */  IDS_NEW_IMAGE_IT,
/* nl */  IDS_NEW_IMAGE_NL,
/* pl */  IDS_NEW_IMAGE_PL,
/* sv */  IDS_NEW_IMAGE_SV},

/* en */ {IDS_NEW_TAP_IMAGE,
/* de */  IDS_NEW_TAP_IMAGE_DE,
/* fr */  IDS_NEW_TAP_IMAGE_FR,
/* it */  IDS_NEW_TAP_IMAGE_IT,
/* nl */  IDS_NEW_TAP_IMAGE_NL,
/* pl */  IDS_NEW_TAP_IMAGE_PL,
/* sv */  IDS_NEW_TAP_IMAGE_SV},

/* en */ {IDS_OVERWRITE_EXISTING_IMAGE,
/* de */  IDS_OVERWRITE_EXISTING_IMAGE_DE,
/* fr */  IDS_OVERWRITE_EXISTING_IMAGE_FR,
/* it */  IDS_OVERWRITE_EXISTING_IMAGE_IT,
/* nl */  IDS_OVERWRITE_EXISTING_IMAGE_NL,
/* pl */  IDS_OVERWRITE_EXISTING_IMAGE_PL,
/* sv */  IDS_OVERWRITE_EXISTING_IMAGE_SV},

/* en */ {IDS_IMAGE_CONTENTS,
/* de */  IDS_IMAGE_CONTENTS_DE,
/* fr */  IDS_IMAGE_CONTENTS_FR,
/* it */  IDS_IMAGE_CONTENTS_IT,
/* nl */  IDS_IMAGE_CONTENTS_NL,
/* pl */  IDS_IMAGE_CONTENTS_PL,
/* sv */  IDS_IMAGE_CONTENTS_SV},

/* en */ {IDS_ABOUT,
/* de */  IDS_ABOUT_DE,
/* fr */  IDS_ABOUT_FR,
/* it */  IDS_ABOUT_IT,
/* nl */  IDS_ABOUT_NL,
/* pl */  IDS_ABOUT_PL,
/* sv */  IDS_ABOUT_SV},

/* en */ {IDMS_DISABLED,
/* de */  IDMS_DISABLED_DE,
/* fr */  IDMS_DISABLED_FR,
/* it */  IDMS_DISABLED_IT,
/* nl */  IDMS_DISABLED_NL,
/* pl */  IDMS_DISABLED_PL,
/* sv */  IDMS_DISABLED_SV},

/* en */ {IDS_ENABLED,
/* de */  IDS_ENABLED_DE,
/* fr */  IDS_ENABLED_FR,
/* it */  IDS_ENABLED_IT,
/* nl */  IDS_ENABLED_NL,
/* pl */  IDS_ENABLED_PL,
/* sv */  IDS_ENABLED_SV},

/* en */ {IDS_RS232_DEVICE_1,
/* de */  IDS_RS232_DEVICE_1_DE,
/* fr */  IDS_RS232_DEVICE_1_FR,
/* it */  IDS_RS232_DEVICE_1_IT,
/* nl */  IDS_RS232_DEVICE_1_NL,
/* pl */  IDS_RS232_DEVICE_1_PL,
/* sv */  IDS_RS232_DEVICE_1_SV},

/* en */ {IDS_RS232_DEVICE_2,
/* de */  IDS_RS232_DEVICE_2_DE,
/* fr */  IDS_RS232_DEVICE_2_FR,
/* it */  IDS_RS232_DEVICE_2_IT,
/* nl */  IDS_RS232_DEVICE_2_NL,
/* pl */  IDS_RS232_DEVICE_2_PL,
/* sv */  IDS_RS232_DEVICE_2_SV},

/* en */ {IDS_RS232_DEVICE_3,
/* de */  IDS_RS232_DEVICE_3_DE,
/* fr */  IDS_RS232_DEVICE_3_FR,
/* it */  IDS_RS232_DEVICE_3_IT,
/* nl */  IDS_RS232_DEVICE_3_NL,
/* pl */  IDS_RS232_DEVICE_3_PL,
/* sv */  IDS_RS232_DEVICE_3_SV},

/* en */ {IDS_RS232_DEVICE_4,
/* de */  IDS_RS232_DEVICE_4_DE,
/* fr */  IDS_RS232_DEVICE_4_FR,
/* it */  IDS_RS232_DEVICE_4_IT,
/* nl */  IDS_RS232_DEVICE_4_NL,
/* pl */  IDS_RS232_DEVICE_4_PL,
/* sv */  IDS_RS232_DEVICE_4_SV},

/* en */ {IDS_NONE,
/* de */  IDS_NONE_DE,
/* fr */  IDS_NONE_FR,
/* it */  IDS_NONE_IT,
/* nl */  IDS_NONE_NL,
/* pl */  IDS_NONE_PL,
/* sv */  IDS_NONE_SV},

/* en */ {IDS_IRQ,
/* de */  IDS_IRQ_DE,
/* fr */  IDS_IRQ_FR,
/* it */  IDS_IRQ_IT,
/* nl */  IDS_IRQ_NL,
/* pl */  IDS_IRQ_PL,
/* sv */  IDS_IRQ_SV},

/* en */ {IDS_NMI,
/* de */  IDS_NMI_DE,
/* fr */  IDS_NMI_FR,
/* it */  IDS_NMI_IT,
/* nl */  IDS_NMI_NL,
/* pl */  IDS_NMI_PL,
/* sv */  IDS_NMI_SV},

/* en */ {IDS_ACIA_DEVICE,
/* de */  IDS_ACIA_DEVICE_DE,
/* fr */  IDS_ACIA_DEVICE_FR,
/* it */  IDS_ACIA_DEVICE_IT,
/* nl */  IDS_ACIA_DEVICE_NL,
/* pl */  IDS_ACIA_DEVICE_PL,
/* sv */  IDS_ACIA_DEVICE_SV},

/* en */ {IDS_ACIA_INTERRUPT,
/* de */  IDS_ACIA_INTERRUPT_DE,
/* fr */  IDS_ACIA_INTERRUPT_FR,
/* it */  IDS_ACIA_INTERRUPT_IT,
/* nl */  IDS_ACIA_INTERRUPT_NL,
/* pl */  IDS_ACIA_INTERRUPT_PL,
/* sv */  IDS_ACIA_INTERRUPT_SV},

/* en */ {IDS_ACIA_MODE,
/* de */  IDS_ACIA_MODE_DE,
/* fr */  IDS_ACIA_MODE_FR,
/* it */  IDS_ACIA_MODE_IT,
/* nl */  IDS_ACIA_MODE_NL,
/* pl */  IDS_ACIA_MODE_PL,
/* sv */  IDS_ACIA_MODE_SV},

/* en */ {IDS_ACIA_SETTINGS,
/* de */  IDS_ACIA_SETTINGS_DE,
/* fr */  IDS_ACIA_SETTINGS_FR,
/* it */  IDS_ACIA_SETTINGS_IT,
/* nl */  IDS_ACIA_SETTINGS_NL,
/* pl */  IDS_ACIA_SETTINGS_PL,
/* sv */  IDS_ACIA_SETTINGS_SV},

/* en */ {IDS_256K_ENABLED,
/* de */  IDS_256K_ENABLED_DE,
/* fr */  IDS_256K_ENABLED_FR,
/* it */  IDS_256K_ENABLED_IT,
/* nl */  IDS_256K_ENABLED_NL,
/* pl */  IDS_256K_ENABLED_PL,
/* sv */  IDS_256K_ENABLED_SV},

/* en */ {IDS_256K_BASE,
/* de */  IDS_256K_BASE_DE,
/* fr */  IDS_256K_BASE_FR,
/* it */  IDS_256K_BASE_IT,
/* nl */  IDS_256K_BASE_NL,
/* pl */  IDS_256K_BASE_PL,
/* sv */  IDS_256K_BASE_SV},

/* en */ {IDS_256K_SETTINGS,
/* de */  IDS_256K_SETTINGS_DE,
/* fr */  IDS_256K_SETTINGS_FR,
/* it */  IDS_256K_SETTINGS_IT,
/* nl */  IDS_256K_SETTINGS_NL,
/* pl */  IDS_256K_SETTINGS_PL,
/* sv */  IDS_256K_SETTINGS_SV},

/* en */ {IDS_MODEL_LINE,
/* de */  IDS_MODEL_LINE_DE,
/* fr */  IDS_MODEL_LINE_FR,
/* it */  IDS_MODEL_LINE_IT,
/* nl */  IDS_MODEL_LINE_NL,
/* pl */  IDS_MODEL_LINE_PL,
/* sv */  IDS_MODEL_LINE_SV},

/* en */ {IDS_RAM_SIZE,
/* de */  IDS_RAM_SIZE_DE,
/* fr */  IDS_RAM_SIZE_FR,
/* it */  IDS_RAM_SIZE_IT,
/* nl */  IDS_RAM_SIZE_NL,
/* pl */  IDS_RAM_SIZE_PL,
/* sv */  IDS_RAM_SIZE_SV},

/* en */ {IDS_RAM_BLOCK_0400_0FFF,
/* de */  IDS_RAM_BLOCK_0400_0FFF_DE,
/* fr */  IDS_RAM_BLOCK_0400_0FFF_FR,
/* it */  IDS_RAM_BLOCK_0400_0FFF_IT,
/* nl */  IDS_RAM_BLOCK_0400_0FFF_NL,
/* pl */  IDS_RAM_BLOCK_0400_0FFF_PL,
/* sv */  IDS_RAM_BLOCK_0400_0FFF_SV},

/* en */ {IDS_RAM_BLOCK_0800_0FFF,
/* de */  IDS_RAM_BLOCK_0800_0FFF_DE,
/* fr */  IDS_RAM_BLOCK_0800_0FFF_FR,
/* it */  IDS_RAM_BLOCK_0800_0FFF_IT,
/* nl */  IDS_RAM_BLOCK_0800_0FFF_NL,
/* pl */  IDS_RAM_BLOCK_0800_0FFF_PL,
/* sv */  IDS_RAM_BLOCK_0800_0FFF_SV},

/* en */ {IDS_RAM_BLOCK_1000_1FFF,
/* de */  IDS_RAM_BLOCK_1000_1FFF_DE,
/* fr */  IDS_RAM_BLOCK_1000_1FFF_FR,
/* it */  IDS_RAM_BLOCK_1000_1FFF_IT,
/* nl */  IDS_RAM_BLOCK_1000_1FFF_NL,
/* pl */  IDS_RAM_BLOCK_1000_1FFF_PL,
/* sv */  IDS_RAM_BLOCK_1000_1FFF_SV},

/* en */ {IDS_RAM_BLOCK_2000_3FFF,
/* de */  IDS_RAM_BLOCK_2000_3FFF_DE,
/* fr */  IDS_RAM_BLOCK_2000_3FFF_FR,
/* it */  IDS_RAM_BLOCK_2000_3FFF_IT,
/* nl */  IDS_RAM_BLOCK_2000_3FFF_NL,
/* pl */  IDS_RAM_BLOCK_2000_3FFF_PL,
/* sv */  IDS_RAM_BLOCK_2000_3FFF_SV},

/* en */ {IDS_RAM_BLOCK_4000_5FFF,
/* de */  IDS_RAM_BLOCK_4000_5FFF_DE,
/* fr */  IDS_RAM_BLOCK_4000_5FFF_FR,
/* it */  IDS_RAM_BLOCK_4000_5FFF_IT,
/* nl */  IDS_RAM_BLOCK_4000_5FFF_NL,
/* pl */  IDS_RAM_BLOCK_4000_5FFF_PL,
/* sv */  IDS_RAM_BLOCK_4000_5FFF_SV},

/* en */ {IDS_RAM_BLOCK_6000_7FFF,
/* de */  IDS_RAM_BLOCK_6000_7FFF_DE,
/* fr */  IDS_RAM_BLOCK_6000_7FFF_FR,
/* it */  IDS_RAM_BLOCK_6000_7FFF_IT,
/* nl */  IDS_RAM_BLOCK_6000_7FFF_NL,
/* pl */  IDS_RAM_BLOCK_6000_7FFF_PL,
/* sv */  IDS_RAM_BLOCK_6000_7FFF_SV},

/* en */ {IDS_RAM_BLOCK_A000_BFFF,
/* de */  IDS_RAM_BLOCK_A000_BFFF_DE,
/* fr */  IDS_RAM_BLOCK_A000_BFFF_FR,
/* it */  IDS_RAM_BLOCK_A000_BFFF_IT,
/* nl */  IDS_RAM_BLOCK_A000_BFFF_NL,
/* pl */  IDS_RAM_BLOCK_A000_BFFF_PL,
/* sv */  IDS_RAM_BLOCK_A000_BFFF_SV},

/* en */ {IDS_RAM_BLOCK_C000_CFFF,
/* de */  IDS_RAM_BLOCK_C000_CFFF_DE,
/* fr */  IDS_RAM_BLOCK_C000_CFFF_FR,
/* it */  IDS_RAM_BLOCK_C000_CFFF_IT,
/* nl */  IDS_RAM_BLOCK_C000_CFFF_NL,
/* pl */  IDS_RAM_BLOCK_C000_CFFF_PL,
/* sv */  IDS_RAM_BLOCK_C000_CFFF_SV},

/* en */ {IDS_CBM2_SETTINGS,
/* de */  IDS_CBM2_SETTINGS_DE,
/* fr */  IDS_CBM2_SETTINGS_FR,
/* it */  IDS_CBM2_SETTINGS_IT,
/* nl */  IDS_CBM2_SETTINGS_NL,
/* pl */  IDS_CBM2_SETTINGS_PL,
/* sv */  IDS_CBM2_SETTINGS_SV},

/* en */ {IDS_RESET_DATASETTE_WITH_CPU,
/* de */  IDS_RESET_DATASETTE_WITH_CPU_DE,
/* fr */  IDS_RESET_DATASETTE_WITH_CPU_FR,
/* it */  IDS_RESET_DATASETTE_WITH_CPU_IT,
/* nl */  IDS_RESET_DATASETTE_WITH_CPU_NL,
/* pl */  IDS_RESET_DATASETTE_WITH_CPU_PL,
/* sv */  IDS_RESET_DATASETTE_WITH_CPU_SV},

/* en */ {IDS_ADDITIONAL_DELAY,
/* de */  IDS_ADDITIONAL_DELAY_DE,
/* fr */  IDS_ADDITIONAL_DELAY_FR,
/* it */  IDS_ADDITIONAL_DELAY_IT,
/* nl */  IDS_ADDITIONAL_DELAY_NL,
/* pl */  IDS_ADDITIONAL_DELAY_PL,
/* sv */  IDS_ADDITIONAL_DELAY_SV},

/* en */ {IDS_DELAY_AT_ZERO_VALUES,
/* de */  IDS_DELAY_AT_ZERO_VALUES_DE,
/* fr */  IDS_DELAY_AT_ZERO_VALUES_FR,
/* it */  IDS_DELAY_AT_ZERO_VALUES_IT,
/* nl */  IDS_DELAY_AT_ZERO_VALUES_NL,
/* pl */  IDS_DELAY_AT_ZERO_VALUES_PL,
/* sv */  IDS_DELAY_AT_ZERO_VALUES_SV},

/* en */ {IDS_DATASETTE_SETTINGS,
/* de */  IDS_DATASETTE_SETTINGS_DE,
/* fr */  IDS_DATASETTE_SETTINGS_FR,
/* it */  IDS_DATASETTE_SETTINGS_IT,
/* nl */  IDS_DATASETTE_SETTINGS_NL,
/* pl */  IDS_DATASETTE_SETTINGS_PL,
/* sv */  IDS_DATASETTE_SETTINGS_SV},

/* en */ {IDS_NEVER_EXTEND,
/* de */  IDS_NEVER_EXTEND_DE,
/* fr */  IDS_NEVER_EXTEND_FR,
/* it */  IDS_NEVER_EXTEND_IT,
/* nl */  IDS_NEVER_EXTEND_NL,
/* pl */  IDS_NEVER_EXTEND_PL,
/* sv */  IDS_NEVER_EXTEND_SV},

/* en */ {IDS_ASK_ON_EXTEND,
/* de */  IDS_ASK_ON_EXTEND_DE,
/* fr */  IDS_ASK_ON_EXTEND_FR,
/* it */  IDS_ASK_ON_EXTEND_IT,
/* nl */  IDS_ASK_ON_EXTEND_NL,
/* pl */  IDS_ASK_ON_EXTEND_PL,
/* sv */  IDS_ASK_ON_EXTEND_SV},

/* en */ {IDS_EXTEND_ON_ACCESS,
/* de */  IDS_EXTEND_ON_ACCESS_DE,
/* fr */  IDS_EXTEND_ON_ACCESS_FR,
/* it */  IDS_EXTEND_ON_ACCESS_IT,
/* nl */  IDS_EXTEND_ON_ACCESS_NL,
/* pl */  IDS_EXTEND_ON_ACCESS_PL,
/* sv */  IDS_EXTEND_ON_ACCESS_SV},

/* en */ {IDS_NO_TRAPS,
/* de */  IDS_NO_TRAPS_DE,
/* fr */  IDS_NO_TRAPS_FR,
/* it */  IDS_NO_TRAPS_IT,
/* nl */  IDS_NO_TRAPS_NL,
/* pl */  IDS_NO_TRAPS_PL,
/* sv */  IDS_NO_TRAPS_SV},

/* en */ {IDS_SKIP_CYCLES,
/* de */  IDS_SKIP_CYCLES_DE,
/* fr */  IDS_SKIP_CYCLES_FR,
/* it */  IDS_SKIP_CYCLES_IT,
/* nl */  IDS_SKIP_CYCLES_NL,
/* pl */  IDS_SKIP_CYCLES_PL,
/* sv */  IDS_SKIP_CYCLES_SV},

/* en */ {IDS_TRAP_IDLE,
/* de */  IDS_TRAP_IDLE_DE,
/* fr */  IDS_TRAP_IDLE_FR,
/* it */  IDS_TRAP_IDLE_IT,
/* nl */  IDS_TRAP_IDLE_NL,
/* pl */  IDS_TRAP_IDLE_PL,
/* sv */  IDS_TRAP_IDLE_SV},

/* en */ {IDS_DRIVE_TYPE,
/* de */  IDS_DRIVE_TYPE_DE,
/* fr */  IDS_DRIVE_TYPE_FR,
/* it */  IDS_DRIVE_TYPE_IT,
/* nl */  IDS_DRIVE_TYPE_NL,
/* pl */  IDS_DRIVE_TYPE_PL,
/* sv */  IDS_DRIVE_TYPE_SV},

/* en */ {IDS_40_TRACK_HANDLING,
/* de */  IDS_40_TRACK_HANDLING_DE,
/* fr */  IDS_40_TRACK_HANDLING_FR,
/* it */  IDS_40_TRACK_HANDLING_IT,
/* nl */  IDS_40_TRACK_HANDLING_NL,
/* pl */  IDS_40_TRACK_HANDLING_PL,
/* sv */  IDS_40_TRACK_HANDLING_SV},

/* en */ {IDS_DRIVE_EXPANSION,
/* de */  IDS_DRIVE_EXPANSION_DE,
/* fr */  IDS_DRIVE_EXPANSION_FR,
/* it */  IDS_DRIVE_EXPANSION_IT,
/* nl */  IDS_DRIVE_EXPANSION_NL,
/* pl */  IDS_DRIVE_EXPANSION_PL,
/* sv */  IDS_DRIVE_EXPANSION_SV},

/* en */ {IDS_IDLE_METHOD,
/* de */  IDS_IDLE_METHOD_DE,
/* fr */  IDS_IDLE_METHOD_FR,
/* it */  IDS_IDLE_METHOD_IT,
/* nl */  IDS_IDLE_METHOD_NL,
/* pl */  IDS_IDLE_METHOD_PL,
/* sv */  IDS_IDLE_METHOD_SV},

/* en */ {IDS_PARALLEL_CABLE,
/* de */  IDS_PARALLEL_CABLE_DE,
/* fr */  IDS_PARALLEL_CABLE_FR,
/* it */  IDS_PARALLEL_CABLE_IT,
/* nl */  IDS_PARALLEL_CABLE_NL,
/* pl */  IDS_PARALLEL_CABLE_PL,
/* sv */  IDS_PARALLEL_CABLE_SV},

/* en */ {IDS_DRIVE_SETTINGS,
/* de */  IDS_DRIVE_SETTINGS_DE,
/* fr */  IDS_DRIVE_SETTINGS_FR,
/* it */  IDS_DRIVE_SETTINGS_IT,
/* nl */  IDS_DRIVE_SETTINGS_NL,
/* pl */  IDS_DRIVE_SETTINGS_PL,
/* sv */  IDS_DRIVE_SETTINGS_SV},

/* en */ {IDS_GEORAM_SIZE,
/* de */  IDS_GEORAM_SIZE_DE,
/* fr */  IDS_GEORAM_SIZE_FR,
/* it */  IDS_GEORAM_SIZE_IT,
/* nl */  IDS_GEORAM_SIZE_NL,
/* pl */  IDS_GEORAM_SIZE_PL,
/* sv */  IDS_GEORAM_SIZE_SV},

/* en */ {IDS_GEORAM_SETTINGS,
/* de */  IDS_GEORAM_SETTINGS_DE,
/* fr */  IDS_GEORAM_SETTINGS_FR,
/* it */  IDS_GEORAM_SETTINGS_IT,
/* nl */  IDS_GEORAM_SETTINGS_NL,
/* pl */  IDS_GEORAM_SETTINGS_PL,
/* sv */  IDS_GEORAM_SETTINGS_SV},

/* en */ {IDS_JOYSTICK_IN_PORT_1,
/* de */  IDS_JOYSTICK_IN_PORT_1_DE,
/* fr */  IDS_JOYSTICK_IN_PORT_1_FR,
/* it */  IDS_JOYSTICK_IN_PORT_1_IT,
/* nl */  IDS_JOYSTICK_IN_PORT_1_NL,
/* pl */  IDS_JOYSTICK_IN_PORT_1_PL,
/* sv */  IDS_JOYSTICK_IN_PORT_1_SV},

/* en */ {IDS_JOYSTICK_IN_PORT_2,
/* de */  IDS_JOYSTICK_IN_PORT_2_DE,
/* fr */  IDS_JOYSTICK_IN_PORT_2_FR,
/* it */  IDS_JOYSTICK_IN_PORT_2_IT,
/* nl */  IDS_JOYSTICK_IN_PORT_2_NL,
/* pl */  IDS_JOYSTICK_IN_PORT_2_PL,
/* sv */  IDS_JOYSTICK_IN_PORT_2_SV},

/* en */ {IDS_JOYSTICK_SETTINGS,
/* de */  IDS_JOYSTICK_SETTINGS_DE,
/* fr */  IDS_JOYSTICK_SETTINGS_FR,
/* it */  IDS_JOYSTICK_SETTINGS_IT,
/* nl */  IDS_JOYSTICK_SETTINGS_NL,
/* pl */  IDS_JOYSTICK_SETTINGS_PL,
/* sv */  IDS_JOYSTICK_SETTINGS_SV},

/* en */ {IDS_KEYPAD,
/* de */  IDS_KEYPAD_DE,
/* fr */  IDS_KEYPAD_FR,
/* it */  IDS_KEYPAD_IT,
/* nl */  IDS_KEYPAD_NL,
/* pl */  IDS_KEYPAD_PL,
/* sv */  IDS_KEYPAD_SV},

/* en */ {IDS_JOY_PORT_0,
/* de */  IDS_JOY_PORT_0_DE,
/* fr */  IDS_JOY_PORT_0_FR,
/* it */  IDS_JOY_PORT_0_IT,
/* nl */  IDS_JOY_PORT_0_NL,
/* pl */  IDS_JOY_PORT_0_PL,
/* sv */  IDS_JOY_PORT_0_SV},

/* en */ {IDS_JOY_PORT_1,
/* de */  IDS_JOY_PORT_1_DE,
/* fr */  IDS_JOY_PORT_1_FR,
/* it */  IDS_JOY_PORT_1_IT,
/* nl */  IDS_JOY_PORT_1_NL,
/* pl */  IDS_JOY_PORT_1_PL,
/* sv */  IDS_JOY_PORT_1_SV},

/* en */ {IDS_JOY_PORT_2,
/* de */  IDS_JOY_PORT_2_DE,
/* fr */  IDS_JOY_PORT_2_FR,
/* it */  IDS_JOY_PORT_2_IT,
/* nl */  IDS_JOY_PORT_2_NL,
/* pl */  IDS_JOY_PORT_2_PL,
/* sv */  IDS_JOY_PORT_2_SV},

/* en */ {IDS_JOY_PORT_3,
/* de */  IDS_JOY_PORT_3_DE,
/* fr */  IDS_JOY_PORT_3_FR,
/* it */  IDS_JOY_PORT_3_IT,
/* nl */  IDS_JOY_PORT_3_NL,
/* pl */  IDS_JOY_PORT_3_PL,
/* sv */  IDS_JOY_PORT_3_SV},

/* en */ {IDS_STOP_BLUE,
/* de */  IDS_STOP_BLUE_DE,
/* fr */  IDS_STOP_BLUE_FR,
/* it */  IDS_STOP_BLUE_IT,
/* nl */  IDS_STOP_BLUE_NL,
/* pl */  IDS_STOP_BLUE_PL,
/* sv */  IDS_STOP_BLUE_SV},

/* en */ {IDS_SELECT_RED,
/* de */  IDS_SELECT_RED_DE,
/* fr */  IDS_SELECT_RED_FR,
/* it */  IDS_SELECT_RED_IT,
/* nl */  IDS_SELECT_RED_NL,
/* pl */  IDS_SELECT_RED_PL,
/* sv */  IDS_SELECT_RED_SV},

/* en */ {IDS_REPEAT_YELLOW,
/* de */  IDS_REPEAT_YELLOW_DE,
/* fr */  IDS_REPEAT_YELLOW_FR,
/* it */  IDS_REPEAT_YELLOW_IT,
/* nl */  IDS_REPEAT_YELLOW_NL,
/* pl */  IDS_REPEAT_YELLOW_PL,
/* sv */  IDS_REPEAT_YELLOW_SV},

/* en */ {IDS_SHUFFLE_GREEN,
/* de */  IDS_SHUFFLE_GREEN_DE,
/* fr */  IDS_SHUFFLE_GREEN_FR,
/* it */  IDS_SHUFFLE_GREEN_IT,
/* nl */  IDS_SHUFFLE_GREEN_NL,
/* pl */  IDS_SHUFFLE_GREEN_PL,
/* sv */  IDS_SHUFFLE_GREEN_SV},

/* en */ {IDS_FORWARD_CHARCOAL,
/* de */  IDS_FORWARD_CHARCOAL_DE,
/* fr */  IDS_FORWARD_CHARCOAL_FR,
/* it */  IDS_FORWARD_CHARCOAL_IT,
/* nl */  IDS_FORWARD_CHARCOAL_NL,
/* pl */  IDS_FORWARD_CHARCOAL_PL,
/* sv */  IDS_FORWARD_CHARCOAL_SV},

/* en */ {IDS_REVERSE_CHARCOAL,
/* de */  IDS_REVERSE_CHARCOAL_DE,
/* fr */  IDS_REVERSE_CHARCOAL_FR,
/* it */  IDS_REVERSE_CHARCOAL_IT,
/* nl */  IDS_REVERSE_CHARCOAL_NL,
/* pl */  IDS_REVERSE_CHARCOAL_PL,
/* sv */  IDS_REVERSE_CHARCOAL_SV},

/* en */ {IDS_PLAY_PAUSE_GREY,
/* de */  IDS_PLAY_PAUSE_GREY_DE,
/* fr */  IDS_PLAY_PAUSE_GREY_FR,
/* it */  IDS_PLAY_PAUSE_GREY_IT,
/* nl */  IDS_PLAY_PAUSE_GREY_NL,
/* pl */  IDS_PLAY_PAUSE_GREY_PL,
/* sv */  IDS_PLAY_PAUSE_GREY_SV},

/* en */ {IDS_JOY_1_DEVICE,
/* de */  IDS_JOY_1_DEVICE_DE,
/* fr */  IDS_JOY_1_DEVICE_FR,
/* it */  IDS_JOY_1_DEVICE_IT,
/* nl */  IDS_JOY_1_DEVICE_NL,
/* pl */  IDS_JOY_1_DEVICE_PL,
/* sv */  IDS_JOY_1_DEVICE_SV},

/* en */ {IDS_JOY_2_DEVICE,
/* de */  IDS_JOY_2_DEVICE_DE,
/* fr */  IDS_JOY_2_DEVICE_FR,
/* it */  IDS_JOY_2_DEVICE_IT,
/* nl */  IDS_JOY_2_DEVICE_NL,
/* pl */  IDS_JOY_2_DEVICE_PL,
/* sv */  IDS_JOY_2_DEVICE_SV},

/* en */ {IDS_JOY_1_FIRE,
/* de */  IDS_JOY_1_FIRE_DE,
/* fr */  IDS_JOY_1_FIRE_FR,
/* it */  IDS_JOY_1_FIRE_IT,
/* nl */  IDS_JOY_1_FIRE_NL,
/* pl */  IDS_JOY_1_FIRE_PL,
/* sv */  IDS_JOY_1_FIRE_SV},

/* en */ {IDS_JOY_2_FIRE,
/* de */  IDS_JOY_2_FIRE_DE,
/* fr */  IDS_JOY_2_FIRE_FR,
/* it */  IDS_JOY_2_FIRE_IT,
/* nl */  IDS_JOY_2_FIRE_NL,
/* pl */  IDS_JOY_2_FIRE_PL,
/* sv */  IDS_JOY_2_FIRE_SV},

/* en */ {IDS_SET_INPUT_JOYSTICK_1,
/* de */  IDS_SET_INPUT_JOYSTICK_1_DE,
/* fr */  IDS_SET_INPUT_JOYSTICK_1_FR,
/* it */  IDS_SET_INPUT_JOYSTICK_1_IT,
/* nl */  IDS_SET_INPUT_JOYSTICK_1_NL,
/* pl */  IDS_SET_INPUT_JOYSTICK_1_PL,
/* sv */  IDS_SET_INPUT_JOYSTICK_1_SV},

/* en */ {IDS_SET_INPUT_JOYSTICK_2,
/* de */  IDS_SET_INPUT_JOYSTICK_2_DE,
/* fr */  IDS_SET_INPUT_JOYSTICK_2_FR,
/* it */  IDS_SET_INPUT_JOYSTICK_2_IT,
/* nl */  IDS_SET_INPUT_JOYSTICK_2_NL,
/* pl */  IDS_SET_INPUT_JOYSTICK_2_PL,
/* sv */  IDS_SET_INPUT_JOYSTICK_2_SV},

/* en */ {IDS_SET_INPUT_JOYLL_1,
/* de */  IDS_SET_INPUT_JOYLL_1_DE,
/* fr */  IDS_SET_INPUT_JOYLL_1_FR,
/* it */  IDS_SET_INPUT_JOYLL_1_IT,
/* nl */  IDS_SET_INPUT_JOYLL_1_NL,
/* pl */  IDS_SET_INPUT_JOYLL_1_PL,
/* sv */  IDS_SET_INPUT_JOYLL_1_SV},

/* en */ {IDS_SET_INPUT_JOYLL_2,
/* de */  IDS_SET_INPUT_JOYLL_2_DE,
/* fr */  IDS_SET_INPUT_JOYLL_2_FR,
/* it */  IDS_SET_INPUT_JOYLL_2_IT,
/* nl */  IDS_SET_INPUT_JOYLL_2_NL,
/* pl */  IDS_SET_INPUT_JOYLL_2_PL,
/* sv */  IDS_SET_INPUT_JOYLL_2_SV},

/* en */ {IDS_SAVE_SETTINGS_ON_EXIT,
/* de */  IDS_SAVE_SETTINGS_ON_EXIT_DE,
/* fr */  IDS_SAVE_SETTINGS_ON_EXIT_FR,
/* it */  IDS_SAVE_SETTINGS_ON_EXIT_IT,
/* nl */  IDS_SAVE_SETTINGS_ON_EXIT_NL,
/* pl */  IDS_SAVE_SETTINGS_ON_EXIT_PL,
/* sv */  IDS_SAVE_SETTINGS_ON_EXIT_SV},

/* en */ {IDS_NEVER_SAVE_SETTINGS_EXIT,
/* de */  IDS_NEVER_SAVE_SETTINGS_EXIT_DE,
/* fr */  IDS_NEVER_SAVE_SETTINGS_EXIT_FR,
/* it */  IDS_NEVER_SAVE_SETTINGS_EXIT_IT,
/* nl */  IDS_NEVER_SAVE_SETTINGS_EXIT_NL,
/* pl */  IDS_NEVER_SAVE_SETTINGS_EXIT_PL,
/* sv */  IDS_NEVER_SAVE_SETTINGS_EXIT_SV},

/* en */ {IDS_CONFIRM_QUITING_VICE,
/* de */  IDS_CONFIRM_QUITING_VICE_DE,
/* fr */  IDS_CONFIRM_QUITING_VICE_FR,
/* it */  IDS_CONFIRM_QUITING_VICE_IT,
/* nl */  IDS_CONFIRM_QUITING_VICE_NL,
/* pl */  IDS_CONFIRM_QUITING_VICE_PL,
/* sv */  IDS_CONFIRM_QUITING_VICE_SV},

/* en */ {IDS_NEVER_CONFIRM_QUITING_VICE,
/* de */  IDS_NEVER_CONFIRM_QUITING_VICE_DE,
/* fr */  IDS_NEVER_CONFIRM_QUITING_VICE_FR,
/* it */  IDS_NEVER_CONFIRM_QUITING_VICE_IT,
/* nl */  IDS_NEVER_CONFIRM_QUITING_VICE_NL,
/* pl */  IDS_NEVER_CONFIRM_QUITING_VICE_PL,
/* sv */  IDS_NEVER_CONFIRM_QUITING_VICE_SV},

/* en */ {IDS_PET_REU_SIZE,
/* de */  IDS_PET_REU_SIZE_DE,
/* fr */  IDS_PET_REU_SIZE_FR,
/* it */  IDS_PET_REU_SIZE_IT,
/* nl */  IDS_PET_REU_SIZE_NL,
/* pl */  IDS_PET_REU_SIZE_PL,
/* sv */  IDS_PET_REU_SIZE_SV},

/* en */ {IDS_PET_REU_SETTINGS,
/* de */  IDS_PET_REU_SETTINGS_DE,
/* fr */  IDS_PET_REU_SETTINGS_FR,
/* it */  IDS_PET_REU_SETTINGS_IT,
/* nl */  IDS_PET_REU_SETTINGS_NL,
/* pl */  IDS_PET_REU_SETTINGS_PL,
/* sv */  IDS_PET_REU_SETTINGS_SV},

/* en */ {IDS_PLUS256K_SETTINGS,
/* de */  IDS_PLUS256K_SETTINGS_DE,
/* fr */  IDS_PLUS256K_SETTINGS_FR,
/* it */  IDS_PLUS256K_SETTINGS_IT,
/* nl */  IDS_PLUS256K_SETTINGS_NL,
/* pl */  IDS_PLUS256K_SETTINGS_PL,
/* sv */  IDS_PLUS256K_SETTINGS_SV},

/* en */ {IDS_PLUS60K_BASE,
/* de */  IDS_PLUS60K_BASE_DE,
/* fr */  IDS_PLUS60K_BASE_FR,
/* it */  IDS_PLUS60K_BASE_IT,
/* nl */  IDS_PLUS60K_BASE_NL,
/* pl */  IDS_PLUS60K_BASE_PL,
/* sv */  IDS_PLUS60K_BASE_SV},

/* en */ {IDS_PLUS60K_SETTINGS,
/* de */  IDS_PLUS60K_SETTINGS_DE,
/* fr */  IDS_PLUS60K_SETTINGS_FR,
/* it */  IDS_PLUS60K_SETTINGS_IT,
/* nl */  IDS_PLUS60K_SETTINGS_NL,
/* pl */  IDS_PLUS60K_SETTINGS_PL,
/* sv */  IDS_PLUS60K_SETTINGS_SV},

/* en */ {IDS_VALUE_FIRST_BYTE,
/* de */  IDS_VALUE_FIRST_BYTE_DE,
/* fr */  IDS_VALUE_FIRST_BYTE_FR,
/* it */  IDS_VALUE_FIRST_BYTE_IT,
/* nl */  IDS_VALUE_FIRST_BYTE_NL,
/* pl */  IDS_VALUE_FIRST_BYTE_PL,
/* sv */  IDS_VALUE_FIRST_BYTE_SV},

/* en */ {IDS_LENGTH_CONSTANT_VALUES,
/* de */  IDS_LENGTH_CONSTANT_VALUES_DE,
/* fr */  IDS_LENGTH_CONSTANT_VALUES_FR,
/* it */  IDS_LENGTH_CONSTANT_VALUES_IT,
/* nl */  IDS_LENGTH_CONSTANT_VALUES_NL,
/* pl */  IDS_LENGTH_CONSTANT_VALUES_PL,
/* sv */  IDS_LENGTH_CONSTANT_VALUES_SV},

/* en */ {IDS_LENGTH_CONSTANT_PATTERN,
/* de */  IDS_LENGTH_CONSTANT_PATTERN_DE,
/* fr */  IDS_LENGTH_CONSTANT_PATTERN_FR,
/* it */  IDS_LENGTH_CONSTANT_PATTERN_IT,
/* nl */  IDS_LENGTH_CONSTANT_PATTERN_NL,
/* pl */  IDS_LENGTH_CONSTANT_PATTERN_PL,
/* sv */  IDS_LENGTH_CONSTANT_PATTERN_SV},

/* en */ {IDS_RAM_SETTINGS,
/* de */  IDS_RAM_SETTINGS_DE,
/* fr */  IDS_RAM_SETTINGS_FR,
/* it */  IDS_RAM_SETTINGS_IT,
/* nl */  IDS_RAM_SETTINGS_NL,
/* pl */  IDS_RAM_SETTINGS_PL,
/* sv */  IDS_RAM_SETTINGS_SV},

/* en */ {IDS_READ_WRITE,
/* de */  IDS_READ_WRITE_DE,
/* fr */  IDS_READ_WRITE_FR,
/* it */  IDS_READ_WRITE_IT,
/* nl */  IDS_READ_WRITE_NL,
/* pl */  IDS_READ_WRITE_PL,
/* sv */  IDS_READ_WRITE_SV},

/* en */ {IDS_READ_ONLY,
/* de */  IDS_READ_ONLY_DE,
/* fr */  IDS_READ_ONLY_FR,
/* it */  IDS_READ_ONLY_IT,
/* nl */  IDS_READ_ONLY_NL,
/* pl */  IDS_READ_ONLY_PL,
/* sv */  IDS_READ_ONLY_SV},

/* en */ {IDS_RAMCART_READ_WRITE,
/* de */  IDS_RAMCART_READ_WRITE_DE,
/* fr */  IDS_RAMCART_READ_WRITE_FR,
/* it */  IDS_RAMCART_READ_WRITE_IT,
/* nl */  IDS_RAMCART_READ_WRITE_NL,
/* pl */  IDS_RAMCART_READ_WRITE_PL,
/* sv */  IDS_RAMCART_READ_WRITE_SV},

/* en */ {IDS_RAMCART_SIZE,
/* de */  IDS_RAMCART_SIZE_DE,
/* fr */  IDS_RAMCART_SIZE_FR,
/* it */  IDS_RAMCART_SIZE_IT,
/* nl */  IDS_RAMCART_SIZE_NL,
/* pl */  IDS_RAMCART_SIZE_PL,
/* sv */  IDS_RAMCART_SIZE_SV},

/* en */ {IDS_RAMCART_SETTINGS,
/* de */  IDS_RAMCART_SETTINGS_DE,
/* fr */  IDS_RAMCART_SETTINGS_FR,
/* it */  IDS_RAMCART_SETTINGS_IT,
/* nl */  IDS_RAMCART_SETTINGS_NL,
/* pl */  IDS_RAMCART_SETTINGS_PL,
/* sv */  IDS_RAMCART_SETTINGS_SV},

/* en */ {IDS_REU_SIZE,
/* de */  IDS_REU_SIZE_DE,
/* fr */  IDS_REU_SIZE_FR,
/* it */  IDS_REU_SIZE_IT,
/* nl */  IDS_REU_SIZE_NL,
/* pl */  IDS_REU_SIZE_PL,
/* sv */  IDS_REU_SIZE_SV},

/* en */ {IDS_REU_SETTINGS,
/* de */  IDS_REU_SETTINGS_DE,
/* fr */  IDS_REU_SETTINGS_FR,
/* it */  IDS_REU_SETTINGS_IT,
/* nl */  IDS_REU_SETTINGS_NL,
/* pl */  IDS_REU_SETTINGS_PL,
/* sv */  IDS_REU_SETTINGS_SV},

/* en */ {IDS_USERPORT_RS232,
/* de */  IDS_USERPORT_RS232_DE,
/* fr */  IDS_USERPORT_RS232_FR,
/* it */  IDS_USERPORT_RS232_IT,
/* nl */  IDS_USERPORT_RS232_NL,
/* pl */  IDS_USERPORT_RS232_PL,
/* sv */  IDS_USERPORT_RS232_SV},

/* en */ {IDS_USERPORT_DEVICE,
/* de */  IDS_USERPORT_DEVICE_DE,
/* fr */  IDS_USERPORT_DEVICE_FR,
/* it */  IDS_USERPORT_DEVICE_IT,
/* nl */  IDS_USERPORT_DEVICE_NL,
/* pl */  IDS_USERPORT_DEVICE_PL,
/* sv */  IDS_USERPORT_DEVICE_SV},

/* en */ {IDS_USERPORT_BAUD_RATE,
/* de */  IDS_USERPORT_BAUD_RATE_DE,
/* fr */  IDS_USERPORT_BAUD_RATE_FR,
/* it */  IDS_USERPORT_BAUD_RATE_IT,
/* nl */  IDS_USERPORT_BAUD_RATE_NL,
/* pl */  IDS_USERPORT_BAUD_RATE_PL,
/* sv */  IDS_USERPORT_BAUD_RATE_SV},

/* en */ {IDS_RS232_USERPORT_SETTINGS,
/* de */  IDS_RS232_USERPORT_SETTINGS_DE,
/* fr */  IDS_RS232_USERPORT_SETTINGS_FR,
/* it */  IDS_RS232_USERPORT_SETTINGS_IT,
/* nl */  IDS_RS232_USERPORT_SETTINGS_NL,
/* pl */  IDS_RS232_USERPORT_SETTINGS_PL,
/* sv */  IDS_RS232_USERPORT_SETTINGS_SV},

/* en */ {IDS_FAST,
/* de */  IDS_FAST_DE,
/* fr */  IDS_FAST_FR,
/* it */  IDS_FAST_IT,
/* nl */  IDS_FAST_NL,
/* pl */  IDS_FAST_PL,
/* sv */  IDS_FAST_SV},

/* en */ {IDS_INTERPOLATING,
/* de */  IDS_INTERPOLATING_DE,
/* fr */  IDS_INTERPOLATING_FR,
/* it */  IDS_INTERPOLATING_IT,
/* nl */  IDS_INTERPOLATING_NL,
/* pl */  IDS_INTERPOLATING_PL,
/* sv */  IDS_INTERPOLATING_SV},

/* en */ {IDS_RESAMPLING,
/* de */  IDS_RESAMPLING_DE,
/* fr */  IDS_RESAMPLING_FR,
/* it */  IDS_RESAMPLING_IT,
/* nl */  IDS_RESAMPLING_NL,
/* pl */  IDS_RESAMPLING_PL,
/* sv */  IDS_RESAMPLING_SV},

/* en */ {IDS_FAST_RESAMPLING,
/* de */  IDS_FAST_RESAMPLING_DE,
/* fr */  IDS_FAST_RESAMPLING_FR,
/* it */  IDS_FAST_RESAMPLING_IT,
/* nl */  IDS_FAST_RESAMPLING_NL,
/* pl */  IDS_FAST_RESAMPLING_PL,
/* sv */  IDS_FAST_RESAMPLING_SV},

/* en */ {IDS_6581_OLD,
/* de */  IDS_6581_OLD_DE,
/* fr */  IDS_6581_OLD_FR,
/* it */  IDS_6581_OLD_IT,
/* nl */  IDS_6581_OLD_NL,
/* pl */  IDS_6581_OLD_PL,
/* sv */  IDS_6581_OLD_SV},

/* en */ {IDS_8580_NEW,
/* de */  IDS_8580_NEW_DE,
/* fr */  IDS_8580_NEW_FR,
/* it */  IDS_8580_NEW_IT,
/* nl */  IDS_8580_NEW_NL,
/* pl */  IDS_8580_NEW_PL,
/* sv */  IDS_8580_NEW_SV},

/* en */ {IDS_SID_MODEL,
/* de */  IDS_SID_MODEL_DE,
/* fr */  IDS_SID_MODEL_FR,
/* it */  IDS_SID_MODEL_IT,
/* nl */  IDS_SID_MODEL_NL,
/* pl */  IDS_SID_MODEL_PL,
/* sv */  IDS_SID_MODEL_SV},

/* en */ {IDS_SID_STEREO,
/* de */  IDS_SID_STEREO_DE,
/* fr */  IDS_SID_STEREO_FR,
/* it */  IDS_SID_STEREO_IT,
/* nl */  IDS_SID_STEREO_NL,
/* pl */  IDS_SID_STEREO_PL,
/* sv */  IDS_SID_STEREO_SV},

/* en */ {IDS_STEREO_SID_AT,
/* de */  IDS_STEREO_SID_AT_DE,
/* fr */  IDS_STEREO_SID_AT_FR,
/* it */  IDS_STEREO_SID_AT_IT,
/* nl */  IDS_STEREO_SID_AT_NL,
/* pl */  IDS_STEREO_SID_AT_PL,
/* sv */  IDS_STEREO_SID_AT_SV},

/* en */ {IDS_SID_FILTERS,
/* de */  IDS_SID_FILTERS_DE,
/* fr */  IDS_SID_FILTERS_FR,
/* it */  IDS_SID_FILTERS_IT,
/* nl */  IDS_SID_FILTERS_NL,
/* pl */  IDS_SID_FILTERS_PL,
/* sv */  IDS_SID_FILTERS_SV},

/* en */ {IDS_SAMPLE_METHOD,
/* de */  IDS_SAMPLE_METHOD_DE,
/* fr */  IDS_SAMPLE_METHOD_FR,
/* it */  IDS_SAMPLE_METHOD_IT,
/* nl */  IDS_SAMPLE_METHOD_NL,
/* pl */  IDS_SAMPLE_METHOD_PL,
/* sv */  IDS_SAMPLE_METHOD_SV},

/* en */ {IDS_PASSBAND_0_90,
/* de */  IDS_PASSBAND_0_90_DE,
/* fr */  IDS_PASSBAND_0_90_FR,
/* it */  IDS_PASSBAND_0_90_IT,
/* nl */  IDS_PASSBAND_0_90_NL,
/* pl */  IDS_PASSBAND_0_90_PL,
/* sv */  IDS_PASSBAND_0_90_SV},

/* en */ {IDS_NOT_IMPLEMENTED_YET,
/* de */  IDS_NOT_IMPLEMENTED_YET_DE,
/* fr */  IDS_NOT_IMPLEMENTED_YET_FR,
/* it */  IDS_NOT_IMPLEMENTED_YET_IT,
/* nl */  IDS_NOT_IMPLEMENTED_YET_NL,
/* pl */  IDS_NOT_IMPLEMENTED_YET_PL,
/* sv */  IDS_NOT_IMPLEMENTED_YET_SV},

/* en */ {IDS_SID_SETTINGS,
/* de */  IDS_SID_SETTINGS_DE,
/* fr */  IDS_SID_SETTINGS_FR,
/* it */  IDS_SID_SETTINGS_IT,
/* nl */  IDS_SID_SETTINGS_NL,
/* pl */  IDS_SID_SETTINGS_PL,
/* sv */  IDS_SID_SETTINGS_SV},

/* en */ {IDS_FLEXIBLE,
/* de */  IDS_FLEXIBLE_DE,
/* fr */  IDS_FLEXIBLE_FR,
/* it */  IDS_FLEXIBLE_IT,
/* nl */  IDS_FLEXIBLE_NL,
/* pl */  IDS_FLEXIBLE_PL,
/* sv */  IDS_FLEXIBLE_SV},

/* en */ {IDS_ADJUSTING,
/* de */  IDS_ADJUSTING_DE,
/* fr */  IDS_ADJUSTING_FR,
/* it */  IDS_ADJUSTING_IT,
/* nl */  IDS_ADJUSTING_NL,
/* pl */  IDS_ADJUSTING_PL,
/* sv */  IDS_ADJUSTING_SV},

/* en */ {IDS_EXACT,
/* de */  IDS_EXACT_DE,
/* fr */  IDS_EXACT_FR,
/* it */  IDS_EXACT_IT,
/* nl */  IDS_EXACT_NL,
/* pl */  IDS_EXACT_PL,
/* sv */  IDS_EXACT_SV},

/* en */ {IDS_SAMPLE_RATE,
/* de */  IDS_SAMPLE_RATE_DE,
/* fr */  IDS_SAMPLE_RATE_FR,
/* it */  IDS_SAMPLE_RATE_IT,
/* nl */  IDS_SAMPLE_RATE_NL,
/* pl */  IDS_SAMPLE_RATE_PL,
/* sv */  IDS_SAMPLE_RATE_SV},

/* en */ {IDS_BUFFER_SIZE,
/* de */  IDS_BUFFER_SIZE_DE,
/* fr */  IDS_BUFFER_SIZE_FR,
/* it */  IDS_BUFFER_SIZE_IT,
/* nl */  IDS_BUFFER_SIZE_NL,
/* pl */  IDS_BUFFER_SIZE_PL,
/* sv */  IDS_BUFFER_SIZE_SV},

/* en */ {IDS_OVERSAMPLE,
/* de */  IDS_OVERSAMPLE_DE,
/* fr */  IDS_OVERSAMPLE_FR,
/* it */  IDS_OVERSAMPLE_IT,
/* nl */  IDS_OVERSAMPLE_NL,
/* pl */  IDS_OVERSAMPLE_PL,
/* sv */  IDS_OVERSAMPLE_SV},

/* en */ {IDS_SPEED_ADJUSTMENT,
/* de */  IDS_SPEED_ADJUSTMENT_DE,
/* fr */  IDS_SPEED_ADJUSTMENT_FR,
/* it */  IDS_SPEED_ADJUSTMENT_IT,
/* nl */  IDS_SPEED_ADJUSTMENT_NL,
/* pl */  IDS_SPEED_ADJUSTMENT_PL,
/* sv */  IDS_SPEED_ADJUSTMENT_SV},

/* en */ {IDS_SOUND_SETTINGS,
/* de */  IDS_SOUND_SETTINGS_DE,
/* fr */  IDS_SOUND_SETTINGS_FR,
/* it */  IDS_SOUND_SETTINGS_IT,
/* nl */  IDS_SOUND_SETTINGS_NL,
/* pl */  IDS_SOUND_SETTINGS_PL,
/* sv */  IDS_SOUND_SETTINGS_SV},

/* en */ {IDS_VIC_SETTINGS,
/* de */  IDS_VIC_SETTINGS_DE,
/* fr */  IDS_VIC_SETTINGS_FR,
/* it */  IDS_VIC_SETTINGS_IT,
/* nl */  IDS_VIC_SETTINGS_NL,
/* pl */  IDS_VIC_SETTINGS_PL,
/* sv */  IDS_VIC_SETTINGS_SV},

/* en */ {IDS_SPRITE_SPRITE_COL,
/* de */  IDS_SPRITE_SPRITE_COL_DE,
/* fr */  IDS_SPRITE_SPRITE_COL_FR,
/* it */  IDS_SPRITE_SPRITE_COL_IT,
/* nl */  IDS_SPRITE_SPRITE_COL_NL,
/* pl */  IDS_SPRITE_SPRITE_COL_PL,
/* sv */  IDS_SPRITE_SPRITE_COL_SV},

/* en */ {IDS_SPRITE_BACKGROUND_COL,
/* de */  IDS_SPRITE_BACKGROUND_COL_DE,
/* fr */  IDS_SPRITE_BACKGROUND_COL_FR,
/* it */  IDS_SPRITE_BACKGROUND_COL_IT,
/* nl */  IDS_SPRITE_BACKGROUND_COL_NL,
/* pl */  IDS_SPRITE_BACKGROUND_COL_PL,
/* sv */  IDS_SPRITE_BACKGROUND_COL_SV},

/* en */ {IDS_NEW_LUMINANCES,
/* de */  IDS_NEW_LUMINANCES_DE,
/* fr */  IDS_NEW_LUMINANCES_FR,
/* it */  IDS_NEW_LUMINANCES_IT,
/* nl */  IDS_NEW_LUMINANCES_NL,
/* pl */  IDS_NEW_LUMINANCES_PL,
/* sv */  IDS_NEW_LUMINANCES_SV},

/* en */ {IDS_VICII_SETTINGS,
/* de */  IDS_VICII_SETTINGS_DE,
/* fr */  IDS_VICII_SETTINGS_FR,
/* it */  IDS_VICII_SETTINGS_IT,
/* nl */  IDS_VICII_SETTINGS_NL,
/* pl */  IDS_VICII_SETTINGS_PL,
/* sv */  IDS_VICII_SETTINGS_SV},

/* en */ {IDS_ATTACH_TAPE_IMAGE,
/* de */  IDS_ATTACH_TAPE_IMAGE_DE,
/* fr */  IDS_ATTACH_TAPE_IMAGE_FR,
/* it */  IDS_ATTACH_TAPE_IMAGE_IT,
/* nl */  IDS_ATTACH_TAPE_IMAGE_NL,
/* pl */  IDS_ATTACH_TAPE_IMAGE_PL,
/* sv */  IDS_ATTACH_TAPE_IMAGE_SV},

/* en */ {IDS_AUTOSTART_IMAGE,
/* de */  IDS_AUTOSTART_IMAGE_DE,
/* fr */  IDS_AUTOSTART_IMAGE_FR,
/* it */  IDS_AUTOSTART_IMAGE_IT,
/* nl */  IDS_AUTOSTART_IMAGE_NL,
/* pl */  IDS_AUTOSTART_IMAGE_PL,
/* sv */  IDS_AUTOSTART_IMAGE_SV},

};

/* --------------------------------------------------------------------- */

static char *intl_text_table[countof(intl_translate_text_table)][countof(language_table)];

void intl_convert_mui_table(int text_id[], char *text[])
{
  int i;

  for (i=0; text_id[i]!=0; i++)
  {
    text[i]=intl_translate_text(text_id[i]);
  }
  text[i]=NULL;
}

/* codepage conversion is not yet present */

char *intl_convert_cp(char *text, int cp)
{
  int len;
  char *buffer;

  if (text==NULL)
    return NULL;

  len=strlen(text);

  if (len==0)
    return NULL;

  buffer=strdup(text);

  return buffer;
}

static char *intl_get_string_by_id(int id)
{
  int k;

  for (k = 0; k < countof(intl_string_table); k++)
  {
    if (intl_string_table[k].resource_id==id)
      return intl_string_table[k].text;
  }
  return NULL;
}

char *intl_translate_text(int en_resource)
{
  int i;

  for (i = 0; i < countof(intl_translate_text_table); i++)
  {
    if (intl_translate_text_table[i][0]==en_resource)
    {
      if (intl_translate_text_table[i][current_language_index]!=0 &&
          intl_text_table[i][current_language_index]!=NULL &&
          strlen(intl_text_table[i][current_language_index])!=0)
        return intl_text_table[i][current_language_index];
      else
        return intl_text_table[i][0];
    }
  }
  return "";
}

/* pre-translated main window caption text so the emulation won't
   slow down because of all the translation calls */

char *intl_speed_at_text;

/* --------------------------------------------------------------------- */

void intl_init(void)
{
  int i,j;

  for (i = 0; i < countof(language_table); i++)
  {
    for (j = 0; j < countof(intl_translate_text_table); j++)
    {
      if (intl_translate_text_table[j][i]==0)
        intl_text_table[j][i]=NULL;
      else
        intl_text_table[j][i]=(char *)intl_get_string_by_id(intl_translate_text_table[j][i]);
    }
  }
}

int intl_translate_res(int resource)
{
  return resource;
}

void intl_shutdown(void)
{
}

/* --------------------------------------------------------------------- */


static void intl_update_pre_translated_text(void)
{
  intl_speed_at_text=intl_translate_text(IDS_S_AT_D_SPEED);
}

char *intl_arch_language_init(void)
{
  int i;
  struct Locale *amigalang;

  amigalang=OpenLocale(NULL);
  CloseLocale(amigalang);
  for (i = 0; amiga_to_iso[i].iso_language_code != NULL; i++)
  {
    if (!strcasecmp(amiga_to_iso[i].amiga_locale_language, amigalang->loc_LanguageName))
      return amiga_to_iso[i].iso_language_code;
  }
  return "en";
}

void intl_update_ui(void)
{
  intl_update_pre_translated_text();
  ui_update_menus();
}
