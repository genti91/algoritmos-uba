#include "carteles.h"

#define AZUL "\e[36m"
#define VIOLETA "\e[35m"
#define RESET "\e[0m"

// Procedimiento que imprime un mac y bloo por dos segundos antes de que epiece el juego.
void imprimir_intro(){
	
	system("clear");

	printf(VIOLETA "\
		\n \
	##     ##    ###     ######     ##    ##    ########  ##        #######   #######  \n \
	###   ###   ## ##   ##    ##     ##  ##     ##     ## ##       ##     ## ##     ## \n \
	#### ####  ##   ##  ##            ####      ##     ## ##       ##     ## ##     ## \n \
	## ### ## ##     ## ##             ##       ########  ##       ##     ## ##     ## \n \
	##     ## ######### ##             ##       ##     ## ##       ##     ## ##     ## \n \
	##     ## ##     ## ##    ##       ##       ##     ## ##       ##     ## ##     ## \n \
	##     ## ##     ##  ######        ##       ########  ########  #######   #######  \n" RESET);

	sleep(2);
}

// Procedimiento que imprime ganaste.
void imprimir_ganaste(){
	
	system("clear");

	printf(VIOLETA "\
		\n \
	 ######      ###    ##    ##    ###     ######  ######## ######## \n \
	##    ##    ## ##   ###   ##   ## ##   ##    ##    ##    ##       \n \
	##         ##   ##  ####  ##  ##   ##  ##          ##    ##       \n \
	##   #### ##     ## ## ## ## ##     ##  ######     ##    ######   \n \
	##    ##  ######### ##  #### #########       ##    ##    ##       \n \
	##    ##  ##     ## ##   ### ##     ## ##    ##    ##    ##       \n \
 	 ######   ##     ## ##    ## ##     ##  ######     ##    ########  \n" RESET);
}

// Procedimiento que imprime perdiste.
void imprimir_perdiste(){
	
	system("clear");

	printf(VIOLETA "\
		\n \
		########  ######## ########  ########  ####  ######  ######## ######## \n \
		##     ## ##       ##     ## ##     ##  ##  ##    ##    ##    ##       \n \
		##     ## ##       ##     ## ##     ##  ##  ##          ##    ##       \n \
		########  ######   ########  ##     ##  ##   ######     ##    ######   \n \
		##        ##       ##   ##   ##     ##  ##        ##    ##    ##       \n \
		##        ##       ##    ##  ##     ##  ##  ##    ##    ##    ##       \n \
		##        ######## ##     ## ########  ####  ######     ##    ########  \n" RESET);
}

