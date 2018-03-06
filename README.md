

#Liczby pierwsze
## Napisać program który: 
1. na wejściu dostaje N liczb (czytane z pliku)  (liczby są nie większe niż  9223372036854775807
2. implementuje 3 algorytmy sprawdzanie czy liczba jest pierwsza
3. sprawdza N liczb każdym algorytmem i podaje na wyjściu czas wykonania dla każdego algorytmy (std::chrono)
4. Program ma używać wielu wątków do sprawdzania, ale nie więcej niż ilość rdzeni na używanej maszynie (jeżeli N == 4 to odpalamy 4 wątki, jeżeli N == 100, to odpalamy nproc i dzielimy zbiór liczba na odpowiednie podzbiory)

Przykładowo: nproc =2 && N = 17. Mamy tylko dwa wątki, tak więc dzielimy zbiór na dwa podzbiory - jeden o wielkości
algorytmy to: 
Trial division (https://en.wikipedia.org/wiki/Primality_test#Simple_methods)
Miller–Rabin
mpz_probab_prime_p (https://gmplib.org/manual/Prime-Testing-Algorithm.html)
