# ASCII-Pokemon-Game

## Assignment 1.01

An ASCII Pokemon game written in C for my C/C++ class. The program
generates the maps using a seeding technique where 30 random points
are selected on the map and are assigned values for terrain and some
other values like coordinates and elevations. The roads are built using
the elevation values as previously mentioned, I assigned low values for
terrain which would be easy to move through like grass or tall grass. Large
elevations are reserved for things like rocks or water, to also deal with
the issue of the paths possible riding the edges of the map I set those
elevations to have a value of 999. Pokemarts and Pokecenters are placed on
the map randomly and then have roads build to them. A checking algorithm is
also implemented so that if either a Pokemart or Pokecenter is already next to
a road, there will be no road build to it

## Assignment 1.02

First, since this is a continuing assignment, I'm going to keep adding on
to this one readme rather than creating a new one. The next iteration of the
project I built using much of the recommended steps. The project stores individual
maps as a struct now, rather than a 2d array in the main method. Using these structs
I then constructed the world map which is a 2d array of size 401 * 401 of these map
structs. The way that Pokemarts and Pokecenters are placed has been redone because
placing them randomly and building roads to them wasn't scalable to the edge cases of
the world. To compensate for my change in logic on this, I changed the Pokemarts and
Pokecenters to instead be one cell rather than the original 2 * 2. I do have plans to
revert them back to 2 * 2, but it's not feasable right now. In addition to the placement
changes for Pokemarts and Pokecenters I have also used the recommended equation in a
slighly tweaked way to generate difficuly (reduce Pokemart and Pokecenter spawns) as
distance increases from the center of the map. For taking in instructions from a user
I used a combination of if statements and scanf to check if the user has input 1 or 3
inputs. Those being a single character or a character and two integers for the fly mechanic.
Adding to taking in inputs, I did try to add good error statements so that the program doesn't
just crash, but also tells you what you did wrong with the inputs. Finally, generating
the new maps and making sure the gates lined up wasn't too hard, having Dijkstra's available
came in handy for the edge cases as I could just tell it to go from cell x to cell y. The gates
stay lined up because the struct I created to represent a single map also stores that map's
gates. So lining gates up is a matter of being careful with the edge cases and just checking
if the map next to it has been created and then assigning gates properly.

## Assignment 1.03

For assignment 1.03 I was able to modify my Dijkstra's algorithm for path gen to function for
the cost maps. This process actually went way smoother than anticipated with only a few hiccups.
As I mentioned earlier, the way I accomplish cost maps is through the use of Dijkstra's algorithm,
which I had previously defined. I made a new Dijkstra's method to specifically build the cost maps
for the hiker and rival npcs. This new Dijkstra's algorithm needed a different neighbor function
as it must consider eight neighbors as opposed to four neighbors. I constructed a new function to find
eight neighbors, its very simple, just the four neighbor function expanded to consider corners. In
the previous assignment I had an issue with my program crashing, I have since discovered and fixed
the error, which was a small case of accessing a value which was not set yet. Going back to the cost
maps, I did include the borders on the maps so the printed maps should all be surrounded by a layer of
empty spaces. In addition, if the player character appears in any of the gates the player character is
considered unreachable and the cost map will have a 00 for the player and everything else will be empty
spaces. This is intentional by design as an npc should not be able to step onto a gate, but it does look
as if the program has a bug in it. Although I don't think this should happen because I have since
restricted the ability for this to happen, regardless, I feel as though it was deserving of a comment.

## Assignment 1.04

I had a rough time on this assignment solving some of my bugs from the previous assignment, but I think
I managed to get a good bit of them solved. Moving onto this assignment, I chose to re-use my priority queue
which I had implemented through a minimum heap, albiet with some slight alterations. I had a lot of trouble
using new variables as I found that for some reason even just adding them would cause a seg fault, this is something
I intend to look into when I have more time. Re-using my min heap worked out perfectly for me and I have a solid
turn order function for future assignments. I did decide to leave the player character stationary as I was running
out of time. I used two new structs, one a PlayerCharacter struct and the other for NPCs as a whole, these structs
sort of act as enums which I intend to use more when we add functionality to the game, but for now I have let them
just sit and serve the purpose of being integrated early. During this assignment I also created a ton of files and 
split up my main.c file because I was getting annoyed with how crowded the file was getting. This caused me to run
into some include issues which I solved by creating a struct.h file which contains all of the structs for my project. I
also managed to implement the switch which was fun and interesting to find out how to do that.

## Assignment 1.05

