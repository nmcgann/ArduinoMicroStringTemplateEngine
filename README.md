# ArduinoMicroStringTemplateEngine
Micro String Template Engine for Arduino - Modified for event driven system

Essentially a re-write, but based on the original concept.
 
- Re-written to use the same logic in all routines (state machine).
- C++ templated to handle variable array sizes without using sizeof()
- Callback verions added to call routines corresponding to each template placeholder.
- Placeholders can run from 0 to 99. Can handle single digit and two digit versions.
- Callback pointer table can be in PROGMEM when template strings are also in PROGMEM.
- Illegal placeholders are ignored.
- Single .h file, no .cpp.

The callback idea allows a printed string to be self-populating as the callback routines
can go off and fetch data to print. The callbacks are passed the `Print& pr` parameter so
can do `pr.print()` themselves.

The point behind the re-write was to allow a simple event-driven refresh of an LCD screen page 
where the values of variables were automatically fetched and formatted and inserted into the 
shell of the screen text. 
