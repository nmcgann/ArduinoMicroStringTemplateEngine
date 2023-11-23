#include "TemplatePrinter.h"
#include "PString.h"
#include "test.h"

#define FSH(x) (__FlashStringHelper*)(x)
#define FPSTR(pstr_pointer) (reinterpret_cast<const __FlashStringHelper *>(pstr_pointer))

const char line0[] PROGMEM = "This is the first line of text - %0 hook\r\n"; //
const char line1[] PROGMEM = "This is the second line of text - %1 flap\r\n"; //

const char* const screenTable[] PROGMEM = {
  line0,
  line1,
};

const char* const fValues[]  = {"RAT", "SHREW", "THEN"};

void printP0(Print& pr){
  pr.print("FISH");
}

void printP1(Print& pr){
  pr.print("CAT");
}

void printP2(Print& pr){
  pr.print("MOUSE");
}

const TemplateCallback* const callbackArr[] PROGMEM = { 
  &printP0,
  &printP1,
  &printP2,
};

const TemplateCallback* const callbackArr1[] PROGMEM = { 
  &printP0,
  &printP1,
  &printP2,
};

const TemplateCallback* const cbValues[] = { 
  &printP0,
  &printP1,
  &printP2,
};


void setup(){
  Serial.begin(115200);
  char buff[65];
  PString pbuff(buff, sizeof(buff));

  TemplatePrinter::printToCallbacks(pbuff, FPSTR((char*) pgm_read_word(&(screenTable)[0])), callbackArr);

  Serial.print(pbuff);//Serial.println();

  pbuff.begin();

  TemplatePrinter::printToCallbacks(pbuff, FPSTR((char*) pgm_read_word(&(screenTable)[1])), callbackArr);

  Serial.print(pbuff);//Serial.println();

  pbuff.begin();

  testCallbacks(pbuff, callbackArr);

  Serial.print(pbuff);Serial.println();

  pbuff.begin();

  testCallbacks1(pbuff, callbackArr1);

  Serial.print(pbuff);Serial.println();

//###

  const char* values[] = {"HORSE", "DOG", "NOW"};

  TemplatePrinter::printTo(Serial, "Test template %0 and %1 are printed %2\r\n", values);

  TemplatePrinter::printToCallbacks(Serial, "Test template %0 and %1 are printed %2\r\n", cbValues);

  TemplatePrinter::printTo(Serial, F("Test template %0 and %1 are printed %2\r\n"), fValues);
}

void loop(){


}