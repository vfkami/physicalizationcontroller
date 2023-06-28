#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <Stepper.h>
#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
 #include <avr/power.h> // Required for 16 MHz Adafruit Trinket
#endif

//Configuração das fitas de LED
Adafruit_NeoPixel vetorFita[6] = {
Adafruit_NeoPixel (6, 1, NEO_GRB + NEO_KHZ800),
  Adafruit_NeoPixel (6, 2, NEO_GRB + NEO_KHZ800),
  Adafruit_NeoPixel (6, 3, NEO_GRB + NEO_KHZ800),
  Adafruit_NeoPixel (6, 4, NEO_GRB + NEO_KHZ800),
  Adafruit_NeoPixel (6, 5, NEO_GRB + NEO_KHZ800),
  Adafruit_NeoPixel (6, 6, NEO_GRB + NEO_KHZ800)
};
// Adafruit_NeoPixel (6, 8, NEO_GRB + NEO_KHZ800),
// Adafruit_NeoPixel (6, 9, NEO_GRB + NEO_KHZ800),
// Adafruit_NeoPixel (6, 10, NEO_GRB + NEO_KHZ800),
// Adafruit_NeoPixel (6, 11, NEO_GRB + NEO_KHZ800),
// Adafruit_NeoPixel (6, 12, NEO_GRB + NEO_KHZ800),
// Adafruit_NeoPixel (6, 13, NEO_GRB + NEO_KHZ800)
//Linhas e colunas dos displays 
#define colunas   16
#define linhas    2
LiquidCrystal_I2C vetorLcd[3]= {
LiquidCrystal_I2C (0x24, colunas, linhas),  //endereçoFísico,colunas,linhas
LiquidCrystal_I2C (0x23, colunas, linhas),
LiquidCrystal_I2C (0x25, colunas, linhas)};
//LiquidCrystal_I2C (0x25, colunas, linhas),
//LiquidCrystal_I2C (0x26, colunas, linhas),
//LiquidCrystal_I2C (0x27, colunas, linhas)
LiquidCrystal_I2C lcd(0x27, 20, 4);

//Configuração dos motores
Stepper motor1(2048, 40,42,41,43);  //passos/giro,portas
Stepper motor2(2048, 36,38,37,39);
Stepper motor3(2048, 32,34,33,35);
Stepper motor4(2048, 28,30,29,31);
Stepper motor5(2048, 24,26,25,27);
Stepper motor6(2048, 44,46,45,47); 
//Stepper motor7(2048, 46, 50, 48, 52);  
//Stepper motor8(2048, 53, 49, 51, 47);  
//Stepper motor9(2048, 57, 55, 56, 54);  
//Stepper motor10(2048, 61, 59, 60, 58); 
//Stepper motor11(2048, 65, 63, 64, 62); 
//Stepper motor12(2048, 69, 67, 68, 66); 
Stepper motores[6] = {motor1, motor2, motor3, motor4, motor5, motor6};

//Botões para interação
// int upButton = 8;
// int downButton = 9;
// int selectButton = 10;
// int menu = 1;
//Bases de dados
double baseBel[6] = {3, 6, 5, 2, 4, 1};
const int tam_vetor = (int)(sizeof(baseBel)/sizeof(baseBel[0]));
String loading = "Loading";
String b1 = "Database 1";
int cores[3][3] = {{255, 0, 0},{0, 255, 0},{0, 0, 255}};
int corBarra[6] = {1,2,3,4,5,6};
String dadosRotulos[12] = {"A","B","C","D","B","C"};
String categoriasBaseBel[6] = {"B","B","A","D","C","A"}; 
int vetorColor[6][3] = {{255,127,0},{255, 255, 0},{255, 0, 255},{0,255,255},{0,0,255},{0, 255, 0}};
int amarelo[6][3] = {{255, 255, 0},{255, 255, 0},{255, 255, 0},{255, 255, 0},{255, 255, 0},{255, 255, 0}};
int rosa[6][3] = {{255, 0, 255},{255, 0, 255},{255, 0, 255},{255, 0, 255},{255, 0, 255},{255, 0, 255}};
int laranja[6][3] = {{255,127,0},{255,127,0},{255,127,0},{255,127,0},{255,127,0},{255,127,0}};
int vermelho[6][3] = {{255,0,0},{255,0,0},{255,0,0},{255,0,0},{255,0,0},{255,0,0}};
int verde[6][3] = {{0,255,0},{0,255,0},{0,255,0},{0,255,0},{0,255,0},{0,255,0}};
int azul[6][3] = {{0,0,255},{0,0,255},{0,0,255},{0,0,255},{0,0,255},{0,0,255}};
int apagado[6][3] = {{0,0,0},{0,0,0},{0,0,0},{0,0,0},{0,0,0},{0,0,0}};
int vetorDadosCor[tam_vetor][3];
String vetorCategoriasUnicas[6];
double dados[tam_vetor] = {0, 0, 0, 0, 0, 0};
double passos[tam_vetor] = {0, 0, 0, 0, 0, 0};
double vetorDeZeros[tam_vetor] = {0, 0, 0, 0, 0, 0};
char caractere;

