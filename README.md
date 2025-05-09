Собрать программу можно так:

```
make
```

Запуск:

```
./main [args]
```

Аргументы программе передаются через командную строку в одном из двух вариантов:

```<fmt> <округление> <число>```

```<fmt> <округление> <число1> <операция> <число2>```

где:

fmt – формат представления входных чисел в виде A.B, где A и B - целые неотрицательные числа, обозначающие целую и дробную часть. A + B <= 32 и A >= 1.

операция – символ арифметической операции: +, -, *, /.

число – одно или два числа, записанные в 16-ричной побитовой форме с префиксом ‘0x’ в любом регистре, умещающиеся в 32 бита.

округление – задаёт тип округления:

0 к нулю

1 к ближайшему чётному

2 к $+\infty$

3 к $-\infty$


Примеры

```
$ ./main 16.12 0 0x17360
23.210
$ ./main 8.8 1 0xdc9f + 0xd736
-76.168
$ ./main 8.8 2 0xdc9F + 0xd736
-76.167
$ ./main 4.4 3 0x12 / 0x100
div_by_0
```
