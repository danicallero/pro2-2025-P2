# **Programación II - Práctica 2: Subasta de Consolas Retro con Lista Dinámica y Pila Estática**

## **Visión general**
Este proyecto forma parte del curso de **Programación II** del grado en **Ingeniería Informática** en la **Universidad de A Coruña (UDC)**. Su objetivo es desarrollar un sistema de **gestión de subastas** de **consolas retro**, utilizando una combinación de:

- Una **lista dinámica ordenada** para almacenar consolas.
- Una **pila estática** para gestionar el historial de pujas por consola.

El sistema permite registrar consolas, realizar subastas, adjudicar consolas al mejor postor y generar informes estadísticos.

---

## **Objetivos principales**

- Comprender e implementar distintos **Tipos Abstractos de Datos (TAD)** interconectados.
- Manipular memoria dinámica para gestionar estructuras enlazadas.
- Aplicar pilas como mecanismo de historial (modelo LIFO).
- Gestionar errores básicos y condiciones especiales de negocio.

---

## **Estructuras de Datos Implementadas**

### 📁 Lista Dinámica Ordenada (`tList`)
- Implementación basada en nodos enlazados.
- Las consolas se almacenan en orden alfabético según su ID.
- Soporta operaciones: `createEmptyList`, `isEmptyList`, `insertItem`, `deleteAtPosition`, `getItem`, `updateItem`, `findItem`, `first`, `last`, `previous`, `next`.

### 📁 Pila Estática (`tStack`)
- Basada en un array con tamaño fijo (`SMAX`).
- Asociada individualmente a cada consola para registrar las pujas.
- Soporta: `createEmptyStack`, `isEmptyStack`, `push`, `pop`, `peek`.

---

## **Comandos soportados**

| Comando | Descripción |
|--------|-------------|
| `N`    | Registra una nueva consola indicando su ID, marca, precio y vendedor. |
| `D`    | Elimina una consola del sistema y libera su pila de pujas. |
| `B`    | Añade una puja sobre una consola existente. |
| `A`    | Adjudica la consola al mayor postor y elimina la consola del sistema. |
| `S`    | Genera estadísticas globales, por marca y muestra la consola con mayor incremento. |
| `R`    | Elimina todas las consolas sin pujas registradas. |
| `I`    | Invalida las pujas de consolas que exceden el rango aceptable. |

---

## **Formato y ejemplos de uso por comando**

### 🔹 `New`
**Formato:**
```plaintext
New <númeroDeComando> <ConsoleID> <vendedor> <marca> <precio>
```
**Errores comunes:**
- ID duplicado
- Lista llena (no aplicable a implementación dinámica)
- Cadena inválida o marca desconocida

**Ejemplo de uso:**
```plaintext
01 N console1 seller1 sega 150.00
```
**Salida esperada:**
```plaintext
********************
01 N: console console1 seller seller1 brand sega price 150.00
* New: console console1 seller seller1 brand sega price 150.00
```

### 🔹 `Delete`
**Formato:**
```plaintext
Delete <númeroDeComando> <ConsoleID>
```
**Errores comunes:**
- ID no encontrado

**Ejemplo de uso:**
```plaintext
09 D console2
```
**Salida esperada:**
```plaintext
********************
09 D: console console2
* Delete: console console2 seller user1 brand sega price 120.00 bids 0
```

### 🔹 `Bid`
**Formato:**
```plaintext
Bid <númeroDeComando> <ConsoleID> <postor> <nuevoPrecio>
```
**Errores comunes:**
- Precio menor al actual
- Postor igual al vendedor o postor actual
- ID inexistente

**Ejemplo de uso correcto:**
```plaintext
09 B console2 bidder1 125.00
```
**Salida esperada:**
```plaintext
********************
09 B: console console2 bidder bidder1 price 125.00
* Bid: console console2 bidder bidder1 brand sega price 125.00 bids 1
```
**Ejemplo con error:**
```plaintext
09 B console1 bidder1 120.00
```
**Salida esperada:**
```plaintext
********************
09 B: console console1 bidder bidder1 price 120.00
+ Error: Bid not possible
```

### 🔹 `Award`
**Formato:**
```plaintext
Award <númeroDeComando> <ConsoleID>
```
**Errores comunes:**
- ID inexistente
- Pila de pujas vacía

**Ejemplo de uso correcto:**
```plaintext
12 A console2
```
**Salida esperada:**
```plaintext
********************
12 A: console console2
* Award: console console2 bidder bidder1 brand sega price 125.00
```

### 🔹 `Stats`
**Formato:**
```plaintext
Stats <númeroDeComando>
```
**Errores comunes:**
- Lista vacía

**Ejemplo de uso correcto:**
```plaintext
04 S
```
**Salida esperada:**
```plaintext
********************
04 S
Console console1 seller seller1 brand sega price 150.00 bids 0
Console console2 seller seller2 brand nintendo price 120.00 bids 1

Brand     Consoles    Price  Average
Nintendo         1    120.00   120.00
Sega             1    150.00   150.00
Top bid: console console2 seller seller2 brand nintendo price 120.00 bidder bidder1 top price 125.00 increase 4.17%
```

### 🔹 `Remove`
**Formato:**
```plaintext
Remove <númeroDeComando>
```
**Errores comunes:**
- Lista vacía
- No hay consolas sin pujas

**Ejemplo de uso:**
```plaintext
10 R
```
**Salida esperada:**
```plaintext
********************
10 R
Removing console console1 seller seller1 brand sega price 150.00 bids 0
```

### 🔹 `InvalidateBids`
**Formato:**
```plaintext
InvalidateBids <númeroDeComando>
```
**Errores comunes:**
- Lista vacía
- Ninguna consola supera el umbral de pujas

**Ejemplo de uso:**
```plaintext
11 I
```
**Salida esperada:**
```plaintext
********************
11 I
* InvalidateBids: console console5 seller seller5 brand sega price 120.00 bids 6 average bids 2.50
```

---

## **Configuración del proyecto con CMake**

1. Configura el archivo `config.cmake` si se usa configuración automática.
2. Compila con:
```bash
cmake -S . -B build
cmake --build build
```
3. Ejecuta el programa:
```bash
./build/retro_auction [archivo_comandos.txt]
```

---

## **Archivos del Proyecto**
- `main.c`: Lógica principal y procesamiento de comandos.
- `console_list.c/h`: Lista enlazada ordenada.
- `bid_stack.c/h`: Pila estática.
- `types.h`: Tipos de datos y constantes.
- `config.cmake`: Archivo de configuración.
- `script.sh`, `test.c`: Herramientas de prueba.

---

## **Limitaciones y notas**
- La pila tiene un límite (`SMAX`) que restringe la cantidad de pujas por consola.
- Las funciones no gestionan entradas inválidas como punteros nulos por directrices docentes.

---
## Agradecimientos
Este proyecto fue desarrollado como parte de la asignatura Programación II de la Universidad de A Coruña (UDC) en el curso académico 2024-2025. Agradezco a mis compañeros y a la comunidad de desarrolladores por los debates y recursos compartidos, que resultaron fundamentales para complementar los contenidos de la asignatura y mejorar la implementación del proyecto.

---

## **Autoría**
- **Daniel Callero Costales** </br> – [daniel.callero@udc.es](mailto:daniel.callero@udc.es) </br> – [hola@danicallero.es](mailto:hola@danicallero.es)

---

<div align="center">
  Made with ❤️ & C by Daniel Callero
</div>

