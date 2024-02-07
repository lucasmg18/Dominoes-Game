//Lucas Martin Garcia y Lucas Cuesta Araujo doble grado informatica y mates

#include<iostream>
#include<fstream>
#include<string>
#include<ctime>
using namespace std;

const bool fichasOcultas = true, soloMaquina = false, probarMaq = false, usarEstrategia4 = true;//probarMaq es un modo para probar distintas estrategias
const short int numFichas = 55, MinJugadores = 2, MaxJugadores = 4, MinDigito = 6, MaxDigito = 9, numRobadas = 7, numVueltas = 200;
//numVueltas es el numero de partidas en el modo probarMaq

typedef short int tArrayNumerosF[MaxDigito + 1];//son x numeros(9) mas el 0 (en total 10) para las etrategias opcionales

typedef struct {
	short int izq;
	short int der;
}tFicha;

typedef tFicha tArrayFichas[numFichas];

typedef struct {
	tArrayFichas fichas;
	short int cont;
}tListaFichas;

typedef short int tPuntosJugadores[MaxJugadores];

typedef tListaFichas tJugadores[MaxJugadores];

typedef struct {
	short int numJug;
	short int maxValor;
	tListaFichas pozo;
	tJugadores jugadores;
	tPuntosJugadores puntosJug;
	string tablero;
}tJuego;


void mostrarTablero(const tJuego& juego);
void mostrarMenu();
short int menuInicio();
void iniciar(tJuego& juego, short int& jugInicial, bool& finPartida);
short int numeroJugadores();
short int maximoValor();
void turnoJugador(tJuego& juego, bool& salirJuego, bool& finPartida);
bool estrategia1(tJuego& juego, short int maqNum);
bool estrategia2(tJuego& juego, short int maqNum);
bool estrategia3(tJuego& juego, short int maqNum);
bool estrategia4(tJuego& juego, short int maqNum);
void robarFichaMaquina(tListaFichas& pozo, tListaFichas& jugador, short int maqNum);
void ponerFichaMaquina(string& tablero, tListaFichas& jugador, short int maqNum, short int posicionFicha, short int lado);
void subEstrategia2(const tJuego& juego, short int maqNum, short int& posicionFicha, short int& lado);
bool subEstrategia3(const tJuego& juego, short int maqNum, short int& posicionFicha, short int& lado, tArrayNumerosF numerosf);
void cantidadFichas(const tJuego& juego, tArrayNumerosF numerosf, short int maqNum);
bool puedeSeguir(const tListaFichas& jugador, short int fichaNum, short int numerof);
bool cierreSubestrategia(const tJuego& juego, short int maqNum, short int& posicionFicha, short int& lado, tArrayNumerosF numerosf);
short int elegirOpcionMenu(const tJuego& juego);
void generaPozo(tArrayFichas& pozo, short int maxValor);
void desordenaPozo(tArrayFichas& pozo, short int maxValor);
short int quienEmpieza(const tJuego& juego, short int& indice);
void robaFicha(tListaFichas& pozo, tListaFichas& fichasJug);
string toStr(short int n);
string fichaStr(short int izquierda, short int derecha);
short int aleat(short int maxValor);
bool sinSalida(const tJuego& juego);
bool puedeColocarFicha(const tListaFichas& jugador, string tablero);
bool puedeIzq(const string tablero, const tFicha ficha);
bool puedeDer(const string tablero, const tFicha ficha);
void ponerIzq(string& tablero, const tFicha ficha);
void ponerDer(string& tablero, const tFicha ficha);
void eliminaFicha(tListaFichas& jugador, short int fichaNum);
void sumaPuntos(tJuego& juego);
void mostrarPuntos(const tJuego& juego);
bool leerJuego(tJuego& juego);
void leerListaFichas(ifstream& entrada, tListaFichas& listaFichas);
void escribirJuego(const tJuego& juego);
void escribirListaFichas(ofstream& salida, const tListaFichas& listaFichas);


