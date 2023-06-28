#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <Stepper.h>
#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
 #include <avr/power.h> // Required for 16 MHz Adafruit Trinket
#endif

//defines
#define MAXIMOPASSOS      4096
#define COLUNAS             16
#define LINHAS               2
#define QTDFITASLED          6
#define QTDMOTORES           6
#define QTDVISORLCD          3
#define QTDLEDSPORFITA       6

//Configuracao das Fitas de LED
Adafruit_NeoPixel fitaLED[QTDFITASLED] = {
  Adafruit_NeoPixel (6, 2, NEO_GRB + NEO_KHZ800),
  Adafruit_NeoPixel (6, 3, NEO_GRB + NEO_KHZ800),
  Adafruit_NeoPixel (6, 4, NEO_GRB + NEO_KHZ800),
  Adafruit_NeoPixel (6, 5, NEO_GRB + NEO_KHZ800),
  Adafruit_NeoPixel (6, 6, NEO_GRB + NEO_KHZ800),
  Adafruit_NeoPixel (6, 7, NEO_GRB + NEO_KHZ800) //LEDs/fita,porta,padrãoDeCor+freq
};

//Configuracao dos displays LCD
LiquidCrystal_I2C visorLCD[QTDVISORLCD]= {
  LiquidCrystal_I2C (0x25, COLUNAS, LINHAS),  //endereçoFísico,COLUNAS,LINHAS
  LiquidCrystal_I2C (0x23, COLUNAS, LINHAS),
  LiquidCrystal_I2C (0x24, COLUNAS, LINHAS)
};

//Configuração dos motores de passos - ok
Stepper motores[QTDMOTORES] = {
  Stepper (2048, 46, 48, 47, 49),
  Stepper (2048, 50, 52, 51, 53),
  Stepper (2048, 28, 30, 29, 31), 
  Stepper (2048, 32, 34, 33, 35),
  Stepper (2048, 36, 38, 37, 39),
  //adicionar a ultima linha de codigo
 };

// Global Var
int cores[QTDFITASLED][3] = {
  {0  , 0  , 255},   // Azul
  {255, 0  , 0  },   // Vermelho
  {0  , 255, 0  },   // Verde
  {255, 255, 0  }, // Amarelo
  {0  , 255, 255}, // Roxo
  {255, 0  , 255}  // Ciano
};

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  Serial.println();
  Serial.println("Iniciando...");

  // inicializar fitas de led
  for (int i = 0; i < QTDFITASLED; i++){
    fitaLED[i].begin();
    fitaLED[i].setBrightness(255);
    fitaLED[i].clear();
  }

  // inicializar motor de passo
  for (int i = 0; i < QTDMOTORES; i++){
    motores[i].setSpeed(15);
  }

  // inicializar vetor LCD
  for (int i = 0; i < QTDVISORLCD; i++){
      visorLCD[i].init();
      visorLCD[i].backlight();
      visorLCD[i].clear();
    }  


  // Temp Code
  movimentarMotores();
  //escreverNosLCDs();
  //ligarLEDCores();
  
}

void loop() {
  // verificar comunicacao serial
  if(Serial.available() > 0){
  
  
  
  }


}

void movimentarMotores(){
  Serial.println("---- Start Movimentacao dos Motores ----");

  double vetor[] = {1.2, 3.4, 5.6, 7.8, 4.1, 2.6}; //vetor deverá ser recebido via json
  int size = 6;// tamanho será fixo - 6
  
  double vetorEmPassos[size];

  preencheArrayDePassos(vetor, vetorEmPassos, size);
  levantarBarras(vetorEmPassos);
  zerarBarras(vetorEmPassos);
  
  Serial.println("---- End Movimentacao dos Motores ----");
}

void escreverNosLCDs(){
  Serial.println("---- Start Escrever nos LCDs ----");
  limparDisplays();

  String s1 = "Hello";
  String s2 = "World - ";

  mostrarDisplay(s1, s2);
  Serial.println("---- End Escrever nos LCDs ----");
}

void ligarLEDCores(){
  Serial.println("---- Start Pintar LEDs ----");

  String categorias[QTDFITASLED] = {"A", "B", "C", "D", "E", "F"};
  String categoriasUnicas[QTDFITASLED];

  removerDuplicatas(categorias, categoriasUnicas);

  for(int i=0; i < QTDFITASLED; i++) {
    // para cada fita de led, precisamos descobrir qual o index da categoria atual
    // categoriaatual = categorias[i]

    int indexCategoriaAtual = -1;

    for(int c = 0; c < QTDFITASLED; c++){
      if(categorias[i] == categoriasUnicas[c]){
        indexCategoriaAtual = c;
        break;
      }
    }

    if (indexCategoriaAtual < 0){
      Serial.println("Algum erro aconteceu ao encontrar o valor no vetor de categorias unicas");
      return;
    }

    // j < 6 sendo 6 a qtd de leds na fita
    for(int j = 0; j < QTDLEDSPORFITA; j++){    
      fitaLED[i].setPixelColor(j, 
        fitaLED[i].Color(
          cores[indexCategoriaAtual][0], //r
          cores[indexCategoriaAtual][1], //g
          cores[indexCategoriaAtual][2]) //b
        );
    }
    
    fitaLED[i].show();
  }

  Serial.println("---- End Pintar LEDs ----");
}

