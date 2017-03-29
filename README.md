# The-Windows-Intel-Fastest-Mouse-Clicker

The fastest hardware-limited mouse clicker for Windows with command line support.

This is a very simple application that does just one task but does it 
perfectly. The program emulates Windows mouse clicks once a triggered 
key is pressed by end-user. The application performs this job extremely 
quickly with theoretical speed limit reached. I have rewritten this 
program inspired by a currently obsolete project "Fast Mouse Clicker" by
 xytor.

My application utilizes the cutting-edge feature of the SendInput() 
system call that aggregates some 100-1000 mouse events in one array and 
fires this array to a computer hardware at once. Furthermore, my program
 calls Sleep() between SendInput() very precisely, based on the Windows 
High Resolution Timer, to completely eliminate any idle moments that 
could be spent otherwise to the mouse clicks.

*NEW* Command line support: TheFastestMouseClicker.exe -c 
<clicks/s> -t <trigger key> -s <stop at> -m {p|t} -b 
{l|m|r}, where '-m {p|t}' is the 'trigger key mode' {'press'|'toggle'} and '-b 
{l|m|r}' is the 'mouse button to click' {'left'|'middle'|'right'}. One 
may specify any part of arguments; unspecified or unrecognized values 
will be treated as defaults (see them by running the app without command
line, e.g. from Windows programs menu).