int main() {
	srand(time(NULL));
	system("chcp 1250");
	system("cls");
	short int op = 1, start, turno = 0, jugInicial;
	string seguir;
	bool salirJuego = false, finPartida = false, colocado;//inicializadas desde el principio para la primera ronda
	tJuego juego;

	start = menuInicio();
	if (start == 2) {
		cout << "como se llama la partida que quieres cargar: ";
		if (!leerJuego(juego)) {
			start = 1;// si no se ha cargado la partida empieza una nueva
			cout << " No se pudo cargar esa partida, empezando una partida nueva...." << endl;
		}
		else {
			cout << " Partida cargada correctamente." << endl;
			system("pause");
			system("cls");
			mostrarTablero(juego);
		}
	}
	if (start == 1) {
		juego.numJug = numeroJugadores();
		for (short int t = 0; t < juego.numJug; t++) {// inicializa los puntos de cada jugador
			juego.puntosJug[t] = 0;
		}
		juego.maxValor = maximoValor();
		iniciar(juego, jugInicial, finPartida);
		turno = (jugInicial + 1) % (juego.numJug);// para que empiece el jugador siguiente al que ha empezado
	}

	while (start != 3 && !salirJuego) {// bucle principal que sigue hasta que el jugador quiere salir del programa
		short int vueltas;// para probar distintas estrategias
		if (!probarMaq) vueltas = 1;
		else vueltas = numVueltas;// probar x partidas seguidas
		for (short int w = 0; w < vueltas; w++) {
			while (!finPartida) {// while se ejecuta mientras no ganes, pierdas o le des a salir
				while (turno < juego.numJug && !finPartida) {// para que cuando gane un jugador o maquina no coloquen mas piezas (finPartida será false)
					colocado = true;//inicializar el bool (para que no se meta en la función en el turno del jugador
					switch (turno) {
					case 0:
					{
						if (!soloMaquina) {//si esta en modo solomaquina no juega el jugador
							turnoJugador(juego, salirJuego, finPartida);
						}
						else {
							colocado = estrategia3(juego, turno);
						}
					}
					break;
					case 1:
					{
						colocado = estrategia2(juego, turno);
					}
					break;
					case 2:
					{
						colocado = estrategia1(juego, turno);
					}
					break;
					case 3:
					{
						if (usarEstrategia4) {
							colocado = estrategia4(juego, turno);
						}
						else {
							colocado = estrategia1(juego, turno);
						}
					}
					break;
					default:
					{
						colocado = estrategia1(juego, turno);
					}
					}
					if (!probarMaq && !colocado) {// si la maquina no ha colocado
						cout << endl << " La Máquina #" << turno << " no puede colocar ni robar, pasa al siguiente turno." << endl;
						system("pause");
						system("cls");
						mostrarTablero(juego);
					}
					if (juego.jugadores[turno].cont == 0) {//la maquina gana y se puede jugar otra partida
						finPartida = true;
					}
					if (sinSalida(juego) && !finPartida) {//si no quedan fichas y no pueden poner y todavia no ha ganado nadie(finPartida)
						finPartida = true;
						if (!probarMaq) {
							cout << endl << endl << " No quedan fichas y ningun jugador puede poner ficha." << endl << endl;
							system("pause");
						}
					}
					turno++;
				}
				turno = 0;//para la siguiente ronda
			}
			finPartida = false; // para la siguiente partida
			if (!salirJuego) {// si ha elegido salir (opcion 4 del menu) "salir" valdrá true porque no debería mostrar los puntos 
				system("cls");
				mostrarTablero(juego);
				sumaPuntos(juego);
				mostrarPuntos(juego);
				if (!probarMaq) {
					cout << endl << endl << "Quieres seguir jugando?[S/N]: ";
					cin >> seguir;
				}
				else seguir = "s";
				if (seguir == "S" || seguir == "s") {
					iniciar(juego, jugInicial, finPartida);
					turno = (jugInicial + 1) % (juego.numJug);// para que empiece el jugador siguiente al que ha puesto ficha
				}
				else {
					salirJuego = true;
				}
			}
		}
		if (probarMaq) {// al acabar las vueltas en el modo de probar estrategias muestra la puntucaión total
			mostrarPuntos(juego);
			system("pause");
		}
	}
	system("cls");
	cout << endl << endl << "      Gracias por jugar." << endl;
	return 0;
}

short int menuInicio() {
	short int start;
	cout << " ------------------- " << endl;
	cout << "|    D O M I N Ó    |" << endl;
	cout << " ------------------- " << endl << endl;
	cout << " 1. Nueva partida." << endl;
	cout << " 2. Cargar partida." << endl;
	cout << " 3. Salir del juego." << endl;
	cout << " Elige una opción: ";
	cin >> start;
	while (start < 1 || start > 3) {
		cout << " Opción no valida, elige otra opción: ";
		cin >> start;
	}
	return start;
}

void iniciar(tJuego& juego, short int& jugInicial, bool& finPartida) {//establece todos los valores para una nueva partida
	short int indice;
	jugInicial = -1;
	while (jugInicial < 0) {//se repite hasta que alguien tenga un doble
		generaPozo(juego.pozo.fichas, juego.maxValor);
		desordenaPozo(juego.pozo.fichas, juego.maxValor);
		juego.pozo.cont = (juego.maxValor + 1) * (juego.maxValor + 2) / 2;
		for (short int t = 0; t < juego.numJug; t++) {//los jugadores roban las x primeras fichas
			juego.jugadores[t].cont = 0;
			for (short int i = 0; i < numRobadas; i++) {
				robaFicha(juego.pozo, juego.jugadores[t]);
			}
		}
		jugInicial = quienEmpieza(juego, indice);
	}
	if (!probarMaq) {// solo se muestra si no esta el programa en modo "probarMaq" en el que prueba distintas estrategias
		cout << endl << " Empieza ";
		if (jugInicial == 0 && !soloMaquina) { cout << " el Jugador"; }
		else { cout << " la Máquina #" << jugInicial; }
		cout << " con la ficha " << fichaStr(juego.jugadores[jugInicial].fichas[indice - 1].izq, juego.jugadores[jugInicial].fichas[indice - 1].der) << "." << endl;
		system("pause");
	}
	juego.tablero = fichaStr(juego.jugadores[jugInicial].fichas[indice - 1].izq, juego.jugadores[jugInicial].fichas[indice - 1].der);
	eliminaFicha(juego.jugadores[jugInicial], indice);
	if (juego.jugadores[jugInicial].cont == 0) {//por si se roba solo una ficha
		finPartida = true;
	}
	else {
		finPartida = false;
	}
	system("cls");
	mostrarTablero(juego);
}

short int numeroJugadores() {//numero de jugadores en la partida
	short int numJug;
	cout << " Numero de jugadores [" << MinJugadores << "-" << MaxJugadores << "]:";
	cin >> numJug;
	while (numJug < MinJugadores || numJug > MaxJugadores) {
		cout << " El numero de jugadores debe estar entre " << MinJugadores << " y " << MaxJugadores << ":";
		cin >> numJug;
	}
	return numJug;
}

short int maximoValor() {//maximo valor de los numeros de las fichas del dominó
	short int maxValor;
	cout << " Valor máximo de las fichas de dominó [" << MinDigito << "-" << MaxDigito << "]:";
	cin >> maxValor;
	while (maxValor < MinDigito || maxValor >  MaxDigito) {
		cout << " El numero de jugadores debe estar entre " << MinDigito << " y " << MaxDigito << ":";
		cin >> maxValor;
	}
	return maxValor;
}

