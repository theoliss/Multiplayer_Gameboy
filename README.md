# Multiplayer_Gameboy

## Wizard fight : The project

We realize this project with comrades during a class called How To Make Almost Everything. The purpose of this project was to make a game, similar to a Gameboy, from scratch, using homemade ESP32 Dev Kits. All of this with a twist ! The game had to be playable at two players on two separated consoles connected by wifi (which is supported in the ESP32 chips). 

![pres open](https://github.com/theoliss/Multiplayer_Gameboy/assets/118252057/74c4d4d2-cc59-4872-afcd-a961c4d7b629)

As you may see we sold a custom shield on a custom ESP32 Dev Kit me made in an earlier class. 

Also, we modelized and 3d printed the casing and the buttons around it.  

![pres](https://github.com/theoliss/Multiplayer_Gameboy/assets/118252057/996d9d07-12b5-42f1-a669-e3c107abbeaa)

## The game 

![exemple](https://github.com/theoliss/Multiplayer_Gameboy/assets/118252057/d9bf7441-d507-4237-861f-42f6d5b153bd)

In this game, you incarnate a wizard who encounters a strange portal which shots dangerous spells at him ! 
So the only solution is to fight back ! With your four spells, you must act quickly to overcome this dangerous trap. 

In practise, the two Gameboys are linked, and whatever a player sends through his portal comes out of the other player's. 
So the goal is to act faster that the person you fight against. 

## The code 

For this project, all the software part is condensed in just one Arduino script (which can therefore look a bit long). All the sprites are encoded directly in the script, just like every sentences the message box could show, or the animations the sprites can make. 

This code requires many libraries. Knowing that, some of them are not in the basic Arduino's library list. So you will have to install 

```c:
<Adafruit_SSD1306.h>
<Keypad.h>
<esp_now.h>
````
And all their dependencies. 

## The upgrades we still have to make 

Even if we consider that this project is quite successful compare to what we have been asked. Due to the like of time during its realisation, we still have some upgrades to do. 

For instance, the WIFI MAC addresses the Gameboys use to connect to each other are hard coded, which impacts the reproducibility of this project. 

We also would have liked to have more animation for the win and lose screen. 

## If you want to make this project 

If you want to reproduce this project in its entirety. You will have to :

-Get two ESP32 chips with an I2C OLED screen and a matrix of button linked to it. (We personally used an ESP32-C3)

-Print the casing for the ESP (the 3d models are available in the GitHub files) 

-Upload the code in both ESP32 (in this current version of the code, you will have to know the MAC address of each ESP32 and replace this line (n°450) with the right ones) : 

```c
uint8_t broadcastAddress[] = {0x58, 0xCF, 0x79, 0x06, 0x30, 0x9C};
```

-Enjoy and have fun playing Wizard Fight!
