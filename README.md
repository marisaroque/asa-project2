asa-project2
===========
Second project assignment for the Analysis and Synthesis of Algorithms course.

LEIC-T # 2013/2014 www.tecnico.ulisboa.pt/

The Problem
-
A segurança é uma preocupação importante em vários tipos de eventos, particularmente aqueles que envolvem grandes grupos de pessoas, multidões. Eventos como jogos de futebol ou manifestações exigem bastante trabalho logístico por parte das forças de segurança envolvidas. Neste trabalho iremos considerar um modelo simples de controlo de multidões. Consideramos o mapa de uma cidade, onde existem pontos, onde as pessoas se podem concentrar e ligações entre esses pontos. Dois pontos podem estar ligados, no máximo, de uma forma. A nossa preocupação é garantir que não há comunicação, entre um conjunto de pontos críticos. Se houver comunicação entre todos os pontos críticos é mais provável que seja iniciado um motim, num desses pontos. Se tal acontecer a desordem irá extender-se a todo os outros pontos que lhes estejam directamente ligados, e assim sucessivamente a todos os pontos que sejam atingíveis a partir do ponto inicial. Para evitar comprometer a segurança da cidade inteira as forças de segurança podem barrar algumas das ligações, por forma a evitar que seja iniciado um motim. Para minimizar os custos desta operação, queremos minimizar o número de ligações que é necessário barrar. Assim o objectivo deste projecto consiste em modelar este problema, utilizando estruturas de dados e algoritmos adequados, por forma a que, dado o mapa de uma cidade e uma lista de pontos críticos, o algoritmo calcule o número mínimo de ligações que é necessário barrar. O objectivo é que os pontos críticos não estejam todos em comunição. Queremos que pelo menos um conjunto não vazio de pontos críticos fique isolado dos restantes, minimizando o número de ligações a barrar.

Input
-
A primeira linha do input contém dois inteiros. O primeiro indica o número de pontos n e o segundo inteiro o número de ligações m.

Seguem-se as ligações, uma por linha. Cada linha contém dois inteiros, que identiﬁcam os pontos que estão ligados. Estes números u e v respeitam as seguintes restrições: 0 ≤ u < v < n.

Após as linhas das ligações existe uma linha que contém apenas um número h, a indicar o número de problemas que se seguem. Todos estes problemas, i.e., conjuntos de pontos, devem ser resolvidos sobre o grafo lido anteriormente. As seguintes h linhas contém a listas de pontos críticos. O primeiro número de cada linha, k indica quantos pontos críticos existem nessa linha, com 1 ≤ k ≤ n. Os seguintes k números são os dos pontos críticos.

Todos os números que esteja numa mesma linha são separados por um espaço em branco.

Output
-
Para cada caso de testes o output deve consistir em h linhas, cada uma com apenas um número, o número mínimo de ligações a barrar, para o respectivo problema. A ordem dos números deve ser a dada no input. Cada linha de output não deve conter mais nenhum caracter, para além dos dígitos da resposta e do ’\n’.

Examples
-

- Input 1<br/>

  4 4<br/>
  0 2<br/>
  0 3<br/>
  1 2 <br/>
  1 3 <br/>
  2 <br/>
  2 0 1 <br/>
  2 0 2 <br/>


- Output 1<br/>

  2 <br/>
  2 <br/>

- Input 2<br/>

  5 5<br/> 
  0 2 <br/>
  0 3 <br/>
  1 2 <br/>
  1 3 <br/>
  0 4 <br/>
  1 <br/>
  3 0 1 4 <br/>

- Output 2<br/>

  1 <br/>

- Input 3

  6 4 <br/>
  0 4 <br/>
  0 5 <br/>
  1 4 <br/>
  1 5 <br/>
  1 <br/>
  2 0 1 <br/>

- Output 3<br/>

  2 <br/>

