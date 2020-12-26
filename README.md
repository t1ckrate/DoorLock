# DoorLock

DoorLock is a __personal project__ that I realized in order to **secure the door of my personal office at home**.
It uses various components such as an electric lock, an Arduino, an ESP8266 and relies on a web server executed by Docker on a Linux server.

## Detailed presentation

The central part is in the **Arduino Uno**. The Arduino Uno is connected to a **Velleman RFID reader** *(activating a relay when a recognized badge is detected)*, to **an electric door lock** *(powered by a transistor)*. 
There is also an additional system managing the **emergency system** *(composed of a button and a switch, allowing, in case of fire for example, the continuous unlocking of the door)*.

The second part is connected to **the control panel on the side of the door**. This panel is **controlled by an ESP8266**, linked to a **3x4 keypad and a 20x4 character LCD screen**.

The last part is a **Java application**, managing HTTP requests, in order to allow the different elements of this project to communicate together. All requests **require authentication** *(different depending on the system)*, in order to verify the **source of the request**.


During **an authentication attempt**, on the keypad for example, **a request from ESP8266** is sent to the server. The server **hashes** the composed code and **compares it** with the one in the database. If **a match is found**, it retrieves the user and **incorporates it** into the query response. Also, another request is sent to the Arduino (managing the electric strike) in order to unlock it.

**Thus, the screen displays a welcome message with the user's name, and the door is unlocked.**

## Usage

**I don't think this project is of any real use in view of its complexity to implement.** I think I broke my head unnecessarily on some features and if I'll have to do it again I won't do it this way.
*Others projects have a better structure, reliability and much secure.*

**If you want to reproduce my project**, **you can help yourself** with the Arduino code (which is not really complex) to make an electric circuit.

**If you need instructions**, please contact me by email: mail@t1ckrate.fr, and I will do the necessary to post the instructions here.

## Contributing
Pull requests are welcome. For major changes, please open an issue first to discuss what you would like to change.

Please make sure to update tests as appropriate.

## License
[MIT](https://choosealicense.com/licenses/mit/)

Feel free to reuse this code and improve/adapt it to your needs.