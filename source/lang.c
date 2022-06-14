#include "lang.h"

/* WILL PROBABLY MAKE FUNCS WHICH CAN CREATE THEM THIS BOXES */

char *lang_boot_it = { // italian
    "# # # # # # # # # # # # # CONFIGURAZIONE GENERALE # # # # # # # # # # # #\n"
    "#             Questo file e stato generato nel primo boot.              #\n"
    "#                                                                       #\n"
    "#       Qui puoi modificare la dinamica del gioco a tuo piacimento,     #\n"
    "#        Qualunque modifica applicata nel gioco sarà salvato qui.       #\n"
    "#       Tutte le configurazioni saranno applicate durante il boot       #\n"
    "#                                                                       #\n"
    "# Per maggiori informazioni visita https://github.com/waifro/c4chess    #\n"
    "# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #" };

char *lang_boot_en = { // english
    "# # # # # # # # # # # # # GENERAL CONFIGURATION # # # # # # # # # # # # #\n"
    "#              This file was generated on first time boot.              #\n"
    "#                                                                       #\n"
    "#          Here you can configure the game dinamics as you wish,        #\n"
    "#  Every change's applied on the game will be saved here for next boot. #\n"
    "#              Any applicable changes are set during boot               #\n"
    "#                                                                       #\n"
    "# For more information visit https://github.com/waifro/c4chess		     #\n"
    "# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #" };

char *lang_boot_nl = { // dutch
    "# # # # # # # # # # # # # GENERALE CONFIGURATIE # # # # # # # # # # # # #\n"
    "#      Dit dossier werd gegenereerd op mijn eerste keer opstarten.      #\n"
    "#                                                                       #\n"
    "#        Hier kun je de spelinstellingen naar wens configureren,        #\n"
    "#           Elke wijziging die op het spel wordt aangebracht,           #\n"
    "#         wordt hier opgeslagen voor de volgende keer opstarten.        #\n"
    "# Alle toepasselijke wijzigingen worden ingesteld tijdens het opstarten #\n"
    "#                                                                       #\n"
    "# Voor meer informatie bezoek https://github.com/waifro/c4chess         #\n"
	"# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #" };

const char *lang_cfg_it[] = {
    "Errore", "Attenzione", "C'e' qualcosa che non va", "Riprova", "Si", "No", "Okay", "Torna nel gioco", "Esci", "Continua",
    "Gioca", "Info", "Impostazioni", "Statistiche", "Prossimamente", "Sei Sicuro?", "Premi",
    "stile", "personalizza", "immetti", "sfondo immagine", "suoni", "musica", "locale", "globale", "giocatori",
    "lobby trovate", "indirizzo Ip", "per uscire", "per entrare", "in chat" };

const char *lang_cfg_en[] = {
    "Error", "Attention", "There is something wrong", "Retry", "Yes", "No", "Okay", "Back to the game", "Exit", "Continue",
    "Play", "Info", "Settings", "Statistics", "Coming soon", "Are you sure?", "Press",
    "style", "personalize", "insert", "background", "sounds", "music", "local", "global", "players",
    "lobby found", "address Ip", "to exit", "to enter", "in chat" };

const char *lang_cfg_nl[] = {
    "Error", "Attentie", "Er is iets misgelopen", "Probeer opnieuw", "Ja", "Nee", "Oké", "Terug naar de game", "Verlaat", "Ga verder",
    "Speel", "Informatie", "Instellingen", "Statistieken", "Binnenkort beschikbaar", "Ben je zeker?", "Druk",
    "stijl", "personalizeer", "invoegen", "achtergrond", "sounds", "muziek", "lokaal", "globaal", "spelers",
    "lobby gevonden", "address Ip", "verlaten", "binnengaan", "in de chat" };
