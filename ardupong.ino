/*
      programmato da Claudio Belato per il canale YouTube Games'n Goblins
      visita il canale a questo indirizzo https://www.youtube.com/c/GamesnGoblinsReal/
*/

#include <Arduboy2.h>

Arduboy2 arduboy;
int puntiGiocatoreA = 0;
int puntiGiocatoreB = 0;
int direzionePallinaX = 1;
int direzionePallinaY = 1;
int allaBattuta = 1;
int pallinaX = 7;
int pallinaY = 35;
int posizioneRacchettaA = 30;
int posizioneRacchettaB = 30;
int faseGioco = 0;


constexpr uint8_t logo[] PROGMEM
{
  16, 16,
  0x00, 0xF8, 0x04, 0x74, 0x04, 0x04, 0x04, 0xC4, 0xC4, 0x05, 0x06, 0x05, 0xC5, 0x04, 0xF8, 0x00,
  0x00, 0x9F, 0xE0, 0x48, 0x50, 0x50, 0x50, 0x50, 0x50, 0x50, 0x50, 0x50, 0x49, 0xE0, 0x9F, 0x00,
};

//funzione di arduboy per l'eventuale inizializzazione di variabili etc.
void setup() {
  arduboy.begin();
}

//funzione du arduboy, ad ogni ciclo esegue quanto è contenuto al suo interno (logica del gioco)
void loop() {
  if (!(arduboy.nextFrame()))
    return;

  //funzioni di arduboy per inizializzare lo schermo e leggere gli input
  arduboy.clear();
  arduboy.pollButtons();


  //se sono nella schermata di titolo
  if (faseGioco == 0) {
    arduboy.setCursor(50, 5);
    arduboy.print("PONG");
    //funzione di arduboy per stampare gli sprite (usato qui per il logo)
    Sprites::drawOverwrite(55, 25, logo, 0);
    arduboy.setCursor(6, 50);
    arduboy.print("PREMI B PER GIOCARE");

    //se premo il pulsante B passo alla schermata di gioco
    if ((arduboy.justPressed(B_BUTTON))) {
      faseGioco++;
    }

  }
  //se sono in gioco
  else {

    //setto la posizione del cursore e stampo il punteggio
    arduboy.setCursor(10, 0);
    arduboy.print(puntiGiocatoreA);

    arduboy.setCursor(110, 0);
    arduboy.print(puntiGiocatoreB);


    //disegno il campo
    arduboy.drawRect(0, 10, 128, 54);
    arduboy.drawLine(64, 10, 64, 64);

    //disegno la racchetta del giocatore A
    arduboy.drawLine(5, posizioneRacchettaA, 5, posizioneRacchettaA + 10);

    //disegno la racchetta del giocatore B
    arduboy.drawLine(122, posizioneRacchettaB, 122, posizioneRacchettaB + 10);

    //disegna la pallina
    arduboy.fillRect(pallinaX, pallinaY, 2, 2);

    //se uno dei due giocatori arriva a 5 punti la partita si conclude
    if (puntiGiocatoreA == 5 || puntiGiocatoreB == 5) {
      arduboy.setCursor(35, 0);
      if (puntiGiocatoreA == 5) {
        arduboy.print("HAI VINTO!");
      }
      else {
        arduboy.print("HAI PERSO!");
      }
      //se premo il pulsante B reinizializzo le variabili e ricomincio la partita
      if ((arduboy.justPressed(B_BUTTON))) {
        allaBattuta = 0;
        puntiGiocatoreA = 0;
        puntiGiocatoreB = 0;
        posizioneRacchettaA = 30;
        posizioneRacchettaB = 30;
        pallinaX = 7;
        pallinaY = 35;
      }
    }
    //se nessuno dei due giocatori ha fatto ancora 5 punti
    else {

      //se premo la croce direzionale SU o GIU' muovo la mia racchetta
      if (arduboy.pressed(DOWN_BUTTON) && posizioneRacchettaA < 52) {
        posizioneRacchettaA++;
      }
      else if (arduboy.pressed(UP_BUTTON) && posizioneRacchettaA > 11) {
        posizioneRacchettaA--;
      }

      //logica per muovere la racchetta dell'avversario (la CPU)
      if (direzionePallinaX && pallinaX > 95) {
        if (posizioneRacchettaB < pallinaY - 5) {
          if (posizioneRacchettaB < 52) {
            posizioneRacchettaB++;
          }
        }
        else {
          if (posizioneRacchettaB > 11) {
            posizioneRacchettaB--;
          }
        }
      }

      //lancio la pallina
      if ((arduboy.justPressed(B_BUTTON) && allaBattuta == 1) || allaBattuta == 2) {
        allaBattuta = 0;
      }

      //la pallina segue la racchetta quando il giocatore è al servizio
      if (allaBattuta) {
        if (direzionePallinaX) {
          pallinaY = posizioneRacchettaA + 5;
        }
        else {
          pallinaY = posizioneRacchettaB + 5;
        }

      }
      //se la pallina è in movimento
      else {
        //se la pallina tocca il bordo superiore o inferiore, cambia direzione Y
        if (pallinaY > 60 || pallinaY < 12) {
          direzionePallinaY = (direzionePallinaY + 1) % 2;
        }

        //creao le hitbox della pallina e delle due racchette
        Rect hitBoxPallina;
        hitBoxPallina.x = pallinaX;
        hitBoxPallina.y = pallinaY;
        hitBoxPallina.width = 2;
        hitBoxPallina.height = 2;

        Rect hitBoxRacchettaA;
        hitBoxRacchettaA.x = 5;
        hitBoxRacchettaA.y = posizioneRacchettaA;
        hitBoxRacchettaA.width = 1;
        hitBoxRacchettaA.height = 10;

        Rect hitBoxRacchettaB;
        hitBoxRacchettaB.x = 123;
        hitBoxRacchettaB.y = posizioneRacchettaB;
        hitBoxRacchettaB.width = 1;
        hitBoxRacchettaB.height = 10;


        //se la pallina tocca una delle racchette cambia direzione X
        if (arduboy.collide(hitBoxPallina, hitBoxRacchettaA) || arduboy.collide(hitBoxPallina, hitBoxRacchettaB)) {
          direzionePallinaX = (direzionePallinaX + 1) % 2;
        }

        if (direzionePallinaX) {
          pallinaX++; //verso destra
        }
        else {
          pallinaX--; //verso sinistra
        }
        if (direzionePallinaY) {
          pallinaY++; //in basso
        }
        else {
          pallinaY--; //in alto
        }

        //se la pallina va in una delle reti
        if (pallinaX < 1) {
          puntiGiocatoreB++; //assegna un punto
          allaBattuta = 2; //manda il giocatore in battuta
          pallinaX = 120; //posiziona la pallina
          pallinaY = posizioneRacchettaB + 5;

        }
        else if (pallinaX > 128) {
          puntiGiocatoreA++;
          allaBattuta = 1;
          pallinaX = 7;
        }
      }
    }
  }
  arduboy.display();
}
