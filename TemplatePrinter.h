//#################################################################################################
// Very modified Template version re-written to use the same logic in all routines.
// Callback verions to call routines matching each template variable.
// Placeholders can run from 0 to 99. Can handle single digit and two digit versions.
//#################################################################################################
#pragma once

#include <Print.h>

/**
 * The original documentaion below:
 *
 * A very simplistic string template engine that helps preventing the usage of string function 
 * to reduce heap fragmentation.  
 * Idea is: give it a Print - object that is the output target and a template strig along 
 * with the values to fill in the place holder.
 *
 * The "%0" to "%9" are the placeholder for the runtime supplied values.
 * If the character after the '%' sign is no decimal digit or another '% then just 
 * a single '%' is printed.
 *
 * Example:
 *
 * Instead of creating a String with concatination (or '+') that results in many 
 * temporary objects write a template string and use this class:
 *
 * \code{.cpp}

   String name ("Dave");
   String game ("TicTacToe");
   ////// instead of 
   String a = "Hello, " + name + ". I want to play a game. Would you like to play \"" + game + "\" with me?" ;
   Serial.print(a);

   ////// use that
   //  array with template values:
   const char* values[] = {name->c_str(), game->c_str()};
   
   //  output to serial - template parameter is C string literal from RAM (initially copied from flash on startup)
   TemplatePrinter::printTo(Serial, "Hello, %0. I want to play a game. Would you like to play \"%1\" with me?", values);
   ////// or
   //  output to serial - template parameter is inline C string literal from FLASH (without being duplicated to RAM)
   TemplatePrinter::printTo(Serial, F("Hello, %0. I want to play a game. Would you like to play \"%1\" with me?"), values);
   //  output to serial - same as above but uses a pointer to a FLASH string literal
   static const char PROGMEM s_greeting[] = R"(Hello, %0. I want to play a game. Would you like to play "%1" with me?)";
   TemplatePrinter::printTo(Serial, FPSTR(s_greeting), values);

   \endcode

 * The above second method also makes it more easy to put whole strings into flash (see 3rd variant);
 *
 */
typedef void (TemplateCallback)(Print& pr);

class TemplatePrinter {

public:
  /** 
   *  Prints the i_template string (RAM) to pr and replaces occurences of "%0" - "%9" by the strings from the i_values table.
   *  '%' signs can be excaped by "%%" but will printed on there own if the second character is not a digit
   */

  template <typename T, size_t N> 
  static void printTo(Print& pr, const char* i_template, T* const (&i_values)[N]);

//new - added version that calls a callback for each template parameter
  template <typename T, size_t N> 
  static void printToCallbacks(Print& pr, const char* i_template,  T* const (&i_values)[N]);

  /**
   *  Prints the i_template string (FLASH) to pr and replaces occurences of "%0" - "%9" by the strings from the i_values table.
   *  '%' signs can be excaped by "%%" but will printed on there own if the second character is not a digit
   */
  template <typename T, size_t N> 
  static void printTo(Print& pr, const __FlashStringHelper* i_template, T* const (&i_values)[N]);
  
//new - added version that calls a callback for each template parameter
  template <typename T, size_t N> 
  static void printToCallbacks(Print& pr, const __FlashStringHelper* i_template, T* const (&i_values)[N]);

private:

};

//-------------------------------------------------------------------------------------------------

//in normal memory versions
template <typename T, size_t N> 
void TemplatePrinter::printTo(Print& pr, const char* i_template, T* const (&i_values)[N])
{
  const char* ps=i_template;
 
  char c;
  uint8_t state = 0; //start. look for %
  uint8_t idx1;
  uint8_t idx2;

  for(;;){
    c = *ps;

    if(!c) { //end terminator found
      if(state == 2){ //found 1 digit already, no 2nd digit
        if(idx1<N) pr.write(i_values[idx1]);
      }
      break; //done
    }
      
    switch(state){
      case 0:
        if(c=='%'){ //start of parameter
          state = 1;
        }else{ //just print
          pr.write(c);
        }
      break;
      case 1:
        if(c=='%'){
          //escaped % with 2nd one
          pr.write("%%");
          state = 0;
        }else{
          //look for 1st digit
          idx1 = c - '0';
          if(idx1 >=0 && idx1 <=9){
            //need to see if 2nd digit before doing the parameter
            state = 2;
          }else{
            //not a digit, print out the % and char & go back to look for a %
            pr.write("%"); pr.write(c);
            state = 0;
          }       
        }
      break;
      case 2:
        //1 digit already found, look for 2nd
        idx2 = c - '0';
        if(idx2 >=0 && idx2 <=9){
          //2 digits found
          idx1 = idx1*10+idx2;
          if((idx1)<N) pr.write(i_values[idx1]);
          state = 0; //back to look for %
        }else{
          //only 1 digit
          if(idx1<N) pr.write(i_values[idx1]);
          pr.write(c);
          state = 0;
        }
      break;
      default:
      break;
    }   
    ps++;    
  }

}

