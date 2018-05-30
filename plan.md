Spotkanie 1
===========

  * Bliższe poznanie bilbioteki `libuv`
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

  * Pełna implementacja modułu **uv_handle_t**
  * Pełna implementacja modułu **uv_stream_t**
  * Częściowa/pełna implementacja modułu **uv_tcp_t**

Spotkanie 3
===========

  * Cześciowa implementacja modułu **uv_fs_t** umożliwiająca prostą
    komunikację z plikami

Spotkanie 4
===========

  * Ostatnie szlify do modułu Lua
  * Zaprojektowanie i zaimplementowanie obiektowego wrappera na surowy modul Lua
  * Przykładowy, prosty, asynchroniczny scrapper WWW.

