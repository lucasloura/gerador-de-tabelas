# Gerador de Tabelas

  Gerador de Tabelas é um pequeno projeto desenvolvido por mim como atividade de aprendizado autoimposta durante Agosto de 2022. A proposta desta atividade era de utilizar os conhecimentos adquiridos na matéria Estatística Aplicada para confeccionar um algoritmo que criaria uma tabela que teria como base as normas do IBGE abordadas em sala.

  Antes de começar, é necessário ter em mãos um arquivo de texto com um número decimal em cada linha, pois esta será a fonte de dados do programa.

  Ao inciar, o programa pergunta o nome do arquivo a ser lido e, caso respondido com um nome válido, começa a ler e armazenar os números decimais encontrados, depois perguntará se o limite inferior encontrado é o desejado e também como devem ser os intervalos da tabela (fechado em x e aberto em y ou aberto em x e fechado em y), e então começará a fazer os cáculos.

  Ao encerrar, são devolvidas e gravadas em disco as informações sobre as amplitudes dos intervalos (hk), os pontos médios (xi), as frequências absolutas (fi), as frequências relativas (fri, tanto em forma decimal quanto percentual), a frequência absoluta acumulativa (FI) e a frequência relativa (Fri) em forma percentual dos intervalos dos valores obtidos. 
  Também são devolvidos e escritas em disco a quantidade total de dados lidos (n), os valores dos limites gerais inferior (li) e superior (Li), da amplitude geral (h), do ponto médio geral (xi), do número de intervalos (k) e da amplitude dos mesmos (hk).