void turnoJugador(tJuego& juego, bool& salirJuego, bool& finPartida) {//turno del jugador humano
	bool centinela = true;
	short int op, fichaNum;
	string guardar;

	while (centinela) { // while por si el usuario se equivoca de opcion
		mostrarMenu();
		op = elegirOpcionMenu(juego);
		switch (op) {
		case 0:// se sale del juego y le ofrece guardar partida
		{
			cout << " Quieres guardar la partida? [S/N]: ";
			cin >> guardar;
			if (guardar == "s" || guardar == "S") {
				cout << " Introduce un nombre para la partida: ";
				escribirJuego(juego);
			}
			salirJuego = true;
			finPartida = true;
			centinela = false;
		}
		break;
		case 1://los dos primeros casos de poner ficha comparten codigo
		case 2:
		{
			cout << " Que ficha quieres colocar [1-" << juego.jugadores[0].cont << "]: ";
			cin >> fichaNum;
			while (fichaNum <1 || fichaNum > juego.jugadores[0].cont) {
				cout << " No existe esa ficha, elige otra:";
				cin >> fichaNum;
			}
			if (op == 1) {
				if (puedeIzq(juego.tablero, juego.jugadores[0].fichas[fichaNum - 1])) {
					ponerIzq(juego.tablero, juego.jugadores[0].fichas[fichaNum - 1]);
					eliminaFicha(juego.jugadores[0], fichaNum);
					centinela = false;
				}
				else if (!puedeIzq(juego.tablero, juego.jugadores[0].fichas[fichaNum - 1])) {
					cout << " No se puede colocar esa ficha por la izquierda." << endl;
					system("pause");
				}
			}
			else if (op == 2) {
				if (puedeDer(juego.tablero, juego.jugadores[0].fichas[fichaNum - 1])) {
					ponerDer(juego.tablero, juego.jugadores[0].fichas[fichaNum - 1]);
					eliminaFicha(juego.jugadores[0], fichaNum);
					centinela = false;
				}
				else if (!puedeDer(juego.tablero, juego.jugadores[0].fichas[fichaNum - 1])) {
					cout << " No se puede colocar esa ficha por la derecha." << endl;
					system("pause");
				}
			}
		}
		break;
		case 3:
		{
			if (juego.pozo.cont > 0) {
				robaFicha(juego.pozo, juego.jugadores[0]);
			}
			else {// si no puede poner (la restriccion ya está aplicada en la función elegirOpciónMenu) y no quedan fichas en el pozo pasa al siguiente turno
				cout << endl << " No quedan fichas, pasa al siguiente jugador." << endl;
				system("pause");
				centinela = false;
			}
		}
		}
		system("cls");
		mostrarTablero(juego);
	}
}

bool estrategia1(tJuego& juego, short int maqNum) {
	bool centinela1 = true, colocado = false;
	short int fichaNum;

	while (centinela1) {
		if (puedeColocarFicha(juego.jugadores[maqNum], juego.tablero)) {
			if (!probarMaq) cout << endl << " La Máquina #" << maqNum << " va a colocar la ficha ";
			fichaNum = 1;// empieza con la primera ficha
			while (fichaNum <= juego.jugadores[maqNum].cont && centinela1) {
				if (puedeIzq(juego.tablero, juego.jugadores[maqNum].fichas[fichaNum - 1])) {
					ponerIzq(juego.tablero, juego.jugadores[maqNum].fichas[fichaNum - 1]);
					if (!probarMaq) cout << fichaStr(juego.jugadores[maqNum].fichas[fichaNum - 1].izq, juego.jugadores[maqNum].fichas[fichaNum - 1].der) << " por la izquierda." << endl;
					eliminaFicha(juego.jugadores[maqNum], fichaNum);
					centinela1 = false;
				}
				else if (puedeDer(juego.tablero, juego.jugadores[maqNum].fichas[fichaNum - 1]) && centinela1) {
					ponerDer(juego.tablero, juego.jugadores[maqNum].fichas[fichaNum - 1]);
					if (!probarMaq) cout << fichaStr(juego.jugadores[maqNum].fichas[fichaNum - 1].izq, juego.jugadores[maqNum].fichas[fichaNum - 1].der) << " por la derecha." << endl;
					eliminaFicha(juego.jugadores[maqNum], fichaNum);
					centinela1 = false;
				}
				fichaNum++;
			}
			if (!probarMaq) system("pause");
			colocado = true;
		}
		else if (juego.pozo.cont > 0 && centinela1) {
			robarFichaMaquina(juego.pozo, juego.jugadores[maqNum], maqNum);
		}
		else {
			centinela1 = false;
		}
		system("cls");
		mostrarTablero(juego);
	}
	return colocado;
}

bool estrategia2(tJuego& juego, short int maqNum) {
	bool centinela1 = true, colocado = false;
	short int posicionFicha, lado;// la variable lado es solo de escritura en la subestrategia2 que se usa para el resto de estrategias pero aqui no hace falta

	while (centinela1) {
		if (puedeColocarFicha(juego.jugadores[maqNum], juego.tablero)) {
			subEstrategia2(juego, maqNum, posicionFicha, lado);
			ponerFichaMaquina(juego.tablero, juego.jugadores[maqNum], maqNum, posicionFicha, lado);
			centinela1 = false;
			colocado = true;
		}
		else if (juego.pozo.cont > 0 && centinela1) {
			robarFichaMaquina(juego.pozo, juego.jugadores[maqNum], maqNum);
		}
		else {
			centinela1 = false;
		}
		system("cls");
		mostrarTablero(juego);
	}
	return colocado;
}