void setup() {
  Serial.begin(9600);
  #if defined(__AVR_ATtiny85__) && (F_CPU == 16000000)
  clock_prescale_set(clock_div_1);
  #endif
  
  for (int i = 0; i < (int) tam_vetor; i++){
    vetorFita[i].begin();
    vetorFita[i].setBrightness(25);
    vetorFita[i].clear();
  }  
  for (int i = 0; i < (int) tam_vetor; i++){
    motores[i].setSpeed(15);
  }
  for (int i = 0; i < (int) tam_vetor/2; i++){
    vetorLcd[i].init();
    vetorLcd[i].backlight();
    vetorLcd[i].clear();
  }
  lcd.init();
  lcd.backlight();
  lcd.clear();
  // pinMode(upButton, INPUT_PULLUP);
  // pinMode(downButton, INPUT_PULLUP);
  // pinMode(selectButton, INPUT_PULLUP);
  // updateMenu();
}

void loop() {  
  if(Serial.available() > 0){
    caractere = Serial.read();
      if (caractere == '1'){
        Serial.println("dados selecionados");
        if (!compararVetores(dados,baseBel)){
          apagarFitas(vetorFita); // ok
          limparDisplays(vetorLcd); // ok
          //zeraBarras(passos); // ok
          mostrarDisplay(loading, b1, vetorLcd);          
          preencherDados(baseBel);
          preencherPassos(dados);          
          //levantaBarras(passos);          
          criaVetorDadosCor(categoriasBaseBel);
          mostrarVetor(baseBel);
          mostrarVetorString(vetorCategoriasUnicas);
          for(int i=0; i < tam_vetor; i++) { //antes era tam_vetor
            for(int j=0; j<6; j++){    
              vetorFita[i].setPixelColor(j,vetorFita[i].Color(vetorColor[i][0], vetorColor[i][1], vetorColor[i][2]));
            }  
          vetorFita[i].show();
          }
        //  for(int i=0; i<5; i++) {
        //    mostrarDisplay(dadosRotulosCorBel, LegenCategoriasBaseBel, vetorLcd);
        //    delay(500);
        //    mostrarDisplay(dadosRotulos, baseBel, vetorLcd);
        //    delay(500);
        //  }         
        }                
      }                
      else if (caractere == 'z'){
        Serial.println("Zerando as barras");
        apagarFitas(vetorFita);
        limparDisplays(vetorLcd);
        zeraBarras(passos);
        preencherDados(vetorDeZeros);          
        preencherPassos(vetorDeZeros);        
        delay(50);                        
      }
  }
}

//ok
void apagarCor(int corAux, Adafruit_NeoPixel vetorAux[]){
  for (int i=0; i<tam_vetor; i++) {
    if (i!=corAux) {
      vetorAux[i].clear();
      vetorAux[i].show();
    }
  }
}

//ok
void apagarFitas(Adafruit_NeoPixel vetorAux[]){
  for(int i=0; i<tam_vetor; i++) {
    vetorAux[i].clear();
    vetorAux[i].show();
  }
}

//ok
void limparDisplays(LiquidCrystal_I2C vetorAux[]){
  for (int i = 0; i < (int) tam_vetor/2; i++){
    vetorAux[i].clear();
  }
}
void selecionarDadosUnicos(String vetorAux[]){
  int soma = 0;
  String aux[5] = {"", "", "", "", ""};
  int pos = 0;
  aux[0] = vetorAux[0];
  for(int i=0; i< tam_vetor; i++) {
    soma = 0;
    for(int j=0; j< 5; j++) {
      if(vetorAux[i] == aux[j]){
        soma = soma + 1;
      }
    }
    if(soma == 0){
      aux[pos] = vetorAux[i];
      pos = pos + 1;
    }
  }
  for(int j=0; j< 5; j++) {
    vetorCategoriasUnicas[j] = aux[j]; 
  }  
}
void criaVetorDadosCor(String vetorAux[]){  
selecionarDadosUnicos(vetorAux);  
limparVetorCores(vetorDadosCor);
for(int i=0; i< 5; i++) {
    for(int j=0; j< 12; j++) {
    if(vetorCategoriasUnicas[i] == vetorAux[j]){
        vetorDadosCor[j][0] =  vetorColor[i][0];        
        vetorDadosCor[j][1] =  vetorColor[i][1];        
        vetorDadosCor[j][2] =  vetorColor[i][2];        
    }
    }
}  
}

