# Lab3Problema1
FAZ O SAMPLEY DE GUITARRA

*Efeitos de Audio para Guitarra Elétrica*


Este trabalho tem por objetivo apresentar alguns dos principais efeitos de
áudio presentes no mercado, será abordada a teoria por trás do processamento do
sinal dentro de cada efeito, bem como a estruturação e uma implementação na linguagem
Matlab e do DSP TMS320C5502 de cada um dos efeitos apresentados.


*Flanger*

O efeito de flanger, assim como o os efeitos de vibrato e chorus, apresenta as suas
principais variações no domínio da frequência. Entretanto, por apresentar um resultado
com mais ruído e variações do que os efeitos descritos anteriormente, a diferenca principal entre
os dois e a variacao do atraso que, no caso do  flanger e entre 1 e 10 ms. O efeito se originou nas 
gravacoes feitas com fita, onde duas fitas eram tocadas em sincronia. O flanger acontecia quando cada 
uma delas era forcadamente atrasada, atraves de uma pressao fisica nas fitas. Perceptualmente, a audicao
humana e que diferencia os dois efeitos, e nao os seus algoritmos.

Utilizando um delay variavel entre 1 e 10ms , produz-se a diferenciacao de timbre característico de um flanger, 
simulando o pequeno atraso que ocorre. Modulado por um LFO senoidal, o delay varia lentamente, e este efeito segue as equacoes a seguir:

y[n] = x[n] + g * x[n -M[n]]

M[n] = Alfo * cos(2*pi* Flfo *(n/fs))


*Delay*

O delay e um efeito linear e invariante no tempo que se caracteriza pela repeticao do
sinal apos um certo período de tempo, chamado atraso ou delay. Dependendo da extensao
do atraso, pode-se classificar o delay como um echo. Especificamente, para intervalos de
tempo onde e perceptível a distancia temporal entre o som original e o atrasado, tem-se
um eco.
A implementacao do delay pode ser feita atraves de um buffer circular, programando filtros 
comb. Os tipos de filtro comb se diferenciam no tipo de delay produzido. No caso do filtro FIR, 
a repeticao do sinal ocorre apenas uma vez e, no IIR, pode-se implementar um delay com feedback, 
tambem chamado delay multiplo, onde as repeticoes ocorrem mais que uma vez.

Matematicamente, o delay simples pode ser definido como uma funcao da entrada x[n] somada a essa 
mesma entrada multiplicada por um ganho g e atrasada de um tempo n0, como mostra a equacao :

y[n] = x[n] + g * x[n - n0]
