
# Trabalho estrura de dados

O trabalho lida com diversos sensores, criando arquivos para teste com diversos sensores com diferentes tipos de valores, organizando-os e fazendo uma busca binaria pelos arquivos usando uma data e hpra desejada.






## Fluxo de funcionamento

### Criando um arquivo com sensores aleatorios

Na pasta rais do projeto execute os seguintes comandos

```bash
gcc testGen.c timestamp.c -o testgen
```

logo em seguida execute
```bash
.\testgen dd/mm/yyyy hh:mm:ss dd/mm/yyyy hh:mm:ss nome-type nome-type nome-type...
```



o os primeiros argumentos "dd/mm/yyyy" e "hh:mm:ss" são a data e hora de inicio do escopo do arquivo de testes, então colocando 01/01/2001, o arquivo gerado tera timestamps apartir de 01/01/2001

o os segundos argumentos "dd/mm/yyyy" e "hh:mm:ss" são as datas limites do escopo de testes.

os argumentos "nome-types" são os sensores e seus respectivos tipos então colocando VIBR-0 o arquivo ira criar sensores com o nome de VIBR do tipo int

Lista de tipos 

- 0 = Inteiro
- 1 = Booleano
- 2 = Double
- 3 = String

### Organizando o arquivo

Nesta etapa você ja tem um arquivo com sensores, mas ele ainda não esta organizado por tempo.

para isso compile o sort.c usando 

```bash
gcc sort.c timestamp.c -o sort
```

e execute com 

```bash
.\sort.exe sensores.txt
```

agora voce tem não so um arquivo organizado por tempo, mas um arquivo para cada sensor, tambem organizado por tempo. agora é hora de porcurar por um sensor

### Buscando sensores pelo tempo

Para buscar um sensor usando um tempo especifico use 

```bash 
gcc search.c timestamp.c -o search
```

e para executar 

```bash 
.\search.exe dd/mm/yyyy hh:mm:ss arquivo1.txt arquivo2.txt...
```

aqui é seguido o mesmo padrão de execução do primeiro programa passando uma data e uma hora, mas logo após é passado um ou mais arquivos para serem buscados. o programa retorna os sensores mais proxmios em cada arquivo especificado