This iteration has a lot of bug fixes so hopefully it is more stable. I forgot to mention in the last assignment that
when using --numtrainers, it must be used as --numtrainers='number here' which will be necessary to verify the paging
of trainers when they exceed the height of the terminal. To keep the terminal clean when displaying trainers I decided to
limit the display to about 21 or 22, this was so that it would display as many as the height of the map. When the user
is over a Center or Mart and presses >, they are entered into a temporary menu where various things will take place later.
For now, the menu simply serves as a place holder until the user presses < to escape the menu. I decided to have pressing t
and, entering and exiting a mart or center count as player moves so the npcs will still move even if these stationary actions
are being used. The temporary battle mechanic I managed to get working with the player character attacking npcs, but I wasn't able to
get it to work with the npcs attacking the player so this is a known bug. My decided movement for the hikers and rivals after
they have been defeated was for them to sit still as they are sad and crying at having been beaten and won't be moving
anytime soon. Lastly, I'm certain the player movement is pretty good, I myself did not find any errors while testing the
player's movement so I believe it is very stable. My only recommendation is that when moving the player, take a small amount
of time between key presses, maybe something like half a second just so that everything will load properly. Something fun you can
do that I discovered is that holding in a direction makes the player rapidly move in the entered direction and I simply think
it looks funny. All in all, I had to code this on Pyrite in vim because I could not find a good way to go about programming
with ncurses on Windows, if you have any tips on this, please let me know.

SOME KNOWN BUGS
-npcs will not attack the player (I was not able to get this done in time)
-immediate seg fault (This occurs rarely, I'm guessing it occurs in a rare case where an npc tries to move but cannot. I have not been able
to pinpoint the cause of the error yet.)

## Assignment 1.06

This assignment saw a lot of refactors in certain ways that I handle npcs, movement, and map moving from the previous assignment 1.02. My process was to finish the functionality portion of the assignment in C and then update everything to C++ after. The C++ transition went fairly well, I personally wanted to move to using new and delete and I believe a few bugs were born from it. The initial iteration is about as rough as I was expecting.

Bug fixes: (1) The immediate seg fault from 1.05 has been resolved as it was an error with how npcs were being placed which would sometimes cause rivals to be placed in such a way that they could never reach the player.

Known bugs: (1) The edge cases of the world, something like (400, 400) produces an infinite loop which requires shutting down the terminal. After porting to C++ this now just crashes, so doing a command like: f, 200, 200, with [enter] pressed after each integer, will crash the code with an error message. Unfortunately I still don't have an answer to this problem with the edge cases. (note you will have to close your terminal if you hit this issue).

Fears: (1) I have tested it a fair amount, probably gone through about 50 map transitions and flies without crashing, but I'm always scared that it won't work when it matters. 

New Functionality: The fly command should be executed as such: 1st: press f, this will open a menu with some descriptive text. 2nd: enter in the desired x coordinate of the new map on the overall 401 x 401 world, hit enter. 3rd: repeat step 2, but for the desired y coordinate.

Also note that since moving around can be pretty annoying with ten other trainers use the following after using make:
```
./PokeGame --numtrainers=2
```
This will only spawn two trainers, a hiker and a rival.

## Assignment 1.07

For this assignment I finally switched over to using Shaeffer's code, I had a scare with 1.06 and I don't want to deal with it again. I added a switch for this assignment as described in the document, I wanted to stick with how Shaeffer did his --seed switch so I've coded them as follows.
```
./poke327 --pokemon
```
This will open, parse, and print out the pokemon.csv file with the format of header: value. I could not find a specification for printing out the parsed file so I went with this format. In addition, if a field is empty, I denote it with INT_MAX in the code, but I print out an empty string. I couldn't find anything concrete in the pdf which said to not print out such values, all I found was to not print out INT_MAX itself. As for the file paths going between the one with /share/cs327/... and $HOME/.poke327/... I check /share/cs327/... first and can verify that it works well. $HOME/.poke327/... should work, but I cannot verify it 100% as I did not make a copy of the database. Although I maintain that it should work since I used Shaeffer's code from class to accomplish it. I'll list the commands to print out every file below.

NOTE: pokemon_moves takes about 1-2 minutes to full print out
```
./poke327 --pokemon
./poke327 --pokemon_moves
./poke327 --pokemon_species
./poke327 --pokemon_stats
./poke327 --pokemon_types
./poke327 --moves
./poke327 --experience
./poke327 --type_names
./poke327 --stats
```
Note: each of these should be entered in individually.