//ok
void limparVetorCores(int vetorAux[tam_vetor][3]){
  for(int i=0; i< tam_vetor; i++) {
    for(int j=0; j< 3; j++) {
      vetorAux[i][j] = 0;
    }  
  }
}
void mostrarDisplay(String vetorAux[], double vetorAux1[], LiquidCrystal_I2C vetorLcdAux[]){  
  int pos = 0;
  for (int d = 0; d < (int)tam_vetor/2; d++){
      vetorLcd[d].clear();
      vetorLcd[d].backlight();
      mostrarDadoDisplay(0,0,vetorAux[pos],vetorLcdAux[d]);
      mostrarDadoDisplay(0,1,(String)(vetorAux1[pos]),vetorLcdAux[d]);      
      pos=pos+1;
      mostrarDadoDisplay(9,0,vetorAux[pos],vetorLcdAux[d]);
      mostrarDadoDisplay(9,1,(String)(vetorAux1[pos]),vetorLcdAux[d]);
      pos=pos+1;
  }  
}

void mostrarDisplay(String vetorAux[], String vetorAux1[], LiquidCrystal_I2C vetorLcdAux[]){  
  int pos = 0;
  for (int d = 0; d < (int)tam_vetor/2; d++){
      vetorLcd[d].clear();
      vetorLcd[d].backlight();
      mostrarDadoDisplay(0,0,vetorAux[pos],vetorLcdAux[d]);
      mostrarDadoDisplay(0,1,(String)(vetorAux1[pos]),vetorLcdAux[d]);      
      pos=pos+1;
      mostrarDadoDisplay(9,0,vetorAux[pos],vetorLcdAux[d]);
      mostrarDadoDisplay(9,1,(String)(vetorAux1[pos]),vetorLcdAux[d]);
      pos=pos+1;
  }  
}

void mostrarDisplay(String aux, String aux1, LiquidCrystal_I2C vetorLcdAux[]){  
  for (int d = 0; d < (int)tam_vetor/2; d++){
      vetorLcd[d].clear();
      vetorLcd[d].backlight();
      mostrarDadoDisplay(0,0,aux,vetorLcdAux[d]);
      mostrarDadoDisplay(0,1,aux1,vetorLcdAux[d]);            
  }  
}

void mostrarDadoDisplay(int coluna, int linha, String dado, LiquidCrystal_I2C lcd){
  Serial.println((String)coluna + " " + (String)linha + " " + dado);
  lcd.backlight();
  lcd.setCursor(coluna, linha);
  lcd.print(dado);
}


// -- ver onde é usado
void mostrarDisplayInte(int coluna,int linha,String msg){  
  lcd.setCursor(coluna,linha);
  lcd.print(msg);
}

//ok 
boolean compararVetores(double vetorAux1[], double vetorAux2[]){
  int soma = 0;
  for (int i = 0; i < tam_vetor; i++){
    if (vetorAux1[i] == vetorAux2[i]){
      soma = soma + 1;      
    }    
  }
  if (soma == tam_vetor){
      return true;      
  }else{
    return false;
  }
}
//usado para preencher a variavel global dados[]
void preencherDados(double vetorAux[]){
  for (int i = 0; i < tam_vetor; i++){
    dados[i] = vetorAux[i];
    delay(50);
  }
}
// usado para preencher o vetor global de passos 
void preencherPassos(double vetorAux[]){
  double valorMaxi = valorMaximo(vetorAux);
  for (int i = 0; i < tam_vetor; i++){
    passos[i] = convertValorPasso(vetorAux[i], valorMaxi);
    delay(50);
  }
}

//ok
void zeraBarras(double passosAux[]){  
  for (int i = 0; i < tam_vetor; i++){
    if(i == tam_vetor - 1){
      motores[i].step(+passosAux[i]);
      continue;
    }
    motores[i].step(-passosAux[i]);
    delay(50);
  }
}

//ok
void levantaBarras(double passosAux[]){  
  for (int i = 0; i < tam_vetor; i++){
    if(i == tam_vetor - 1){
      motores[i].step(-passosAux[i]);
      continue;
    }
    motores[i].step(passosAux[i]);
    delay(50);
  }
}

//ok
double convertValorPasso(double aux, double maxiAux){
  double resultado = (double)((4096 * aux)/(maxiAux));
  return resultado;
}

//ok
double valorMaximo(double vetor[]){
   double maior = vetor[0];
   for (int i = 0; i < tam_vetor; i++) {
    if (vetor[i] > maior) {
        maior = vetor[i];
    } 
  }
  delay(50);
  return maior;
}
//ok
double valorMinimo(double vetor[]){
   double menor = vetor[0];
   for (int i = 0; i < tam_vetor; i++) {
    if (vetor[i] < menor) {
        menor = vetor[i];
    } 
  }
  delay(50);
  return menor;
}

//ok se necessario criar um metodo mais adequado
void mostrarVetor(double vetor[]){ 
  for (int i = 0; i < tam_vetor; i++){
     Serial.print(String(vetor[i])+ " ");
    delay(50);  
  }  
   Serial.println("");
}
//duplicado
void mostrarVetorString(String vetor[]){ 
  for (int i = 0; i < tam_vetor; i++){
     Serial.print(String(vetor[i])+ " ");
    delay(50);  
  }  
   Serial.println("");
}