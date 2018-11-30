#include <stdio.h>
#include <string.h>

#define KOKO 18

void valikko(char tilinumero[]);
void rahanNosto(char tilinumero[]);
void saldonTarkistus(char tilinumero[]);
void rahanTalletus(char tilinumero[]);
void lueRoskat(void);
int lueKokonaisluku(void);


/* Funktio main kuvaus
    Funktio ei saa parametrej�.
    Funktio palauttaa arvon funktion toiminnan mukaan.
    Ensin kysyt��n k�ytt�j�lt� tilinumero, johon lis�-
    t��n loppuun tiedostotyyppi. Kaikki tarvittavat sy�t-
    teentarkastukset tehd��n my�s. Jos tili� ei l�ydy, antaa
    ohjelma virheen. Seuraavaksi kysyt��n pin-koodia. Jos pin
    koodin sy�tt�� 3 kertaa v��rin, loppuu ohjelman suoritus.
    Oikean pin-koodin sy�tetty� siirryt��n Valikko-funktioon.

*/
int main(){

    FILE *tiedLuku;
    char tilinumero[KOKO];
    char syottoPin[KOKO], tarkistusPin[KOKO];
    int pinVaarin = 3;
    int pinOikein = 0;

    printf("Tervetuloa kayttamaan pankkiautomaattia!\nSyota tilinumerosi:");
    fgets(tilinumero, KOKO, stdin);
    if( tilinumero[ strlen(tilinumero) -1] == '\n'){
        tilinumero[ strlen(tilinumero) -1] = '\0';
    }else{
        lueRoskat();
    }
    strcat(tilinumero, ".tili");

    tiedLuku = fopen(tilinumero, "r");
    if (tiedLuku == NULL){
        printf("Tilinumerolla ei loytynyt tilia, tai tilin avaus epaonnistui.\n");
        return 0;
    }
    fgets(tarkistusPin, KOKO, tiedLuku);
    while(pinVaarin > 0 && pinOikein == 0){
        printf("Syota tilin PIN-koodi:");
        fgets(syottoPin, KOKO, stdin);
        if(strcmp(syottoPin, tarkistusPin) == 0){
            printf("PIN oikein!\n");
            fclose(tiedLuku);
            valikko(tilinumero);
            pinOikein = 1;
        }else{
            pinVaarin--;
            printf("PIN vaarin, %d yritysta jaljella\n", pinVaarin);
        }
    }

    printf("Kiitos kaynnista!");
    return 0;
}


/*  Funktio valikko kuvaus
    Funktio saa parametrin� tilinumeron jonka k�ytt�j� alun perin sy�tti.
    Funktio ei palauta mit��n.
    Funktion p��tarkoitus on toimia hubina muille funktioille, k�ytt�j�n
    sy�tteen perusteella menn��n tiettyyn funktioon. Ainoastaan sy�tt�m�ll�
    0 p��see funktiosta pois.
*/
void valikko(char tilinumero[]){

    int valinta = 0;
    do{
        printf("Tee valinta.\n1 = Rahan nosto\n2 = Saldon tarkistus\n3 = Rahan talletus\n0 = Lopetus\n");
        valinta = lueKokonaisluku();
        switch (valinta){
            case 1:
                rahanNosto(tilinumero);
                break;
            case 2:
                saldonTarkistus(tilinumero);
                break;
            case 3:
                rahanTalletus(tilinumero);
                break;
            case 0:
                break;
            default:
                break;
        }
    }while(valinta != 0);
}


/* Funktio rahanNosto kuvaus
    Funktio saa parametrin� tilinumeron jonka k�ytt�j� alun perin sy�tti.
    Funktio ei palauta mit��n.
    Funktiossa ensin avataan tilinumeron mukainen tiedosto ja otetaan pin
    koodi ja tilin saldo talteen. Sen j�lkeen k�ytt�j�n sy�tteen perusteella
    algoritmi laskee annettavien setelien m��r�n, avaa tilin kirjoitusta
    varten ja tallettaa sinne pin-koodin sek� uuden tilin saldon. Lopuksi
    kysyt��n viel� halutaanko tarkistaa tilin saldo.
*/
void rahanNosto(char tilinumero[]){

    FILE *tiedLuku;
    FILE *tiedKirjoitus;
    int pin, tilinSaldo;
    int nostonMaara = 0;
    int nostoValinta = 0;
    int seteli20 = 0;
    int seteli50 = 0;

    tiedLuku = fopen(tilinumero, "r");
    fscanf(tiedLuku, "%d", &pin);
    fscanf(tiedLuku, "%d", &tilinSaldo);
    fclose(tiedLuku);

    printf("Syota nostettava maara:");
    nostonMaara = lueKokonaisluku();
    if(nostonMaara >= 20 && nostonMaara %10 == 0 && nostonMaara != 30 && nostonMaara <= tilinSaldo){
        tilinSaldo = tilinSaldo - nostonMaara;
        tiedKirjoitus = fopen(tilinumero, "w");
        fprintf(tiedKirjoitus, "%d\n", pin);
        fprintf(tiedKirjoitus, "%d", tilinSaldo);
        fclose(tiedKirjoitus);
        do{
            if(nostonMaara == 20 || nostonMaara == 40 || nostonMaara == 60 || nostonMaara == 80){
                seteli20++;
                nostonMaara = nostonMaara - 20;
            }else{
                seteli50++;
                nostonMaara = nostonMaara - 50;
            }
        }while(nostonMaara > 0);

        printf("Saat %dkpl 50 setelia ja %dkpl 20 setelia\n", seteli50, seteli20);
        printf("Haluatko tarkistaa tilin saldon? Tee valinta.\n1 = Kylla\n2 = Ei\n");
        nostoValinta = lueKokonaisluku();
        if(nostoValinta == 1){
            saldonTarkistus(tilinumero);
        }
    }else if(tilinSaldo < nostonMaara){
        printf("Tililla ei tarpeeksi rahaa.\n");
    }else{
        printf("Virheellinen summa syotetty.\n");
    }
}


