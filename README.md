# PeliCAN
PeliCAN is a CAN FD library for the Flipper Zero.
~~Currently supported chips:~~
~~- MCP2518FD~~

I don't have a lot of time to work on this, ~~so help is appreciated!~~
Since PeliCAN was designed to work with CANDog, as I am abandoning the development of CANDog, development of PeliCAN will similarly be abandoned.

If you need to interface a CAN FD chip with a Flipper Zero, I recommend taking the same approach as Matthew KuKanich with [CAN Commander](https://github.com/MatthewKuKanich/CAN_Commander), and use an Arduino or ESP32 to provide a chip-agnostic interface - that way, you can rely on existing Arduino libraries, and the interfacing between Flipper and whatever interposer you choose can be cut down to the bare minimum.