////////////////////////// Callback version
template <typename T, size_t N> 
void TemplatePrinter::printToCallbacks(Print& pr, const char* i_template, T* const (&i_values)[N]){
  const char* ps=i_template;
 
  char c;
  uint8_t state = 0; //start. look for %
  uint8_t idx1;
  uint8_t idx2;

  for(;;){
    c = *ps;

    if(!c) { //end terminator found
      if(state == 2){ //found 1 digit already, no 2nd digit
        if(idx1<N) i_values[idx1](pr);
      }
      break; //done
    }
      
    switch(state){
      case 0:
        if(c=='%'){ //start of parameter
          state = 1;
        }else{ //just print
          pr.write(c);
        }
      break;
      case 1:
        if(c=='%'){
          //escaped % with 2nd one
          pr.write('%');
          state = 0;
        }else{
          //look for 1st digit
          idx1 = c - '0';
          if(idx1 >=0 && idx1 <=9){
            //need to see if 2nd digit before doing the parameter
            state = 2;
          }else{
            //not a digit, print out the % and char & go back to look for a %
            pr.write("%"); pr.write(c);
            state = 0;
          }       
        }
      break;
      case 2:
        //1 digit already found, look for 2nd
        idx2 = c - '0';
        if(idx2 >=0 && idx2 <=9){
          //2 digits found
          idx1 = idx1*10+idx2;
          if((idx1)<N) i_values[idx1](pr);
          state = 0; //back to look for %
        }else{
          //only 1 digit
          if(idx1<N) i_values[idx1](pr);
          pr.write(c);
          state = 0;
        }
      break;
      default:
      break;
    }   
    ps++;    
  }

}

//in flash memory versions - this only works with the i_values table in normal memory
//to put i_values in PROGMEM needs the same idea as the templates with a table of pointers
//The callback version is the one that will be used almost all of the time.
template <typename T, size_t N> 
void TemplatePrinter::printTo(Print& pr, const __FlashStringHelper* i_template, T* const (&i_values)[N])
{
  PGM_P ps = reinterpret_cast<PGM_P>(i_template);

  char c;
  uint8_t state = 0; //start. look for %
  uint8_t idx1;
  uint8_t idx2;

  for(;;){
    c = pgm_read_byte(ps++);

    if(!c) { //end terminator found
      if(state == 2){ //found 1 digit already, no 2nd digit
        if(idx1<N) pr.write(i_values[idx1]);
      }
      break; //done
    }
      
    switch(state){
      case 0:
        if(c=='%'){ //start of parameter
          state = 1;
        }else{ //just print
          pr.write(c);
        }
      break;
      case 1:
        if(c=='%'){
          //escaped % with 2nd one
          pr.write("%%");
          state = 0;
        }else{
          //look for 1st digit
          idx1 = c - '0';
          if(idx1 >=0 && idx1 <=9){
            //need to see if 2nd digit before doing the parameter
            state = 2;
          }else{
            //not a digit, print out the % and char & go back to look for a %
            pr.write("%"); pr.write(c);
            state = 0;
          }       
        }
      break;
      case 2:
        //1 digit already found, look for 2nd
        idx2 = c - '0';
        if(idx2 >=0 && idx2 <=9){
          //2 digits found
          idx1 = idx1*10+idx2;
          if((idx1)<N) pr.write(i_values[idx1]);
          state = 0; //back to look for %
        }else{
          //only 1 digit
          if(idx1<N) pr.write(i_values[idx1]);
          pr.write(c);
          state = 0;
        }
      break;
      default:
      break;
    }   
  }

}
/////// Callback version - both templates and callback function table in PROGMEM
template <typename T, size_t N> 
void TemplatePrinter::printToCallbacks(Print& pr, const __FlashStringHelper* i_template, T* const (&i_values)[N])
{
  PGM_P ps = reinterpret_cast<PGM_P>(i_template);
  
  char c;
  uint8_t state = 0; //start. look for %
  uint8_t idx1;
  uint8_t idx2;

  for(;;){
    c = pgm_read_byte(ps++);
    
    if(!c) { //end terminator found
      if(state == 2){ //found 1 digit already, no 2nd digit
        if(idx1<N) ((T*) pgm_read_ptr(&i_values[idx1]))(pr);
      }
      break; //done
    }
      
    switch(state){
      case 0:
        if(c=='%'){ //start of parameter
          state = 1;
        }else{ //just print
          pr.write(c);
        }
      break;
      case 1:
        if(c=='%'){
          //escaped % with 2nd one
          pr.write('%');
          state = 0;
        }else{
          //look for 1st digit
          idx1 = c - '0';
          if(idx1 >=0 && idx1 <=9){
            //need to see if 2nd digit before doing the parameter
            state = 2;
          }else{
            //not a digit, print out the % and char & go back to look for a %
            pr.write("%"); pr.write(c);
            state = 0;
          }       
        }
        //ps++;
      break;
      case 2:
        //1 digit already found, look for 2nd
        idx2 = c - '0';
        if(idx2 >=0 && idx2 <=9){
          //2 digits found
          idx1 = idx1*10+idx2;
          if((idx1)<N) ((T*) pgm_read_ptr(&i_values[idx1]))(pr);
          state = 0; //back to look for %
        }else{
          //only 1 digit
          if((idx1)<N) ((T*) pgm_read_ptr(&i_values[idx1]))(pr);
          pr.write(c);
          state = 0;
        }
      break;
      default:
      break;
    }     
  }
 
}