bool estrategia3(tJuego& juego, short int maqNum) {//estrategia de la parte opcional 2
	bool centinela1 = true, colocado = false;
	short int posicionFicha, lado;
	tArrayNumerosF numerosf;

	while (centinela1) {
		cantidadFichas(juego, numerosf, maqNum);//calcular el numero de fichas con cada numero que tienen el resto de jugadores
		if (puedeColocarFicha(juego.jugadores[maqNum], juego.tablero)) {
			if (subEstrategia3(juego, maqNum, posicionFicha, lado, numerosf)) {// si no ha sacado una ficha de la estrategia 3(subestrategia3() va a ser true), se usa la estrategia2
				subEstrategia2(juego, maqNum, posicionFicha, lado);
			}
			ponerFichaMaquina(juego.tablero, juego.jugadores[maqNum], maqNum, posicionFicha, lado);
			centinela1 = false;
			colocado = true;
		}
		else if (juego.pozo.cont > 0 && centinela1) {
			robarFichaMaquina(juego.pozo, juego.jugadores[maqNum], maqNum);
		}
		else {
			centinela1 = false;
		}
		system("cls");
		mostrarTablero(juego);
	}
	return colocado;
}

bool estrategia4(tJuego& juego, short int maqNum) {//estrategia de la parte opcional 3
	bool centinela1 = true, centinela3 = true, colocado = false;
	short int posicionFicha, lado;
	tArrayNumerosF numerosf;

	while (centinela1) {
		cantidadFichas(juego, numerosf, maqNum);//calcular el numero de fichas con cada numero
		if (puedeColocarFicha(juego.jugadores[maqNum], juego.tablero)) {
			centinela3 = true;
			centinela3 = cierreSubestrategia(juego, maqNum, posicionFicha, lado, numerosf);// si encuentra una ficha para cerrar centinela3 sera false y la coloca directamente
			if (centinela3) {// aqui mira a ver si puede poner algún doble que sea mayor o igual que la mitad de maxValor
				for (short int t = 0; t < juego.jugadores[maqNum].cont; t++) {
					if ((juego.jugadores[maqNum].fichas[t].izq == juego.jugadores[maqNum].fichas[t].der) && (juego.jugadores[maqNum].fichas[t].izq
						>= juego.maxValor / 2) && (puedeIzq(juego.tablero, juego.jugadores[maqNum].fichas[t]) || puedeDer(juego.tablero, juego.jugadores[maqNum].fichas[t]))) {
						posicionFicha = t + 1;
						centinela3 = false;
						lado = 3;// poner por cualquiera de los lados
					}
				}
			}
			if (centinela3) {// si no puede poner ningun doble, pasa a la siguiente estrategia(estrategia3)
				centinela3 = subEstrategia3(juego, maqNum, posicionFicha, lado, numerosf);
			}
			if (centinela3) {// si no ha sacado una ficha se usa la estrategia2
				subEstrategia2(juego, maqNum, posicionFicha, lado);
			}
			ponerFichaMaquina(juego.tablero, juego.jugadores[maqNum], maqNum, posicionFicha, lado);
			centinela1 = false;
			colocado = true;
		}
		else if (juego.pozo.cont > 0 && centinela1) {
			robarFichaMaquina(juego.pozo, juego.jugadores[maqNum], maqNum);
		}
		else {
			centinela1 = false;
		}
		system("cls");
		mostrarTablero(juego);
	}
	return colocado;
}

void ponerFichaMaquina(string& tablero, tListaFichas& jugador, short int maqNum, short int posicionFicha, short int lado) {//pone la ficha de la máquina y muestra cual ha puesto y donde
	if (!probarMaq) cout << endl << " La Máquina #" << maqNum << " va a colocar la ficha "
		<< fichaStr(jugador.fichas[posicionFicha - 1].izq, jugador.fichas[posicionFicha - 1].der);
	if (puedeIzq(tablero, jugador.fichas[posicionFicha - 1]) && (lado == 1 || lado == 3)) {//coloca la ficha a partir de su posición en las fichas y el lado 
		ponerIzq(tablero, jugador.fichas[posicionFicha - 1]);
		if (!probarMaq) cout << " por la izquierda." << endl;
	}
	else if (puedeDer(tablero, jugador.fichas[posicionFicha - 1]) && (lado == 2 || lado == 3)) {//si el lado es 3 es que da igual por que lado se ponga
		ponerDer(tablero, jugador.fichas[posicionFicha - 1]);
		if (!probarMaq) cout << " por la derecha." << endl;
	}
	if (!probarMaq) system("pause");
	eliminaFicha(jugador, posicionFicha);
}

void robarFichaMaquina(tListaFichas& pozo, tListaFichas& jugador, short int maqNum) {//roba la ficha para la máquina y muestra lo que roba
	robaFicha(pozo, jugador);
	if (!probarMaq) {
		cout << endl << " La Máquina #" << maqNum << " va a robar ";
		if (fichasOcultas) {
			cout << "una ficha." << endl;
		}
		else {
			cout << "la ficha " << fichaStr(jugador.fichas[jugador.cont - 1].izq, jugador.fichas[jugador.cont - 1].der) << "." << endl;
		}
		system("pause");
	}
}