// Procedimiento que imprime un cartel.
void imprimir_bloo(){

	printf(VIOLETA "\
		\n \
	MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMWNX0OxdollllllooooooooodxkOKNWMMMMMMMMMMM \n \
	MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMWNKOxdl:clodddddolc:loollooolllodk0NWMMMMMMMM \n \
	MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMWNKkdoc:cokKNWMMMMMWX0xl:cllc:cloolc:ldONMMMMMMM \n \
	MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMWKOdool:c0NWMMMMMMMMN0KWMNx:;:oOXNWNX0ko:cxKWMMMMM \n \
	MMMMMMMMMMMMMMMMMMMMMMMMMMMWN0xoolooc:OMMMMMMMMMMWd.;0MMWo,xNMMMMMMMMW0l:oKWMMMM \n \
	MMMMMMMMMMMMMMMMMMMMMMMMMWXOxoooolooc:OMMMMMMMMMMMX0KWMMWo:KMMMMMMMMMMMNd:dXMMMM \n \
	MMMMMMMMMMMMMMMMMMMMMMMWXOdoooooooool:c0NWMMMMMMMMMMMMMNx,:KMW0oxXMMMMMMKl:kNMMM \n \
	MMMMMMMMMMMMMMMMMMMMMWNOdoooooooooooolcccokKNWMMMMWWX0xl:::dNWx,cKMMMMMMNo;xXMMM \n \
	MMMMMMMMMMMMMMMMMMMMW0xoooooooooooloooollc:clodddddocc:lolc:oKWNWMMMMMMM0c:d0WMM \n \
	MMMMMMMMMMMMMMMMMMWXkoooooooooooooooooolloooolllllllloooooooclx0NWMMMWXkc:lo0WMM \n \
	MMMMMMMMMMMMMMMMMW0xooooooooooooooooooollllolooooodxkO0KOdONXKOxdddddoc:cloo0WMM \n \
	MMMMMMMMMMMMMMMMNOoooooooooooooooooooooooloooodod0NWWMMM0dKMMMMMN0xllllooolo0WMM \n \
	MMMMMMMMMMMMMMWXkoolooooooooooooooollooooooxOKNOkNMMMMMM0dKMMMMMMW0olooololdKWMM \n \
	MMMMMMMMMMMMMWKxoooloooooooooooooooloooxkOKNWMMOkNMMMMMM0dKMMMMMNk,.,looooldKMMM \n \
	MMMMMMMMMMMMWKdoooloooloooooooooooolc;cdxxdllcc,':ccllodl:dO0KX0c.  .:oooooxXMMM \n \
	MMMMMMMMMMMW0doooooloooooooooooool:'.                       ....     'looooOWMMM \n \
	MMMMMMMMMMWKdooooloooooooooooool;.                                   .cooldKMMMM \n \
	MMMMMMMMMWKxooooooloooooooooll;.                                     .:oookNMMMM \n \
	MMMMMMMMMXxoooooooooooooooll;.                                       .;ooxXMMMMM \n \
	MMMMMMMMNkoloolooolllccccc;.                                         .;od0WMMMMM \n \
	MMMMMMMW0oooooolool:;;:::;,..                 .'''.. ....            .:d0WMMMMMM \n \
	MMMMMMMKdooooooooollloooooolc,.';;;;;;;;;;;,.,llollc;,;cc:,.         .l0WMMMMMMM \n \
	MMMMMMNkooooollooooooooolloool;,cooooooooooc,;llollloc,;lllc'.....   'kWMMMMMMMM \n \
	MMMMMW0dooololoooooooooolooool;,coooooollool:,colollolc:llolc,;lllc:;:OMMMMMMMMM \n \
	MMMMMXxoooooooooolloooollooooc,;loooooooooool,;lloolollllllooc,:loool;dWMMMMMMMM \n \
	MMMMW0ooooooooooooooooooolooc,;looooooloollol,,lllllllllllllol;,loool:oNMMMMMMMM \n \
	MMMMXxooooooooooooooooooool:,:loooooooooooool,,lollllllllllool:,coool:oNMMMMMMMM \n \
	MMMW0doloooooooooooooooolc,;clooooooooooooool,;llllllllllllooo:,:oooc:kWMMMMMMMM \n \
	MMMNkoloooooooooooooool:;;:looooooooooooooool;,llllolllloollll;':lc:ckNMMMMMMMMM \n \
	MMMKdoooooooooooooolc;;;:loooooooooooooooooll:,;loolllllllllc::oddxOXWMMMMMMMMMM \n \
	MMW0oooooooooooool:;;:clooooooloooooooooloololc;;;:clllllc::lkXMMMMMMMMMMMMMMMMM \n \
	MMNkooooooooooooolccloooooooooolooooooooooooooolc:;;;;:codk0NMMMMMMMMMMMMMMMMMMM \n \
	MMXxoooooooooooooooolooooooooooooooooooooooooooooooolldKWMMMMMMMMMMMMMMMMMMMMMMM \n \
	MWKdlooooooooooooooooooooooooooooooooooooooooooooolood0WMMMMMMMMMMMMMMMMMMMMMMMM \n \
	MWOolooooooooooooooooooooooooooooooooooooooooooooooookNMMMMMMMMMMMMMMMMMMMMMMMMM \n \
	MNkooloooooooooooooooooooooooooooooooooooooooooooooldKWMMMMMMMMMMMMMMMMMMMMMMMMM \n \
	MNkoolooooooooooooooooooooooooooooooooooooooooooooooxXMMMMMMMMMMMMMMMMMMMMMMMMMM \n \
	MXxlolloooooooooooooooooooooooooooooooooooooooooolookNMMMMMMMMMMMMMMMMMMMMMMMMMM \n \
	MXxoooooooooooooooooooooooooooooooooooooooooooooooooOWMMMMMMMMMMMMMMMMMMMMMMMMMM \n \
	MKdoooooooooooooooooooooooooooooooooooooooooooooooloOWMMMMMMMMMMMMMMMMMMMMMMMMMM \n \
	MKdlooooooooooooooooooooooooooooooooooooooooooooooookNMMMMMMMMMMMMMMMMMMMMMMMMMM \n \
	MKdlooooooooooooooooooooooooooooooooooooooooooooooookNMMMMMMMMMMMMMMMMMMMMMMMMMM \n \
	MKdlooooooooooooooooooooooooooooooooooooooooooooooloxXMMMMMMMMMMMMMMMMMMMMMMMMMM \n \
	MKdloooooooooooooooooooooooooooooooooooooooooooooolldKMMMMMMMMMMMMMMMMMMMMMMMMMM \n \
	MKdloooooooooooooooooooooooooooooooooooooooooooooooloOWMMMMMMMMMMMMMMMMMMMMMMMMM \n \
	MKdloooooooolooooooooooooooooooooooooooooooooooooooooxXMMMMMMMMMMMMMMMMMMMMMMMMM \n \
	MXxoooloooooooooooooooooooooooooooooooooooooooooooooodKWMMMMMMMMMMMMMMMMMMMMMMMM \n \
	MXxoooooooooooooooooooooooooooooooooooooooooooooooooookNMMMMMMMMMMMMMMMMMMMMMMMM \n \
	MNkoloooooooollloooooooooooooooooooooooooooooooooooooodKWMMMMMMMMMMMMMMMMMMMMMMM \n \
	MWOolooooooooooooooooooooooooooooooooooooooooooooooooookNMMMMMMMMMMMMMMMMMMMMMMM \n \
	MWKxddddddxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxkXMMMMMMMMMMMMMMMMMMMMMMM \n \
	          ┌─┐┌┐┌┌┬┐┬─┐┌─┐┌─┐┌─┐  ┌┐┌┌─┐  ┌─┐┌─┐┌─┐┌─┐┌┬┐┌─┐┌┬┐┌─┐				 \n \
	          ├┤ │││ │ ├┬┘├┤ │ ┬├─┤  ││││ │  ├─┤│  ├┤ ├─┘ │ ├─┤ ││├─┤				 \n \
	          └─┘┘└┘ ┴ ┴└─└─┘└─┘┴ ┴  ┘└┘└─┘  ┴ ┴└─┘└─┘┴   ┴ ┴ ┴─┴┘┴ ┴				 \n" RESET);
}

// Procedimiento que imprime la informacion del personaje y sombra.
void imprimir_stats(personaje_t personaje,  sombra_t sombra, int nivel){
	
	char vida_bloo[7];
	char tiene_llaves[3];

	if (sombra.esta_viva){
		strcpy(vida_bloo, "vivo");
	}else{
		strcpy(vida_bloo, "muerto");
	}

	if (personaje.tiene_llave){
		strcpy(tiene_llaves, "Si");
	}else{
		strcpy(tiene_llaves, "No");
	}

	system("clear");

	printf(AZUL " HP: %i | Puntos: %i | Bloo: %s | Llave: %s | LV: %i\n" RESET, personaje.vida, personaje.puntos, vida_bloo, tiene_llaves, nivel);
}