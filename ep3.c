/* ************************************************************************** */
/* Nome: Giovana Oshiro da Silva                                              */
/* Numero USP: 8022103                                                        */
/* Exercício-programa 3                                                       */
/* ************************************************************************** */

#include <stdio.h>
#include <math.h>
#define PI 3.141592
#define PART 6000
#define MAX 30000

/*Função que gera números aleatórios a partir do método da congruência linear.
São gerados n números aleatórios. Esses números são armazenados em um vetor e o
último número aleatório gerado (antes de ser normalizado) é retornado para ser
utilizado como a nova semente quando a função for chamada novamente.*/
/*Os parâmetros utilizados foram a = 22695477 e b = 1, como no EP1, e m como 2^30,
e não a 32, uma vez que 2^32 estoura o valor de int (e conforme nos foi recomen-
dado a fazer o EP1).*/

int congLinear (int x, float v [MAX], int n){
  long int a = 22695477, m = 1073741824;
  int b = 1, i, xn = 0;
  float num = 0.0;
  for (i = 0; i < n; i++){
    x = (a * x + b) % m;
    num = ((float)x)/m;
    v [i] = num;
  }
  xn = x;
  return xn;
}

/*Função que calcula a gaussiana, dado a média, o desvio padrao e um valor de x,
retorna o valor da gaussiana*/

float gaussiana (float media, float desvio_padrao, float x){
  float aux = 0.0, exponencial = 0.0, fx = 0.0;
  aux = 1 / (desvio_padrao * (sqrt (2 * PI)));
  exponencial = exp ((-0.5 * (x - media) * (x - media)) / (desvio_padrao * desvio_padrao));
  fx = aux * exponencial;
  return fx;
}

/*A função abaixo calcula a área da gaussiana através da soma de riemman, isto é
ela calcula a integral da função gaussiana. A integral é calculada de -3sigma a
+3sigma. A base de cada retângulo (dx) é definida como a amplitude da gaussiana
no intervalo considerado dividido por PART = 6000. A altura do retângulo é dada
pelo valor de f(x) no ponto; para isso, a função gaussiana é chamada. A cada
partição, a área é calculada e armazenada em um vetor y_integral. O valor de x
correspondente a esse determinado f(x) é armazenado em outro vetor, x_integral.*/

void somaderiemman (float y_integral [PART], float x_integral [PART], float media, float desvio_padrao){
    float x, dx, altura = 0.0, soma = 0.0;
    int i = 0;
		x = media - 3 * desvio_padrao;
		dx = (6 * desvio_padrao)/6000;
		for (i = 0; i < 6000; i++){
			altura = gaussiana (media, desvio_padrao, x);
			soma = soma + (dx * altura);
			y_integral [i] = soma;
			x_integral [i] = x;
      x = x + dx;
		}
}

/*A função buscabinaria vai percorrer o vetor y_integral e procurar o valor que
corresponde ou se aproxima do número aleatório gerado pelo método das congruências
lineares. O valor de x correspondente a esse valor da integral é, então, armazenado
e formará o conjunto pedido no enunciado. A função é uma busca binária simples,
que, no caso de não encontrar o valor exato, pega a casa mais próxima pela direita
e a casa mais próxima pela esquerda e tira a média aritmética entre os dois valores.*/

float buscabinaria (float y_integral [PART], float x_integral [PART], int n, float x){
	float numaleatorio = 0.0;
	int e, m, d, achei = 0;
  e = 0;
	d = n-1;
  while (e <= d && !achei) {
    m = (e + d)/2;
    if (y_integral [m] == x){
			achei = 1;
			numaleatorio = x_integral [m];
		}
    if (y_integral [m] < x) e = m + 1;
    else d = m - 1;
  }
	if (!achei){
		numaleatorio = (x_integral [e] + x_integral [d])/2;
	}
	return numaleatorio;
}

/*A função abaixo calcula a média aritmética entre os valores de um vetor qualquer*/

float media_aritmetica (float v [MAX], int n){
  float media = 0.0, total = 0.0;
  int i;
  for (i = 0; i < n; i++){
    total = total + v [i];
  }
  media = total/n;
  return media;
}

/*Função calcula o coeficiente de Pearson, dado um conjunto X, um Y (armazenados
em vetores) e o tamanho dos vetores, que é o mesmo*/

float coef_pearson (float X [MAX], float Y [MAX], int n){
  float mediaX = 0.0, mediaY = 0.0, num = 0.0, dem1 = 0.0, dem2 = 0.0, coef = 0.0;
  int i;
  mediaX = media_aritmetica (X, n);
  mediaY = media_aritmetica (Y, n);
  for (i = 0; i < n; i++){
    num = num + (X [i] - mediaX) * (Y [i] - mediaY);
    dem1 = dem1 + (X [i] - mediaX) * (X [i] - mediaX);
    dem2 = dem2 + (Y [i] - mediaY) * (Y [i] - mediaY);
  }
  coef = num/(sqrt (dem1 * dem2));
  return coef;
}