void subEstrategia2(const tJuego& juego, short int maqNum, short int& posicionFicha, short int& lado) {//esta es la estrategia2 que se usa en varias estrategias
	short int fichaNum = 1, suma = -1;//suma es -1 para que la primera ficha que encuentre la use y luego ya va comparando
	for (fichaNum; fichaNum <= juego.jugadores[maqNum].cont; fichaNum++) {
		if (puedeIzq(juego.tablero, juego.jugadores[maqNum].fichas[fichaNum - 1])
			&& (suma < juego.jugadores[maqNum].fichas[fichaNum - 1].izq + juego.jugadores[maqNum].fichas[fichaNum - 1].der)) {
			posicionFicha = fichaNum;
			lado = 1;
			suma = juego.jugadores[maqNum].fichas[posicionFicha - 1].izq + juego.jugadores[maqNum].fichas[posicionFicha - 1].der;
		}
		if (puedeDer(juego.tablero, juego.jugadores[maqNum].fichas[fichaNum - 1])
			&& (suma < juego.jugadores[maqNum].fichas[fichaNum - 1].izq + juego.jugadores[maqNum].fichas[fichaNum - 1].der)) {
			posicionFicha = fichaNum;
			lado = 2;
			suma = juego.jugadores[maqNum].fichas[posicionFicha - 1].izq + juego.jugadores[maqNum].fichas[posicionFicha - 1].der;
		}
	}
}

bool subEstrategia3(const tJuego& juego, short int maqNum, short int& posicionFicha, short int& lado, tArrayNumerosF numerosf) {
	//esta es la base para la estrategia3 que también se usa en la 4. Se podría haber reutilizado codigo y acortar la función pero impilcaría cambiar las partes no opcionales, asique me he abstenido a modificar las partes no opcionales
	short int cantidadf;//cantidadf es el numero de fichas que tienen los rivales con el numero exterior de la ficha que se va a poner
	bool resultado = true;
	cantidadf = juego.maxValor + 1;// de esta forma la primera ficha con la que pueda seguir tendrá un número menor y entrará en la condición
	for (short int fichaNum = 1; fichaNum <= juego.jugadores[maqNum].cont; fichaNum++) {// de aqui saca la ficha con la que luego pueda seguir con minimizacion si existe
		if (puedeIzq(juego.tablero, juego.jugadores[maqNum].fichas[fichaNum - 1])) {
			if (((juego.jugadores[maqNum].fichas[fichaNum - 1].der == juego.tablero[1] - char('0'))
				&& (cantidadf > numerosf[juego.jugadores[maqNum].fichas[fichaNum - 1].izq]))) {//caso en el que puede poner sin rotar
				if (puedeSeguir(juego.jugadores[maqNum], fichaNum, juego.jugadores[maqNum].fichas[fichaNum - 1].izq)) {
					cantidadf = numerosf[juego.jugadores[maqNum].fichas[fichaNum - 1].izq];
					posicionFicha = fichaNum;
					lado = 1; //poner por la izquierda
				}
			}
			if (((juego.jugadores[maqNum].fichas[fichaNum - 1].izq == juego.tablero[1] - char('0'))
				&& (cantidadf > numerosf[juego.jugadores[maqNum].fichas[fichaNum - 1].der]))) {//caso en el que la ficha se coloca rotada
				if (puedeSeguir(juego.jugadores[maqNum], fichaNum, juego.jugadores[maqNum].fichas[fichaNum - 1].der)) {
					cantidadf = numerosf[juego.jugadores[maqNum].fichas[fichaNum - 1].der];
					posicionFicha = fichaNum;
					lado = 1; //poner por la izquierda
				}
			}
		}
		if (puedeDer(juego.tablero, juego.jugadores[maqNum].fichas[fichaNum - 1])) {
			if (((juego.jugadores[maqNum].fichas[fichaNum - 1].izq == juego.tablero[juego.tablero.length() - 2] - char('0'))
				&& (cantidadf > numerosf[juego.jugadores[maqNum].fichas[fichaNum - 1].der]))) {//caso en el que puede poner sin rotar 
				if (puedeSeguir(juego.jugadores[maqNum], fichaNum, juego.jugadores[maqNum].fichas[fichaNum - 1].der)) {
					cantidadf = numerosf[juego.jugadores[maqNum].fichas[fichaNum - 1].der];
					posicionFicha = fichaNum;
					lado = 2; //poner por la derecha
				}
			}
			if (((juego.jugadores[maqNum].fichas[fichaNum - 1].der == juego.tablero[juego.tablero.length() - 2] - char('0'))
				&& (cantidadf > numerosf[juego.jugadores[maqNum].fichas[fichaNum - 1].izq]))) {//caso en el que la ficha se coloca rotada
				if (puedeSeguir(juego.jugadores[maqNum], fichaNum, juego.jugadores[maqNum].fichas[fichaNum - 1].izq)) {
					cantidadf = numerosf[juego.jugadores[maqNum].fichas[fichaNum - 1].izq];
					posicionFicha = fichaNum;
					lado = 2; //poner por la derecha
				}
			}
		}
	}
	if (cantidadf != juego.maxValor + 1) resultado = false;//si ha encontrado una ficha para cerrar, cantidadf sera menor que 9 porque el numero total de fichas con cada numero es 9
	return resultado;
}

