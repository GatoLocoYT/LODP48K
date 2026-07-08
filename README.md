# La Orden del Plan 48K

> Un roguelike arcade desarrollado bajo las limitaciones y la filosofía de diseño de principios de los años 80.

---

## Filosofía

La Orden del Plan 48K no busca parecer un juego retro.

Busca ser desarrollado **como si hubiera sido creado a comienzos de los años 80**, donde cada byte, cada píxel y cada línea de código debían justificar su existencia.

La simplicidad es una decisión de diseño.

> **Todo debe justificar su existencia.**

---

## Especificaciones

- Lenguaje: C
- Librería gráfica: SDL2
- Sin motores de juego
- Audio generado mediante Bytebeat
- Sin imágenes externas
- Sin audio grabado
- Sprites de 8×8 píxeles
- Mapas procedurales mediante plantillas
- Filosofía KISS (Keep It Simple, Stupid)

---

## Restricciones

- Memoria propia máxima: **48 KiB**
- Una única sala cargada en memoria.
- Un único tipo de enemigo.
- El suelo no existe como sprite.
- Todo recurso gráfico pertenece al sprite sheet.
- Cada recurso debe tener un propósito.

---

## Gameplay

- Explorar salas generadas proceduralmente.
- Encontrar el obelisco de salida.
- Evitar o derrotar enemigos.
- Conseguir la mayor puntuación posible antes de morir.

---

## Condiciones de derrota

- El temporizador llega a cero.
- El jugador pierde sus tres vidas.

---

## Recursos gráficos

Todo el arte del juego se encuentra en un único archivo:

```
assets/assets.aseprite
```

Contiene:

- Héroe
- Enemigo
- Pared
- Obelisco de salida
- Corazón
- Números
- Letras necesarias para la interfaz

No existe ningún recurso gráfico que no sea utilizado por el juego.

---

## Salas

Cada sala se almacena de forma independiente.

```
art/rooms/

room_000.aseprite
room_001.aseprite
room_002.aseprite
...
```

Cada sala contiene únicamente:

- Muros
- Obstáculos

El jugador, los enemigos y el obelisco son colocados por el código al cargar la sala.

---

## Interfaz

El HUD muestra únicamente la información indispensable.

- TIME
- SCORE
- HP

Pantallas especiales:

- L0DP48K
- START
- GAME OVER

Para ahorrar memoria, el carácter **O** reutiliza el sprite del número **0**.

---

## Estilo visual

- Paleta ARNE16
- Fondo negro
- Sin tile de suelo
- Sprites 8×8
- Pixel Art minimalista

---

## ¿Por qué "48K"?

El nombre **La Orden del Plan 48K** rinde homenaje al legendario **ZX Spectrum 48K**.

Aunque el juego está desarrollado para PC moderna utilizando C y SDL2, adopta la filosofía de diseño de los videojuegos de principios de los años 80: restricciones técnicas, simplicidad, y aprovechamiento máximo de los recursos disponibles.

El límite de **48 KiB de memoria propia** es una referencia directa a aquella computadora que inspiró este proyecto.

---

## Objetivo del proyecto

Demostrar que un videojuego completo puede construirse utilizando restricciones técnicas severas sin sacrificar diversión ni personalidad.

Las limitaciones forman parte del diseño.

---

## Universo

La Orden del Plan 48K comparte universo con el proyecto principal **La Orden del Plan**, reinterpretando sus personajes, enemigos y conceptos bajo una estética arcade de principios de los años 80.

No pretende contar toda la historia, sino representar una versión simplificada y jugable de ese mundo.

---

## Desarrollo

Durante el desarrollo se sigue una única regla:

> **Si eliminar algo no empeora el juego, entonces nunca debió existir.**