/*A função ordena um vetor através do método insertion sort*/

void insertionsort (float v [MAX], int n){
  int i, j;
  float aux = 0.0;
  for (i = 0; i < n; i++){
    for (j = i; j > 0 && v [j] < v [j-1]; j--){
      aux = v [j];
      v [j] = v [j-1];
      v [j-1] = aux;
    }
  }
}

int main (){
  int seed, n, N, i, j, xn1 = 0, xn2 = 0, quartil1 = 0, quartil2 = 0, quartil3 = 0;
  float media, desvio_padrao, media0 = 0.0, desvio_padrao1 = 1.0, k;
  float y_integral1 [PART], x_integral1 [PART], y_integral2 [PART], x_integral2 [PART];
  float numAleatorios_x [MAX], numAleatorios_eps [MAX], aux1 = 0.0, aux2 = 0.0;
  float X [MAX], Y [MAX], x = 0.0, epsilon = 0.0, coeficiente [MAX];

  printf ("Entre com uma seed:\n");
  scanf ("%d", &seed);
  printf ("\n");
  printf ("Entre com a média e o desvio padrão da gaussiana:\n");
  scanf ("%f %f", &media, &desvio_padrao);
  printf ("\n");
  printf ("Entre com dois números inteiros n e N:\n");
  scanf ("%d %d", &n, &N);
  printf ("\n");
  printf ("Entre com um número real pertecente ao intervalo [-1, 1]:\n");
  scanf ("%f", &k);
  printf ("\n");

  /*A função que calcula a integral é chamada. O vetor y_integral1 contém os
  valores de f(x) da integral da gaussiana com média e desvio padrão interativos;
  o vetor x_integral1 contém os valores de x correspondentes a f(x). O vetor
  y_integral2 contém os valores da gaussiana com média 0 e desvio padrão 1; o
  x_integral2 possui os valores de x correspondentes a f(x) da integral da gaussiana
  com média 0 e dsvio 1.*/

  somaderiemman (y_integral1, x_integral1, media, desvio_padrao);

  somaderiemman (y_integral2, x_integral2, media0, desvio_padrao1);

  /*A função que gera números aleatórios pelo método das congruências lineares
  é chamada. Vão ser gerados dois vetores com n números aleatórios distintos,
  numAleatorios_x e numAleatorios_eps.*/
  /*Duas variáveis auxiliares recebem os números aleatórios correspondentes à
  casa j dos vetores numAleatorios_x e numAleatorios_eps. A função buscabinaria
  vai procurar no vetor y_integral1 e y_integral2 os números aleatórios gerados
  e devolve os valores de x correspondentes ao valor da integral naquele ponto.*/
  /*Os vetores X e Y recebem o valor de x correspondente ao f(x) da integral
  encontrado. São gerados, portanto, 2n números aleatórios pelo método da função
  inversa a cada interação de N; n números (x) são armazenados em X e n números
  (epsilon) são armazenados em Y na forma k * x + epsilon.*/
  /*A cada interação de N, depois que os vetores X e Y estão preenchidos, o
  coeficiente de correlação de Pearson é calculado e armazenado no vetor
  coeficiente.*/

  for (i = 0; i < N; i++){
    xn1 = congLinear (seed, numAleatorios_x, n);
    xn2 = congLinear (xn1, numAleatorios_eps, n);
    for (j = 0; j < n; j++){
      aux1 = numAleatorios_x [j];
      x = buscabinaria (y_integral1, x_integral1, PART, aux1);
      X [j] = x;
      aux2 = numAleatorios_eps [j];
      epsilon = buscabinaria (y_integral2, x_integral2, PART, aux2);
      Y [j] = k * x + epsilon;
    }
    coeficiente [i] = coef_pearson (X, Y, n);
    seed = xn2;
  }

  /*A função insertionsort é chamada para ordenar o vetor com os coeficientes de
  correlação. O vetor coeficiente [MAX] possui N casas.*/

  insertionsort (coeficiente, N);

  /*O primeiro, segundo e terceiro quartis do vetor coeficiente são armazenados
  em variáveis.*/

  quartil1 = N/4;

  quartil2 = (2*N)/4;

  quartil3 = (3*N)/4;

  printf ("Primeiro quartil: %f\nSegundo quartil: %f\nTerceiro quartil: %f\n", coeficiente [quartil1], coeficiente [quartil2], coeficiente [quartil3]);

  return 0;
}
