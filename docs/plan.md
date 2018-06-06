Spotkanie 1
===========

  * Dogłębne poznanie bilbioteki `libuv`
  * Ustalenie architektury projektu
  * Stworzenie repo ze szkieletem projektu
  * Testowa implementacja modułu uruchamiania funkcji okresowo **uv_handle_t**


  ```
   +--- nodelua ------------------------------------------------------+
   |                                                                  |
   |  +------------+      +---------+      +--------+     +-------+   |
   |  |            |      |         |      |        |     |       |   |
   |  | Event loop | -->  |  libuv  | -->  |  Lua   | --> |  Lua  |   |
   |  |  (libuv)   | <--  |         | <--  | module | <-- | state |   |
   |  |            |      |         |      |        |     |       |   |
   |  +------------+      +---------+      +--------+     +-------+   |
   |                                                                  |
   +------------------------------------------------------------------+
  
   Moduł będzie pomostem pomiędzy surowym libuv a intepreterem Lua.
   Większość typów z libuv zostanie zaimplementowana jako userdata
   z własną metatablicą. Dla każdego handler'a będzie przypisana tablica
   referencji Luowych funkcji umieszczonych w rejestrze interpretera.
   
   Sam moduł będzie całkiem wierną kopią interfejsu C'owego, zatem na
   jego podstawie zostanie napisany czysty, obiektowy interfejs wygodny
   w użyciu.
  
  ```

Spotkanie 2
===========

  Głównie celem będzie pełna implementacja modułów, dzięki którym
  będzie możliwe tworzenie gniazd TCP i wykonywanie połączeń.
  Zostanie również napisany prosty skrypt w Lua demonstrujący
  możliwości.

  * Pełna implementacja modułu **uv_handle_t**
  * Pełna implementacja modułu **uv_stream_t**
  * Częściowa/pełna implementacja modułu **uv_tcp_t**

Spotkanie 3
===========

  Celem będzie zaimplementowanie modułu umozliwiającgo komunikację z
  systemem plików. Przykładowy, dzięki temu wcześniejszy serwer będzie
  w stanie wysyłać informacje pobierane z plików.

  * Ewentualne dokończenie implementacji modułu **uv_tcp_t**
  * Cześciowa implementacja modułu **uv_fs_t** umożliwiająca prostą
    komunikację z plikami

Oddanie projektu
================

  Dokończenie implementacji modułu napisanego w C oraz utworzenie w
  pełni funkcjonalnego, obiektowego wrappera. Nastepnie na jego
  podstawie zostanie napisany prosty serwer WWW lub server czatowy.

  * Ostatnie szlify do modułu Lua
  * Zaprojektowanie i zaimplementowanie obiektowego wrappera na surowy modul Lua
  * Przykładowy, prosty, asynchroniczny server http / server czatowy.