bool cierreSubestrategia(const tJuego& juego, short int maqNum, short int& posicionFicha, short int& lado, tArrayNumerosF numerosf) {
	//esta función mira a ver si puede poner alguna ficha tal que nadie más pueda poner excepto la máquina. Se podría haber reutilizado codigo y acortar la función pero impilcaría cambiar las partes no opcionales, asique me he abstenido a modificar las partes no opcionales
	short int maqPunt = 0, punTot = 0;
	bool cierre, resultado = true;
	for (short int i = 0; i < juego.jugadores[maqNum].cont; i++) {//calcula el numero de puntos que suman las fichas que tiene actualmente la maquina
		maqPunt = maqPunt + juego.jugadores[maqNum].fichas[i].izq + juego.jugadores[maqNum].fichas[i].der;
	}
	punTot = (juego.maxValor + 2) * ((juego.maxValor) * (juego.maxValor + 1) / 2);// calcula el número total de puntos que hay sumando todas las fichas de la partida
	for (short int num = 1; num <= juego.jugadores[maqNum].cont; num++) {//aquí calcula si puede poner una ficha tal que nadie más pueda poner (excepto él)
		if (puedeIzq(juego.tablero, juego.jugadores[maqNum].fichas[num - 1])) {
			cierre = juego.tablero[juego.tablero.length() - 2] - char('0') == juego.jugadores[maqNum].fichas[num - 1].izq;
			if ((juego.tablero[1] - char('0') == juego.jugadores[maqNum].fichas[num - 1].der) && cierre
				&& (numerosf[juego.jugadores[maqNum].fichas[num - 1].izq] == 0)) {// caso en el que pone sin rotar
				if (puedeSeguir(juego.jugadores[maqNum], num, juego.jugadores[maqNum].fichas[num - 1].izq) || punTot * 1 / (juego.numJug + 1) >= maqPunt) {//solo si sus puntos son menores que la media, cierra la partida
					posicionFicha = num;
					lado = 1;//poner por la izquierda
					resultado = false;
				}
			}
			cierre = juego.tablero[juego.tablero.length() - 2] - char('0') == juego.jugadores[maqNum].fichas[num - 1].der;
			if ((juego.tablero[1] - char('0') == juego.jugadores[maqNum].fichas[num - 1].izq) && cierre
				&& (numerosf[juego.jugadores[maqNum].fichas[num - 1].der] == 0)) {// caso en el que pone rotando la ficha
				if (puedeSeguir(juego.jugadores[maqNum], num, juego.jugadores[maqNum].fichas[num - 1].der) || punTot * 1 / (juego.numJug + 1) >= maqPunt) {//solo si sus puntos son menores que la media, cierra la partida
					posicionFicha = num;
					lado = 1;//poner por la izquierda
					resultado = false;
				}
			}
		}
		if (puedeDer(juego.tablero, juego.jugadores[maqNum].fichas[num - 1])) {
			cierre = juego.tablero[1] - char('0') == juego.jugadores[maqNum].fichas[num - 1].der;
			if ((juego.tablero[juego.tablero.length() - 2] - char('0') == juego.jugadores[maqNum].fichas[num - 1].izq) && cierre
				&& (numerosf[juego.jugadores[maqNum].fichas[num - 1].der] == 0)) {// caso en el que pone sin rotar
				if (puedeSeguir(juego.jugadores[maqNum], num, juego.jugadores[maqNum].fichas[num - 1].der) || punTot * 1 / (juego.numJug + 1) >= maqPunt) {//solo si sus puntos son menores que la media, cierra la partida
					posicionFicha = num;
					lado = 2;//poner por la izquierda
					resultado = false;
				}
			}
			cierre = juego.tablero[1] - char('0') == juego.jugadores[maqNum].fichas[num - 1].izq;
			if ((juego.tablero[juego.tablero.length() - 2] - char('0') == juego.jugadores[maqNum].fichas[num - 1].der) && cierre
				&& (numerosf[juego.jugadores[maqNum].fichas[num - 1].izq] == 0)) {// caso en el que pone rotando la ficha
				if (puedeSeguir(juego.jugadores[maqNum], num, juego.jugadores[maqNum].fichas[num - 1].izq) || punTot * 1 / (juego.numJug + 1) >= maqPunt) {//solo si sus puntos son menores que la media, cierra la partida
					posicionFicha = num;
					lado = 2;//poner por la izquierda
					resultado = false;
				}
			}
		}
	}
	return resultado;
}

void cantidadFichas(const tJuego& juego, tArrayNumerosF numerosf, short int maqNum) {// calcula cuantas fichas tiene el resto de jugadores con cada numero   
	for (short int i = 0; i <= juego.maxValor; i++) {// pone el numero de fichas totales que hay ene el juego con cada número
		numerosf[i] = juego.maxValor + 1;
	}
	for (short int i = 0; i < juego.tablero.length() / 5; i++) {//mira cada ficha del tablero y resta uno a cada número por cada ficha con ese número en el tablero
		short int h = (i * 5) + 1;// las posiciones de los numeros de las fichas del tablero
		for (short int j = 0; j <= juego.maxValor; j++) {
			if ((juego.tablero[h] - char('0') == j) || (juego.tablero[h + 2] - char('0') == j)) {
				numerosf[j]--;
			}
		}
	}
	for (short int i = 0; i < juego.jugadores[maqNum].cont; i++) {//mira cada ficha de la maquina y resta uno a cada número por cada ficha con ese número 
		for (short int j = 0; j <= juego.maxValor; j++) {
			if ((juego.jugadores[maqNum].fichas[i].izq == j) || (juego.jugadores[maqNum].fichas[i].der == j)) {
				numerosf[j]--;
			}
		}
	}
}

bool puedeSeguir(const tListaFichas& jugador, short int fichaNum, short int numerof) {
	//aqui mira si al colocar esa ficha puede colocar otra despues, recibe numerof que es el numero de la ficha con el que debe comparar
	bool encontrado = false;
	short int num = 0;
	while (num < jugador.cont && !encontrado) {
		if (((numerof == jugador.fichas[num].izq) || (numerof == jugador.fichas[num].der))) {
			if (fichaNum - 1 != num) {// de este modo evita comparar la ficha consigo misma
				encontrado = true;
			}
		}
		num++;
	}
	return encontrado;
}

