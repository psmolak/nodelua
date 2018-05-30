nodelua
=======
 
  **nodelua** jest próbą przeniesienia ideii asynchronicznego interpretera
  opertego o pętle zdarzeń - **nodejs** - do świata Lua.    

  **nodejs** jest środowiskiem uruchomieniowym dla JavaScriptu, gdzie każda operacja,
  która potęcjalnie może zablokować proces w oczekiwaniu na jakiś
  zasób jest wykonywana asynchronicznie. Przykładowo zwykłe pobranie
  zawartości pliku jest funkcją, która jedynie sygnalizuje chęć
  pobrania zawartości pliku i dodatkowo przyjmuje funkcję, która zostanie
  uruchomiona, gdy rządanie zostanie spełnione przez system operacyjny.
  Środowisko uruchomieniowe wykonuje zapytanie do systemu operacyjnego
  i przechodzi do dalszsego wykonywania programu. W momencie, gdy system
  operacyjny dostarczy rządany zasób, środowisko uruchomieniowe wykonuje
  zakolejkowany callback.
  
```javascript
function on_read(data) {
    console.log(data);
}

fs.read(stdin, on_read);

// w momencie, gdy dane będą dostarcozne przez OS,
// on_read zostanie wywołany przez event loop
```

  Cały mechanizm jest zaimplementowany przy użyciu biblioteki `libuv`
  oraz zagnieżdzonego śilnika js `V8`. Zadaniem tego projektu jest
  wykorzystanie bilbioteki `libuv` aby stworzyć takie środowisko
  uruchomieniowe, gdzie językiem skryptowym będzie Lua.

Przykładowe użycie
==================

```bash
$ cat script.lua
function callback()
    print("This prints every 1000ms")
end
  
timer = module.timer_new()
module.stimer_start(timer, 1000, callback)

$ nodelua script.lua
This prints every 1000ms.
This prints every 1000ms.
This prints every 1000ms.
```

Zakres projektu
===============

  Oczywiście, sama bilbioteka `libuv` jest spora i składa się z wielu
  komponentów. Z powodu ograniczonego czasu i mojego poziomu znajomości
  samej bilbioteki zadecydowałem się zaimplementować:   
   * moduł umożliwiający **komunikację z gniazdami TCP**  
   * moduł odpowiedzialny za **komunikację z plikami**
   * moduł pozwalający na **cykliczne uruchamanie funkcji w zadanych interwałach czasowych**

Całość zostanie dodatkowo opakowana w wygodny, obiektowy interfejs.
Jeśli wystarczy mi czasu zaimplementuję prosty serwer http.
