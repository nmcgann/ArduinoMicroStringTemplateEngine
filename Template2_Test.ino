#include "TemplatePrinter.h"
// Very modified Template printer test/example code

#define FSH(x) (__FlashStringHelper*)(x)
#define FPSTR(pstr_pointer) (reinterpret_cast<const __FlashStringHelper *>(pstr_pointer))

//based on Arduino example of a table of strings & pointers in PROGMEM
//This is the most typical use case.
const char line0[] PROGMEM = "This is the first line of text - %0 hook.\r\n"; //
const char line1[] PROGMEM = "This is the second line of text - %1 flap.\r\n"; //
const char line2[] PROGMEM = "This is the third line of text - %2 hole.\r\n"; //

const char* const screenTable[] PROGMEM = {
  line0,
  line1,
  line2,
};

//template callbacks
void printP0(Print& pr){ //%0
  pr.print("FISH");
}

void printP1(Print& pr){ //%1
  pr.print("CAT");
}

void printP2(Print& pr){ //%2
  pr.print("MOUSE");
}

//table of pointers to callbacks. Note: order must correspond to placeholder number
const TemplateCallback* const callbackArr[] PROGMEM = { 
  &printP0, //%0
  &printP1, //%1
  &printP2, //%2
};

//ditto but not in PROGMEM
const TemplateCallback* const cbValues[] = { 
  &printP0,
  &printP1,
  &printP2,
};

//substitution values
const char* values[] = {"HORSE", "DOG", "NOW"};

const char* const fValues[]  = {"RAT", "SHREW", "THEN"};

const char* const xValues[]  = {"The", "Quick", "Brown", "Fox", "Jumps", "Over", "The", "Lazy", "Dog", "How", "vexingly", "quick", "daft", "zebras", "jump"};

const char template1[] = "%0 %1 %2 %3 %4 %5 %6 %7 %8.\n\r%9 %10 %11 %12 %13 %14. Invalid:>%15 %16 %17 %18 %19<\r\n";

//-------------------------------------------------------------------------------------------------

void setup(){
  Serial.begin(115200);
  //various examples and tests
  
  TemplatePrinter::printToCallbacks(Serial, FPSTR((char*) pgm_read_word(&(screenTable)[0])), callbackArr); //both in PROGMEM

  TemplatePrinter::printToCallbacks(Serial, FPSTR((char*) pgm_read_word(&(screenTable)[1])), callbackArr); //both in PROGMEM

  TemplatePrinter::printToCallbacks(Serial, FPSTR((char*) pgm_read_word(&(screenTable)[2])), callbackArr); //both in PROGMEM

  TemplatePrinter::printTo(Serial, "Test template %0 and %1 are printed %2\r\n", values); //string and substitutions table in RAM

  TemplatePrinter::printToCallbacks(Serial, "Test template %0 and %1 are printed %2\r\n", cbValues); //string and substitutions table in RAM

  TemplatePrinter::printTo(Serial, F("Test template %0 and %1 are printed %2\r\n"), fValues);// template string in PROGMEM, substitutions table in RAM

  TemplatePrinter::printTo(Serial, template1, xValues);

}

void loop(){
  //nothing here

}