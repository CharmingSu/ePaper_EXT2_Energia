# ePaper_EXT2_Energia
* The example code is for PDi iTC EPD(1.54"/2.13"/2.71"/2.87"/4.2"/7.4"/12") on EXT2 board, it's verified by TI Launchpad MSP-EXP432P401R(red PCB).

 ### Launchpad Pin assignment for EXT2 Board
 ```bash
#define SCL_PIN 7   //EXT2 BOARD J5 pin 7
#define SDA_PIN 15   //EXT2 BOARD J5 pin 15
#define CS_PIN 19    //EXT2 BOARD J5 pin 19
#define DC_PIN 9    //EXT2 BOARD J5 pin 9
#define RESET_PIN 10  //EXT2 BOARD J5 pin 10
#define BUSY_PIN 8   //EXT2 BOARD J5 pin 8
#define PNLON_PIN 11  //EXT2 BOARD J5 pin 11
#define CSS_PIN 6    //EXT2 BOARD J5 pin 2 Slave CSB is only required of 12" operation
                     //EXT2 BOARD J5 pin 20 connected to GND
                     //EXT2 BOARD J5 pin 17 connected to GND for 4 wire SPI
                     //EXT2 BOARD J5 pin 1 connected to 3V3   
 ```
 
 ### EXT2 Board DIP Switch(J7) configuration
 
| Driver type and EPD size | S1 | S2 | S3 | S4 | S5 | S6 | S7 | S8 |
| :--- | :---: | :---: | :---: | :---: | :---: | :---: | :---: | :---: |
| (eTC) 1.44/ 2.0 | 0 | 0 | 0 | 0 | 0 | 0 | 1 | - |
| (eTC) 1.9/ 2.60/ 2.71 | 0 | 0 | 0 | 0 | 0 | 0 | 0 | - |
| (iTC) 2.15 | 1 | 0 | 1 | 0 | 0 | 0 | 0 | - |
| (iTC) 1.54/ 2.13/ 2.66/ 2.71/ 2.87/ 4.2/ 7.4/ 12.0 | 0 | 1 | 0 | 1 | 0 | 1 | 0 | - |

 #  Support
 *  For more information about PDi EPD and EXT2 board, please visit 
 *  [PERVASIVE DISPLAYS, INC. (龍亭新技股份有限公司)](http://www.pervasivedisplays.com/)
 *  [PDi Product Line up(iTC EPD)](http://www.pervasivedisplays.com/products#iTC)
 *  [EPD Extension Kit Gen 2 (EXT2)](http://www.pervasivedisplays.com/kits/ext2_kit)
 *  [24/7 Service](http://www.pervasivedisplays.com/contact/technical-support)
