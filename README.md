# planetEngine
An engine that simulates universal gravity and star systems built using C++ and SFML. You can simulate environments 
such as a star system, or a binary star system, or even a 3 star system, if you know a stable configuration that is.
You can also click to add in new stellar bodies that has a mass simular to our sun and see how it effects the systems.

---
![Demo picture 1](/media/screenshot1.png "Chaotic 3 body system.")

---

![Demo picture 2](/media/screenshot2.png "Chaotic 3 body system.")

---

![Demo picture 3](/media/screenshot3.png "Stable 3 body system.")

---

![Demo picture 4](/media/screenshot4.png "Solar System Simulation")

---
### Libraries Used:
For this project, I mainly used the SFML library as well as some of the standard C++ libraries. 
To install SFML, visit: https://www.sfml-dev.org/download.php
I believe you need to add SFML to your INCLUDE PATH, otherwise it won't work as intended when you do "g++ main.cpp"
Alternatively, you can also specify the include path in your compilation command, but that makes it quite lengthy
```
g++ main.cpp -I/opt/homebrew/Cellar/sfml/2.5.1_2/include -o app -L/opt/homebrew/Cellar/sfml/2.5.1_2/lib -lsfml-graphics -lsfml-window -lsfml-system
```
Something like that would work. Follow up with `./app` and you can run the program.

---
### How it works:
P h y s i c s & lots of P h y s i c s. . .
I basically just used the formula for universal gravity of any bodies of mass.

---
### Next steps:
I am hoping to take this project into the 3rd dimension, so I will be doing some matrix mathematics with SFML.
Any feedback is appreciated!