void mostrarTablero(const tJuego& juego) {
	short int maquinaInt;
	if (!probarMaq) {
		cout << endl << " ------------------ " << endl;
		cout << "|  T A B L E R O   |" << endl;
		cout << " ------------------ " << endl;
		cout << endl;
		cout << juego.tablero << endl;
		cout << endl;
		if (soloMaquina) {
			maquinaInt = 0;
		}
		else {
			maquinaInt = 1;
		}
		if (fichasOcultas) {
			for (short int f = maquinaInt; f < juego.numJug; f++) {
				cout << "Máquina #" << f << ": " << juego.jugadores[f].cont << endl;
			}
		}
		else {
			for (short int f = maquinaInt; f < juego.numJug; f++) {
				cout << "Máquina #" << f << ": ";
				for (short int i = 0; i < juego.jugadores[f].cont; i++) {
					cout << fichaStr(juego.jugadores[f].fichas[i].izq, juego.jugadores[f].fichas[i].der) << " ";
				}
				cout << endl;
			}
		}
		if (!soloMaquina) {
			cout << endl << "Jugador:    ";
			for (short int i = 0; i < juego.jugadores[0].cont; i++) {
				cout << fichaStr(juego.jugadores[0].fichas[i].izq, juego.jugadores[0].fichas[i].der) << " ";
			}
			cout << endl << "            ";
			for (short int i = 1; i <= juego.jugadores[0].cont; i++) {// para enumerar las fichas del jugador y hacer más fácil a la hora de elegirlas
				if (i < 10) {
					cout << "  " << i << "   ";
				}
				else {
					cout << " " << i << "   ";
				}
			}
		}
		cout << endl;
	}
}

void mostrarMenu() {
	cout << endl;
	cout << endl;
	cout << " ----------------- " << endl;
	cout << "| O P C I O N E S |" << endl;
	cout << " ----------------- " << endl;
	cout << "1. Poner ficha por la iquierda. " << endl;
	cout << "2. Poner ficha por la derecha. " << endl;
	cout << "3. Robar ficha nueva." << endl;
	cout << "0. Salir." << endl;
	cout << endl << " Elige una opcion: ";

}

short int elegirOpcionMenu(const tJuego& juego) {//aqui el usuario elige la opción y aplica restricciones
	short int op;
	cin >> op;
	while ((op > 3 || op < 0) || (puedeColocarFicha(juego.jugadores[0], juego.tablero) && (op == 3))) {// si hay algun probelma al elegir le vuelve pedir elegir al usuario hasta que sea correcto
		if (op > 3 || op < 0) {// si se sale del rango le vuelve pedir elegir opción
			cout << " Opcion no valida" << endl;
			system("pause");
			system("cls");
			mostrarTablero(juego);
			mostrarMenu();
			cin >> op;
		}
		else if (puedeColocarFicha(juego.jugadores[0], juego.tablero) && (op == 3)) { //si elige robar pero aun puede colocar ficha le vuelve pedir elegir opción
			cout << " ¡No puedes robar, aun puedes colocar ficha!" << endl;
			system("pause");
			system("cls");
			mostrarTablero(juego);
			mostrarMenu();
			cin >> op;
		}
	}
	return op;
}

string toStr(short int n) {//convierte un int en un string con el numero
	char caracter = char(n) + char('0');
	string resultado = "";
	resultado = caracter;
	return resultado;
}

string fichaStr(short int izquierda, short int derecha) {//convierte dos numeros de una ficha en un string con la ficha
	string ficha;
	ficha = "|" + toStr(izquierda) + "-" + toStr(derecha) + "|";
	return ficha;
}

void generaPozo(tArrayFichas& fichasPozo, short int maxValor) {
	short int k = 0;
	for (short int p1 = 0; p1 <= maxValor; p1++) {
		for (short int p2 = p1; p2 <= maxValor; p2++) {
			fichasPozo[k].izq = p1;
			fichasPozo[k].der = p2;
			k++;
		}
	}
}

void desordenaPozo(tArrayFichas& fichasPozo, short int maxValor) {
	int idx;
	tFicha tmp;
	for (int i = ((maxValor + 1) * (maxValor + 2) / 2) - 1; i >= 0; i--) {
		idx = aleat(i);
		if (i != idx) {
			tmp = fichasPozo[i];
			fichasPozo[i] = fichasPozo[idx];
			fichasPozo[idx] = tmp;
		}
	}
}

short int quienEmpieza(const tJuego& juego, short int& indice) {
	short int jugInicial = -1, doble = -1;//doble es el numero del mayor doble encontrado hast ahora

	for (short int jug = 0; jug < juego.numJug; jug++) { // busca dobles y se queda con el mayor si existe
		for (short int fich = 0; fich < juego.jugadores[jug].cont; fich++) {
			if (juego.jugadores[jug].fichas[fich].izq == juego.jugadores[jug].fichas[fich].der && juego.jugadores[jug].fichas[fich].izq > doble) {
				jugInicial = jug;
				indice = fich + 1;
				doble = juego.jugadores[jug].fichas[fich].izq;
			}
		}

	}
	return jugInicial;
}

void robaFicha(tListaFichas& pozo, tListaFichas& fichasJug) {
	fichasJug.fichas[fichasJug.cont].izq = pozo.fichas[pozo.cont - 1].izq;
	fichasJug.fichas[fichasJug.cont].der = pozo.fichas[pozo.cont - 1].der;
	pozo.cont--;
	fichasJug.cont++;
}

void eliminaFicha(tListaFichas& jugador, short int fichaNum) { // recibe fichaNum, que es el numero de la ficha pero su posicion va a ser una menos
	for (short int t = fichaNum - 1; t < jugador.cont; t++) {
		jugador.fichas[t].izq = jugador.fichas[t + 1].izq;
		jugador.fichas[t].der = jugador.fichas[t + 1].der;
	}
	jugador.cont--;
}

short int aleat(short int maxValor) {
	return ((0 + rand()) % (maxValor + 1));
}

