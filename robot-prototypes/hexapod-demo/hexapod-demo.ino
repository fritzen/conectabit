//Adaptado para o portugues - br
//Este código é para controlar um robô que se move através de oscilações em seus servomotores.
//Direitos autorais
//https://github.com/Obijuan/ArduSnake


#include <Servo.h>
#include <Oscillator.h>



//-- Declare the oscillators
Oscillator osc_middle, osc_right, osc_left;

//-- Parâmetros globais para os osciladores
unsigned int A=40; // Amplitude (maior -> passos mais longos) ajuste 10-40
unsigned int T=5000; // Período (menor -> movimentos mais rápidos)
void(* resetFunc) (void) = 0;//declara a função de reset no endereço 0

void setup()
{
delay(1000); // Pequeno atraso de inicialização, substituir por pressionar o botão de início (se presente)

//-- Anexar os osciladores aos servos
osc_middle.attach(2); // 2,3 e 4 são os pinos digitais
osc_right.attach(3);
osc_left.attach(4);

//-- Definir os parâmetros
osc_middle.SetO(-15); // Correção para o desvio dos servos
osc_right.SetO(-20);
osc_left.SetO(25);

osc_middle.SetA(35); // Motor do meio precisa de uma amplitude pequena (5-10)
osc_right.SetA(A);
osc_left.SetA(A);

osc_middle.SetT(T); // Definir o período de trabalho
osc_right.SetT(T);
osc_left.SetT(T);

//-- Atualizar os osciladores
osc_middle.refresh();
osc_right.refresh();
osc_left.refresh();

//-- Definir a diferença de fase
//-- Isso define o tipo de movimento que o robô faz
osc_middle.SetPh(DEG2RAD( 90 ));
osc_left.SetPh( DEG2RAD( 0 ));
osc_right.SetPh( DEG2RAD( 0 )); // Por exemplo, colocando 180 aqui fará o robô girar no sentido horário

}

void loop()
{
unsigned long time;
time = millis(); // Usamos isso para determinar a duração dos movimentos

if(time < 10000) { // Primeiro o robô vai para frente com uma aceleração
T = map(time,1000,10000,5000,1000);
osc_middle.SetT(T); // Definir o período de trabalho (cada vez menor, de T=5000 para T=1000)
osc_right.SetT(T);
osc_left.SetT(T);

} else if(time < 15000) { // Em seguida, o robô girará no sentido horário
osc_right.SetPh( DEG2RAD( 180 ));
osc_left.SetPh( DEG2RAD( 0 ));

} else if(time < 20000) { // Para frente
osc_right.SetPh( DEG2RAD( 0 ));
osc_left.SetPh( DEG2RAD( 0 ));

} else if(time < 25000) { // Sentido anti-horário
osc_right.SetPh( DEG2RAD( 0 ));
osc_left.SetPh( DEG2RAD( 180 ));

} else if(time < 30000) { // Para trás
osc_right.SetPh( DEG2RAD( 180 ));
osc_left.SetPh( DEG2RAD( 180 ));


 } else if (time < 35000) { // Sentido anti-horário
osc_right.SetPh(DEG2RAD(0));
osc_left.SetPh(DEG2RAD(180));

} else if (time < 40000) { // Posição de repouso
osc_right.SetPh(DEG2RAD(90));
osc_left.SetPh(DEG2RAD(90));

} else if (time < 45000) { // Para frente
osc_right.SetPh(DEG2RAD(0));
osc_left.SetPh(DEG2RAD(0));

} else if (time < 55000) { // Desacelerando lentamente
T = map(time, 45000, 55000, 1000, 10000);
osc_middle.SetT(T); // Define o período de trabalho (cada vez maior, de T=1000 a T=10000)
osc_right.SetT(T);
osc_left.SetT(T);

} else { // Até parar, podemos parar o programa
resetFunc(); // Chama o reset
}

//-- Atualiza os osciladores
osc_middle.refresh();
osc_right.refresh();
osc_left.refresh();
}