# SDG2: Cosas que realmente hay que hacer al configurar un proyecto de cero

1. “Cross compiler prefix”: arm-linux-gnueabihf-
2. “Cross compiler path”: 
3. Properties proyecto -> C/C++ Build -> Settings -> All Configurations -> Includes -> Include paths (-I): Añadir C:\SysGCC\Raspberry\include y C:\SysGCC\Raspberry\include\wiringPi
4. Properties proyecto -> C/C++ Build -> Settings -> All Configurations -> Libraries -> Library search path (-L): Añadir C:\SysGCC\Raspberry\lib
5. Properties proyecto -> C/C++ Build -> Settings -> All Configurations -> Libraries -> Libraries (-l): Añadir wiringPi y rt
6. Run configurations -> Copiar otro fichero .launch que tenga y personalizarlo con el nombre del proyecto