/* Funktio saldonTarkistus kuvaus
    Funktio saa parametrin� tilinumeron jonka k�ytt�j� alun perin sy�tti.
    Funktio ei palauta mit��n.
    Funktiossa ensin avataan tilinumeron mukainen tiedosto ja otetaan pin
    koodi ja tilin saldo talteen. T�m�n j�lkeen tulostetaan tilin saldo
    k�ytt�j�lle joko n�yt�lle tai simuloidaan saldon tulostusta kuitille.

*/
void saldonTarkistus(char tilinumero[]){

    FILE *tiedLuku;
    char roska[KOKO];
    int saldoValinta, tilinSaldo = 0;

    tiedLuku = fopen(tilinumero, "r");
    fgets(roska, KOKO, tiedLuku);
    fscanf(tiedLuku, "%d", &tilinSaldo);
    fclose(tiedLuku);

    printf("Haluatko tilin saldon naytolle tai kuitille? Tee valinta.\n1 = Naytolle\n2 = Kuitille\n0 = Peruutus\n");
    saldoValinta = lueKokonaisluku();
        switch(saldoValinta){
        case 1:
            printf("Tililla rahaa %d\n", tilinSaldo);
            break;
        case 2:
            printf("\n*****************\n\n\nTilin saldo: %d\n\n\n\n*****************\n", tilinSaldo);
            break;
        case 0:
            break;
        default:
            break;
    }
}


/* Funktio rahanTalletus kuvaus
    Funktio saa parametrin� tilinumeron jonka k�ytt�j� alun perin sy�tti.
    Funktio ei palauta mit��n.
    Funktiossa ensin avataan tilinumeron mukainen tiedosto ja sielt� otetaan
    talteen pinkoodi ja sen hetkinen tilin saldo. Funktio sen j�lkeen simuloi
    rahantalletusta pyyt�m�ll� talletuksen m��r�� k�ytt�j�lt�. T�m�n j�lkeen
    avataan aiemmin avattu tiedosto ja tallennetaan sinne pinkoodi ja uusi
    tilin saldo. Lopuksi kysyt��n viel� haluaako k�ytt�j� tarkistaa viel� tilin
    saldon.
*/
void rahanTalletus(char tilinumero[]){

    FILE *tiedLuku;
    FILE *tiedKirjoitus;

    int talletusValinta;
    int talletusMaara = 0;
    int pin, tilinSaldo;

    tiedLuku = fopen(tilinumero, "r");
    fscanf(tiedLuku, "%d", &pin);
    fscanf(tiedLuku, "%d", &tilinSaldo);
    fclose(tiedLuku);

    do{
        printf("Syota rahat automaattiin:");
        talletusMaara = lueKokonaisluku();
        if(talletusMaara <= 0){
            printf("Et syottanyt rahaa automaattiin. Yrita uudestaan.\n");
        }
    }while(talletusMaara <= 0);

    tilinSaldo = talletusMaara + tilinSaldo;
    tiedKirjoitus = fopen(tilinumero, "w");
    fprintf(tiedKirjoitus, "%d\n", pin);
    fprintf(tiedKirjoitus, "%d", tilinSaldo);
    fclose(tiedKirjoitus);

    printf("Talletit %d\n", talletusMaara);
    printf("Haluatko tarkistaa tilin saldon? Tee valinta.\n1 = Kylla\n2 = Ei\n");
    talletusValinta = lueKokonaisluku();

    if(talletusValinta == 1){
        saldonTarkistus(tilinumero);
    }
}


/* Funktio lueRoskat kuvaus
    Funktio ei saa parametrej� eik� se palauta mit��n.
    Funktio tarkistaa while lauseella onko sy�teess� jotain ylim��r�ist�.

    Funktio on lainattu luentoesimerkeist�.
*/
void lueRoskat(void){

   while( fgetc(stdin) != '\n');
}


/* Funktio lueKokonaisluku kuvaus
    Funktion teht�v� on varmistaa ett� k�ytt�j� sy�tt�� n�pp�imist�lt� kokonaisluvun.
    Jos sy�tetty luku ei ole kokonaisluku, kysyy funktio sy�tett� uudelleen.
    Funktio palauttaa sy�tetyn luvun (int).
    Funktiolle ei annetta parametrej�, varsinaisen luvun kysyminen
    tehd��n itse kutsuvassa funktiossa.

    Funktio on lainattu luentoesimerkeist�.

*/
int lueKokonaisluku(void){

   int luku;
   char mki;
   int status;

   while((status = scanf("%d%c", &luku, &mki))==0  || (2 == status && mki != '\n') ){
      lueRoskat();
      printf("Et syottanyt kokonaislukua > ");
   }
   return luku;
}
