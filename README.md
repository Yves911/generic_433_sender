generic_433_sender
==================

Arduino sketch to send values over the air (433Mhz)

This code will simply send a value over the air using a specific protocol as descibed below:
* codeKit: the unique identifier of your station, 4 digits (by default 1000d)
* BytesType: is an identifier of the type of the value that you are sending, 2 digits (by default 10d)
* 1 bit to indicate if you are sending a positive or negative value
* the value you want to send over the air

You can define the PIN on which is wired the data pin of the RF433 sender by changing 
**#define senderPin 12**

Protocol look likes:
```
CodeKit (ID)    type   Positive/Negative  Value
1000            10              0           67129                  (decimal representation)
00001111101000  1010            0           000000000000010000011000111001 (binary representation)
```
The whole message is then encoded using manchester method http://en.wikipedia.org/wiki/Manchester_code 

To read message created by this program you can use https://github.com/pimatic/pimatic-homeduino (with the "generic protocol")