bool sinSalida(const tJuego& juego) {//establece si ningún jugador puede colocar o robar
	bool noPuede = true;//este bool indica si no pueden colocar ficha todos los jugadores y no pueden robar 
	short int jug = 0;
	while (jug < juego.numJug && noPuede) {// en cuanto un jugador pueda poner ficha o si quedan fichas en el pozo se sale del while y devuelve false
		if (juego.pozo.cont != 0 || puedeColocarFicha(juego.jugadores[jug], juego.tablero)) {
			noPuede = false;
		}
		jug++;
	}
	return noPuede;
}

bool puedeColocarFicha(const tListaFichas& jugador, const string tablero) {//dice si el jugador o maquina puede colocar alguna ficha
	bool puede = false;
	short int fich = 0;
	while (fich < jugador.cont && !puede) {// busca si puede hasta encontrar la ficha o hasta mirar todas las fichas
		if (jugador.fichas[fich].izq == tablero[1] - char('0') || jugador.fichas[fich].der == tablero[1] - char('0') ||
			jugador.fichas[fich].izq == tablero[tablero.length() - 2] - char('0') || jugador.fichas[fich].der == tablero[tablero.length() - 2] - char('0')) {
			puede = true;
		}
		fich++;
	}
	return puede;
}

bool puedeIzq(const string tablero, const tFicha ficha) {
	return ((tablero[1] - char('0') == ficha.izq) || (tablero[1] - char('0') == ficha.der));
}

bool puedeDer(const string tablero, const tFicha ficha) {
	return ((tablero[tablero.length() - 2] - char('0') == ficha.izq) || (tablero[tablero.length() - 2] - char('0') == ficha.der));
}

void ponerIzq(string& tablero, const tFicha ficha) {
	if (tablero[1] - char('0') == ficha.der) {
		tablero = fichaStr(ficha.izq, ficha.der) + tablero;
	}
	else if (tablero[1] - char('0') == ficha.izq) {
		tablero = fichaStr(ficha.der, ficha.izq) + tablero;
	}
}

void ponerDer(string& tablero, const tFicha ficha) {
	if (tablero[tablero.length() - 2] - char('0') == ficha.der) {
		tablero = tablero + fichaStr(ficha.der, ficha.izq);
	}
	else if (tablero[tablero.length() - 2] - char('0') == ficha.izq) {
		tablero = tablero + fichaStr(ficha.izq, ficha.der);
	}
}

void sumaPuntos(tJuego& juego) {//suma los puntos de cada jugador
	short int suma;
	for (short int t = 0; t < juego.numJug; t++) {
		suma = 0;
		for (short int i = 0; i < juego.jugadores[t].cont; i++) {
			suma = suma + juego.jugadores[t].fichas[i].izq + juego.jugadores[t].fichas[i].der;
		}
		juego.puntosJug[t] = juego.puntosJug[t] + suma;
	}
}

void mostrarPuntos(const tJuego& juego) { //muestra los puntos y a demas dice quien ha ganado
	short int jug = 0, ganador;
	bool encontrado = false;//para que pare de buscar si ya ha encotrado al ganador
	if (!soloMaquina) {
		if (juego.jugadores[0].cont == 0) {
			cout << endl << "       Has ganado." << endl << endl;
			encontrado = true;
		}
		jug = 1;//si hay jugador humano entonces habrá una máquina menos
	}
	ganador = jug;//dos variables iguales que se necesitan para dos cosas distintas y van a ser modificadas en distintos lugares
	while (ganador < juego.numJug && !encontrado) {// muestra quien ha ganado esta ronda, si no hay ganador solo muestra la puntuación
		if (juego.jugadores[ganador].cont == 0) {
			cout << endl << "       La Máquina #" << ganador << " ha ganado." << endl << endl;
			encontrado = true;
		}
		ganador++;
	}
	cout << endl << "  PUNTUACIÓN " << endl;
	if (!soloMaquina) {
		cout << "Jugador: " << juego.puntosJug[0] << endl;
	}
	for (jug; jug < juego.numJug; jug++) {
		cout << "Máquina #" << jug << ": " << juego.puntosJug[jug] << endl;
	}
}

bool leerJuego(tJuego& juego) {//cargar partida
	string partida;
	bool leido;
	ifstream archivo;
	cin >> partida;
	archivo.open(partida);
	if (archivo.is_open()) {
		leido = true;
		archivo >> juego.numJug >> juego.maxValor >> juego.tablero;
		leerListaFichas(archivo, juego.pozo);// primero lee el pozo ya que no tiene la variable puntos
		for (short int i = 0; i < juego.numJug; i++) {
			leerListaFichas(archivo, juego.jugadores[i]);
			archivo >> juego.puntosJug[i];
		}
	}
	else {
		leido = false;
	}
	archivo.close();
	return leido;
}

void leerListaFichas(ifstream& entrada, tListaFichas& listaFichas) {//lee las listas de fichas de cada jugador y del pozo
	entrada >> listaFichas.cont;
	for (short int i = 0; i < listaFichas.cont; i++) {
		entrada >> listaFichas.fichas[i].izq >> listaFichas.fichas[i].der;
	}
}

void escribirJuego(const tJuego& juego) {//guardar partida
	string partida;
	ofstream archivo;
	cin >> partida;
	archivo.open(partida);
	archivo << juego.numJug << " " << juego.maxValor << endl << juego.tablero << endl;
	escribirListaFichas(archivo, juego.pozo);
	for (short int i = 0; i < juego.numJug; i++) {
		escribirListaFichas(archivo, juego.jugadores[i]);
		archivo << juego.puntosJug[i] << endl;
	}
	archivo.close();
}

void escribirListaFichas(ofstream& salida, const tListaFichas& listaFichas) {//escribe las listas de fichas de cada jugador y del pozo
	salida << listaFichas.cont << endl;
	for (short int i = 0; i < listaFichas.cont; i++) {
		salida << listaFichas.fichas[i].izq << " " << listaFichas.fichas[i].der << " ";
	}
	salida << endl;
}