//region display_LCD - refatorar codigo dos displays lcd
void limparDisplays(){
  for (int i = 0; i < QTDVISORLCD; i++){
    visorLCD[i].clear();
  }
}

void mostrarDisplay(String primeiraLinha, String segundaLinha){ 
  for (int i = 0; i < QTDVISORLCD; i++){
    String segundaLinhaMod = segundaLinha + " " + (String) i;
    visorLCD[i].clear();
    visorLCD[i].backlight();

    mostrarDadoDisplay(0, 0, primeiraLinha, visorLCD[i]);
    mostrarDadoDisplay(0, 1, segundaLinhaMod, visorLCD[i]);            
  }  
}

void mostrarDadoDisplay(int coluna, int linha, String dado, LiquidCrystal_I2C lcd){
  //Serial.println((String)coluna + " " + (String)linha + " " + dado);
  lcd.backlight();
  lcd.setCursor(coluna, linha);
  lcd.print(dado);
}

//endregion metodosLCD

//region fitas_LED
void apagarFitas(){
  for(int i = 0; i < QTDFITASLED; i++) {
    fitaLED[i].clear();
    fitaLED[i].show();
  }
}

void apagarFitaByIndex(int index){
  fitaLED[index].clear();
  fitaLED[index].show();
}

void colorirFitaByIndex(int index, int r, int g, int b){
  fitaLED[index].clear();

  for(int j=0; j < QTDLEDSPORFITA; j++){
    fitaLED[index].setPixelColor(j, 
    fitaLED[index].Color(r, g, b)
    );
  }

  fitaLED[index].show();
}

void limparVetorCores(int* corFitas[QTDFITASLED][3]){
  for(int i = 0; i < QTDFITASLED; i++){ 
    for(int j = 0; j < 3; j++){ //3 = RGB
      corFitas[i][j] = 0;
    }
  }
}

//Remove valores duplicados do array de categorias, se houver
void removerDuplicatas(String categorias[], String vetorCategoriasUnicas[]) {
  int pos = 0;
  
  for (int i = 0; i < QTDFITASLED; i++) {
    bool duplicata = false;
    
    for (int j = 0; j < pos; j++) {
      if (categorias[i] == vetorCategoriasUnicas[j]) {
        duplicata = true;
        break;
      }
    }
    
    if (!duplicata) {
      vetorCategoriasUnicas[pos] = categorias[i];
      pos++;
      
      if (pos >= QTDFITASLED) {
        break; // Para evitar acessar além dos limites do vetorCategoriasUnicas
      }
    }
  }
}

//endregion fitas_LED

//region motores
void levantarBarras(double* vetorDePassos){  
  
  for (int i = 0; i < QTDMOTORES; i++){
    motores[i].step(vetorDePassos[i]);
    delay(50);
  }
}

void zerarBarras(double* vetorDePassos){  
  for (int i = 0; i < QTDMOTORES; i++){
    motores[i].step(-vetorDePassos[i]);
    delay(50);
  }
}
//endregion motores



//region utils
double valorParaPasso(double valor, double maximo){
  return ((MAXIMOPASSOS * valor)/(maximo));
}

void preencheArrayDePassos(double* vetorEntrada, double* vetorSaida, int tamanho) {
  double valorMaximo = findMax(vetorEntrada, tamanho);
  for (int i = 0; i < tamanho; i++) {
    vetorSaida[i] = valorParaPasso(vetorEntrada[i], valorMaximo);
  }
}


double findMax(double* vetorEntrada, int tamanho){
  double maximo = 0;

  for (int i=0; i < tamanho; i++){
    if (vetorEntrada[i] > maximo){
      maximo = vetorEntrada[i];
    }
  }
  return maximo;
}
//endregion utils

/*metodo misterioso dos displays


void mostrarDisplay(String vetorAux[], double vetorAux1[], LiquidCrystal_I2C visorLCDAux[]){  
  int pos = 0;
  for (int d = 0; d < QTDVISORLCD; d++){
      visorLCD[d].clear();
      visorLCD[d].backlight();
      
      mostrarDadoDisplay(0,0,vetorAux[pos],visorLCDAux[d]);
      mostrarDadoDisplay(0,1,(String)(vetorAux1[pos]),visorLCDAux[d]);      
      pos++;

      mostrarDadoDisplay(9,0,vetorAux[pos],visorLCDAux[d]);
      mostrarDadoDisplay(9,1,(String)(vetorAux1[pos]),visorLCDAux[d]);
      pos++;
  }  
}

void mostrarDisplay(String vetorAux[], String vetorAux1[]){  
  int pos = 0;
  for (int d = 0; d < QTDVISORLCD; d++){
      visorLCD[d].clear();
      visorLCD[d].backlight();

      mostrarDadoDisplay(0,0,vetorAux[pos],visorLCD[d]);
      mostrarDadoDisplay(0,1,(String)(vetorAux1[pos]),visorLCD[d]);      
      pos++;

      mostrarDadoDisplay(9,0,vetorAux[pos],visorLCD[d]);
      mostrarDadoDisplay(9,1,(String)(vetorAux1[pos]),visorLCD[d]);
      pos++;
  }  
}


*/
